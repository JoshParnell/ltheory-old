#ifndef LTE_Hash_h__
#define LTE_Hash_h__

#include "Common.h"
// #define ALLOW_64_HASH

#if defined(ARCH_32) || !defined(ALLOW_64_HASH)
  const HashT kFNVOffset = 2166136261U;
  const HashT kFNVPrime = 16777619;
#else
  const HashT kFNVOffset = 14695981039346656037UL;
  const HashT kFNVPrime = 1099511628211;
#endif

namespace LTE {
  inline HashT Hash(void const* data, size_t bytes) {
    unsigned char const* d = (unsigned char const*)data;
    HashT hash = kFNVOffset;
    for (size_t i = 0; i < bytes; ++i) {
      hash *= kFNVPrime;
      hash ^= d[i];
    }
    return hash;
  }

  template <class T>
  HashT Hash(T const& t) {
    return Hash((void const*)&t, sizeof(T));
  }

  struct Hasher {
    HashT hash;

    Hasher() :
      hash(kFNVOffset)
      {}

    operator HashT() const {
      return hash;
    }

    template <class T>
    Hasher& operator|(T const& t) {
      for (size_t i = 0; i < sizeof(t); ++i) {
        hash *= kFNVPrime;
        hash ^= ((unsigned char const*)&t)[i];
      }
      return *this;
    }

    Hasher& operator()(void const* data, size_t bytes) {
      for (size_t i = 0; i < bytes; ++i) {
        hash *= kFNVPrime;
        hash ^= ((unsigned char const*)data)[i];
      }
      return *this;
    }
  };
}

#endif
