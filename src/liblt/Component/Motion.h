#ifndef Component_Motion_h__
#define Component_Motion_h__

#include "Common.h"
#include "Orientation.h"
#include "Game/Item.h"
#include "LTE/AutoClass.h"
#include "LTE/V3.h"

const float kLinearDrag = 0.8f;
const float kAngularDrag = 2;

/* TODO : Reduce memory usage. */

AutoClass(ComponentMotion,
  V3, force,
  V3, torque,
  V3, velocity,
  V3, velocityA,
  Mass, mass,
  Mass, inertia,
  float, speed)

  ComponentMotion() :
    force(0),
    torque(0),
    velocity(0),
    velocityA(0),
    mass(1),
    inertia(1),
    speed(0)
    {}

  LT_API void Run(ObjectT* self, UpdateState& state);
};

AutoComponent(Motion)
  void OnUpdate(UpdateState& s) {
    Motion.Run(this, s);
    BaseT::OnUpdate(s);
  }

  void SetSupertype(Item const& type) {
    LTE_ASSERT(type->GetMass() > 0);
    Motion.mass = type->GetMass();
    Motion.inertia = type->GetInertia();

    BaseT::SetSupertype(type);
  }

  void ApplyForce(V3 const& force) {
    Motion.force += force;
  }

  void ApplyTorque(V3 const& torque) {
    Motion.torque += torque;
  }

  V3 GetAngularVelocity() const {
    return Motion.velocityA;
  }

  Mass GetMass() const {
    return Motion.mass;
  }

  float GetSpeed() const {
    return Motion.speed;
  }

  V3 GetVelocity() const {
    return Motion.velocity;
  }

  V3 GetVelocityA() const {
    return Motion.velocityA;
  }

  float GetTopSpeed() const {
    return this->GetMaxThrust() / (kLinearDrag * Motion.mass);
  }

  bool IsMovable() const {
    return true;
  }
};

#endif
