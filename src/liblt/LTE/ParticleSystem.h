#ifndef LTE_ParticleSystem_h__
#define LTE_ParticleSystem_h__

#include "BaseType.h"
#include "DeclareFunction.h"
#include "Reference.h"
#include "V3.h"

struct ParticleSystemT : public RefCounted {
  BASE_TYPE(ParticleSystemT)

  virtual void Draw(DrawState* state) const = 0;
  virtual void Run(float dt) = 0;
};

DeclareFunction(ParticleSystem_Add, void,
  ParticleSystem, particleSystem,
  ShaderInstance, particle,
  V3D, position,
  V3, velocity,
  float, scale,
  float, life,
  V3, attribute)

DeclareFunctionNoParams(ParticleSystem_Create, ParticleSystem)

DeclareFunctionNoParams(ParticleSystem_Get, ParticleSystem)

DeclareFunctionNoParams(ParticleSystem_Pop, void)

DeclareFunction(ParticleSystem_Push, void,
  ParticleSystem, ps)

#endif
