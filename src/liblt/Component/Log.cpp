#include "Log.h"
#include "Game/Object.h"
#include "LTE/Function.h"

AutoClass(LogIterator,
  Object, object,
  uint, index)
  LogIterator() {}
};

VoidFreeFunction(Object_AddLog,
  "Add 'message' to 'object's list of logged messages",
  Object, object,
  String, message)
{
  object->AddLogMessage(message);
} FunctionAlias(Object_AddLog, AddLog);

FreeFunction(LogIterator, Object_GetLog,
  "Return an iterator to the log entries of 'object'",
  Object, object)
{
  return LogIterator(object, 0);
} FunctionAlias(Object_GetLog, GetLog);

VoidFreeFunction(LogIterator_Advance,
  "Advance 'iterator'",
  LogIterator, iterator)
{
  Mutable(iterator).index++;
} FunctionAlias(LogIterator_Advance, Advance);

FreeFunction(LogEntry, LogIterator_Get,
  "Return the contents of 'iterator'",
  LogIterator, iterator)
{
  return iterator.object->GetLog()->elements[iterator.index];
} FunctionAlias(LogIterator_Get, Get);

FreeFunction(bool, LogIterator_HasMore,
  "Return whether 'iterator' has more elements",
  LogIterator, iterator)
{
  return iterator.object->GetLog() &&
    iterator.index < iterator.object->GetLog()->elements.size();
} FunctionAlias(LogIterator_HasMore, HasMore);

FreeFunction(int, LogIterator_Size,
  "Return the total number of elements in 'iterator'",
  LogIterator, iterator)
{
  return iterator.object->GetLog()
    ? (int)iterator.object->GetLog()->elements.size()
    : 0;
} FunctionAlias(LogIterator_Size, Size);
