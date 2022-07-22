#include "Targets.h"
#include "LTE/Function.h"

AutoClass(TargetIterator,
  Object, object,
  uint, index)
  TargetIterator() {}
};

VoidFreeFunction(Object_AddTarget,
  "Add 'target' to 'object's list of targets",
  Object, object,
  Object, target)
{
  object->GetTargets()->elements.push(target);
} FunctionAlias(Object_AddTarget, AddTarget);

FreeFunction(TargetIterator, Object_GetTargets,
  "Return an iterator to the targets of 'object'",
  Object, object)
{
  return TargetIterator(object, 0);
} FunctionAlias(Object_GetTargets, GetTargets);

FreeFunction(bool, Object_HasTarget,
  "Return whether 'object' has 'target' currently targetted",
  Object, object,
  Object, target)
{
  return object->GetTargets() && object->GetTargets()->elements.contains(target);
} FunctionAlias(Object_HasTarget, HasTarget);

VoidFreeFunction(Object_RemoveTarget,
  "Remove 'target' from 'object's list of targets",
  Object, object,
  Object, target)
{
  object->GetTargets()->elements.remove(target);
} FunctionAlias(Object_RemoveTarget, RemoveTarget);

VoidFreeFunction(TargetIterator_Advance,
  "Advance 'iterator'",
  TargetIterator, iterator)
{
  Mutable(iterator).index++;
} FunctionAlias(TargetIterator_Advance, Advance);

FreeFunction(Object, TargetIterator_Get,
  "Return the contents of 'iterator'",
  TargetIterator, iterator)
{
  return iterator.object->GetTargets()->elements[iterator.index];
} FunctionAlias(TargetIterator_Get, Get);

FreeFunction(bool, TargetIterator_HasMore,
  "Return whether 'iterator' has more elements",
  TargetIterator, iterator)
{
  return iterator.object->GetTargets() &&
    iterator.index < iterator.object->GetTargets()->elements.size();
} FunctionAlias(TargetIterator_HasMore, HasMore);

FreeFunction(int, TargetIterator_Size,
  "Return the total number of elements in 'iterator'",
  TargetIterator, iterator)
{
  return iterator.object->GetTargets()
    ? (int)iterator.object->GetTargets()->elements.size()
    : 0;
} FunctionAlias(TargetIterator_Size, Size);
