#include "AssemblyChip.h"

#include "Component/Resources.h"

#include "Game/Icons.h"
#include "Game/Universe.h"

#include "LTE/RNG.h"

DefineFunction(Item_AssemblyChip) {
  Reference<AssemblyChip> self = new AssemblyChip;
  self->blueprint = args.blueprint;
  self->duration = 1.0f;
  self->icon = Icon_Create()
    ->Add(args.source->GetIcon(), 0.5f, 0.5f)
    ->Add(Icon_Disc(), -0.5f, 0.5f);
  self->item = args.source;
  self->name = args.source->GetName() + " Assembly Chip";
  RNG rg = RNG_MTG(args.source->GetHash());

#if 0
  Quantity value = self->item->GetValue();
  ObjectT* context = inventor->piloting->GetContainer();

  /* TODO : Multiple resource requirements. */
  LTE_ASSERT(context);
  LTE_ASSERT(context->GetResources());
  Item resource = context->GetResources()->elements.sample(rg->GetFloat());
  Quantity count = Max((Quantity)1, value / resource->GetValue());
  self->requirements.push(ItemQuantity(resource, count));
#endif

  return self;
}
