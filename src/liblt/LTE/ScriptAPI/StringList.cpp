#include "LTE/StringList.h"
#include "LTE/Function.h"

TypeAlias(Reference<StringListT>, StringList);

FreeFunction(StringList, StringList_Get,
  "Return the 'i'th child of 'list'",
  StringList, list,
  int, i)
{
  return list->Get(i);
} FunctionAlias(StringList_Get, Get);

FreeFunction(String, StringList_GetValue,
  "Return the value of 'list'",
  StringList, list)
{
  return list->GetValue();
} FunctionAlias(StringList_GetValue, GetValue);

FreeFunction(bool, StringList_IsAtom,
  "Return whether 'list' is an atom",
  StringList, list)
{
  return list->IsAtom();
} FunctionAlias(StringList_IsAtom, IsAtom);

FreeFunction(int, StringList_Size,
  "Return the size of 'list'",
  StringList, list)
{
  return list->GetSize();
} FunctionAlias(StringList_Size, Size);
