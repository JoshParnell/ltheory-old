#include "Player.h"

#include "Component/Asset.h"
#include "Component/Pilotable.h"

#include "Game/Tasks.h"

AutoClassDerived(PlayerImpl, PlayerT,
  bool, isHuman)
  DERIVED_TYPE_EX(PlayerImpl)

  PlayerImpl() {}

  Health GetHealth() const {
    return 100;
  }

  Icon GetIcon() const {
    Icon icon;
    ScriptFunction_Load("Icons:Person")->Call(icon);
    return icon;
  }

  Health GetMaxHealth() const {
    return 100;
  }

  bool IsHuman() const {
    return isHuman;
  }

  void OnAttacked(Player const& attacker) {
    ModOpinion(attacker, -1.0f);
  }

  void Pilot(Object const& object) {
    Unpilot();
    Pointer<ComponentPilotable> p = object->GetPilotable();
    LTE_ASSERT(!p->pilot);
    p->pilot = this;
    piloting = object;
    container = object;
  }

  void Unpilot() {
    if (piloting) {
      Pointer<ComponentPilotable> p = piloting->GetPilotable();
      LTE_ASSERT(p);
      container = nullptr;
      p->pilot = nullptr;
      piloting = nullptr;
    }
  }
};

DERIVED_IMPLEMENT(PlayerImpl)

DefineFunction(Player_AI) {
  Reference<PlayerImpl> self = new PlayerImpl(false);
  self->traits = args.traits;
  return self;
}

DefineFunction(Player_Human) {
  return new PlayerImpl(true);
}
