#include "Collidable.h"
#include "Motion.h"

#include "Game/Object.h"

#include "LTE/Debug.h"
#include "LTE/Math.h"
#include "LTE/StackFrame.h"

void ComponentMotion::Run(ObjectT* self, UpdateState& state) { AUTO_FRAME;
  LTE_ASSERT(force.IsFinite());
  LTE_ASSERT(torque.IsFinite());

  /* Notify the collidable component that we're not a passive object, since
     we've got the ability to move. */
  ComponentCollidable* c = self->GetCollidable();
  if (c)
    c->passive = false;

  /* Apply drag. */
  force -= velocity * (mass * kLinearDrag);
  torque -= velocityA * (inertia * kAngularDrag);

  velocity += force * (state.dt / (float)mass);

  /* Note that this should actually be inertial tensor, not mass. But
   * for now this will do as an approximation. */
  velocityA += torque * (state.dt / (float)inertia);

  const float tolerance = 0;

  ComponentOrientation* ori = self->GetOrientation();
  if (Abs(velocity).GetMax() > tolerance)
    ori->GetTransformW().pos += (Position)velocity * state.dt;
  
  if (Abs(velocityA).GetMax() > tolerance)
    ori->Rotate(velocityA * state.dt);

  force = 0;
  torque = 0;
  speed = Length(velocity);

  LTE_ASSERT(velocity.IsFinite());
  LTE_ASSERT(velocityA.IsFinite());
}

VoidFreeFunction(Object_ApplyForce,
  "Apply 'force' to 'object'",
  Object, object,
  V3, force)
{
  return object->ApplyForce(force);
} FunctionAlias(Object_ApplyForce, ApplyForce);

FreeFunction(Mass, Object_GetMass,
  "Return the mass of 'object'",
  Object, object)
{
  return object->GetMass();
} FunctionAlias(Object_GetMass, GetMass);

FreeFunction(float, Object_GetSpeed,
  "Return the current speed of 'object'",
  Object, object)
{
  return object->GetSpeed();
} FunctionAlias(Object_GetSpeed, GetSpeed);

FreeFunction(float, Object_GetTopSpeed,
  "Return the top speed of 'object' under normal propulsion",
  Object, object)
{
  return object->GetTopSpeed();
} FunctionAlias(Object_GetTopSpeed, GetTopSpeed);

FreeFunction(V3, Object_GetVelocity,
  "Return the current velocity of 'object'",
  Object, object)
{
  return object->GetVelocity();
} FunctionAlias(Object_GetVelocity, GetVelocity);

FreeFunction(V3, Object_GetVelocityAngular,
  "Return the current angular velocity of 'object'",
  Object, object)
{
  return object->GetVelocityA();
} FunctionAlias(Object_GetVelocityAngular, GetVelocityAngular);

VoidFreeFunction(Object_SetMass,
  "Set the mass of 'object' to 'mass'",
  Object, object,
  Mass, mass)
{
  if (object->GetMotion())
    object->GetMotion()->mass = mass;
} FunctionAlias(Object_SetMass, SetMass);

AutoClass(Impact,
  float, t,
  Position, position)
  Impact() {}
};

FreeFunction(Impact, GetImpact,
  "Compute the time and location of impact for a munition with 'speed' going from 'source' to 'dest'",
  Position, sourcePos,
  Position, destPos,
  V3, sourceVelocity,
  V3, destVelocity,
  float, speed)
{
  Position hitPoint;
  float t = ComputeImpact(sourcePos, destPos, sourceVelocity, destVelocity, speed, &hitPoint);
  return Impact(t, hitPoint);
}
