#include "LTE/Reference.h"
#include "LTE/RNG.h"
#include "LTE/VectorNP.h"

TypeAlias(ListNP, List);

namespace Priv1 {
  FreeFunctionNoParams(ListNP, List, "Create an empty list") {
    return new ListNPT;
  }
}

namespace Priv2 {
  FreeFunction(ListNP, List,
    "Create a list",
    Data, elem)
  {
    ListNP list = new ListNPT(elem.type);
    list->Append(DataRef(elem.type, elem.data));
    return list;
  }
}

VoidFreeFunction(List_Append,
  "Append 'elem' to 'list'",
  ListNP, list,
  Data, elem)
{
  list->Append(DataRef(elem.type, elem.data));
} FunctionAlias(List_Append, +=);

VoidFreeFunction(List_Clear,
  "Clear the contents of 'list'",
  ListNP, list)
{
  list->Clear();
} FunctionAlias(List_Clear, Clear);

FreeFunction(Data, List_Get,
  "Return element at index 'i' in 'list'",
  ListNP, list,
  int, index)
{
  return (*list)[index];
} FunctionAlias(List_Get, Get);

FreeFunction(Data, List_GetRandom,
  "Returns a random element of 'list' using 'rng'",
  ListNP, list,
  RNG, rng)
{
  return (*list)[rng->GetInt(0, list->size - 1)];
} FunctionAlias(List_GetRandom, GetRandom);

VoidFreeFunction(List_Set,
  "Set element at index 'i' in 'list' to 'elem'",
  ListNP, list,
  int, index,
  Data, elem)
{
  (*list)[index] = elem;
} FunctionAlias(List_Set, Set);

FreeFunction(int, List_Size,
  "Return the number of elements in 'list'",
  ListNP, list)
{
  return list->size;
} FunctionAlias(List_Size, Size);

VoidFreeFunction(List_Shuffle,
  "Randomize the order of elements in 'list' using 'rng'",
  ListNP, list,
  RNG, rng)
{
  for (size_t i = 0; i + 1 < list->Size(); ++i) {
    int index = rng->GetInt2(i, list->Size() - 1);
    Data elem = (*list)[i];
    (*list)[i] = (*list)[index];
    (*list)[index] = elem;
  }
} FunctionAlias(List_Shuffle, Shuffle);
