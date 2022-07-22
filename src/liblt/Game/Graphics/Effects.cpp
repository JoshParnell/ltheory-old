#include "Effects.h"

#include "Component/Drawable.h"

#include "Game/Particles.h"
#include "Game/Objects.h"

#include "Module/SoundEngine.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"
#include "LTE/Matrix.h"
#include "LTE/ParticleSystem.h"
#include "LTE/Transform.h"
#include "LTE/Vector.h"

void Effect_BeamHit(
  Position const& origin,
  V3 const& baseVelocity,
  float scale,
  V3 const& color)
{
  V3 velocity = baseVelocity + scale * SampleSphere();
  float life = Rand(1.0f, 2.0f);
  ParticleSystem_Add(
    ParticleSystem_Get(),
    Particle_Firefly(),
    origin,
    velocity,
    Rand(0.5f, 1) * scale,
    life,
    color);
}

void Effect_ParticleFirefly(
  Position const& origin,
  V3 const& velocity,
  V3 const& color,
  float size,
  float lifeTime)
{
  ParticleSystem_Add(
    ParticleSystem_Get(),
    Particle_Firefly(),
    origin,
    velocity,
    size,
    lifeTime,
    color);
}

void Effect_MultiExplosionRadial(
  Object const& object,
  float scale,
  ExplosionType type)
{
  ComponentDrawable* d = object->GetDrawable();
  scale *= Length(object->GetExtent());

  for (int i = 0; i < 15; ++i) {
    float age = -Pow(RandExp(), 1.5f);
    float duration = Rand(1.0f, 5.0f);

    Object e = Object_Explosion(type, age, duration);
    /* CRITICAL : Negative scale o.o */
    object->Attach(e, Transform_ST(-Sqrt(RandExp()), d->renderable()->Sample()));
  }

  if (type == ExplosionType_Plasma)
    object->GetContainer()->AddInterior(
      Object_SoundEmitter("shield/explosion.ogg",
                          object->GetPos(), 1, scale / 15));
  else if (type == ExplosionType_Fire)
    object->GetContainer()->AddInterior(
      Object_SoundEmitter("explosion/altsmall3.ogg",
                          object->GetPos(), 0.5f, scale / 15));

}

void Effect_SmallPlume(
  Position const& origin,
  V3 const& baseVelocity,
  V3 const& color,
  float size)
{
  ParticleSystem const& ps = ParticleSystem_Get();
  for (size_t i = 0; i < 15; ++i) {
    V3 p = origin + 0.5f * size * SampleSphere();
    V3 v = baseVelocity + 8.0f * size * SampleSphere();
    V3 c = color * RandV3(1.0f, 1.2f);
    float life = Rand(0.25f, 0.5f);

    ParticleSystem_Add(
      ps,
      Particle_Fire(),
      p,
      v,
      (1.0f + RandExp()) * size,
      life,
      c);
  }
}
