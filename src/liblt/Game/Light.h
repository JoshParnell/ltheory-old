#ifndef Light_h__
#define Light_h__

#include "Object.h"
#include "ObjectWrapper.h"

#include "Component/Attachable.h"
#include "Component/Orientation.h"
#include "LTE/Color.h"

typedef ObjectWrapper
  < Component_Attachable
  < Component_Orientation
  < ObjectWrapperTail<ObjectType_Light>
  > > >
  LightBaseT;

struct Light : public LightBaseT {
  typedef Light SelfType;
  Color color;
  float radius;
  float visibility;
  bool flare;

  Light() :
    color(1),
    radius(1),
    visibility(1),
    flare(true)
    {}

  FIELDS {
    MAPFIELD(color)
    MAPFIELD(radius)
    LightBaseT::MapFields(type, addr, m, aux);
  }
};

LT_API LightRef Light_Create(ObjectT* parent);

#endif
