#ifndef LTE_AutoClass_h__
#define LTE_AutoClass_h__

#include "Type.h"

struct EmptyBase {
  friend Type _Type_Get(EmptyBase const& t) {
    return nullptr;
  }

  FIELDS {}
};

#define DefineInitializer void Initializer(int)

#define AUTOCLASS0(name, base)                                                 \
  struct name : public base {                                                  \
    typedef name SelfType;                                                     \
    typedef base BaseType;                                                     \
    DefineMetadataInline(name)                                                 \
                                                                               \
    FIELDS {                                                                   \
      base::MapFields(type, addr, m, aux);                                     \
    }                                                                          \
                                                                               \
    template <class StreamT>                                                   \
    friend void _ToStream(StreamT& s, name const& t) {                         \
      ToStream(s, #name);                                                      \
    }

#include "AutoClass_Generated.h"

#define AUTOCLASS_IMPL(name, base,                                             \
     T0,  n0,  T1,  n1,  T2,  n2,  T3,  n3,                                    \
     T4,  n4,  T5,  n5,  T6,  n6,  T7,  n7,                                    \
     T8,  n8,  T9,  n9, T10, n10, T11, n11,                                    \
    T12, n12, T13, n13, T14, n14, T15, n15,                                    \
    x, ...)                                                                    \
  MACRO_IDENTITY(AUTOCLASS##x(name, base,                                      \
     T0,  n0,  T1,  n1,  T2,  n2,  T3,  n3,                                    \
     T4,  n4,  T5,  n5,  T6,  n6,  T7,  n7,                                    \
     T8,  n8,  T9,  n9, T10, n10, T11, n11,                                    \
    T12, n12, T13, n13, T14, n14, T15, n15))

#define AutoClass(name, ...)                                                   \
  MACRO_IDENTITY(AUTOCLASS_IMPL(name, EmptyBase, __VA_ARGS__,                  \
    32, 31, 30, 29, 28, 27, 26, 25,                                            \
    24, 23, 22, 21, 20, 19, 18, 17,                                            \
    16, 15, 14, 13, 12, 11, 10,  9,                                            \
     8,  7,  6,  5,  4,  3,  2,  1))

#define AutoClassDerived(name, base, ...)                                      \
  MACRO_IDENTITY(AUTOCLASS_IMPL(name, base, __VA_ARGS__,                       \
    32, 31, 30, 29, 28, 27, 26, 25,                                            \
    24, 23, 22, 21, 20, 19, 18, 17,                                            \
    16, 15, 14, 13, 12, 11, 10,  9,                                            \
     8,  7,  6,  5,  4,  3,  2,  1))

#define AutoClassEmpty(name)                                                   \
  MACRO_IDENTITY(AUTOCLASS0(name, EmptyBase))

#define AutoClassDerivedEmpty(name, base)                                      \
  MACRO_IDENTITY(AUTOCLASS0(name, base))

#endif
