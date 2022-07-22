#ifndef Attribute_Common_h__
#define Attribute_Common_h__

#define ATTRIBUTE_COMMON(x)                                                    \
  FIELDS {                                                                     \
    SelfType* self = (SelfType*)addr;                                          \
    m(&self->x, #x, Type_Get(self->x), aux);                                   \
    T::MapFields(type, addr, m, aux);                                          \
  }

#endif
