#ifndef Attribute_Hash_h__
#define Attribute_Hash_h__

#include "Common.h"

template <class T>
struct Attribute_Hash : public T {
  typedef Attribute_Hash SelfType;
  ATTRIBUTE_COMMON(hash)
  HashT hash;

  Attribute_Hash() :
    hash(0)
    {}

  HashT const& GetHash() const {
    return hash;
  }

  bool HasHash() const {
    return true;
  }
};

#endif
