#ifndef Game_Template_h__
#define Game_Template_h__

#include "Common.h"
#include "Item.h"
#include "LTE/AutoClass.h"

AutoClassDerived(TemplateT, RefCounted,
  String, name,
  Item, item,
  Vector<Item>, hardpoints,
  Vector<ItemQuantity>, cargo)

  TemplateT() {}
};

#endif
