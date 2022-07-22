#ifndef Component_Interior_h__
#define Component_Interior_h__

#include "Common.h"
#include "Game/Object.h"
#include "LTE/AutoClass.h"
#include "LTE/Map.h"
#include "LTE/ParticleSystem.h"
#include "LTE/Pointer.h"
#include "LTE/V3.h"
#include "LTE/Vector.h"

typedef Vector<Object> ObjectListT;
typedef Map<ObjectType, ObjectListT> ObjectMapT;

AutoClass(ComponentInterior,
  ObjectListT, objects,
  ObjectMapT, objectMap,
  bool, allowMovement)

  ParticleSystem particles;

  ComponentInterior() :
    particles(ParticleSystem_Create())
    {}

  LT_API ~ComponentInterior();

  LT_API void Draw(ObjectT* self, DrawState* state);
  LT_API void Run(ObjectT* self, UpdateState& state);

  LT_API void Add(ObjectT* self, Object const& o);
  LT_API void Remove(ObjectT* self, Object const& o);
};

AutoComponent(Interior)
  void AddInterior(Object const& child) {
    Interior.Add(this, child);
  }

  void Broadcast(Data& message) {
    for (size_t i = 0; i < Interior.objects.size(); ++i)
      Interior.objects[i]->Broadcast(message);
    BaseT::Broadcast(message);
  }

  void OnDraw(DrawState* s) {
    Interior.Draw(this, s);
    BaseT::OnDraw(s);
  }

  void OnUpdate(UpdateState& s) {
    Interior.Run(this, s);
    BaseT::OnUpdate(s);
  }

  void RemoveInterior(Object const& child) {
    Interior.Remove(this, child);
  }
};

AutoClass(InteriorIterator,
  ComponentInterior*, interior,
  size_t, index)

  InteriorIterator() :
    interior(nullptr)
    {}

  void Advance() {
    index++;
  }

  Object const& Get() const {
    return interior->objects[index];
  }
  
  bool HasMore() const {
    return 
      interior &&
      index < interior->objects.size();
  }
};

AutoClass(InteriorTypeIterator,
  ComponentInterior*, interior,
  ObjectType, type,
  size_t, index)

  InteriorTypeIterator() :
    interior(nullptr)
    {}

  void Advance() {
    index++;
  }

  Object const& Get() const {
    return interior->objectMap[type][index];
  }
  
  bool HasMore() const {
    return
      interior &&
      interior->objectMap.contains(type) &&
      index < interior->objectMap[type].size();
  }

  size_t Size() const {
    if (!interior)
      return 0;
    if (!interior->objectMap.contains(type))
      return 0;
    return interior->objectMap[type].size();
  }
};

inline InteriorIterator Object_GetInteriorObjects(Object const& object) {
  return InteriorIterator(object->GetInterior(), 0);
}

inline InteriorTypeIterator Object_GetInteriorObjects(
  Object const& object,
  ObjectType type)
{
  return InteriorTypeIterator(object->GetInterior(), type, 0);
}

#endif
