#include "LTE/Function.h"
#include "LTE/Mesh.h"
#include "LTE/PlateMesh.h"
#include "LTE/Warp.h"

TypeAlias(Reference<PlateMeshT>, PlateMesh);

VoidFreeFunction(PlateMesh_AddPlate,
  "Add a plate with 'center,' 'scale,' 'rotation,' and 'bevel' to 'plateMesh'",
  PlateMesh, plateMesh,
  V3, center,
  V3, scale,
  V3, rotation,
  float, bevel)
{
  plateMesh->Add(center, scale, rotation, bevel);
} FunctionAlias(PlateMesh_AddPlate, Add);

VoidFreeFunction(PlateMesh_AddPlateMesh,
  "Add 'source' to 'plateMesh' with translation 'offset' and scale 'scale'",
  PlateMesh, plateMesh,
  PlateMesh, source,
  V3, offset,
  V3, scale)
{
  plateMesh->Add(source, offset, scale);
} FunctionAlias(PlateMesh_AddPlateMesh, Add);

VoidFreeFunction(PlateMesh_AddWarp,
  "Add 'warp' to 'plateMesh'",
  PlateMesh, plateMesh,
  Warp, warp)
{
  plateMesh->Add(warp);
} FunctionAlias(PlateMesh_AddWarp, Add);

FreeFunction(Mesh, PlateMesh_GetMesh,
  "Create a mesh using 'plateMesh'",
  PlateMesh, plateMesh)
{
  return plateMesh->GetMesh();
} FunctionAlias(PlateMesh_GetMesh, GetMesh);

VoidFreeFunction(PlateMesh_ReflectX,
  "Reflect 'plateMesh' over the X axis",
  PlateMesh, plateMesh)
{
  plateMesh->ReflectX();
} FunctionAlias(PlateMesh_ReflectX, ReflectX);

VoidFreeFunction(PlateMesh_ReflectY,
  "Reflect 'plateMesh' over the Y axis",
  PlateMesh, plateMesh)
{
  plateMesh->ReflectY();
} FunctionAlias(PlateMesh_ReflectY, ReflectY);

VoidFreeFunction(PlateMesh_ReflectZ,
  "Reflect 'plateMesh' over the Z axis",
  PlateMesh, plateMesh)
{
  plateMesh->ReflectZ();
} FunctionAlias(PlateMesh_ReflectZ, ReflectZ);
