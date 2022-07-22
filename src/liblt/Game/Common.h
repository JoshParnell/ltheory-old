#ifndef Game_Common_h__
#define Game_Common_h__

#include "LTE/Common.h"
#include "LTE/Enum.h"

struct Capability;
struct EventT;
struct ItemT;
struct ItemPropertyT;
struct Message;
struct MissionT;
struct MissionConstraintT;
struct ObjectT;
struct OrderT;
struct ProjectT;
struct ResourceT;
struct Signature;
struct Socket;
struct TaskT;
struct TaskInstance;
struct TemplateT;
struct UpdateState;

typedef Reference<struct ActionT> Action;
typedef Reference<struct CameraT> Camera;
typedef Reference<struct EventT> Event;
typedef Reference<struct ItemT> Item;
typedef Reference<struct ItemPropertyT> ItemProperty;
typedef Reference<struct Light> LightRef;
typedef Reference<struct MissionT> Mission;
typedef Reference<struct MissionConstraintT> MissionConstraint;
typedef Reference<struct ObjectT> Object;
typedef Reference<struct OrderT> Order;
typedef Reference<struct PlayerT> Player;
typedef Reference<struct ProjectT> Project;
typedef Reference<struct ResourceT> Resource;
typedef Reference<struct TaskT> Task;
typedef Reference<struct TemplateT> Template;

typedef V3T<DistanceT> Position;
typedef DistanceT Distance;
typedef RayT<PointT, V3> WorldRay;
typedef MatrixT<DistanceT> WorldMatrix;

typedef int32 Damage;
typedef int32 Health;
typedef int64 ItemID;
typedef float Mass;
typedef int64 ObjectID;
typedef int64 Quantity;
typedef int64 Time;

#define CAPABILITY_X                                                           \
  X(Attack)                                                                    \
  X(Defense)                                                                   \
  X(Detection)                                                                 \
  X(Motion)                                                                    \
  X(Power)                                                                     \
  X(Production)                                                                \
  X(Research)                                                                  \
  X(Storage)                                                                   \
  X(Transference)

#define XTYPE CapabilityType
#define XLIST CAPABILITY_X
#include "LTE/XEnum.h"
#undef XLIST
#undef XTYPE

#define COMPONENT_X                                                            \
  X(Account)                                                                   \
  X(Affectable)                                                                \
  X(Asset)                                                                     \
  X(Assets)                                                                    \
  X(Attachable)                                                                \
  X(BoundingBox)                                                               \
  X(Cargo)                                                                     \
  X(Collidable)                                                                \
  X(Crew)                                                                      \
  X(Cullable)                                                                  \
  X(Damager)                                                                   \
  X(Database)                                                                  \
  X(Detectable)                                                                \
  X(Dockable)                                                                  \
  X(Drawable)                                                                  \
  X(Economy)                                                                   \
  X(Explodable)                                                                \
  X(History)                                                                   \
  X(Integrity)                                                                 \
  X(Interior)                                                                  \
  X(Log)                                                                       \
  X(Market)                                                                    \
  X(Mineable)                                                                  \
  X(MissionBoard)                                                              \
  X(Missions)                                                                  \
  X(Motion)                                                                    \
  X(MotionControl)                                                             \
  X(Nameable)                                                                  \
  X(Navigable)                                                                 \
  X(Opinions)                                                                  \
  X(Orders)                                                                    \
  X(Orientation)                                                               \
  X(Pilotable)                                                                 \
  X(Pluggable)                                                                 \
  X(Projects)                                                                  \
  X(ProximityTracker)                                                          \
  X(Queryable)                                                                 \
  X(Resources)                                                                 \
  X(Scriptable)                                                                \
  X(Sockets)                                                                   \
  X(Storage)                                                                   \
  X(Supertyped)                                                                \
  X(Targets)                                                                   \
  X(Tasks)                                                                     \
  X(Zoned)

#define XTYPE ComponentType
#define XLIST COMPONENT_X
#include "LTE/XEnum.h"
#undef XTYPE
#undef XLIST

#define ITEM_X                                                                 \
  Z(AssemblyChip, AssemblyChip, "Assembly Chip")                               \
  Z(Blueprint, Blueprint, "Blueprint")                                         \
  Z(ColonyType, ColonyType, "Colony")                                          \
  Z(Commodity, Commodity, "Commodity")                                         \
  Z(DataDamaged, DataDamaged, "Data")                                          \
  Z(DataDiscovered, DataDiscovered, "Data")                                    \
  Z(DataDestroyed, DataDestroyed, "Data")                                      \
  Z(DroneBayType, DroneBayType, "Drone Bay")                                   \
  Z(DroneType, DroneType, "Drone")                                             \
  Z(JumpDriveType, JumpDriveType, "Jump Drive")                                \
  Z(OreType, OreType, "Ore")                                                   \
  Z(PlanetType, PlanetType, "Planet")                                          \
  Z(PowerGeneratorType, PowerGeneratorType, "Power Generator")                 \
  Z(ProductionLabType, ProductionLabType, "Production Lab")                    \
  Z(ScannerType, ScannerType, "Scanner")                                       \
  Z(ShieldType, ShieldType, "Shield")                                          \
  Z(ShipType, ShipType, "Ship")                                                \
  Z(StationType, StationType, "Station")                                       \
  Z(TechLabType, TechLabType, "Tech Lab")                                      \
  Z(ThrusterType, ThrusterType, "Thruster")                                    \
  Z(TransferUnitType, TransferUnitType, "Transfer Unit")                       \
  Z(TurretType, TurretType, "Turret")                                          \
  Z(WeaponType, WeaponType, "Weapon")                                          \
  Z(Worker, Worker, "Work")

#define XTYPE ItemType
#define XLIST ITEM_X
#include "LTE/XEnum.h"
#undef XTYPE
#undef XLIST

#define ITEMPROPERTY_X                                                         \
  X(Capability, Capability, Capability())                                      \
  X(Color, Color, 0)                                                           \
  X(String, Description, "")                                                   \
  X(HashT, Hash, (HashT)(id % HASHT_MAX))                                      \
  X(Icon, Icon, nullptr)                                                       \
  X(Health, Integrity, 0)                                                      \
  X(Mass, Mass, 0)                                                             \
  X(Data, Metatype, Data())                                                    \
  X(String, Name, "")                                                          \
  X(Object, Object, nullptr)                                                   \
  X(float, PowerDrain, 0)                                                      \
  X(float, Range, 0)                                                           \
  X(float, Rate, 1)                                                            \
  X(Renderable, Renderable, nullptr)                                           \
  X(float, Scale, 1)                                                           \
  X(uint, Seed, 0)                                                             \
  X(float, Speed, 0)                                                           \
  X(StorageType, StorageType, StorageType_Cargo)                               \
  X(Task, Task, nullptr)                                                       \
  X(Traits, Traits, Traits())                                                  \
  X(ItemType, Type, ItemType_SIZE)                                             \
  X(Quantity, Value, 0)

#define OBJECT_X                                                               \
  Z(Asteroid, Asteroid, "Asteroid")                                            \
  Z(AsteroidRich, AsteroidRich, "Asteroid")                                    \
  Z(Beam, Beam, "Beam")                                                        \
  Z(Colony, Colony, "Colony")                                                  \
  Z(Custom, Custom, "Custom")                                                  \
  Z(Drone, Drone, "Drone")                                                     \
  Z(DroneBay, DroneBay, "Drone Bay")                                           \
  Z(Dynamic, Dynamic, "Dynamic")                                               \
  Z(Effect, Effect, "Effect")                                                  \
  Z(Explosion, Explosion, "Explosion")                                         \
  Z(JumpDrive, JumpDrive, "Jump Drive")                                        \
  Z(Light, Light, "Light")                                                     \
  Z(Missile, Missile, "Missile")                                               \
  Z(Payload, Payload, "Payload")                                               \
  Z(Planet, Planet, "Planet")                                                  \
  Z(PlayerT, Player, "Player")                                                 \
  Z(Pod, Pod, "Pod")                                                           \
  Z(PowerGenerator, PowerGenerator, "Power Generator")                         \
  Z(ProductionLab, ProductionLab, "Production Lab")                            \
  Z(Pulse, Pulse, "Pulse")                                                     \
  Z(Rail, Rail, "Rail")                                                        \
  Z(Region, Region, "Region")                                                  \
  Z(Scanner, Scanner, "Scanner")                                               \
  Z(Shield, Shield, "Shield")                                                  \
  Z(Ship, Ship, "Ship")                                                        \
  Z(SoundEmitter, SoundEmitter, "Sound Emitter")                               \
  Z(Star, Star, "Star")                                                        \
  Z(Static, Static, "Static")                                                  \
  Z(Station, Station, "Station")                                               \
  Z(System, System, "System")                                                  \
  Z(TechLab, TechLab, "Tech Lab")                                              \
  Z(Thruster, Thruster, "Thruster")                                            \
  Z(Trail, Trail, "Trail")                                                     \
  Z(TransferUnit, TransferUnit, "Transfer Unit")                               \
  Z(Turret, Turret, "Turret")                                                  \
  Z(Universe, Universe, "Universe")                                            \
  Z(Unknown, Unknown, "Unknown")                                               \
  Z(WarpNode, WarpNode, "Warp Node")                                           \
  Z(WarpRail, WarpRail, "Warp Rail")                                           \
  Z(Weapon, Weapon, "Weapon")                                                  \
  Z(Wormhole, Wormhole, "Wormhole")                                            \
  Z(Zone, Zone, "Zone")                                                        \
  Z(CameraT, Camera, "Camera")

#define XTYPE ObjectType
#define XLIST OBJECT_X
#include "LTE/XEnum.h"
#undef XTYPE
#undef XLIST

#define SOCKET_X                                                               \
  X(Generator)                                                                 \
  X(Interior)                                                                  \
  X(Thruster)                                                                  \
  X(Turret)

#define XTYPE SocketType
#define XLIST SOCKET_X
#include "LTE/XEnum.h"
#undef XTYPE
#undef XLIST

/* STATISTICS ~ NOTE : All statistics are measured in Ru
 * Accumulated ---> Ru of assets (ownership)
 * Befriended  ---> Ru of friends (diplomacy)
 * Controlled  ---> Ru of subordinates (control)
 * Created     ---> Ru created (production)
 * Destroyed   ---> Ru destroyed (attack)
 * Discovered  ---> Ru discovered (exploration)
 * Invented    ---> Ru of technological discoveries (research) */

#define STATS_X                                                                \
  X(Accumulated)                                                               \
  X(Befriended)                                                                \
  X(Controlled)                                                                \
  X(Created)                                                                   \
  X(Destroyed)                                                                 \
  X(Discovered)                                                                \
  X(Invented)

#define XTYPE StatType
#define XLIST STATS_X
#include "LTE/XEnum.h"
#undef XTYPE
#undef XLIST

#define STORAGE_X                                                              \
  X(Cargo)                                                                     \
  X(CrewQuarters)                                                              \
  X(Database)

#define XTYPE StorageType
#define XLIST STORAGE_X
#include "LTE/XEnum.h"
#undef XTYPE
#undef XLIST

#define Z(x, y, z) struct x;
OBJECT_X
#undef Z

#define Z(x, y, z) struct x;
ITEM_X
#undef Z

#define X(x) struct Component##x;
COMPONENT_X
#undef X

#define EXPLOSION_X                                                            \
  X(Fire)                                                                      \
  X(Plasma)

#define XTYPE ExplosionType
#define XLIST EXPLOSION_X
#include "LTE/XEnum.h"
#undef XTYPE
#undef XLIST

#define WEAPONCLASS_X                                                          \
  X(Beam)                                                                      \
  X(Missile)                                                                   \
  X(Pulse)                                                                     \
  X(Rail)

#define XTYPE WeaponClass
#define XLIST WEAPONCLASS_X
#include "LTE/XEnum.h"
#undef XTYPE
#undef XLIST

typedef unsigned char InfoLevelT;
const InfoLevelT InfoLevel_None       = 0; // Never heard of it in any way

const InfoLevelT InfoLevel_Vague      = 1; // Might have heard rumors / vague information about it
const InfoLevelT InfoLevel_Existence  = 2; // Knows that the object exists, vague idea of location

const InfoLevelT InfoLevel_Specific   = 3; // Marks the beginning of anything that should expire
const InfoLevelT InfoLevel_Location   = 3; // Knows where the object is, knows basic properties
const InfoLevelT InfoLevel_Scan       = 4; // Has full scan information on the object
const InfoLevelT InfoLevel_Max        = 4;

LT_API Time Universe_Age();

/* How many time quanta in a second? */
const Time kTimeScale = 1000;

#endif
