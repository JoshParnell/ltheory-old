#include "Model.h"
#include "Bound.h"
#include "DrawState.h"
#include "Geometry.h"
#include "Math.h"
#include "Matrix.h"
#include "Mesh.h"
#include "Renderer.h"
#include "RenderStyle.h"
#include "ShaderInstance.h"
#include "Vector.h"

namespace {
  struct Entry {
    Geometry geometry;
    ShaderInstance shader;
    bool collidable;
  };
}

struct ModelData {
  Vector<Entry> entries;
  Bound3 cachedBox;
  short cachedBoxVersion;

  ModelData() :
    cachedBoxVersion(-1)
    {}
};

ModelT::ModelT() :
  d(*new ModelData),
  version(0)
  {}

ModelT::~ModelT() {
  delete &d;
}

Model ModelT::Add(
  Geometry const& geom,
  ShaderInstance const& shader,
  bool collidable)
{
  LTE_ASSERT(geom);
  d.entries.push(Entry());
  Entry& e = d.entries.back();
  e.geometry = geom;
  e.shader = shader;
  e.collidable = collidable;
  UpdateVersion();
  return this;
}

Model ModelT::Add(Model const& model) {
  LTE_ASSERT(model);
  for (size_t i = 0; i < model->d.entries.size(); ++i)
    d.entries.push(model->d.entries[i]);
  UpdateVersion();
  return this;
}

void ModelT::Render(DrawState* state) const {
  for (size_t i = 0; i < d.entries.size(); ++i)
    RenderPiece(i, state);
}

void ModelT::RenderPiece(size_t piece, DrawState* state) const {
  RenderStyle const& style = RenderStyle_Get();
  style->SetShader(d.entries[piece].shader);
  style->Render(d.entries[piece].geometry);
}

Bound3 ModelT::GetBound() const {
  LTE_ASSERT(d.entries.size());
  short currentVersion = GetVersion();
  if (d.cachedBoxVersion != currentVersion) {
    d.cachedBoxVersion = currentVersion;
    d.cachedBox = d.entries[0].geometry->GetBound();
    for (size_t i = 1; i < d.entries.size(); ++i)
      d.cachedBox.Union(d.entries[i].geometry->GetBound());
  }
  return d.cachedBox;
}

Mesh ModelT::GetCollisionMesh() const {
  if (d.entries.empty())
    return nullptr;
  Mesh collisionMesh;
  for (size_t i = 0; i < d.entries.size(); ++i) {
    if (!d.entries[i].collidable)
      continue;
    Mesh thisMesh = d.entries[i].geometry->GetCollisionMesh();
    if (thisMesh) {
      if (!collisionMesh)
        collisionMesh = Mesh_Create();
      collisionMesh->AddMesh(thisMesh);
    }
  }
  return collisionMesh;
}

bool ModelT::Intersects(Ray const& r, float* tOut, V3* normalOut) const {
  float t = FLT_MAX;
  for (size_t i = 0; i < d.entries.size(); ++i) {
    if (!d.entries[i].collidable)
      continue;
    float thisT;
    V3 thisNormal;
    if (d.entries[i].geometry->Intersects(r, &thisT, normalOut ? &thisNormal : nullptr)
        && thisT < t)
    {
      if (normalOut)
        *normalOut = thisNormal;
      t = thisT;
    }
  }

  if (tOut)
    *tOut = t;
  return t < FLT_MAX;
}

V3 ModelT::Sample() const {
  return d.entries.random().geometry->Sample();
}

Model ModelT::UpdateVersion()  {
  version = 0;
  for (size_t i = 0; i < d.entries.size(); ++i)
    version = (short)(version + d.entries[i].geometry->GetVersion());
  return this;
}
