#include "Resources.h"

#include "Game/Item.h"
#include "Game/Object.h"

#include "LTE/Function.h"

VoidFreeFunction(Object_AddResource,
  "Add 'item' to 'objects' list of natural resources with weight 'w'",
  Object, object,
  Item, item,
  float, weight)
{
  ComponentResources* resources = object->GetResources();
  LTE_ASSERT(resources != nullptr);
  resources->elements[item] = weight;
} FunctionAlias(Object_AddResource, AddResource);
