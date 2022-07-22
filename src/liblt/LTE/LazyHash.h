#ifndef LTE_LazyHash_h__
#define LTE_LazyHash_h__

#include "Common.h"

namespace LTE {
  template <class T>
  struct LazyHash {
    mutable HashT hash;
    
    LazyHash() :
      hash(0)
      {}

    HashT GetHash() const {
      return hash ? hash : hash = ((T*)this)->ComputeHash();
    }
  };
}

#endif
