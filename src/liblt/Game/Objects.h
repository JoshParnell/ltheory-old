#ifndef Game_Objects_h__
#define Game_Objects_h__

#include "Object.h"
#include "ObjectWrapper.h"
#include "Item.h"

#include "LTE/Color.h"
#include "LTE/DeclareFunction.h"
#include "LTE/Distribution.h"
#include "LTE/SDF.h"

DeclareFunction(Object_Asteroid, Object,
  uint, seed)

DeclareFunction(Object_AsteroidRich, Object,
  uint, seed,
  Item, resource,
  Quantity, quantity)

DeclareFunction(Object_Colony, Object,
  uint, seed,
  Item, type,
  Object, planet,
  Quantity, population)

DeclareFunction(Object_Custom, Object,
  Data, data)

DeclareFunctionNoParams(Object_DustFlecks, Object)

LT_API Object Object_Dynamic(Generic<Renderable, void> const& renderable);

DeclareFunction(Object_Explosion, Object,
  ExplosionType, type,
  float, age,
  float, duration)

DeclareFunction(Object_Light, Object,
  Color, color,
  float, radius,
  bool, lensFlare)

DeclareFunction(Object_Missile, Object,
  V3, thrust,
  V3, velocity,
  Object, target,
  V3, targetOffset)

DeclareFunction(Object_Pod, Object,
  Mass, capacity)

DeclareFunction(Object_Payload, Object,
  Object, source,
  Item, payload,
  Position,  position,
  V3, thrust,
  V3, velocity)

DeclareFunction(Object_Planet, Object,
  Item, type)

DeclareFunction(Object_PowerGenerator, Object,
  Item, type)

DeclareFunction(Object_ProductionLab, Object,
  Item, type)

LT_API Object Object_Pulse(
  V3 const& velocity,
  V3 const& drift,
  float width);

LT_API Object Object_Rail(
  Position const& origin,
  V3 const& direction,
  V3 const& velocity);

DeclareFunctionArgBind(Object_Region, Object,
  int, level,
  Position, pos,
  float, radius,
  Distribution<Item>, resources,
  uint, seed)

typedef Object_Region_Args RegionType;

DeclareFunction(Object_Scanner, Object,
  Item, type)

DeclareFunction(Object_Shield, Object,
  Item, type)

DeclareFunction(Object_Ship, Object,
  Item, type)

DeclareFunction(Object_SoundEmitter, Object,
  String, filename,
  Position, position,
  float, volume,
  float, distanceDiv)

DeclareFunction(Object_Star, Object,
  Color, color)

DeclareFunction(Object_Station, Object,
  Item, type)

LT_API Object Object_Static(Generic<Renderable, void> const& renderable);

DeclareFunctionArgBind(Object_System, Object,
  Position, position,
  uint, seed)

typedef Object_System_Args SystemType;

DeclareFunction(Object_TechLab, Object,
  Item, type)

LT_API Object Object_Thruster(Item const& type, ObjectT* parent);

LT_API Object Object_Trail(
  Object const& parent,
  int length,
  Color const& color,
  float size);

DeclareFunction(Object_TransferUnit, Object,
  Item, type)

DeclareFunction(Object_Turret, Object,
  Item, type)

DeclareFunction(Object_Universe, Object,
  uint, seed,
  uint, depth)

DeclareFunctionNoParams(Object_WarpNode, Object)

DeclareFunction(Object_WarpRail, Object,
  Object, node1,
  Object, node2)

DeclareFunction(Object_Weapon, Object,
  Item, type)

DeclareFunctionNoParams(Object_Wormhole, Object)

LT_API void Object_Wormholes(ObjectT* a, ObjectT* b);

DeclareFunction(Object_Zone, Object,
  Object, parent,
  uint, seed,
  Position, position,
  V3, scale,
  SDF, shape,
  uint, statics,
  float, asteroids,
  float, gas,
  float, ice,
  float, planet)

#endif
