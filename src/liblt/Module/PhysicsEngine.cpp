#include "PhysicsEngine.h"

#include "Game/Object.h"

#include "Module/FrameTimer.h"

#include "LTE/CollisionMesh.h"
#include "LTE/HashMap.h"
#include "LTE/Matrix.h"
#include "LTE/Mesh.h"
#include "LTE/Renderable.h"
#include "LTE/StackFrame.h"
#include "LTE/Transform.h"
#include "LTE/Vector.h"

/* TODO : Should multithread the creation of collision meshes. */

const float kEvictionTime = 120;

namespace {
  Vector<PhysicsEngine*> gPhysicsEngineStack;
}

PhysicsEngine::PhysicsEngine() {
  if (gPhysicsEngineStack.empty())
    Push();
}

PhysicsEngine::~PhysicsEngine() {
  if (gPhysicsEngineStack.back() == this)
    Pop();
}

PhysicsEngine* GetPhysicsEngine() {
  LTE_ASSERT(!gPhysicsEngineStack.empty());
  return gPhysicsEngineStack.back();
}

void PhysicsEngine::Pop() {
  LTE_ASSERT(!gPhysicsEngineStack.empty());
  LTE_ASSERT(gPhysicsEngineStack.back() == this);
  gPhysicsEngineStack.pop();
}

void PhysicsEngine::Push() {
  LTE_ASSERT(gPhysicsEngineStack.size() < 100);
  gPhysicsEngineStack << this;
}

struct PhysicsEngineImpl : public PhysicsEngine {
  struct Entry {
    CollisionMesh mesh;
    float lastUse;

    Entry() :
      lastUse(0)
      {}
  };

  typedef HashMap<size_t, Entry> MeshMapT;
  MeshMapT meshes;

  bool CheckCollision(
    ObjectT* object1,
    ObjectT* object2,
    V3* contactNormal)
  {
    AUTO_FRAME;
    CollisionMesh const& mesh1 = GetCollisionMesh(object1->GetRenderable());
    CollisionMesh const& mesh2 = GetCollisionMesh(object2->GetRenderable());

    if (!mesh1 || !mesh2)
      return false;

    Matrix const& world1 = object1->GetTransform().GetMatrix();
    Matrix const& world2 = object2->GetTransform().GetMatrix();
    return mesh1->Intersects(mesh2, world1, world2, contactNormal);
  }

  CollisionMesh const& GetCollisionMesh(Renderable const& renderable) {
    size_t id = renderable->GetHash();
    if (!meshes[id].mesh) {
      Mesh source = renderable->GetCollisionMesh();
      if (source)
        FRAME("Build Collision Mesh")
          meshes[id].mesh = CollisionMesh_Create(source);
    }

    Entry& e = meshes[id];
    e.lastUse = 0;
    return e.mesh;
  }

  char const* GetName() const {
    return "PhysicsEngine";
  }

  bool Raycast(
    WorldRay const& ray,
    ObjectT* object,
    float tMax,
    float& tOut,
    V3* normalOut)
  {
    CollisionMesh const& mesh = GetCollisionMesh(object->GetRenderable());
    Matrix const& matrix = object->GetTransform().GetMatrix();
    return mesh ? mesh->Intersects(ray, matrix, tMax, tOut, normalOut) : false;
  }

  void Update() {
    SFRAME("Physics Update");
    float dt = FrameTimer_Get();
    for (MeshMapT::iterator it = meshes.begin(); it != meshes.end(); ++it) {
      if ((it->second.lastUse += dt) >= kEvictionTime) {
        it = meshes.erase(it);
        if (it == meshes.end())
          break;
      }
    }
  }
};

struct PhysicsEngineNullImpl : public PhysicsEngine {
  bool CheckCollision(
    ObjectT* e1,
    ObjectT* e2,
    V3* contactNormal)
  {
    return false;
  }

  bool Raycast(
    WorldRay const& ray,
    ObjectT* e,
    float tMax,
    float& tOut,
    V3* normalOut)
  {
    return false;
  }

  char const* GetName() const {
    return "PhysicsEngineNull";
  }

  void Update() {}
};

PhysicsEngine* CreatePhysicsEngine() {
  return new PhysicsEngineImpl;
}

PhysicsEngine* CreatePhysicsEngineNull() {
  return new PhysicsEngineNullImpl;
}
