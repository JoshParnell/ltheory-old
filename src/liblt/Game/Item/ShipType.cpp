#include "../Items.h"

#include "Game/Constants.h"
#include "Game/Materials.h"
#include "Game/Objects.h"
#include "Game/Attribute/Capability.h"
#include "Game/Attribute/Icon.h"
#include "Game/Attribute/Integrity.h"
#include "Game/Attribute/Mass.h"
#include "Game/Attribute/Metatype.h"
#include "Game/Attribute/Name.h"
#include "Game/Attribute/Renderable.h"
#include "Game/Attribute/Scale.h"
#include "Game/Attribute/Sockets.h"
#include "Game/Attribute/Value.h"

#include "LTE/Math.h"
#include "LTE/Meshes.h"
#include "LTE/Model.h"
#include "LTE/PlateMesh.h"
#include "LTE/RNG.h"
#include "LTE/Ray.h"
#include "LTE/Script.h"
#include "LTE/SDFs.h"
#include "LTE/SDFMesh.h"
#include "LTE/StackFrame.h"

#include "UI/Glyphs.h"

#include "Game/Renderables.h"

const uint kThrusterAttempts = 10;
const uint kTurretAttempts = 100;
const float kThrusterTolerance = 0.8f;

typedef
    Attribute_Capability
  < Attribute_Icon
  < Attribute_Integrity
  < Attribute_Mass
  < Attribute_Metatype
  < Attribute_Name
  < Attribute_Renderable
  < Attribute_Scale
  < Attribute_Sockets
  < Attribute_Value
  < ItemWrapper<ItemType_ShipType>
  > > > > > > > > > >
  ShipTypeBase;

AutoClassDerived(ShipType, ShipTypeBase,
  Item, standardGenerator,
  Item, standardScanner,
  Item, standardThruster)
  DERIVED_TYPE_EX(ShipType)

  ShipType() {}

  Object Instantiate(ObjectT* parent) {
    Object ship = Object_Ship(this);
    while (ship->Plug(standardThruster)) {}
    ship->Plug(standardGenerator);
    ship->Plug(standardScanner);
    return ship;
  }
};

DERIVED_IMPLEMENT(ShipType)

Icon GetIcon(float scale) {
  if (scale <= 2)
    return Icon_Create()->Add(Glyph_Circle(0, 1, 1, 1));

  if (scale <= 5)
    return Icon_Create()->Add(Glyph_Arc(0, 0.10f, 0, 1, 1, 0, 1));

  if (scale <= 10)
    return Icon_Create()
      ->Add(Glyph_Circle(0, 1, 1, 1))
      ->Add(Glyph_Arc(0, 0.10f, 0, 1, 1, 0, 1));

  if (scale <= 20)
    return Icon_Create()
      ->Add(Glyph_Arc(0, 0.5f, 0.1f, 1, 1, 0.25f, 0.1f))
      ->Add(Glyph_Arc(0, 0.05f, 0.05f, 1, 1, 0, 1));

  if (scale <= 40)
    return Icon_Create()
      ->Add(Glyph_Arc(0, 0.5f, 0.05f, 1, 1, 0.25f, 0.08f))
      ->Add(Glyph_Arc(0, 0.5f, 0.05f, 1, 1, 0.75f, 0.08f))
      ->Add(Glyph_Arc(0, 0.05f, 0.05f, 1, 1, 0, 1));
  
  return Icon_Create()
    ->Add(Glyph_Arc(0, 0.5f, 0.05f, 1, 1, 0.00f, 0.08f))
    ->Add(Glyph_Arc(0, 0.5f, 0.05f, 1, 1, 0.25f, 0.08f))
    ->Add(Glyph_Arc(0, 0.5f, 0.05f, 1, 1, 0.50f, 0.08f))
    ->Add(Glyph_Arc(0, 0.5f, 0.05f, 1, 1, 0.75f, 0.08f))
    ->Add(Glyph_Arc(0, 0.05f, 0.05f, 1, 1, 0, 1));
}

V3 GetIntersection(
  Renderable const& renderable,
  V3 const& origin,
  V3 const& target,
  V3* normal = nullptr)
{
  Ray r(origin, target - origin);
  float t;
  return renderable->Intersects(r, &t, normal)
    ? r(t) :
    V3(FLT_MAX);
}

bool AttachThrusterPair(
  Renderable const& renderable,
  Vector<Socket>& sockets,
  V3 const& thrustDir,
  RNG const& rng,
  V3 const& scale)
{ AUTO_FRAME;
  V3 target, origin, normal, inter;
  Bound3 box = renderable->GetBound();

  for (uint i = 0; i < kThrusterAttempts; ++i) {
    target = box.Sample(rng->GetV3(0, 1));
    origin = target + 1000.0f * thrustDir;
    inter = GetIntersection(renderable, origin, target, &normal);
    if (inter.x != FLT_MAX && Dot(normal, thrustDir) >= kThrusterTolerance) {
      V3 look = thrustDir;
      sockets <<
        Socket(Transform_LookUp(inter, look, Cross(look, V3(1, 0, 0))) *
               Transform_Scale(scale), SocketType_Thruster);
      inter.x *= -1;
      normal.x *= -1;
      look.x *= -1;

      sockets <<
        Socket(Transform_LookUp(inter, look, Cross(look, V3(1, 0, 0))) *
               Transform_Scale(scale), SocketType_Thruster);
      return true;
    }
  }

  return false;
}

void AttachTurrets(
  Renderable const& renderable,
  Vector<Socket>& sockets,
  uint num,
  RNG const& rng)
{ AUTO_FRAME;
  /* Affix Turret Sockets. */
  Bound3 bound = renderable->GetBound();
  for (uint i = 0; i < num; ++i) {
    for (uint j = 0; j < kTurretAttempts; ++j) {
      V3 target = bound.Sample(rng->GetV3(0, 1));
      V3 normal = rng->GetFloat() < 0.5f ? V3(0, 1, 0) : V3(0, -1, 0);
      V3 origin = target + 100.0f * normal;
      V3 inter = GetIntersection(renderable, origin, target);

      if (inter.x != FLT_MAX) {
        origin = inter + 0.01f * normal;
        target = origin + V3(0, 0, 100);

        /* Make sure that the line-of-sight doesn't intersect the ship...that
           would be a bad place to put a turret. */
        if (GetIntersection(renderable, origin, target, nullptr).x == FLT_MAX) {
          sockets << Socket(
            Transform_LookUp(inter, V3(0, 0, 1), normal),
            SocketType_Turret,
            JointType::AxisY);

          sockets << Socket(
            Transform_LookUp(inter * V3(-1, 1, 1), V3(0, 0, 1), normal),
            SocketType_Turret,
            JointType::AxisY);
          break;
        }
      }
    }
  }
}

DefineFunction(Item_ShipType) { AUTO_FRAME;
  RNG rng = RNG_MTG(args.seed);

  double valueRemaining = args.value;
  double scannerValue = 1000.0;
  // valueRemaining -= scannerValue;

  double hullValue = 0.6 * valueRemaining;
  valueRemaining -= hullValue;
  double thrusterValue = Saturate(0.5 / Sqrt(Sqrt(valueRemaining / 10000.0))) * valueRemaining;
  valueRemaining -= thrusterValue;
  double generatorValue = valueRemaining;

  Mass capacity = Constant_ValueToCapacity(hullValue, args.capacity);
  Health integrity = Constant_ValueToIntegrity(hullValue, args.integrity);
  Mass mass = Constant_ValueToMass(hullValue, args.compactness);

  Reference<ShipType> self = new ShipType;
  self->capability = Capability_Storage(capacity);
  self->integrity = integrity;
  self->mass = mass;
  self->metatype = Item_ShipType_Args(args);
  self->name = "Ship";
  self->scale = Constant_MassToScale(mass);
  self->value = hullValue;

  self->icon = GetIcon(self->scale);

  float logScale = Log(self->scale);
  int thrusterCount = 2;
  int turretCount = 4;
  int generatorCount = (int)(rng->GetFloat(1, 2) + logScale);
  int interiorCount = 2 * (int)(logScale / Log(10.0f));

  Script_Reload("Item/ShipType/Generate");
  ScriptFunction_Load("Item/ShipType/Generate:Main")
    ->Call(self->renderable, self->scale, (int)rng->GetInt());

  /* Sockets. */ {
    Vector<Socket> sockets;

    /* Create thruster sockets. */
    int actualThrusterCount = 0;
    for (int i = 0; i < thrusterCount; ++i)
      if (AttachThrusterPair(self->renderable, sockets, V3(0, 0, -1), rng, V3(1)))
        actualThrusterCount += 2;

    if (AttachThrusterPair(self->renderable, sockets, V3(1, 0, 0), rng, V3(0.25f)))
      actualThrusterCount += 2;
    if (AttachThrusterPair(self->renderable, sockets, V3(0, 1, 0), rng, V3(0.25f)))
      actualThrusterCount += 2;
    if (AttachThrusterPair(self->renderable, sockets, V3(0,-1, 0), rng, V3(0.25f)))
      actualThrusterCount += 2;
    if (AttachThrusterPair(self->renderable, sockets, V3(0, 0, 1), rng, V3(0.25f)))
      actualThrusterCount += 2;

    /* Default Power Generator. */
    self->standardGenerator = Item_PowerGeneratorType(generatorValue, rng->GetInt());
    sockets << Socket(Transform_Translation(V3(0, 0, 1)), SocketType_Generator);

    /* Default Scanner. */
    self->standardScanner = Item_ScannerType(scannerValue, rng->GetInt());
    sockets << Socket(Transform_Translation(V3(0, 0, -1)), SocketType_Generator);

    /* Thrusters. */
    thrusterValue /= actualThrusterCount;
    self->standardThruster = Item_ThrusterType(thrusterValue, rng->GetInt());

    /* Empty Slots. */
    AttachTurrets(self->renderable, sockets, turretCount, rng);
    for (int i = 0; i < generatorCount; ++i)
      sockets << Socket(Transform(), SocketType_Generator);
    for (int i = 0; i < interiorCount; ++i)
      sockets << Socket(Transform(), SocketType_Interior);
    self->sockets = Array<Socket>(sockets.size(), sockets.data());
  }

  return self;
}
