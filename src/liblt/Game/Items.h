#ifndef Game_Items_h__
#define Game_Items_h__

#include "ItemWrapper.h"
#include "Capability.h"
#include "Task.h"

#include "LTE/DeclareFunction.h"

#include "UI/Icon.h"

/* Attributes -
 * value - Overall, how 'good' is the item?
 * seed - Number for randomizing other generated parameters.
 *
 * capacity - How much internal mass can the item hold?
 * compactness - What's the mass of the item?
 * damage - How much damage does the item inflict?
 * efficiency - How much power does the item draw?
 * integrity - How much health does the item have?
 * rate - How quickly does the item perform it's task?
 * speed - How fast is the item? */

DeclareFunctionArgBind(Item_AssemblyChip, Item,
  Item, blueprint,
  Item, source)

LT_API Item Item_Blueprint(DataRef const& properties);

LT_API Item Item_Blueprint_Derived(Reference<Blueprint> const& source);

DeclareFunction(Item_ColonyType, Item,
  String, name,
  Icon, icon,
  Task, task,
  Traits, traits)

LT_API Item Item_Commodity(int id);

LT_API Item Item_Data_Damaged(Object const& object);

LT_API Item Item_Data_Destroyed(Object const& object);

DeclareFunctionArgBind(Item_DroneBayType, Item,
  double, value,
  uint, seed,

  float, compactness,
  float, speed)

inline Item Item_DroneBayType(double value, uint seed) {
  return Item_DroneBayType(value, seed, 1, 1);
}

typedef Item_DroneBayType_Args Meta_DroneBayType;

DeclareFunctionArgBind(Item_DroneConstructionType, Item,
  double, value,
  uint, seed)

DeclareFunctionArgBind(Item_DroneProspectingType, Item,
  double, value,
  uint, seed)

DeclareFunction(Item_PlanetType,
  Item,
  uint, seed)

DeclareFunctionArgBind(Item_ProductionLabType, Item,
  double, value,
  uint, seed,

  float, compactness,
  float, efficiency,
  float, rate)

inline Item Item_ProductionLabType(double value, uint seed) {
  return Item_ProductionLabType(value, seed, 1, 1, 1);
}

typedef Item_ProductionLabType_Args Meta_ProductionLabType;

#if 0
DeclareFunction(Item_PulseType, Item,
  double, value,
  uint, seed,

  float, compactness,
  float, damage,
  float, efficiency,
  float, integrity,
  float, rate,
  float, speed)

typedef Item_PulseType_Args Meta_PulseType;
#endif

DeclareFunctionArgBind(Item_OreType, Item,
  double, value,
  uint, seed)

typedef Item_OreType_Args Meta_OreType;

DeclareFunctionArgBind(Item_PowerGeneratorType, Item,
  double, value,
  uint, seed)

typedef Item_PowerGeneratorType_Args Meta_PowerGeneratorType;

DeclareFunctionArgBind(Item_ScannerType, Item,
  double, value,
  uint, seed,

  float, range)

inline Item Item_ScannerType(double value, uint seed) {
  return Item_ScannerType(value, seed, 1);
}

typedef Item_ScannerType_Args Meta_ScannerType;

DeclareFunctionArgBind(Item_ShieldType, Item,
  double, value,
  uint, seed,

  float, compactness,
  float, efficiency,
  float, integrity,
  float, rate)

inline Item Item_ShieldType(double value, uint seed) {
  return Item_ShieldType(value, seed, 1, 1, 1, 1);
}

typedef Item_ShieldType_Args Meta_ShieldType;

DeclareFunctionArgBind(Item_ShipType, Item,
  double, value,
  uint, seed,

  float, capacity,
  float, compactness,
  float, integrity,
  float, propulsion,
  float, systems,
  float, turrets)

inline Item Item_ShipType(double value, uint seed) {
  return Item_ShipType(value, seed, 1, 1, 1, 1, 1, 1);
}

typedef Item_ShipType_Args Meta_ShipType;

DeclareFunctionArgBind(Item_StationType, Item,
  double, value,
  uint, seed,

  float, capacity,
  float, integrity,
  float, systems,
  float, turrets)

inline Item Item_StationType(double value, uint seed) {
  return Item_StationType(value, seed, 1, 1, 1, 1);
}

DeclareFunctionArgBind(Item_TechLabType, Item,
  double, value,
  uint, seed,

  float, compactness,
  float, efficiency,
  float, rate)

inline Item Item_TechLabType(double value, uint seed) {
  return Item_TechLabType(value, seed, 1, 1, 1);
}

typedef Item_TechLabType_Args Meta_TechLabType;

DeclareFunctionArgBind(Item_ThrusterType, Item,
  double, value,
  uint, seed,

  float, compactness,
  float, efficiency,
  float, integrity,
  float, rate)

inline Item Item_ThrusterType(double value, uint seed) {
  return Item_ThrusterType(value, seed, 1, 1, 1, 1);
}

typedef Item_ThrusterType_Args Meta_ThrusterType;

DeclareFunctionArgBind(Item_TransferUnitType, Item,
  double, value,
  uint, seed,

  float, compactness,
  float, efficiency,
  float, range,
  float, rate)

typedef Item_TransferUnitType_Args Meta_TransferUnitType;

DeclareFunctionArgBind(Item_TurretType, Item,
  uint, sockets,
  float, trackingSpeed)

typedef Item_TurretType_Args Meta_TurretType;

DeclareFunction(Item_WeaponType, Item, int, id)

DeclareFunctionArgBind(Item_Worker_Engineer, Item,
  uint, level,
  Item, nextLevel)

DeclareFunctionArgBind(Item_Worker_Miner, Item,
  uint, level,
  Item, nextLevel)

DeclareFunctionArgBind(Item_Worker_Pilot, Item,
  uint, level,
  Item, nextLevel)

#endif
