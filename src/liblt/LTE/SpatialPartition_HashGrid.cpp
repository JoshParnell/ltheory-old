#include "SpatialPartition.h"
#include "Array.h"
#include "Bound.h"
#include "Math.h"
#include "Ray.h"

namespace {
  struct HashGrid : public SpatialPartition {
    typedef V3T<int> Coord;
    typedef Vector<void*> Cell;

    mutable uint version;
    Array<Cell> cellData;
    Array<uint> cellVersion;
    V3 cellSize, invCellSize;

    HashGrid(V3 const& cellSize, size_t cells) :
      version(0),
      cellSize(cellSize),
      invCellSize(V3(1) / cellSize)
    {
      cellData.resize(cells);
      cellVersion.resize(cells);
      for (size_t i = 0; i < cells; ++i)
        cellVersion[i] = 0;
    }

    void Add(void* object, Bound3 const& box, AddFn callback, void* aux) {
      version++;
      Coord minCoord = ToLocal(box.lower);
      Coord maxCoord = ToLocal(box.upper);

      /* TODO : Speed up with max and mins? */
      for (int x = minCoord.x; x <= maxCoord.x; ++x)
      for (int y = minCoord.y; y <= maxCoord.y; ++y)
      for (int z = minCoord.z; z <= maxCoord.z; ++z) {
        Bound3 box = Bound3::FromPoints(ToWorld(x, y, z),
                                        ToWorld(x + 1, y + 1, z + 1));
        size_t cell = HashCoord(Coord(x, y, z)) % cellData.size();
        if (cellVersion[cell] != version &&
            callback(box, aux))
        {
          cellData[cell] << object;
          cellVersion[cell] = version;
        }
      }
    }

    void Add(void* object, Bound3 const& box) {
      version++;
      Coord minCell = ToLocal(box.lower);
      Coord maxCell = ToLocal(box.upper);

      for (int x = minCell.x; x <= maxCell.x; ++x)
      for (int y = minCell.y; y <= maxCell.y; ++y)
      for (int z = minCell.z; z <= maxCell.z; ++z) {
        size_t cell = HashCoord(Coord(x, y, z)) % cellData.size();
        if (cellVersion[cell] != version) {
          cellData[cell] << object;
          cellVersion[cell] = version;
        }
      }
    }

    void Clear() {
      for (size_t i = 0; i < cellData.size(); ++i)
        cellData[i].clear();
    }

    Cell& GetCell(V3 const& location) const {
      return cellData[HashCoord(ToLocal(location)) % cellData.size()];
    }

    size_t GetEmptyCells() const {
      size_t empty = 0;
      for (size_t i = 0; i < cellData.size(); ++i)
        if (cellData[i].empty())
          empty++;
      return empty;
    }

    size_t GetMemoryUsage() const {
      size_t memory = sizeof(*this);
      for (size_t i = 0; i < cellData.size(); ++i) {
        memory += sizeof(cellData[i]);
        memory += sizeof(void*) * cellData[i].size();
      }
      memory += sizeof(uint) * cellVersion.size();
      return memory;
    }

    float GetUtilization() const {
      return (float)(cellData.size() - GetEmptyCells()) / (float)cellData.size();
    }

    size_t HashCoord(Coord const& c) const {
      const uint p1 = 2376512323U;
      const uint p2 = 3625334849U;
      const uint p3 = 3407524639U;
      return p1*(uint)c.x + p2*(uint)c.y + p3*(uint)c.z;
    }

    void Query(V3 const& point, QueryFn callback, void* aux) const {
      Cell& cell = GetCell(point);
      for (size_t i = 0; i < cell.size(); ++i)
        if (!callback(cell[i], aux))
          return;
    }

    void Query(Bound3 const& box, QueryFn callback, void* aux) const {
      version++;
      Coord minCell = ToLocal(box.lower);
      Coord maxCell = ToLocal(box.upper);

      for (int x = minCell.x; x <= maxCell.x; ++x)
      for (int y = minCell.y; y <= maxCell.y; ++y)
      for (int z = minCell.z; z <= maxCell.z; ++z) {
        size_t cellIndex = HashCoord(Coord(x, y, z)) % cellData.size();
        if (cellVersion[cellIndex] != version) {
          cellVersion[cellIndex] = version;
          Cell& cell = cellData[cellIndex];
          for (size_t i = 0; i < cell.size(); ++i)
            if (!callback(cell[i], aux))
              return;
        }
      }
    }

    void Query(Ray const& ray, float tMax, QueryFn callback, void* aux) const {
      V3 cellTime = Abs(cellSize / ray.direction);
      V3 relative = ray.origin / cellSize;
      V3 signOffset = V3(.5f) + .5f * Sign(ray.direction);
      V3 t = Abs(cellTime * (Floor(relative) - relative + signOffset));

      Coord stepSign = Sign(ray.direction);
      Coord coord = ToLocal(ray.origin);
      Coord stepsToTarget = (ToLocal(ray(tMax)) - coord) * stepSign;

      while (stepsToTarget >= 0) {
        Cell& cell = cellData[HashCoord(coord) % cellData.size()];
        for (size_t i = 0; i < cell.size(); ++i)
          if (!callback(cell[i], aux))
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

    void Remove(void* object, Bound3 const& box) {
      version++;
      Coord minCell = ToLocal(box.lower);
      Coord maxCell = ToLocal(box.upper);

      for (int x = minCell.x; x <= maxCell.x; ++x)
      for (int y = minCell.y; y <= maxCell.y; ++y)
      for (int z = minCell.z; z <= maxCell.z; ++z) {
        size_t cell = HashCoord(Coord(x, y, z)) % cellData.size();
        if (cellVersion[cell] != version) {
          cellData[cell].remove(object);
          cellVersion[cell] = version;
        }
      }
    }

    Coord ToLocal(V3 const& point) const {
      return Coord(Floor(point * invCellSize));
    }

    V3 ToWorld(int x, int y, int z) const {
      return V3((float)x, (float)y, (float)z) * cellSize;
    }
  };
}

SpatialPartition* SpatialPartition_Hash(V3 const& cellSize, size_t cells) {
  return new HashGrid(cellSize, cells);
}
