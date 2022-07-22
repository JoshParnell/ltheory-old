#include "Blueprint.h"
#include "AssemblyChip.h"

#include "Game/Player.h"

#include "LTE/Math.h"
#include "LTE/RNG.h"
#include "LTE/StackFrame.h"

#include "UI/Glyphs.h"

#include "LTE/Debug.h"

DERIVED_IMPLEMENT(Blueprint)

const int kModifierSamples = 256;
const float kResearchVariance = 0.5f;

const Icon kIcon = Icon_Create()
  ->Add(Glyph_Line(V2(-0.80f, -0.50f), V2( 0.80f, -0.50f), 1, 0.1f))
  ->Add(Glyph_Line(V2(-0.80f,  0.00f), V2( 0.80f,  0.00f), 1, 0.1f))
  ->Add(Glyph_Line(V2(-0.80f,  0.50f), V2( 0.80f,  0.50f), 1, 0.1f))
  ->Add(Glyph_Line(V2(-0.50f, -0.80f), V2(-0.50f,  0.80f), 1, 0.1f))
  ->Add(Glyph_Line(V2( 0.00f, -0.80f), V2( 0.00f,  0.80f), 1, 0.1f))
  ->Add(Glyph_Line(V2( 0.50f, -0.80f), V2( 0.50f,  0.80f), 1, 0.1f));

#define CONSTRUCTABLE_X                                                        \
  X(ProductionLabType)                                                         \
  X(ScannerType)                                                               \
  X(ShipType)                                                                  \
  X(StationType)                                                               \
  X(TransferUnitType)                                                          \
  X(ThrusterType)

namespace {
  Type Metatype_Get(ItemType type) {
    switch (type) {
#define X(x)                                                                   \
      case ItemType_##x :                                                      \
        return Type_Get(*(Item_##x##_Args* const)0);
      CONSTRUCTABLE_X
#undef X
      default:
        return nullptr;
    }
  }
  
  void Metatype_Mutate(Data const& type) {
    float multiplier = Exp(-kResearchVariance * RandExp());

    size_t fields = type.type->GetFieldCount(type.data);
    if (fields < 4)
      return;

    fields -= 2;
    size_t positive = 2 + rand() % fields;
    size_t negative = positive;

    while (negative == positive)
      negative = 2 + rand() % fields;

    FieldType positiveField = type.type->GetField(type.data, positive);
    FieldType negativeField = type.type->GetField(type.data, negative);

    LTE_ASSERT(positiveField.type == Type_Get<float>());
    LTE_ASSERT(negativeField.type == Type_Get<float>());

    *(float*)positiveField.address *= multiplier;
    *(float*)negativeField.address /= multiplier;
  }
}

Item Item_Blueprint(DataRef const& properties) { AUTO_FRAME;
  Reference<Blueprint> self = new Blueprint;
  self->metatype = properties;

  Item item;

  if (false) {}
#define X(x)                                                                   \
  else if (self->metatype.type == Type_Get<Item_##x##_Args>())                 \
    item = Item_##x(*(Item_##x##_Args const*)self->metatype.data);
    CONSTRUCTABLE_X
#undef X

  if (item)
    self->assemblyChip = Item_AssemblyChip(self, item);

  self->icon = item->GetIcon()
    ? Icon_Create()
      ->Add(item->GetIcon(), 0, 0.75f)
      ->Add(kIcon, 0, 1)
    : kIcon;
  self->name = item->GetName() + " Blueprint";
  self->value = item->GetValue();
  return self;
}

Item Item_Blueprint_Derived(Reference<Blueprint> const& source) { AUTO_FRAME;
  Reference<Blueprint> self = new Blueprint;
  self->parent = source.t;
  self->metatype = source->metatype;
  Metatype_Mutate(self->metatype);

  Item item;

  if (false) {}
#define X(x)                                                                   \
  else if (self->metatype.type == Type_Get<Item_##x##_Args>())                 \
    item = Item_##x(*(Item_##x##_Args const*)self->metatype.data);
    CONSTRUCTABLE_X
#undef X

  if (item)
    self->assemblyChip = Item_AssemblyChip(self, item);
  
  self->icon = item->GetIcon()
    ? Icon_Create()
      ->Add(item->GetIcon(), 0, 0.75f)
      ->Add(kIcon, 0, 1)
    : kIcon;
  self->name = item->GetName() + " Blueprint";
  self->value = item->GetValue();
  return self;
}
