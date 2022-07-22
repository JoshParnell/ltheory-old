#ifndef Volume_Array3D_h__
#define Volume_Array3D_h__

#include "Common.h"
#include "LTE/Array.h"
#include "LTE/Bound.h"

template <class T>
struct Array3D {
  Array<T> data;
  size_t resX;
  size_t resY;
  size_t resZ;
  size_t resXY;

  Array3D(size_t resX, size_t resY, size_t resZ) :
    resX(resX),
    resY(resY),
    resZ(resZ)
  {
    resXY = resX * resY;
    data.resize(resX * resY * resZ);
  }

  T const& GetData(size_t x, size_t y, size_t z) const {
    return data[x + y*resX + z*resXY];
  }

  void SetData(T const& value, size_t x, size_t y, size_t z) {
    data[x + y*resX + z*resXY] = value;
  }
};

LT_API Mesh Mesh_Volume(
  Array3DFloat const& grid,
  Bound3 const& bound,
  float isoValue = 0);

#endif
