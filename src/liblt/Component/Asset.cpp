#include "Asset.h"
#include "Assets.h"
#include "Info.h"

#include "LTE/StackFrame.h"

void ComponentAsset::Run(ObjectT* self, UpdateState& state) { AUTO_FRAME;
  if (owner) {
    /* An asset should always be fully visible to its owner. */
    ComponentInfo* info = owner->GetInfo();
    if (info)
      info->Add(self, InfoLevel_Scan);

    /* Notify the owner if the asset is destroyed. */
    if (self->GetIntegrity() && !self->IsAlive()) {
      owner->AddLogMessage("Your " + self->GetName() + " has been destroyed");
      owner->RemoveAsset(self);
    }
  }
}

FreeFunction(Player, Object_GetOwner,
  "Return the owner of 'object'",
  Object, object)
{
  return object->GetOwner();
} FunctionAlias(Object_GetOwner, GetOwner);
