#include "Mesh.h"

#include "Array.h"
#include "Bound.h"
#include "GL.h"
#include "Math.h"
#include "Matrix.h"
#include "Program.h"
#include "Ray.h"
#include "Renderer.h"
#include "Sphere.h"

#include "ThirdParty/KDTree.h"

namespace {
  void CreateAdjacencyTable(MeshT const* mesh, Array<Vector<uint> >& table) {
    table.resize(mesh->vertices.size());
    for (size_t i = 0; i < mesh->indices.size(); i += 3)
    for (size_t j = 0; j < 3; ++j) {
      uint i1 = mesh->indices[i + j];
      uint i2 = mesh->indices[i + (j + 1) % 3];
      if (!table[i1].contains(i2))
        table[i1].push(i2);
      if (!table[i2].contains(i1))
        table[i2].push(i1);
    }
  }
}

MeshT::~MeshT() {
  if (!Program_InStaticSection()) {
    if (vbo != GL_NullBuffer)
      GL_DeleteBuffer(vbo);
    if (ibo != GL_NullBuffer)
      GL_DeleteBuffer(ibo);
  }
}

void MeshT::Draw() const {
  Renderer_DrawMesh(this);
}

Bound3 MeshT::GetBound() const {
  if (!vertices.size())
    return Bound3(V3(0));
  Bound3 box(vertices[0].p);
  for (size_t i = 1; i < vertices.size(); ++i)
    box.AddPoint(vertices[i].p);
  return box;
}

Mesh MeshT::GetCollisionMesh() const {
  return Clone();
}

short MeshT::GetVersion() const {
  return version;
}

bool MeshT::Intersects(
  Ray const& r,
  float* tOut,
  V3* normalOut,
  V2* uvOut) const
{
  bool hit = false;
  float t = FLT_MAX;

  /* OPTIMIZATION : Lazy computation of normalOut && uvOut */
  for (uint i = 0; i < indices.size(); i += 3) {
    uint i1 = indices[i + 0];
    uint i2 = indices[i + 1];
    uint i3 = indices[i + 2];
    float thisT;
    if (r.IntersectsTri(vertices[i1].p, vertices[i2].p, vertices[i3].p,
                        thisT) && thisT < t) {
      t = thisT;
      if (normalOut)
        *normalOut = Normalize(Cross(vertices[i3].p - vertices[i1].p,
                                     vertices[i2].p - vertices[i1].p));

      if (uvOut) {
        V3 p = r(t);
        V3 e1 = p - vertices[i1].p;
        V3 e2 = p - vertices[i2].p;
        V3 e3 = p - vertices[i3].p;
        float a1 = Length(Cross(e2, e3));
        float a2 = Length(Cross(e1, e3));
        float a3 = Length(Cross(e1, e2));
        *uvOut =
          (a1 * vertices[i1].UV() +
           a2 * vertices[i2].UV() +
           a3 * vertices[i3].UV()) / (a1 + a2 + a3);
      }

      hit = true;
    }
  }

  if (tOut)
    *tOut = t;
  return hit;
}

V3 MeshT::Sample() const {
  return vertices.size() ? vertices.random().p : V3(0);
}

Mesh MeshT::AddMesh(Mesh const& mesh) {
  return AddMesh(mesh, Matrix::Identity());
}

Mesh MeshT::AddMesh(Mesh const& mesh, Matrix const& m) {
  Matrix it = m.Inverse().Transpose();
  uint indexOffset = vertices.size();

  // vertices.reserve(vertices.size() + mesh->vertices.size());
  for (uint i = 0; i < mesh->vertices.size(); ++i) {
    const Vertex& v = mesh->vertices[i];
    V3 newPos = m.TransformPoint(v.p);
    V3 newNormal = it.TransformVector(v.n);
    AddVertex(newPos, newNormal, v.u, v.v);
  }

  // indices.reserve(indices.size() + mesh->indices.size());
  for (size_t i = 0; i < mesh->indices.size(); ++i)
    indices.push(mesh->indices[i] + indexOffset);
  version++;
  return this;
}

Mesh MeshT::AddQuad(uint i1, uint i2, uint i3, uint i4) {
  indices.push(i1);
  indices.push(i2);
  indices.push(i3);
  indices.push(i1);
  indices.push(i3);
  indices.push(i4);
  version++;
  return this;
}

Mesh MeshT::AddQuadR(int i1, int i2, int i3, int i4) {
  size_t offset = vertices.size();
  return AddQuad(offset + i1, offset + i2, offset + i3, offset + i4);
}

Mesh MeshT::AddTriangle(uint i1, uint i2, uint i3) {
  indices.push(i1);
  indices.push(i2);
  indices.push(i3);
  version++;
  return this;
}

Mesh MeshT::AddVertex(const Vertex& vertex) {
  vertices.push(vertex);
  version++;
  return this;
}

Mesh MeshT::AddVertex(
  V3 const& position,
  V3 const& normal,
  float u,
  float v)
{
  vertices.push(Vertex(position, normal, u, v));
  version++;
  return this;
}

Mesh MeshT::Clear() {
  vbo = GL_NullBuffer;
  ibo = GL_NullBuffer;
  version = 0;
  vertices.clear();
  indices.clear();
  return this;
}

Mesh MeshT::Clone() const {
  Mesh m = Mesh_Create();
  m->vertices = vertices;
  m->indices = indices;
  return m;
}

Mesh MeshT::ComputeNormals() {
  for (size_t i = 0; i < vertices.size(); ++i)
    vertices[i].n = V3(0);

  for (size_t i = 0; i < indices.size(); i += 3) {
    Vertex& v1 = vertices[indices[i + 0]];
    Vertex& v2 = vertices[indices[i + 1]];
    Vertex& v3 = vertices[indices[i + 2]];
    V3 e1 = v2.p - v1.p;
    V3 e2 = v3.p - v1.p;
    V3 n = Cross(e2, e1);
    v1.n += n;
    v2.n += n;
    v3.n += n;
  }

  for (size_t i = 0; i < vertices.size(); ++i)
    vertices[i].n = Normalize(vertices[i].n);
  return this;
}

void MeshT::ComputeDecomp(Vector<Mesh>& pieces) const {
  Array<Vector<uint> > adjTable;
  CreateAdjacencyTable(this, adjTable);

  std::vector<bool> visited(vertices.size(), false);
  Vector<int> newIndex(vertices.size(), -1);
  Vector<int> pieceIndex(vertices.size(), -1);

  size_t visitedCount = 0;
  while (visitedCount < vertices.size()) {
    Vector<size_t> vertexStack;
    pieces.push(Mesh_Create());
    for (size_t i = 0; i < visited.size(); ++i) {
      if (!visited[i]) {
        vertexStack.push(i);
        visited[i] = true;
        break;
      }
    }

    while (!vertexStack.empty()) {
      size_t vertex = vertexStack.back();
      vertexStack.pop();
      visitedCount++;

      newIndex[vertex] = pieces.back()->vertices.size();
      pieceIndex[vertex] = pieces.size() - 1;
      pieces.back()->vertices.push(vertices[vertex]);

      for (size_t i = 0; i < adjTable[vertex].size(); ++i) {
        size_t other = adjTable[vertex][i];
        if (visited[other])
          continue;
        vertexStack.push(other);
        visited[other] = true;
      }
    }
  }

  for (size_t i = 0; i < indices.size(); i += 3) {
    uint i0 = indices[i + 0];
    uint i1 = indices[i + 1];
    uint i2 = indices[i + 2];
    pieces[pieceIndex[i0]]->AddTriangle(newIndex[i0], newIndex[i1], newIndex[i2]);
  }
}

Mesh MeshT::ComputeEdgeDistance(float edgeThresh) {
  Array<Vector<uint> > adjTable;
  CreateAdjacencyTable(this, adjTable);

  for (size_t i = 0; i < vertices.size(); ++i)
    vertices[i].v = 1000;

  for (size_t i = 0; i < vertices.size(); ++i) {
    Vertex& self = vertices[i];
    float deviation = 0;

    for (size_t j = 0; j < adjTable[i].size(); ++j) {
      Vertex& other = vertices[adjTable[i][j]];
      deviation = Max(deviation, 1.f - Abs(Dot(self.n, other.n)));
    }

    if (deviation > edgeThresh)
      vertices[i].v = 0;
  }

  for (size_t iter = 0; iter < Min((size_t)10, vertices.size()); ++iter) {
    for (size_t i = 0; i < vertices.size(); ++i) {
      Vertex& self = vertices[i];
      for (size_t j = 0; j < adjTable[i].size(); ++j) {
        Vertex& other = vertices[adjTable[i][j]];
        self.v = Min(self.v, other.v + Length(other.p - self.p));
      }
    }
  }

  return this;
}

Mesh MeshT::ComputePrincipleComponent() const {
  Vector<Mesh> pieces;
  ComputeDecomp(pieces);

  if (!pieces.size())
    return nullptr;

  size_t maxIndex = 0;
  size_t maxSize = pieces[0]->vertices.size();
  for (size_t i = 1; i < pieces.size(); ++i) {
    if (pieces[i]->vertices.size() > maxSize) {
      maxSize = pieces[i]->vertices.size();
      maxIndex = i;
    }
  }

  return pieces[maxIndex];
}

uint* MeshT::GetIndexPointer() {
  return indices.data();
}

uint const* MeshT::GetIndexPointer() const {
  return indices.data();
}

uint MeshT::GetIndices() const {
  return (uint)indices.size();
}

uint MeshT::GetTriangles() const {
  return (uint)indices.size() / 3;
}

Vertex* MeshT::GetVertexPointer() {
  return vertices.data();
}

Vertex const* MeshT::GetVertexPointer() const {
  return vertices.data();
}

V3 const* MeshT::GetVertexNormalPointer() const {
  return &GetVertexPointer()->n;
}

float const* MeshT::GetVertexTexCoordPointer() const {
  return &GetVertexPointer()->u;
}

uint MeshT::GetVertices() const {
  return (uint)vertices.size();
}

Mesh MeshT::RemoveDegeneracies(float minTriArea) const {
  Mesh m = Mesh_Create();
  m->vertices = vertices;
  for (size_t i = 0; i < indices.size(); i += 3) {
    uint i0 = indices[i + 0];
    uint i1 = indices[i + 1];
    uint i2 = indices[i + 2];
    const V3& p0 = vertices[i0].p;
    const V3& p1 = vertices[i1].p;
    const V3& p2 = vertices[i2].p;
    float area = Length(Cross(p2 - p1, p0 - p1));
    if (area > minTriArea) {
      m->indices.push(i0);
      m->indices.push(i1);
      m->indices.push(i2);
    }
  }
  return m;
}

Mesh MeshT::ReverseWinding() {
  for (uint i = 0; i < indices.size(); i += 3) {
    uint i2 = indices[i + 1];
    uint i3 = indices[i + 2];
    indices[i + 1] = i3;
    indices[i + 2] = i2;
  }
  return this;
}

Mesh MeshT::ShareVertices(float maxDistance) {
  Vector<int> indexArray;
  for (uint i = 0; i < vertices.size(); ++i)
    indexArray.push(i);

  Vector<int> forward;
  forward.resize(vertices.size(), -1);

  kdtree* tree = kd_create(3);
  for (uint i = 0; i < vertices.size(); ++i) {
    const V3& p = vertices[i].p;
    kdres* result = kd_nearest3f(tree, p.x, p.y, p.z);
    if (result) {
      int index = *(int*)kd_res_item_data(result);
      if (Length(p - vertices[index].p) <= maxDistance)
        forward[i] = index;
      kd_res_free(result);
    }
    kd_insert3f(tree, p.x, p.y, p.z, &indexArray[i]);
  }
  kd_free(tree);

  Vector<int> indexMap;
  indexMap.resize(vertices.size(), -1);

  Vector<Vertex> newVertices;

  /* Map all parent (non-shared) vertices. */
  for (uint i = 0; i < vertices.size(); ++i) {
    if (forward[i] == -1) {
      indexMap[i] = newVertices.size();
      newVertices.push(vertices[i]);
    }
  }

  /* Map all linked (shared) vertices. */
  for (uint i = 0; i < vertices.size(); ++i) {
    int forwardIndex = forward[i];
    if (forwardIndex == -1)
      continue;

    int parentIndex;
    while (forwardIndex != -1) {
      parentIndex = forwardIndex;
      forwardIndex = forward[forwardIndex];
    }
    indexMap[i] = indexMap[parentIndex];
  }

  vertices = newVertices;

  /* Remap the indices into new vertex space. */
  for (uint i = 0; i < indices.size(); ++i)
    indices[i] = indexMap[indices[i]];
  version++;
  return this;
}

Mesh MeshT::SmoothNormals(float distanceFactor) {
  Array<V3> normalContrib(vertices.size());
  for (size_t i = 0; i < vertices.size(); ++i)
    normalContrib[i] = 0;

  for (size_t i = 0; i < indices.size(); i += 3)
  for (size_t j = 0; j < 3; ++j) {
    uint i0 = indices[i + j];
    uint i1 = indices[i + (j + 1) % 3];
    Vertex& v0 = vertices[i0];
    Vertex& v1 = vertices[i1];
    float factor = Exp(-Length(v0.p - v1.p) / distanceFactor);
    normalContrib[i0] += v1.n * factor;
    normalContrib[i1] += v0.n * factor;
  }

  for (size_t i = 0; i < vertices.size(); ++i)
    vertices[i].n = Normalize(vertices[i].n + normalContrib[i]);
  return 0;
}

Mesh MeshT::UnshareVertices(float minDot) {
  for (size_t i = 0; i < vertices.size(); ++i)
    vertices[i].n = V3(0);

  for (size_t i = 0; i < indices.size(); i += 3) {
    uint* index[3] = {
      &indices[i + 0],
      &indices[i + 1],
      &indices[i + 2],
    };

    Vertex* v[3] = {
      &vertices[*index[0]],
      &vertices[*index[1]],
      &vertices[*index[2]],
    };

    V3 faceNormal = Cross(v[2]->p - v[0]->p, v[1]->p - v[0]->p);
    for (size_t j = 0; j < 3; ++j) {
      V3& curNormal = vertices[*index[j]].n;
      if (LengthSquared(curNormal) > 0.f) {
        float cDot = Dot(Normalize(faceNormal), Normalize(curNormal));
        if (cDot < minDot) {
          vertices.push(vertices[*index[j]]);
          *index[j] = vertices.size() - 1;
          vertices.back().n = faceNormal;
        } else
          curNormal += faceNormal;
      } else
        curNormal += faceNormal;
    }
  }

  for (size_t i = 0; i < vertices.size(); ++i)
    vertices[i].n = Normalize(vertices[i].n);
  return this;
}

Mesh MeshT::Transform(const Matrix& m) {
  Matrix it = m.Inverse().Transpose();
  for (size_t i = 0; i < vertices.size(); ++i) {
    vertices[i].p = m.TransformPoint(vertices[i].p);
    vertices[i].n = Normalize(it.TransformVector(vertices[i].n));
  }
  version++;
  return this;
}

Mesh MeshT::Translate(V3 const& translation) {
  return Transform(Matrix::Translation(translation));
}

Mesh MeshT::TranslateToCenter() {
  return Translate(-GetBound().GetCenter());
}

Mesh MeshT::Scale(V3 const& scale) {
  return Transform(Matrix::Scale(scale));
}

Mesh MeshT::Rotate(V3 const& ypr) {
  return Transform(Matrix::RotationYPR(ypr));
}

Mesh MeshT::SetU(float u) {
  for (size_t i = 0; i < vertices.size(); ++i)
    vertices[i].u = u;
  return this;
}

Mesh MeshT::SetV(float v) {
  for (size_t i = 0; i < vertices.size(); ++i)
    vertices[i].v = v;
  return this;
}
