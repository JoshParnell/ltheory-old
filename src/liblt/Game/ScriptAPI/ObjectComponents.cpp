#include "Component/Account.h"
#include "Component/Affectable.h"
#include "Component/Asset.h"
#include "Component/Assets.h"
#include "Component/Attachable.h"
#include "Component/BoundingBox.h"
#include "Component/Cargo.h"
#include "Component/Collidable.h"
#include "Component/Crew.h"
#include "Component/Cullable.h"
#include "Component/Damager.h"
#include "Component/Database.h"
#include "Component/Detectable.h"
#include "Component/Dockable.h"
#include "Component/Drawable.h"
#include "Component/Economy.h"
#include "Component/Explodable.h"
#include "Component/History.h"
#include "Component/Integrity.h"
#include "Component/Interior.h"
#include "Component/Log.h"
#include "Component/Market.h"
#include "Component/Mineable.h"
#include "Component/MissionBoard.h"
#include "Component/Missions.h"
#include "Component/Motion.h"
#include "Component/MotionControl.h"
#include "Component/Nameable.h"
#include "Component/Navigable.h"
#include "Component/Orders.h"
#include "Component/Orientation.h"
#include "Component/Pilotable.h"
#include "Component/Pluggable.h"
#include "Component/Projects.h"
#include "Component/ProximityTracker.h"
#include "Component/Queryable.h"
#include "Component/Resources.h"
#include "Component/Scriptable.h"
#include "Component/Sockets.h"
#include "Component/Storage.h"
#include "Component/Supertyped.h"
#include "Component/Targets.h"
#include "Component/Tasks.h"
#include "Component/Zoned.h"

#include "LTE/Function.h"

#if 1
#define X(x)                                                                   \
  FreeFunction(Component##x*, Object_GetComponent##x,                          \
    "Return the " #x " component of 'object'",                                 \
    Object, object)                                                            \
  {                                                                            \
    return object->Get##x();                                                   \
  } FunctionAlias(Object_GetComponent##x, GetComponent##x);                    \
                                                                               \
  FreeFunction(bool, Object_HasComponent##x,                                   \
    "Return whether 'object' has a " #x " component",                          \
    Object, object)                                                            \
  {                                                                            \
    return object->Get##x() != nullptr;                                        \
  } FunctionAlias(Object_HasComponent##x, HasComponent##x);
#define Y(x, y) X(x)
COMPONENT_X
#undef X
#undef Y
#endif
