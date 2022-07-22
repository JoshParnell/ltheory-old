#include "../Objects.h"

#include "Component/Pluggable.h"
#include "Component/Sockets.h"
#include "Component/Supertyped.h"

#include "Game/Messages.h"
#include "Game/Player.h"

#include "LTE/Keyboard.h"
#include "LTE/Math.h"
#include "LTE/Pointer.h"
#include "LTE/Pool.h"

#include "Module/Settings.h"

const double kEfficiencyFactor = 1.0;
const float kCapacitance = 8.0f;
const double kChangeSpeed = 2.0;

typedef ObjectWrapper
  < Component_Pluggable
  < Component_Supertyped
  < ObjectWrapperTail<ObjectType_PowerGenerator>
  > > >
  PowerGeneratorBaseT;

AutoClassDerived(PowerGenerator, PowerGeneratorBaseT,
  float, capacitance,
  float, capacitor,
  bool, boost)

  DERIVED_TYPE_EX(PowerGenerator)
  POOLED_TYPE

  PowerGenerator() :
    capacitance(kCapacitance),
    capacitor(0.0f),
    boost(false)
    {}

  void Allocate(Object const& root, double mult, double factor) {
    Pointer<ComponentSockets> socks = root->GetSockets();
    if (!socks)
      return;

    for (size_t i = 0; i < socks->instances.size(); ++i) {
      Object const& o = socks->instances[i];
      if (!o)
        continue;

      Pointer<ComponentPluggable> plug = o->GetPluggable();
      if (plug) {
        double allocated = plug->priority * plug->powerRequest * mult;
        plug->powerIn = Mix((double)plug->powerIn, allocated, factor);
      }

      Allocate(o, mult, factor);
    }
  }

  void CollectRequests(Object const& root, double& total) {
    Pointer<ComponentSockets> socks = root->GetSockets();
    if (!socks)
      return;

    for (size_t i = 0; i < socks->instances.size(); ++i) {
      Object const& o = socks->instances[i];
      if (o) {
        Pointer<ComponentPluggable> plug = o->GetPluggable();
        total += plug ? plug->priority * plug->powerRequest : 0;
        CollectRequests(o, total);
      }
    }
  }

  void OnMessage(Data& m) {
    BaseType::OnMessage(m);
    if (m.type == Type_Get<MessageBoost>())
      boost = true;
  }

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);

    const float chargeRate = 2.00f;
    const float dischargeRate = 4.00f;
    capacitor = Min(capacitance, capacitor + chargeRate * state.dt);

    double totalRequest = 0;
    CollectRequests(parent.t, totalRequest);

    if (totalRequest > 0) {
      double output = Supertyped.type->GetCapability().Power;
      if (boost) {
        float boostAmount = Min(capacitor, dischargeRate);
        output += Supertyped.type->GetCapability().Power * boostAmount;
        capacitor -= boostAmount * state.dt;
      }

      double mult = output / totalRequest;
      if (mult > 1)
        mult = Pow(mult, kEfficiencyFactor);
      double changeSpeed = 1.0 - Exp(-kChangeSpeed * state.dt);
      Allocate(parent.t, mult, changeSpeed);
    }

    boost = false;
  }
};

DERIVED_IMPLEMENT(PowerGenerator)

DefineFunction(Object_PowerGenerator) {
  LTE_ASSERT(args.type->GetType() == ItemType_PowerGeneratorType);
  Reference<PowerGenerator> self = new PowerGenerator;
  self->SetSupertype(args.type);
  return self;
}
