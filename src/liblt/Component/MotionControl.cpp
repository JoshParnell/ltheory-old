#include "MotionControl.h"
#include "Motion.h"

#include "Game/Messages.h"
#include "Game/Object.h"

#include "LTE/Math.h"
#include "LTE/StackFrame.h"

const float kTurnHardness = 4.0f;

namespace {
  inline double GetSDF(ComponentMotionControl* self, V3 const& p) {
    double sum = 0;
    for (size_t i = 0; i < self->elements.size(); ++i)
      sum -= (double)self->elements[i]->Evaluate(p);
    return sum;
  }
}

void ComponentMotionControl::Run(ObjectT* self, UpdateState& state) {
  AUTO_FRAME;
  if (!elements.size()) {
    self->Broadcast(MessageThrustLinear(0));
    return;
  }

  if (self->CanMove()) {
    Pointer<ComponentMotion> motion = self->GetMotion();
    V3D p = self->GetCenter() + motion->velocity / kLinearDrag;

    const double ds = 0.1;
    V3D grad = V3D(
      GetSDF(this, p + V3D(ds, 0, 0)) - GetSDF(this, p - V3D(ds, 0, 0)),
      GetSDF(this, p + V3D(0, ds, 0)) - GetSDF(this, p - V3D(0, ds, 0)),
      GetSDF(this, p + V3D(0, 0, ds)) - GetSDF(this, p - V3D(0, 0, ds))) / ds;

    V3 thrustDir = (V3)Normalize(grad);
    V3 look = self->GetLook();
    thrustDir *= Exp(-kTurnHardness * (1.0f - Saturate(Dot(thrustDir, look))));
    self->Broadcast(MessageThrustLinear(thrustDir));

    /* Directional piloting based on local frame.  Pitch and yaw computed
     * directly from local-space gradients. */ {
      V3 p = self->GetCenter() + motion->velocity / kLinearDrag;
      V3 up = self->GetUp();
      V3 right = self->GetRight();

      RotateBasis(right, up, look, motion->velocityA / kAngularDrag);
      double dYaw = GetSDF(this, (V3D)(p + right + look)) -
                    GetSDF(this, (V3D)(p - right + look));
      double dPitch = GetSDF(this, (V3D)(p + up + look)) -
                      GetSDF(this, (V3D)(p - up + look));
      V3 dAngle = 0.25f * (V3)Normalize(V3D(-dPitch, dYaw, 0));
      self->Broadcast(MessageThrustAngular(dAngle, 1.0f));
    }
  }

  elements.clear();
}
