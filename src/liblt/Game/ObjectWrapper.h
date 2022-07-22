#ifndef ObjectWrapper_h__
#define ObjectWrapper_h__

#include "Object.h"

template <class BaseT>
struct ObjectWrapper : public BaseT {};

template <int ObjectTypeEnum>
struct ObjectWrapperTail : public ObjectT {
  ObjectType GetType() const {
    return (ObjectType)ObjectTypeEnum;
  }
};

#endif
