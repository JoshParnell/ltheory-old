#ifndef Game_Mission_h__
#define Game_Mission_h__

#include "Common.h"
#include "Game/ItemProperty.h"

#include "LTE/AutoClass.h"
#include "LTE/BaseType.h"
#include "LTE/DeclareFunction.h"
#include "LTE/Pool.h"
#include "LTE/Reference.h"
#include "LTE/String.h"
#include "LTE/Vector.h"

struct MissionConstraintT : public RefCounted {
  BASE_TYPE(MissionConstraintT)

  virtual double Evaluate(Item const&) const = 0;

  virtual String GetDescription() const = 0;
};

LT_API MissionConstraint MissionConstraint_Equality(
  ItemProperty const& property,
  Data const& value);

AutoClassDerived(MissionT, RefCounted,
  Object, owner,
  Quantity, pool,
  Quantity, filled,
  double, price,
  ItemProperty, unit,
  ItemType, type,
  Vector<MissionConstraint>, constraints,
  String, name,
  String, description,
  bool, deleted)
  POOLED_TYPE

  MissionT() :
    deleted(false)
    {}

  Quantity GetValue(Item const& data) const {
    if (data->GetType() != type)
      return 0;

    double mult = 1.0;
    for (size_t i = 0; i < constraints.size(); ++i)
      mult *= constraints[i]->Evaluate(data);
    double unitValue = unit ? unit->Evaluate(data).CastReal() : 1.0;
    return (Quantity)(mult * price * unitValue);
  }
};

DeclareFunction(Mission_Create,
  Mission,
  Object, owner)

#endif
