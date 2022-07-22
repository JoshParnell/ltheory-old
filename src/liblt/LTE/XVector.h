/* XNAME - Desired name of the class
 * XLIST - X list of (numeric) fields
 * XSIZE - Number of (numeric) fields
 * XTYPE - Numeric type of field (float, double, int) */

#include "AutoClass.h"
#include "Vec.h"

#define BASE_NAME_2(x) x##Base
#define BASE_NAME(x) BASE_NAME_2(x)

#define NAME_STRING_2(x) #x
#define NAME_STRING(x) NAME_STRING_2(x)

#define STORAGE_NAME_2(x) x##Storage
#define STORAGE_NAME(x) STORAGE_NAME_2(x)

struct STORAGE_NAME(XNAME) : public NullBase<STORAGE_NAME(XNAME)> {
  typedef STORAGE_NAME(XNAME) SelfType;

#define X(x) XTYPE x;
  XLIST
#undef X

  STORAGE_NAME(XNAME)() :
    NullBase<SelfType>()
#define X(x) , x(0)
    XLIST
#undef X
    {}

  XTYPE& operator[](int index) {
    return ((XTYPE*)this)[index];
  }

  XTYPE const& operator[](int index) const {
    return ((XTYPE const*)this)[index];
  }

  size_t size() const {
    return XSIZE;
  }

  FIELDS {
    SelfType* self = (SelfType*)addr;
    static Type type1 = Type_Get(*(XTYPE*)0);
#define X(x) m(&self->x, #x, type1, aux);
    XLIST
#undef X
  }
};

typedef Vec<XTYPE, XSIZE, STORAGE_NAME(XNAME)> BASE_NAME(XNAME);

struct XNAME : public BASE_NAME(XNAME) {
  typedef XNAME SelfType;
  typedef BASE_NAME(XNAME) BaseType;

  XNAME& operator=(BASE_NAME(XNAME) const& other) {
    *((BASE_NAME(XNAME)*)this) = other;
    return *this;
  }

  template <class StreamT>
  friend void _ToStream(StreamT& s, XNAME const& t) {
    ToStream(s, NAME_STRING(XNAME) "(");
    for (int i = 0; i < XSIZE; ++i) {
      if (i)
        ToStream(s, ',');
      ToStream(s, t[i]);
    }
    ToStream(s, ')');
  }

  FIELDS {
    BASE_NAME(XNAME)::MapFields(type, addr, m, aux);
  }

  DefineMetadataInline(XNAME)
};

#undef BASE_NAME
#undef BASE_NAME_2

#undef NAME_STRING
#undef NAME_STRING_2

#undef STORAGE_NAME
#undef STORAGE_NAME_2
#undef STORAGE_NAME
#undef STORAGE_NAME_2
