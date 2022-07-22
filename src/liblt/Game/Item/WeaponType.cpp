#include "WeaponType.h"

#include "Game/Beam.h"
#include "Game/Constants.h"
#include "Game/Icons.h"
#include "Game/NLP.h"
#include "Game/Objects.h"

#include "Component/Motion.h"
#include "Component/Pilotable.h"

#include "Module/SoundEngine.h"

#include "LTE/Grammar.h"
#include "LTE/Math.h"
#include "LTE/Meshes.h"
#include "LTE/RNG.h"
#include "LTE/Script.h"
#include "LTE/V4.h"
#include "LTE/VectorMap.h"

#include "UI/Glyphs.h"

DERIVED_IMPLEMENT(WeaponType)

const Icon kWeaponIcon[WeaponClass_SIZE] = {
  Icon_Crosshair(),
  Icon_Crosshair(),
  Icon_Crosshair(),
  Icon_Crosshair(),
};

const float kWeaponMagazineSizeMult[WeaponClass_SIZE] = {
  0, 1, 6, 10,
};

const float kWeaponMagazineProbability[WeaponClass_SIZE] = {
  0, 1, 0.1f, 0.9f,
};

const float kWeaponPowerDrainMult[WeaponClass_SIZE] = {
  5, 0, 2, 1,
};

const float kWeaponRateMult[WeaponClass_SIZE] = {
  1, 0.01f, 1, 1,
};

const float kWeaponSpreadMult[WeaponClass_SIZE] = {
  0, 1, 2, 5,
};

const float kWeaponWeightMult[WeaponClass_SIZE] = {
  5, 3, 2, 1,
};

const float kAmmoDamageMult[WeaponClass_SIZE] = {
  5, 20, 2, 1,
};

const float kAmmoLifeMult[WeaponClass_SIZE] = {
  2.5f, 10, 1.25f, 1,
};

const float kAmmoProbabilityMult[WeaponClass_SIZE] = {
  0.0f, 0.0f, 1.0f, 0.0f,
};

const float kAmmoSpeedMult[WeaponClass_SIZE] = {
  1e10f, 1, 1, 1e10f,
};

Object WeaponType::Fire(
  ObjectT* w,
  Position const& origin,
  V3 const& heading,
  Object const& target)
{
  V3 myVelocity = w->GetRoot()->GetVelocity();
  Object object;
  WeaponType const* wtype = (WeaponType*)(ItemT*)w->GetSupertype();

  if (type == WeaponClass_Pulse) {
    object = Object_Pulse(
      (heading + wtype->spread * SampleSphere()) * speed,
      myVelocity,
      w->GetScale().GetMax());

    Sound_Play3D(GetSound(), w, wtype->offset, 0.1f)
      ->SetPitch(Rand(0.75f, 1.25f));
  }

  else if (type == WeaponClass_Missile) {
    V3 offset = target ? target->GetDrawable()->renderable()->Sample() : 0;
    object = Object_Missile(heading * speed, myVelocity, target, offset);
    object->SetPos(origin);
    Sound_Play3D("weapon/missile1_fire.ogg", w, wtype->offset, 0.25f)
      ->SetPitch(Rand(0.75f, 1.25f));
  }

  else if (type == WeaponClass_Beam) {
    /* CRITICAL. */
#if 0
    if (!w->beam) {
      float beamScale = w->GetScale().GetMax();
      Beam* b = Beam_Create();
      b->width = beamScale;
      object = b;
      w->beam = b;
      Sound_Play3D("weapon/beam1_fire.wav", w, wtype->offset, 1, beamScale);
    }
#endif
  }

  else if (type == WeaponClass_Rail) {
    object = Object_Rail(origin, Normalize(heading + wtype->spread * SampleSphere()), myVelocity);
    Sound_Play3D("weapon/rail1_fire.ogg", w, wtype->offset, 0.25f)
      ->SetPitch(Rand(0.7f, 1.3f));
  }

  if (object) {
    object->GetDamager()->type = (WeaponType*)w->GetSupertype();
    object->GetDamager()->source = w;
    object->SetPos(origin);
  }

  return object;
}

float WeaponType::GetDPS() const {
  float damage = (float)this->damage;
  if (uses)
    damage /= 1.0f / rate + magazineTime / (float)uses;

  if (type != WeaponClass_Beam)
    damage *= rate;
  return damage;
}

Object WeaponType::Instantiate(ObjectT* parent) {
  Object turret = Item_TurretType(1, kPi)->Instantiate();
  turret->Plug(Object_Weapon(this));
  return turret;
}

DefineFunction(Item_WeaponType) {
  static Renderable renderable;
  if (!renderable)
    ScriptFunction_Load("Item/WeaponType:Generate")->Call(renderable);

  RNG rng = RNG_MTG(args.id);
  Reference<WeaponType> self = new WeaponType;

  float typeValue = rng->GetFloat();
  float thisValue = 0;
  self->type = WeaponClass_Pulse;
  for (int i = 0; i < WeaponClass_SIZE; ++i) {
    thisValue += kAmmoProbabilityMult[i];
    if (typeValue <= thisValue) {
      self->type = i;
      break;
    }
  }

  self->color = 0.25f * Color_White + ToRGB(Color(
    rng->GetFloat(),
    rng->GetFloat(0.6f, 0.99f),
    rng->GetFloat(0.2f, 0.6f)));

  self->damage = (Damage)
    (Constant_AmmoDamageMult() * kAmmoDamageMult[self->type] *
     Sigfigs(1 + 15 * rng->GetExp(), 2));

  self->properties = rng->GetV3(0, 1);

  float life = Constant_AmmoLifeMult() * kAmmoLifeMult[self->type] *
    (1.0f + rng->GetExp());

  self->speed = Constant_AmmoSpeedMult() * kAmmoSpeedMult[self->type] *
    (1.0 + 0.5f * rng->GetExp());

  self->range = life * self->speed;

  if (self->type == WeaponClass_Beam) {
  }

  else if (self->type == WeaponClass_Missile) {
  }

  else if (self->type == WeaponClass_Pulse) {
    const String table[] = {
      "weapon/pulse/1/Pulse1.1.ogg",
      "weapon/pulse/2/Pulse2.1short.ogg",
      "weapon/pulse/3/Pulse3.2.ogg",
      "weapon/pulse/4/Pulse4.1short.ogg",
      "weapon/pulse/5/Pulse5.1.ogg",
      "weapon/pulse/5/Pulse5.1.ogg"
    };

    self->sound = table[rng->GetInt(0, sizeof(table) / sizeof(*table) - 1)];
  }

  else if (self->type == WeaponClass_Rail) {
  }

  self->uses = rng->GetFloat() < kWeaponMagazineProbability[self->type]
    ? 4 * (int)(kWeaponMagazineSizeMult[self->type] * rng->GetFloat(1, 2)) : 0;

  self->icon = kWeaponIcon[self->type];

  self->mass = self->damage * kWeaponWeightMult[self->type];

  self->name = String_Capital(Grammar_Get()->Generate(rng, "$weapon",
    WeaponClass_String[self->type]));

  self->offset = V3(0, 0.5f, 4);

  self->powerDrain = kWeaponPowerDrainMult[self->type];

  self->renderable = renderable;

  self->magazineTime = self->uses ? Round(rng->GetFloat(6, 10)) : 0.0f;

  self->scale = 0.5f;

  self->spread =
    Constant_WeaponSpreadMult() * kWeaponSpreadMult[self->type] * rng->GetErlang(2);

  self->rate = Constant_WeaponRateMult() * kWeaponRateMult[self->type] *
    (1 + Floor(6 * rng->GetErlang(2)));

  if (self->uses)
    self->rate *= 1.5f;

  self->integrity = 100;

  self->capability = Capability_Attack(self->GetDPS());

  return self;
}
