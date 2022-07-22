#include "SpatialSignature.h"
#include "Bound.h"
#include "Ray.h"
#include "V3.h"

#include <vector>

namespace LTE {
  struct UniformGridSignature : public SpatialSignature {
    typedef V3T<int> Coord;

    std::vector<bool> cellData;
    int cellsX, cellsY, cellsZ, cellsYZ;
    size_t totalCells;
    V3 origin;
    V3 step;
    V3 rcpStep;

    UniformGridSignature(
        Bound3 const& box,
        size_t cellsX,
        size_t cellsY,
        size_t cellsZ) :
      cellsX(cellsX),
      cellsY(cellsY),
      cellsZ(cellsZ),
      cellsYZ(cellsY * cellsZ),
      totalCells(cellsX * cellsY * cellsZ),
      origin(box.lower)
    {
      cellData.resize(totalCells, false);
      V3 extents = box.GetSideLengths();

      step = V3((float)extents.x / (float)cellsX,
                (float)extents.y / (float)cellsY,
                (float)extents.z / (float)cellsZ);

      rcpStep = V3((float)cellsX / (float)extents.x,
                   (float)cellsY / (float)extents.y,
                   (float)cellsZ / (float)extents.z);
    }

    UniformGridSignature(Bound3 const& box, size_t cells) :
      cellsX(cells),
      cellsY(cells),
      cellsZ(cells),
      cellsYZ(cells * cells),
      totalCells(cells * cells * cells),
      origin(box.lower)
    {
      cellData.resize(totalCells, false);
      step = box.GetSideLengths();
      rcpStep = V3(1) / step;
    }

    void Add(void* object, Bound3 const& box, AddFn cb, void* aux) {
      Coord minCoord = ToLocal(box.lower);
      Coord maxCoord = ToLocal(box.upper);
      for (int x = minCoord.x; x <= maxCoord.x; ++x)
      for (int y = minCoord.y; y <= maxCoord.y; ++y)
      for (int z = minCoord.z; z <= maxCoord.z; ++z)
        if (cb(Bound3::FromPoints(ToWorld(x, y, z), ToWorld(x+1, y+1, z+1)), aux))
          cellData[GetCellIndex(x, y, z)] = true;
    }

    void Add(void* object, Bound3 const& box) {
      Coord minCoord = ToLocal(box.lower);
      Coord maxCoord = ToLocal(box.upper);
      for (int x = minCoord.x; x <= maxCoord.x; ++x)
      for (int y = minCoord.y; y <= maxCoord.y; ++y)
      for (int z = minCoord.z; z <= maxCoord.z; ++z)
        cellData[GetCellIndex(x, y, z)] = true;
    }

    void Clear() {
      for (size_t i = 0; i < cellData.size(); ++i)
        cellData[i] = false;
    }

    Coord ToLocal(V3 const& point) const {
      return Coord(Floor((point - origin) * rcpStep));
    }

    V3 ToWorld(int x, int y, int z) const {
      return V3((float)x, (float)y, (float)z) * step + origin;
    }

    size_t GetCellIndex(int x, int y, int z) const {
      return (size_t)(x*cellsYZ + y*cellsZ + z) % totalCells;
    }

    size_t GetMemoryUsage() const {
      return sizeof(*this) + cellData.size() / 8;
    }

    float GetOccupancy() const {
      size_t occupied = 0;
      for (size_t i = 0; i < cellData.size(); ++i)
        if (cellData[i])
          occupied++;
      return (float)occupied / (float)cellData.size();
    }

    bool Query(Bound3 const& box) const {
      Coord minCoord = ToLocal(box.lower);
      Coord maxCoord = ToLocal(box.upper);
      for (int x = minCoord.x; x <= maxCoord.x; ++x)
      for (int y = minCoord.y; y <= maxCoord.y; ++y)
      for (int z = minCoord.z; z <= maxCoord.z; ++z)
        if (cellData[GetCellIndex(x, y, z)])
          return true;
      return false;
    }
  };

  SpatialSignature* SpatialSignature_Grid(
    Bound3 const& box,
    size_t cellsX,
    size_t cellsY,
    size_t cellsZ)
  {
    return new UniformGridSignature(box, cellsX, cellsY, cellsZ);
  }

  SpatialSignature* SpatialSignature_Grid(Bound3 const& box, size_t cells) {
    return new UniformGridSignature(box, cells);
  }
}
