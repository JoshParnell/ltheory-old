#include "Account.h"
#include "Game/Object.h"
#include "LTE/Function.h"

VoidFreeFunction(Object_AddCredits,
  "Transfer 'count' credits to 'object's bank account",
  Object, object,
  Quantity, count)
{
  object->AddCredits(count);
} FunctionAlias(Object_AddCredits, AddCredits);

FreeFunction(Quantity, Object_GetCredits, 
  "Return the number of credits in 'object's bank account",
  Object, object)
{
  return object->GetCredits();
} FunctionAlias(Object_GetCredits, GetCredits);

FreeFunction(bool, Object_RemoveCredits, 
  "Try to transfer 'count' credits from 'object's bank account; return success",
  Object, object,
  Quantity, count)
{
  return object->RemoveCredits(count);
} FunctionAlias(Object_RemoveCredits, RemoveCredits);
