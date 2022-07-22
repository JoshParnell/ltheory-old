#include "Array3D.h"
#include "MarchingCubes.h"

#include "LTE/SDF.h"
#include "LTE/Math.h"
#include "LTE/Mesh.h"

#include <iostream>

Mesh Mesh_Volume(
  Array3DFloat const& grid,
  Bound3 const& bound,
  float isoValue)
{
  MarchingCubes mc;
  mc.set_resolution(grid.resX, grid.resY, grid.resZ);
  mc.init_all();

  for (size_t z = 0; z < grid.resZ; ++z)
  for (size_t y = 0; y < grid.resY; ++y)
  for (size_t x = 0; x < grid.resX; ++x) {
    float f = grid.GetData(x, y, z);
    if (!IsFinite(f)) {
      std::cout << f << " @ " << x << ',' << y << ',' << z << std::flush;
      error("Non-finite grid cell.");
    }
    mc.set_data(f, x, y, z);
  }

  mc.run(isoValue);

  Mesh m = Mesh_Create();
  MCVertex* verts = mc.vertices();
  Triangle* tris = mc.triangles();

  V3 range = bound.GetSideLengths();
  for (int i = 0; i < mc.nverts(); ++i) {
    Vertex v;
    v.p.x = bound.lower.x + range.x * (float)verts[i].x / (float)(grid.resX - 1);
    v.p.y = bound.lower.y + range.y * (float)verts[i].y / (float)(grid.resY - 1);
    v.p.z = bound.lower.z + range.z * (float)verts[i].z / (float)(grid.resZ - 1);
    v.n.x = verts[i].nx;
    v.n.y = verts[i].ny;
    v.n.z = verts[i].nz;
    m->AddVertex(v);
  }

  for (int i = 0; i < mc.ntrigs(); ++i)
    m->AddTriangle(tris[i].v1, tris[i].v3, tris[i].v2);

  mc.clean_temps();
  mc.clean_all();
  return m;
}
