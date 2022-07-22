#include "Meshes.h"
#include "Matrix.h"

namespace {
  void BuildPlanarFace(Mesh const& self, V3 origin, V3 d1, V3 d2, uint n) {
    d1 /= (float)(n-1);
    d2 /= (float)(n-1);
    uint indexOffset = self->GetVertices();
    V3 currentPoint;
    V3 normal = Normalize(Cross(d1, d2));

    for (uint i = 0; i < n; ++i)
    for (uint j = 0; j < n; ++j) {
      V3 p = origin + (float)j * d1 + (float)i * d2;
      Vertex thisVertex;
      thisVertex.p = p;
      thisVertex.n = normal;
      thisVertex.u = (float)i / (float)(n - 1);
      thisVertex.v = (float)j / (float)(n - 1);
      self->AddVertex(thisVertex);
    }

    for (uint i = 0; i < n; ++i)
    for (uint j = 0; j < n; ++j) {
      if (i && j) {
        self->AddTriangle(((i-1)*n + (j-1) + indexOffset),
          (i*n + j + indexOffset),
          ((i-1)*n + j + indexOffset));
        self->AddTriangle(((i-1)*n + (j-1) + indexOffset),
          (i*n + (j-1) + indexOffset),
          (i*n + j + indexOffset));
      }
    }
  }
}

Mesh Mesh_Billboard(float minU, float maxU, float minV, float maxV) {
  return Mesh_Create()
    ->AddVertex(0, 0, minU, minV)
    ->AddVertex(0, 0, maxU, minV)
    ->AddVertex(0, 0, maxU, maxV)
    ->AddVertex(0, 0, minU, maxV)
    ->AddQuad(0, 1, 2, 3);
}

DefineFunction(Mesh_Box) {
  Mesh self = Mesh_Create();
  BuildPlanarFace(self, V3(-1,  1, -1), V3(0,  0,  2), V3(2,  0,  0), args.resolution);
  BuildPlanarFace(self, V3(-1,  1, -1), V3(2,  0,  0), V3(0, -2,  0), args.resolution);
  BuildPlanarFace(self, V3(-1,  1, -1), V3(0, -2,  0), V3(0,  0,  2), args.resolution);
  BuildPlanarFace(self, V3( 1,  1, -1), V3(0,  0,  2), V3(0, -2,  0), args.resolution);
  BuildPlanarFace(self, V3(-1, -1, -1), V3(2,  0,  0), V3(0,  0,  2), args.resolution);
  BuildPlanarFace(self, V3(-1,  1,  1), V3(0, -2,  0), V3(2,  0,  0), args.resolution);
  if (args.closed)
    self->ShareVertices();
  return self;
}

DefineFunction(Mesh_BoxRounded) {
  Mesh self = Mesh_Box(args.resolution, false);
  if (args.radius > 0.0f) {
    for (uint i = 0; i < self->GetVertices(); ++i) {
      Vertex& v = self->vertices[i];
      V3 proj = Clamp(v.p, V3(args.radius - 1.0f), V3(1.0f - args.radius));
      v.p = proj + args.radius * (v.p - proj) / PNorm(v.p - proj, 2.0f);
    }
  }
  return self;
}

DefineFunction(Mesh_BoxSphere) {
  Mesh self = Mesh_Box(args.resolution, args.closed);
  struct {
    void operator()(Vertex& v) {
      v.p = Normalize(v.p);
      v.n = v.p;
      v.u = Atan(v.p.z, v.p.x) / kPi;
      v.v = 0.5f * (v.p.y + 1.0f);
    }
  } displacer;
  self->Map(displacer);
  return self;
}

Mesh Mesh_Cone(uint slices) {
  struct {
    float operator()(float height, float angle) const {
      return 1 - height;
    }
  } displacer;
  return Mesh_PolarClosed(displacer, slices, 3, true);
}

Mesh Mesh_Cylinder(uint slices) {
  struct {
    float operator()(float height, float angle) const {
      return 1;
    }
  } displacer;
  return Mesh_PolarClosed(displacer, slices, 2, true)->Transform(
    Matrix::Scale(V3(1, 2, 1)) * Matrix::Translation(V3(0, -0.5f, 0)));
}

/* CRITICAL. */
DeclareFunction(Mesh_CylinderHUD, Mesh,
  float, curvature,
  float, power)

DefineFunction(Mesh_CylinderHUD) {
  Mesh self = Mesh_Plane(V3(-1, -1, 0), V3(2, 0, 0), V3(0, 2, 0), 33, 33);
  for (size_t i = 0; i < self->vertices.size(); ++i) {
    Vertex& v = self->vertices[i];
    float& x = v.p.x;
    float& y = v.p.y;
    float& z = v.p.z;
    z = 1.0f + args.curvature * (1 - Pow(Abs(x), args.power));
    x /= z;
    y /= z;
    z -= 1.0f;
  }
  return self;
}

DefineFunction(Mesh_Plane) {
  Mesh self = Mesh_Create();
  V3 normal = Normalize(Cross(args.y, args.x));

  for (uint j = 0; j < args.cellsY; ++j) {
    float v = (float)j / (float)(args.cellsY - 1);
    for (uint i = 0; i < args.cellsX; ++i) {
      float u = (float)i / (float)(args.cellsX - 1);
      if (i && j) {
        uint index = self->GetVertices();
        self->AddQuad(index, index - 1, index - args.cellsY - 1, index - args.cellsY);
      }
      self->AddVertex(args.origin + args.x * u + args.y * v, normal, u, v);
    }
  }

  return self;
}

DefineFunction(Mesh_Quad) {
  return Mesh_Create()
    ->AddVertex(V3(-1, -1, 0), V3(0, 0, -1), 0, 0)
    ->AddVertex(V3( 1, -1, 0), V3(0, 0, -1), 1, 0)
    ->AddVertex(V3( 1,  1, 0), V3(0, 0, -1), 1, 1)
    ->AddVertex(V3(-1,  1, 0), V3(0, 0, -1), 0, 1)
    ->AddQuad(0, 1, 2, 3);
}

Mesh Mesh_SkirtedPlane(int verticesX, int verticesY) {
  Mesh self = Mesh_Create();

  for (int j = 0; j < verticesY; ++j) {
    float v = (float)(j - 1) / (float)(verticesY - 3);
    float y = 2.0f*v - 1.0f;

    for (int i = 0; i < verticesX; ++i) {
      float u = (float)(i - 1) / (float)(verticesX - 3);
      float x = 2.0f*u - 1.0f;

      if (i && j) {
        uint cVertex = self->GetVertices();
        self->AddQuad(cVertex, cVertex - 1, cVertex - verticesY - 1, cVertex - verticesY);
      }
      bool skirt = i == 0 || j == 0 || i == verticesX - 1 || j == verticesY - 1;
      self->AddVertex(V3(x, 0, y), V3(0, 1, 0), skirt ? -1 : u, skirt ? -1 : v);
    }
  }

  return self;
}

DefineFunction(Mesh_Sphere) {
  struct {
    float operator()(float height, float angle) const {
      height = 2*height - 1;
      return Sqrt(1.0f - height*height);
    }
  } displacer;

  return Mesh_Polar(displacer, args.slices, args.stacks, false)->Transform(
    Matrix::Scale(V3(1, 2, 1)) * Matrix::Translation(V3(0, -0.5f, 0)));
}

Mesh Mesh_Tetrahedron() {
  return Mesh_Create()
    ->AddVertex(V3(-1,  1.0f / Sqrt(2.0f),  0), 0, 0, 0)
    ->AddVertex(V3( 1,  1.0f / Sqrt(2.0f),  0), 0, 0, 0)
    ->AddVertex(V3( 0, -1.0f / Sqrt(2.0f),  1), 0, 0, 0)
    ->AddVertex(V3( 0, -1.0f / Sqrt(2.0f), -1), 0, 0, 0)
    ->AddTriangle(0, 1, 2)
    ->AddTriangle(1, 3, 2)
    ->AddTriangle(0, 2, 3)
    ->AddTriangle(0, 3, 1);
}
