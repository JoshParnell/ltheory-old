#ifndef LTE_Parameter_h__
#define LTE_Parameter_h__

#include "String.h"

struct Parameter {
  typedef NoBase BaseType;
  typedef Parameter SelfType;

  String name;
  Type type;

  Parameter() {}

  Parameter(String const& name, Type const& type) :
    name(name),
    type(type)
    {}

  FIELDS {
    MAPFIELD(name)
    MAPFIELD(type)
  }

  DefineMetadataInline(Parameter)
};

#endif
