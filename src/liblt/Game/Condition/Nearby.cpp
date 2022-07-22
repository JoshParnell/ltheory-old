#include "../Conditions.h"

#include "Game/Object.h"

bool Condition_Nearby(Object const& a, Object const& b, Distance d) {
  if (a->GetContainer() != b->GetContainer())
    return false;
  double totalRadius = a->GetRadius() + b->GetRadius();
  return Length(a->GetCenter() - b->GetCenter()) <= (totalRadius + d);
}
