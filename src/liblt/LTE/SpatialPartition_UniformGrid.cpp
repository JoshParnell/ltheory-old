#include "SpatialPartition.h"
#include "Array.h"
#include "Bound.h"
#include "Ray.h"
#include "V3.h"

namespace {
  struct UniformGrid : public SpatialPartition {
    typedef V3T<int> Coord;
    typedef Vector<void*> Cell;

    Array<Cell*> cellData;
    int cellsX, cellsY, cellsZ, cellsYZ;
    size_t totalCells;
    size_t populatedCells;
    V3 origin, step;

    UniformGrid(Bound3 const& box, size_t cellsX, size_t cellsY, size_t cellsZ) :
      cellsX(cellsX),
      cellsY(cellsY),
      cellsZ(cellsZ),
      cellsYZ(cellsY * cellsZ),
      totalCells(cellsX * cellsY * cellsZ),
      populatedCells(0),
      origin(box.lower)
    {
      cellData.resize(totalCells);
      for (size_t i = 0; i < totalCells; ++i)
        cellData[i] = nullptr;

      V3 extents = box.GetSideLengths();
      step = V3((float)extents.x / (float)cellsX,
                (float)extents.y / (float)cellsY,
                (float)extents.z / (float)cellsZ);
    }

    ~UniformGrid() {
      Clear();
    }

    void Add(void* object, Bound3 const& box, AddFn callback, void* aux) {
      Coord minCoord = ToLocalClamped(box.lower);
      Coord maxCoord = ToLocalClamped(box.upper);

      for (int x = minCoord.x; x <= maxCoord.x; ++x)
      for (int y = minCoord.y; y <= maxCoord.y; ++y)
      for (int z = minCoord.z; z <= maxCoord.z; ++z) {
        Bound3 box = Bound3::FromPoints(ToWorld(x, y, z),
                                        ToWorld(x + 1, y + 1, z + 1));
        if (callback(box, aux)) {
          Cell* cell = GetCell(x, y, z);
          cell->push(object);
        }
      }
    }

    void Add(void* object, Bound3 const& box) {
      Coord minCoord = ToLocalClamped(box.lower);
      Coord maxCoord = ToLocalClamped(box.upper);

      for (int x = minCoord.x; x <= maxCoord.x; ++x)
      for (int y = minCoord.y; y <= maxCoord.y; ++y)
      for (int z = minCoord.z; z <= maxCoord.z; ++z)
        GetCell(x, y, z)->push(object);
    }

    /* NOTE : Actually releases the memory. */
    void Clear() {
      for (size_t i = 0; i < totalCells; ++i) {
        delete cellData[i];
        cellData[i] = nullptr;
      }
    }

    Cell* GetCell(int x, int y, int z) {
      size_t index = x*cellsYZ + y*cellsZ + z;
      if (cellData[index])
        return cellData[index];
      else {
        populatedCells++;
        return cellData[index] = new Cell;
      }
    }

    /* Const version will not create cells. */
    Cell* GetCell(int x, int y, int z) const {
      if (x < 0 || y < 0 || z < 0 || x >= cellsX || y >= cellsY || z >= cellsZ)
        return nullptr;
      size_t index = x*cellsYZ + y*cellsZ + z;
      return cellData[index];
    }

    size_t GetMemoryUsage() const {
      size_t memory = sizeof(*this);
      memory += sizeof(Cell*) * totalCells;
      for (size_t i = 0; i < totalCells; ++i)
        if (cellData[i])
          memory += cellData[i]->size() * sizeof(void*);
      return memory;
    }

    void Remove(void* object, Bound3 const& box) {
      Coord minCoord = ToLocalClamped(box.lower);
      Coord maxCoord = ToLocalClamped(box.upper);

      for (int x = minCoord.x; x <= maxCoord.x; ++x)
      for (int y = minCoord.y; y <= maxCoord.y; ++y)
      for (int z = minCoord.z; z <= maxCoord.z; ++z)
        GetCell(x, y, z)->remove(object);
    }

    Coord ToLocal(V3 const& point) const {
      return Coord(Floor((point - origin) / step));
    }

    Coord ToLocalClamped(V3 const& point) const {
      return Clamp(ToLocal(point), Coord(0),
                   Coord(cellsX - 1, cellsY - 1, cellsZ - 1));
    }

    V3 ToWorld(int x, int y, int z) const {
      return V3((float)x, (float)y, (float)z) * step + origin;
    }

    void Query(V3 const& point, QueryFn callback, void* aux) const {
      Coord c = ToLocal(point);
      Cell* cell = GetCell(c.x, c.y, c.z);
      if (cell)
        for (size_t i = 0; i < cell->size(); ++i)
          if (!callback((*cell)[i], aux))
            return;
    }

    void Query(Bound3 const& box, QueryFn callback, void* aux) const {
      Coord minCoord = ToLocalClamped(box.lower);
      Coord maxCoord = ToLocalClamped(box.upper);

      for (int x = minCoord.x; x <= maxCoord.x; ++x)
      for (int y = minCoord.y; y <= maxCoord.y; ++y)
      for (int z = minCoord.z; z <= maxCoord.z; ++z) {
        Cell* cell = cellData[(size_t)(x*cellsYZ + y*cellsZ + z)];
        if (cell)
          for (size_t i = 0; i < cell->size(); ++i)
            if (!callback((*cell)[i], aux))
              return;
      }
    }

    void Query(Ray const& ray, float tMax, QueryFn callback, void* aux) const {
      V3 cellTime = Abs(step / ray.direction);
      V3 relative = (ray.origin - origin) / step;
      V3 signOffset = V3(.5f) + .5f * Sign(ray.direction);
      V3 t = Abs(cellTime * (Floor(relative) - relative + signOffset));

      Coord stepSign = Sign(ray.direction);
      Coord coord = ToLocal(ray.origin);
      Coord stepsToTarget = (ToLocal(ray(tMax)) - coord) * stepSign;

      while (stepsToTarget >= 0) {
        Cell const* cell = GetCell(coord.x, coord.y, coord.z);
        if (cell)
          for (size_t i = 0; i < cell->size(); ++i)
            if (!callback((*cell)[i], aux))
              return;

        if (t.x <= t.y && t.x <= t.z) {
          t.x += cellTime.x;
          coord.x += stepSign.x;
          stepsToTarget.x--;
        } else if (t.y <= t.x && t.y <= t.z) {
          t.y += cellTime.y;
          coord.y += stepSign.y;
          stepsToTarget.y--;
        } else {
          t.z += cellTime.z;
          coord.z += stepSign.z;
          stepsToTarget.z--;
        }
      }
    }
  };
}

SpatialPartition* SpatialPartition_Uniform(
  Bound3 const& box,
  size_t cellsX,
  size_t cellsY,
  size_t cellsZ)
{
  return new UniformGrid(box, cellsX, cellsY, cellsZ);
}
