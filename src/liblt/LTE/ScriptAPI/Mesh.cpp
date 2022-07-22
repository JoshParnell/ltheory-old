#include "LTE/Function.h"
#include "LTE/Mesh.h"

TypeAlias(Reference<MeshT>, Mesh);

DefineConversion(mesh_to_geometry, Mesh, Geometry) {
  dest = (Geometry)src;
}

VoidFreeFunction(Mesh_Center,
  "Translate 'mesh' such that it is centered about the origin",
  Mesh, mesh)
{
  mesh->TranslateToCenter();
} FunctionAlias(Mesh_Center, Center);

FreeFunctionNoParams(Mesh, Mesh_Create, "Create a new, empty Mesh") {
  return Mesh_Create();
}

VoidFreeFunction(Mesh_SetOcclusion,
  "Set the occlusion factor of 'mesh' to a constant of 'occlusion'",
  Mesh, mesh,
  float, occlusion)
{
  mesh->SetU(occlusion);
} FunctionAlias(Mesh_SetOcclusion, SetOcclusion);
