#ifndef LTE_Mesh_h__
#define LTE_Mesh_h__

#include "AutoClass.h"
#include "Geometry.h"
#include "GLEnum.h"
#include "GLType.h"
#include "V3.h"
#include "Vector.h"
#include "Vertex.h"

AutoClassDerived(MeshT, GeometryT,
  Vector<Vertex>, vertices,
  Vector<uint>, indices)
  DERIVED_TYPE_EX(MeshT)

  mutable GL_Buffer vbo;
  mutable GL_Buffer ibo;
  mutable GL_IndexFormat::Enum indexFormat;
  mutable short bufferVersion;
  short version;

  MeshT() :
    vbo(GL_NullBuffer),
    ibo(GL_NullBuffer),
    bufferVersion(0),
    version(0)
    {}

  LT_API ~MeshT();

  /* Geometry. */
  LT_API void Draw() const;

  LT_API Bound3 GetBound() const;

  LT_API Mesh GetCollisionMesh() const;

  LT_API short GetVersion() const;

  LT_API bool Intersects(
    Ray const& r,
    float* tOut = nullptr,
    V3* normalOut = nullptr,
    V2* uvOut = nullptr) const;

  LT_API V3 Sample() const;

  /* Vertex Manipulation. */
  LT_API Mesh AddMesh(Mesh const&);

  LT_API Mesh AddMesh(Mesh const& mesh, Matrix const& transform);

  LT_API Mesh AddQuad(uint i1, uint i2, uint i3, uint i4);

  LT_API Mesh AddQuadR(int i1, int i2, int i3, int i4);

  LT_API Mesh AddTriangle(uint i1, uint i2, uint i3);

  LT_API Mesh AddVertex(Vertex const& vertex);

  LT_API Mesh AddVertex(
    V3 const& position,
    V3 const& normal,
    float u,
    float v);

  LT_API Mesh Clear();

  LT_API Mesh Clone() const;

  /* Computation. */
  LT_API void ComputeDecomp(Vector<Mesh>& pieces) const;

  LT_API Mesh ComputeEdgeDistance(float edgeThresh);

  LT_API Mesh ComputeNormals();

  LT_API Mesh ComputePrincipleComponent() const;

  /* Queries. */
  LT_API uint* GetIndexPointer();
  LT_API uint const* GetIndexPointer() const;

  LT_API uint GetIndices() const;

  LT_API uint GetTriangles() const;

  LT_API Vertex* GetVertexPointer();
  LT_API Vertex const* GetVertexPointer() const;

  LT_API V3 const* GetVertexNormalPointer() const;

  LT_API float const* GetVertexTexCoordPointer() const;

  LT_API uint GetVertices() const;

  /* Geometric Operations. */
  LT_API Mesh RemoveDegeneracies(float minTriArea = 1e-5f) const;

  LT_API Mesh ReverseWinding();

  LT_API Mesh ShareVertices(float maxDistance = 1e-5f);

  LT_API Mesh SmoothNormals(float distanceFactor = 0.1f);

  LT_API Mesh UnshareVertices(float minDotProduct = 0.8f);

  /* Geometric Transformations. */
  LT_API Mesh Transform(Matrix const& m);

  LT_API Mesh Translate(V3 const& translation);
  LT_API Mesh TranslateToCenter();

  LT_API Mesh Rotate(V3 const& ypr);

  LT_API Mesh Scale(V3 const& scale);

  LT_API Mesh SetU(float u);
  LT_API Mesh SetV(float v);

  template <class VertexFNType>
  Mesh Map(VertexFNType& fn) {
    for (size_t i = 0; i < vertices.size(); ++i)
      fn(vertices[i]);
    return this;
  }
};

inline Mesh Mesh_Create() {
  return new MeshT;
}

#endif
