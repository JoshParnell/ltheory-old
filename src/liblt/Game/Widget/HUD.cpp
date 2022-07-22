
#include "../Widgets.h"

#include "Component/Motion.h"
#include "Component/Sockets.h"
#include "Component/Targets.h"

#include "Game/Camera.h"
#include "Game/Messages.h"
#include "Game/Player.h"

#include "Module/FrameTimer.h"
#include "Module/Settings.h"
#include "Module/SoundEngine.h"

#include "LTE/Axis.h"
#include "LTE/Bound.h"
#include "LTE/Buttons.h"
#include "LTE/Keyboard.h"
#include "LTE/Math.h"
#include "LTE/Mouse.h"
#include "LTE/Pool.h"
#include "LTE/Smooth.h"
#include "LTE/View.h"
#include "LTE/Viewport.h"

#include "UI/Widget.h"

const float kControlDeadZone  = 0.2f;
const float kControlPower = 2.0f;
const float kRigidity = 6;
const float kSensitivity = 1.5f;
const float kShakeStrength = 0.0f;

namespace {
  AutoClassDerived(ShipAffector, Affector,
    V3, thrust,
    V3, rotation,
    Smooth<V3>, lookNoise,
    Smooth<V3>, lookTarget,
    Smooth<V3>, upTarget,
    int, viewpoint,
    float, camDistance,
    bool, cruise,
    bool, isFiring)
    DERIVED_TYPE_EX(ShipAffector)

    ShipAffector() :
      thrust(0),
      rotation(0),
      lookNoise(0),
      lookTarget(V3(0, 0, 1)),
      upTarget(V3(0, 1, 0)),
      viewpoint(1),
      camDistance(2),
      cruise(false),
      isFiring(false)
    {
      life = FLT_MAX;
    }

    void OnRun(ObjectT* self, float dt) {
      lookNoise.Update(2.0f * dt);
      lookTarget.Update(kRigidity * dt);
      upTarget.Update(kRigidity * dt);
      lookTarget.value = Normalize(lookTarget.value);
      upTarget.value = Normalize(upTarget.value);

      ObjectT* o = ((PlayerT*)self)->piloting;
      UpdateCamera(o, dt);
      if (!o->IsMovable() || !o->CanMove())
        return;

      /* Autopilot overrides manual control. */
      if (o->GetCurrentTask())
        return;

      o->Broadcast(MessageThrustLinear(thrust));
      ComponentMotion* motion = o->GetMotion();

      Transform const& transform = o->GetTransform();
      V3 thrustA = transform.InverseDir(Cross(transform.look, lookTarget.value));
      thrustA.z = Dot(transform.look, Cross(transform.up, upTarget));
      thrustA -= o->GetVelocityA() / kAngularDrag;

      float maxTorque = o->GetMaxTorque();
      float limit = dt * maxTorque / motion->inertia;
      if (limit > 0)
        o->Broadcast(MessageThrustAngular(thrustA, Length(thrustA) / limit));

      /* Broadcast messages. */ {
        if (isFiring)
          o->Broadcast(MessageFire());
        if (cruise)
          o->Broadcast(MessageCruise());
      }
    }

    void UpdateCamera(ObjectT* self, float dt) {
      Camera cam = Camera_Get();
      Position pos = 0;

      float rigidity = 0.25f;
     
      if (viewpoint == 0) {
        float radius = camDistance * self->GetRadius();
        pos -= radius * lookTarget;
        pos += 0.2f * radius * upTarget.value;//self->GetUp();
        rigidity = 4.0f;
      } else if (viewpoint == 1) {
        pos += self->GetRadius() * lookTarget;
        rigidity = 1000000.0f;
      }

      V3 look = lookTarget.target;
      V3 up = upTarget.target;
      V3 right = Cross(up, look);

      RotateBasis(right, up, look, dt * rotation);
      lookTarget.target = look;
      upTarget.target = up;

      float shake = 1.0f - Exp(-self->GetSpeed() / 1000.0f);
      lookNoise.target = kShakeStrength * shake * SampleSphere();

      cam->SetRelativePos(pos);
      cam->SetRelativeLookAt(pos + self->GetRadius() * (lookTarget.value + lookNoise.value) * 1000.0);
      cam->SetRelativeUp(upTarget);
      cam->SetRigidity(rigidity);
    }
  };

  AutoClassDerived(WidgetHUD, WidgetComponentT,
    Player, self)
    Pointer<ShipAffector> controller;
    DERIVED_TYPE_EX(WidgetHUD)

    WidgetHUD() {}

    ~WidgetHUD() {
      controller->deleted = true;
    }

    void GetName(Widget const&, String& out) {
      out = "HUD";
    }

    void PostUpdate(Widget const&) {
      if (controller)
        controller->thrust = 0;

      if (!self->piloting) return;

      static Axis yawAxisJoy = Axis_Product(
        Axis_RightStickX()->SetDeadZone(kControlDeadZone)->SetPower(kControlPower),
        Axis_Button(Button_LeftStick(), true));
      static Axis pitchAxisJoy = Axis_RightStickY()
        ->SetDeadZone(kControlDeadZone)->SetPower(kControlPower);
      static Axis rollAxisJoy = Axis_Product(
        Axis_RightStickX()->SetDeadZone(kControlDeadZone)->SetPower(kControlPower),
        Axis_Button(Button_LeftStick())->Invert());

      static GenericButton gBoostButton =
        Settings_Button("Control/Piloting/Use Boost Capacitor",
          Button_Or(Button_RightStick(), Button_Key(Key_Tab)));

      static GenericButton gCruiseButton = 
        Settings_Button("Control/Piloting/Toggle Cruise",
          Button_AndControl(Button_Key(Key_W)));
      static GenericButton gReloadButton =
        Settings_Button("Control/Piloting/Reload", Button_Key(Key_F));

      static GenericButton gLaunchButton =
        Settings_Button("Control/Piloting/Launch Drone", Button_Key(Key_T));

      static GenericButton gFireButton =
        Settings_Button("Control/Piloting/Fire Weapons", 
          Button_Or(
            Button_Mouse(MouseButton_Right),
            Button_Axis(Axis_RightTrigger(), 1.0f)));

      static GenericButton gForwardButton =
        Settings_Button("Control/Piloting/Thrust Forward", Button_Key(Key_W));
      static GenericButton gBackwardButton =
        Settings_Button("Control/Piloting/Thrust Backward", Button_Key(Key_S));
      static GenericButton gLeftButton =
        Settings_Button("Control/Piloting/Strafe Left", Button_Key(Key_A));
      static GenericButton gRightButton =
        Settings_Button("Control/Piloting/Strafe Right", Button_Key(Key_D));
      static GenericButton gRollCCWButton = 
        Settings_Button("Control/Piloting/Roll CCW", Button_Key(Key_Q));
      static GenericButton gRollCWButton = 
        Settings_Button("Control/Piloting/Roll CW", Button_Key(Key_E));

      static GenericButton gViewpointButton =
        Settings_Button("Control/Piloting/Change Viewpoint",
          Button_Mouse(MouseButton_Middle));

      if (!controller) {
        controller = new ShipAffector;
        controller->lookTarget = self->piloting->GetLook();
        controller->upTarget = self->piloting->GetUp();
        self->GetAffectable()->affectors.push(controller.t);
      }

      controller->isFiring = false;
      controller->rotation = 0;

      if (gRollCCWButton()->Down())
        controller->rotation.z += 1.0f;
      if (gRollCWButton()->Down())
        controller->rotation.z -= 1.0f;

      if (Keyboard_Down(Key_Space)) {
        V2 offset = Viewport_Get()->ToNDC(Mouse_GetPos());
        offset =
          Normalize(offset) *
          Pow(Saturate(2.0f * (Length(offset) - 0.2f) / (1.0f - 0.2f)), 2.0f);
        controller->rotation.y += offset.x;
        controller->rotation.x -= offset.y;

        if (gFireButton()->Down())
          controller->isFiring = true;
      }

#if 0
      controller->rotation.x += pitchAxisJoy->Get();
      controller->rotation.y += yawAxisJoy->Get();
      controller->rotation.z += rollAxisJoy->Get();
#endif
      controller->rotation *= kSensitivity;

      /* Camera. */ {
        Camera_Get()->SetTarget(self->piloting);
        if (gViewpointButton()->Pressed())
          controller->viewpoint = (controller->viewpoint + 1) % 2;
        controller->camDistance -= 0.5f * Mouse_GetScrollDelta();
        controller->camDistance = Clamp(controller->camDistance, 1.5f, 8.0f);
      }
      
      if (self->piloting->GetCurrentTask())
        return;

      if (gCruiseButton()->Pressed()) {
        Sound_Play2D("ui/cruise_engage.wav");
        controller->cruise = !controller->cruise;
      }

      if (gReloadButton()->Pressed())
        self->piloting->Broadcast(MessageReload());
      if (gBoostButton()->Down())
        self->piloting->Broadcast(MessageBoost());

      /* Motion. */
      if (self->piloting->CanMove()) {
        if (gLaunchButton()->Down())
          self->piloting->Broadcast(MessageLaunch());

        V3 look = self->piloting->GetLook();
        V3 right = self->piloting->GetRight();
        V3 up = self->piloting->GetUp();

        if (controller->cruise)
          controller->thrust += look;

        if (gForwardButton()->Down())
          controller->thrust += look;
        if (gBackwardButton()->Down())
          controller->thrust -= look;
        if (gLeftButton()->Down())
          controller->thrust -= right;
        if (gRightButton()->Down())
          controller->thrust += right;

        if (Keyboard_Pressed(Key_BackSpace)) {
          Sound_Play2D("ui/targetlost.ogg", 0.1f);
          self->piloting->GetTargets()->elements.clear();
        }

        controller->thrust += right * Axis_LeftStickX()->Get();
        controller->thrust -= up * Axis_LeftStickY()->Get();
        controller->thrust += look * Axis_LeftTrigger()->Get();

        /* Targeting. */ {
          Pointer<ComponentTargets> targets = self->piloting->GetTargets();

          if (targets && targets->elements.size()) {
            Object const& source = self->piloting;
            Position sourcePoint = source->GetCenter();

            Object targetObject = targets->elements[0];
            Position targetPoint = targetObject->GetCenter();
            Distance targetDistance = Length(targetPoint - sourcePoint);

            for (size_t i = 1; i < targets->elements.size(); ++i) {
              Position thisPoint = targets->elements[i]->GetCenter();
              Distance thisDistance = Length(thisPoint - sourcePoint);
              if (thisDistance < targetDistance) {
                targetObject = targets->elements[i];
                targetPoint = thisPoint;
                targetDistance = thisDistance;
              }
            }

            Pointer<ComponentSockets> sockets = source->GetSockets();
            for (size_t i = 0; i < sockets->instances.size(); ++i) {
              Object const& o = sockets->instances[i];
              if (!o || o->GetCapability().Attack <= 0)
                continue;

              Position center = o->GetCenter();
              Item const& weapon = o->GetSockets()->instances[0]->GetSupertype();
              float speed = weapon->GetSpeed();
              float range = weapon->GetRange();
              if (Length(center - targetPoint) > range)
                continue;

              Position hitPoint;
              float t = ComputeImpact(
                center,
                targetPoint,
                source->GetVelocity(),
                targetObject->GetVelocity(),
                speed,
                &hitPoint);

              if (t >= 0) {
                o->Broadcast(MessageTargetPosition(hitPoint));
                o->Broadcast(MessageTargetObject(targetObject));
              }
            }
          } else {
            WorldRay camRay =
              Camera_Get()->GetView(
                Viewport_Get()->GetAspect()).GetRay(
                  Viewport_Get()->ToNDC(Mouse_GetPos()));

            self->piloting->Broadcast(MessageTargetPosition(camRay(1000.0)));
          }
        }
      }

      else {
        controller->cruise = false;
        self->piloting->Broadcast(MessageReload());
      }
    }
  };
}

DefineFunction(Widget_HUD) {
  return Widget_Create1(new WidgetHUD(args.self));
}
