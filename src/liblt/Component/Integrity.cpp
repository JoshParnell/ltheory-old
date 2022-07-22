#include "Integrity.h"

#include "Game/Items.h"
#include "Game/Object.h"

#include "LTE/Mutable.h"

Damage ComponentIntegrity::ApplyDamage(ObjectT* self, Damage damage) {
  Health thisDamage = Min(damage, health);
  health -= thisDamage;
  if (health <= 0)
    self->OnDeath();
  return damage - thisDamage;
}

ItemT* ComponentIntegrity::GetDataDamaged(ObjectT const* self) const {
  if (!dataDamaged)
    Mutable(this)->dataDamaged = Item_Data_Damaged(Mutable(self));
  return dataDamaged;
}

ItemT* ComponentIntegrity::GetDataDestroyed(ObjectT const* self) const {
  if (!dataDestroyed)
    Mutable(this)->dataDestroyed = Item_Data_Destroyed(Mutable(self));
  return dataDestroyed;
}

FreeFunction(Health, Object_GetHealth,
  "Return 'object's current (non-normalized) health",
  Object, object)
{
  return object->GetHealth();
} FunctionAlias(Object_GetHealth, GetHealth);

FreeFunction(Health, Object_GetMaxHealth,
  "Return 'object's maximal health value",
  Object, object)
{
  return object->GetMaxHealth();
} FunctionAlias(Object_GetMaxHealth, GetMaxHealth);

FreeFunction(float, Object_GetHealthNormalized,
  "Return 'object's current health on a scale from 0.0 (dead) to 1.0 (max health)",
  Object, object)
{
  return object->GetHealthNormalized();
} FunctionAlias(Object_GetHealthNormalized, GetHealthNormalized);

VoidFreeFunction(Object_SetHealth,
  "Set 'object's current health to 'health'",
  Object, object,
  Health, health)
{
  if (object->GetIntegrity())
    object->GetIntegrity()->health = health;
} FunctionAlias(Object_SetHealth, SetHealth);
