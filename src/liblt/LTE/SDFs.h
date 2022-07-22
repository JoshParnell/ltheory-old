#ifndef LTE_SDFs_h__
#define LTE_SDFs_h__

#include "DeclareFunction.h"
#include "SDF.h"

DeclareFunction(SDF_Add, SDF,
  SDF, a,
  SDF, b)

DeclareFunction(SDF_Box, SDF,
  V3, center,
  V3, sides)

DeclareFunction(SDF_Capsule, SDF,
  V3, p1,
  V3, p2,
  float, radius)

DeclareFunction(SDF_Cylinder, SDF,
  V3, center,
  V3, axis,
  float, radius)

DeclareFunction(SDF_FractalPerlin, SDF,
  V3, center,
  int, octaves,
  float, lac)

DeclareFunction(SDF_FractalWorley, SDF,
  float, seed,
  int, octaves,
  float, lac)

DeclareFunction(SDF_Multiply, SDF,
  SDF, source,
  float, value)

DeclareFunction(SDF_Polyhedron, SDF,
  Vector<Plane>, planes,
  int, shape)

DeclareFunction(SDF_Radial, SDF,
  SDF, source,
  float, rMin,
  float, rMax)

DeclareFunction(SDF_Repeat, SDF,
  SDF, source,
  V3, frequency,
  V3, spacing)

DeclareFunction(SDF_Ring, SDF,
  V3, center,
  float, radius,
  float, thickness)

DeclareFunction(SDF_RoundBox, SDF,
  V3, center,
  V3, sides,
  float, radius)

DeclareFunction(SDF_Scale, SDF,
  SDF, source,
  V3, scale)

DeclareFunction(SDF_Shell, SDF,
  V3, center,
  float, radius,
  float, thickness)

DeclareFunction(SDF_Sphere, SDF,
  V3, center,
  float, radius)

DeclareFunction(SDF_Subtract, SDF,
  SDF, a,
  SDF, b,
  float, sharpness)

DeclareFunction(SDF_Torus, SDF,
  V3, center,
  float, radius,
  float, thickness)

DeclareFunction(SDF_Translate, SDF,
  SDF, source,
  V3, offset)

DeclareFunction(SDF_Union, SDF,
  SDF, a,
  SDF, b,
  float, sharpness)

DeclareFunction(SDF_Wedge, SDF,
  V3, center,
  float, angle,
  float, angularExtent,
  float, radius,
  float, radialExtent,
  float, height)

#endif
