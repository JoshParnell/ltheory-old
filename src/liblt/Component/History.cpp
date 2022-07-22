#include "History.h"

#include "Game/Object.h"

void ComponentHistory::Run(ObjectT* self, UpdateState& state) {
}

DefineFunction(Object_AddHistory) {
  ComponentHistory* history = args.object->GetHistory();
  LTE_ASSERT(history != nullptr);
  history->elements.push(args.event);
}

AutoClass(HistoryIterator,
  Object, object,
  uint, index)
  HistoryIterator() {}
};

FreeFunction(HistoryIterator, Object_GetHistory,
  "Return an iterator to the elements of 'object's history",
  Object, object)
{
  return HistoryIterator(object, 0);
} FunctionAlias(Object_GetHistory, GetHistory);

VoidFreeFunction(HistoryIterator_Advance,
  "Advance 'iterator'",
  HistoryIterator, iterator)
{
  Mutable(iterator).index++;
} FunctionAlias(HistoryIterator_Advance, Advance);

FreeFunction(Event, HistoryIterator_Get,
  "Return the contents of 'iterator'",
  HistoryIterator, iterator)
{
  return iterator.object->GetHistory()->elements[iterator.index];
} FunctionAlias(HistoryIterator_Get, Get);

FreeFunction(bool, HistoryIterator_HasMore,
  "Return whether 'iterator' has more elements",
  HistoryIterator, iterator)
{
  return iterator.object->GetHistory() &&
    iterator.index < iterator.object->GetHistory()->elements.size();
} FunctionAlias(HistoryIterator_HasMore, HasMore);

FreeFunction(int, HistoryIterator_Size,
  "Return the total number of elements in 'iterator'",
  HistoryIterator, iterator)
{
  return iterator.object->GetHistory()
    ? (int)iterator.object->GetHistory()->elements.size()
    : 0;
} FunctionAlias(HistoryIterator_Size, Size);
