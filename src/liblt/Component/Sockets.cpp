#include "Sockets.h"
#include "Attachable.h"
#include "Motion.h"
#include "Orientation.h"
#include "Pluggable.h"
#include "LTE/Function.h"

bool ComponentSockets::Plug(ObjectT* self, Item const& type) {
  for (uint i = 0; i < sockets.size(); ++i)
    if (!instances[i] && Plug(self, type, i))
      return true;
  return false;
}

bool ComponentSockets::Plug(ObjectT* self, Object const& object) {
  for (uint i = 0; i < sockets.size(); ++i)
    if (!instances[i] && Plug(self, object, i))
      return true;
  return false;
}

bool ComponentSockets::Plug(ObjectT* self, Item const& type, uint index) {
  if (index >= sockets.size())
    return false;

  Socket const& thisSocket = sockets[index];
  if (thisSocket.type != type->GetSocketType())
    return false;

  Unplug(self, index);
  Object newChild = type->Instantiate(self);
  LTE_ASSERT(newChild);
  Plug(self, newChild, index);
  return true;
}

bool ComponentSockets::Plug(ObjectT* self, Object const& object, uint index) {
  Socket const& thisSocket = sockets[index];
  instances[index] = object;
  ComponentAttachable* at = object->GetAttachable();
  if (at)
    at->transform = thisSocket.transform * at->transform;
  
  self->AddChild(object);

  ComponentPluggable* plug = object->GetPluggable();
  if (plug)
    plug->index = index;

  return true;
}

void ComponentSockets::Unplug(ObjectT* self, uint index) {
  LTE_ASSERT(index < instances.size());
  if (instances[index]) {
    self->RemoveChild(instances[index]);
    instances[index]->Delete();
    instances[index] = nullptr;
  }
}

AutoClass(SocketsIterator,
  Object, object,
  uint, index)
  SocketsIterator() {}
};

FreeFunction(bool, Object_Plug,
  "Attempt to plug 'item' into a free socket of 'object', return success",
  Object, object,
  Item, item)
{
  return object->Plug(item);
} FunctionAlias(Object_Plug, Plug);

FreeFunction(SocketsIterator, Object_GetSockets,
  "Return an iterator to the sockets of 'object'",
  Object, object)
{
  return SocketsIterator(object, 0);
} FunctionAlias(Object_GetSockets, GetSockets);

FreeFunction(Object, SocketsIterator_Access,
  "Return the contents of 'iterator'",
  SocketsIterator, iterator)
{
  return iterator.object->GetSockets()->instances[iterator.index];
} FunctionAlias(SocketsIterator_Access, Get);

VoidFreeFunction(SocketsIterator_Advance,
  "Advance 'iterator'",
  SocketsIterator, iterator)
{
  Mutable(iterator).index++;
  ComponentSockets* sockets = iterator.object->GetSockets();
  while (
      iterator.index < sockets->instances.size() &&
      !sockets->instances[iterator.index])
    Mutable(iterator).index++;
} FunctionAlias(SocketsIterator_Advance, Advance);

FreeFunction(bool, SocketsIterator_HasMore,
  "Return whether 'iterator' has more elements",
  SocketsIterator, iterator)
{
  return
    iterator.object->GetSockets() &&
    iterator.index < iterator.object->GetSockets()->instances.size();
} FunctionAlias(SocketsIterator_HasMore, HasMore);
