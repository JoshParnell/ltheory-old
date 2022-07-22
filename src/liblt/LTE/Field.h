#ifndef LTE_Field_h__
#define LTE_Field_h__

#include "String.h"
#include "Type.h"

struct Field {
  String name;
  Type type;
  size_t offset;

  Field() :
    type(0),
    offset(0)
    {}

  Field(String const& name, Type const& type, size_t offset) :
    name(name),
    type(type),
    offset(offset)
    {}
};

#endif
