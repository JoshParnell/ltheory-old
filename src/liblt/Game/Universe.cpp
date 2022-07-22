#include "Universe.h"

#include "Game/Items.h"
#include "Game/Objects.h"
#include "Game/Tasks.h"

#include "LTE/RNG.h"

#include "UI/Glyphs.h"
#include "UI/Icon.h"

double kValueMult = 10.0;

namespace {
  Vector<Universe*> gActive;
}

Universe* Universe_Get() {
  return gActive.size() ? gActive.back() : nullptr;
}

AutoClassDerivedEmpty(UniverseImpl, Universe)
  DERIVED_TYPE_EX(UniverseImpl)

  UniverseImpl() {
    gActive.push(this);
  }

  ~UniverseImpl() {
    gActive.remove(this);
  }
};

DERIVED_IMPLEMENT(UniverseImpl)

Time Universe_Age() {
  Universe* universe = Universe_Get();
  return universe ? universe->age : 0;
}

DefineFunction(Object_Universe) {
  Reference<UniverseImpl> self = new UniverseImpl;
  RNG rg = RNG_MTG(args.seed);
  self->depth = args.depth;
  self->Seeded.seed = args.seed;
  self->Nameable.name = Stringize() | "Universe " | args.seed;
  
  /* Generate the natural resources. */ {
    uint oreSeed = 190;
    RNG rng = RNG_MTG(oreSeed);

    /* Create a basis for the universal currency. */ {
      self->currencyBasis = Item_OreType(1, rng->GetInt());
      self->Resources.elements[self->currencyBasis] = 1;
    }
    
    /* TODO : Factor out generating algorithm. */
    size_t oreTypeCount = 8;
    for (size_t i = 0; i < oreTypeCount; ++i) {
      double value = kValueMult * (5.0f * rng->GetExp() + 1.0);
      uint seed = rng->GetInt();
      self->Resources.elements[Item_OreType(value, seed)] = 1.0f / value;
    }
  }

  /* Generate colony types. */ {
    Icon kIcon = Icon_Create()
      ->Add(Glyph_Box(V2(-0.6f, 0.5f), V2(0.20f, 0.5f), 1, 1))
      ->Add(Glyph_Box(V2( 0.0f, 0.0f), V2(0.30f, 1.0f), 1, 1))
      ->Add(Glyph_Box(V2( 0.6f, 0.3f), V2(0.20f, 0.7f), 1, 1));

    self->colonyTypes.add(Item_ColonyType(
      "Farming",
      kIcon,
      Task_Spawn(self->Resources.elements.entries[0].key, 1),
      Traits()), 1);
  }

  /* Create a region. */
  for (size_t i = 0; i < 1; ++i) {
    RegionType type;
    type.level = args.depth;
    type.pos = rg->GetV3(-10000, 10000);
    type.radius = 100;
    type.seed = rg->GetInt();

    type.resources[self->currencyBasis] += 0.1f;

    /* TODO : Factor out generating algorithm */
    for (size_t i = 0; i < self->Resources.elements.size() / 2; ++i) {
      Item ore = self->Resources.elements.entries.getModulo(rg->GetInt()).key;
      type.resources[ore] += rg->GetFloat() / (float)ore->GetValue();
    }

    Object region = Object_Region(type);
    self->AddInterior(region);
    self->loadedRegions.push(region);
  }

  return self;
}
