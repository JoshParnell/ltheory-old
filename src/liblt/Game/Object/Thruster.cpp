#include "../Objects.h"

#include "Component/Attachable.h"
#include "Component/BoundingBox.h"
#include "Component/Collidable.h"
#include "Component/Cullable.h"
#include "Component/Drawable.h"
#include "Component/Explodable.h"
#include "Component/Integrity.h"
#include "Component/Motion.h"
#include "Component/Orientation.h"
#include "Component/Pluggable.h"
#include "Component/Scriptable.h"
#include "Component/Supertyped.h"

#include "Game/Light.h"
#include "Game/Messages.h"
#include "Game/Graphics/Effects.h"

#include "LTE/Color.h"
#include "LTE/DrawState.h"
#include "LTE/Loader.h"
#include "LTE/Math.h"
#include "LTE/Matrix.h"
#include "LTE/Meshes.h"
#include "LTE/Model.h"
#include "LTE/Pointer.h"
#include "LTE/Pool.h"
#include "LTE/Ray.h"
#include "LTE/Renderable.h"
#include "LTE/RenderStyle.h"
#include "LTE/ShaderInstance.h"
#include "LTE/View.h"

const float kTrailLengthMult = 8;
const float kAcceleration = 1;
const V3 kBoostColor = V3(0.2f, 0.7f, 1.0f);

namespace {
  Model gModel;
  Shader gShader;
  ShaderInstance gShaderInstance;

  void OnLoad() {
    gShader = Shader_Create("billboard_axis.jsl", "thruster_trail.jsl");
    gShaderInstance = ShaderInstance_Create(gShader);
    (*gShaderInstance)
      (RenderStateSwitch_BlendModeAdditive)
      (RenderStateSwitch_ZWritableOff);
    DrawState_Link(gShaderInstance);
    gModel = Model_Create()->Add(Mesh_Billboard(-1, 1, 0, 1), gShaderInstance);
  } bool r = RegisterLoader(OnLoad);
}

typedef ObjectWrapper
  < Component_Attachable
  < Component_BoundingBox
  < Component_Collidable
  < Component_Cullable
  < Component_Drawable
  < Component_Explodable
  < Component_Integrity
  < Component_Orientation
  < Component_Pluggable
  < Component_Scriptable
  < Component_Supertyped
  < ObjectWrapperTail<ObjectType_Thruster>
  > > > > > > > > > > > >
  ThrusterBaseT;

AutoClassDerived(Thruster, ThrusterBaseT,
  float, activation,
  float, age,
  Color, color,
  float, cruise,
  float, thrust,
  LightRef, light)

  DERIVED_TYPE_EX(Thruster)
  POOLED_TYPE

  struct RenderComponent : public RenderableT {
    DERIVED_TYPE(RenderComponent, RenderableT)
    Pointer<Thruster> self;

    RenderComponent(Thruster* self) :
      self(self)
      {}

    Bound3 GetBound() const {
      Bound3 bound = self->Supertyped.type->GetRenderable()->GetBound();
      bound.AddPoint(V3(-1, -1, kTrailLengthMult));
      bound.AddPoint(V3( 1, -1, kTrailLengthMult));
      bound.AddPoint(V3(-1,  1, kTrailLengthMult));
      bound.AddPoint(V3( 1,  1, kTrailLengthMult));
      return bound;
    }

    void Render(DrawState* state) const {
      self->Supertyped.type->GetRenderable()->Render(state);

      float opacity = self->activation;
      if (opacity <= 0.01f)
        return;

      Transform transform = self->GetTransform() *
        Transform_Translation(self->Supertyped.type->GetOffset());

      (*gShader)
        ("axis", transform.look)
        ("size", Length(transform.scale) * V2(0.5f, opacity * kTrailLengthMult))
        ("t", self->age)
        ("baseColor", self->color);

      RenderStyle_Get()->SetTransform(transform);
      gModel->Render(state);
    }
  };

  Thruster() :
    activation(0),
    age(Rand(0, 100)),
    cruise(0),
    thrust(0)
    {}

  /* For performance, only check collisions with things that can damage the
   * thruster. All others are ignored. */
  bool CanCollide(ObjectT const* other) const {
    return other->GetDamager();
  }

  float GetBasePower() const {
    return Supertyped.type->GetPowerDrain();
  }

  float GetPowerFraction() const {
    return Pluggable.powerIn / GetBasePower();
  }

  float GetOutput() {
    return GetPowerFraction() * activation;
  }

  float GetOvercharge() const {
    return Saturate(2.0f * (GetPowerFraction() - 1.0f));
  }

  void OnMessage(Data& m) {
    BaseType::OnMessage(m);
    if (m.type == Type_Get<MessageCruise>())
      cruise = 1.0f;

    else if (m.type == Type_Get<MessageThrustAngular>()) {
      MessageThrustAngular const& v = m.Convert<MessageThrustAngular>();
      ThrustAngular(v.direction, v.amount);
    }

    else if (m.type == Type_Get<MessageThrustLinear>())
      Thrust(m.Convert<MessageThrustLinear>().direction);
  }

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);

    Pluggable.powerRequest = GetBasePower() * (1.0f + 100.0f * cruise);

    if (!light) {
      light = Light_Create(this);
      light->Attachable.transform =
        Transform_Translation(Supertyped.type->GetOffset());
    }

    age += state.dt;

    if (!IsAlive() || !GetRoot()->IsAlive())
      thrust = 0;

    activation = Mix(activation, thrust, 1.0f - Exp(-kAcceleration * state.dt));
    cruise *= Exp(-state.dt);

    if (parent)
      GetRoot()->GetMotion()->force -= GetMaxThrust() * GetOutput() * GetLook();

    color = activation *
      Mix(Supertyped.type->GetColor(), kBoostColor, GetOvercharge());
    light->color = color;
  }

  void Thrust(V3 const& dir) {
    thrust = Saturate(2.0f * Saturate(-Dot(GetLook(), dir)) - 0.5f);
  }

  void ThrustAngular(V3 const& dir, float amount) {
    float torque = GetMaxTorque();
    parent->GetMotion()->torque +=
      Normalize(dir) * torque * Saturate(amount);
  }
};

DERIVED_IMPLEMENT(Thruster)

Object Object_Thruster(Item const& type, ObjectT* parent) {
  Reference<Thruster> self = new Thruster;
  self->SetSupertype(type);
  self->Drawable.renderable = (Renderable)(new Thruster::RenderComponent(self));
  ScriptFunction_Load("Object/Thruster:Init")->VoidCall(0, DataRef((Object)self));
  return self;
}
