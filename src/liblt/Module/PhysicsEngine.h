#ifndef Module_PhysicsEngine_h__
#define Module_PhysicsEngine_h__

#include "Common.h"
#include "LTE/Module.h"
#include "Game/Common.h"

struct PhysicsEngine : public ModuleT {
  LT_API PhysicsEngine();
  LT_API virtual ~PhysicsEngine();

  LT_API void Push();
  LT_API void Pop();

  virtual bool CheckCollision(
    ObjectT* object1,
    ObjectT* object2,
    V3* contactNormal = nullptr) = 0;

  virtual bool Raycast(
    WorldRay const& ray,
    ObjectT* object,
    float tMax,
    float& tOut,
    V3* normalOut = nullptr) = 0;
};

LT_API PhysicsEngine* CreatePhysicsEngine();
LT_API PhysicsEngine* CreatePhysicsEngineNull();
LT_API PhysicsEngine* GetPhysicsEngine();

#endif
