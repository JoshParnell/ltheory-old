#include "Dockable.h"

#include "BoundingBox.h"
#include "Motion.h"
#include "Orientation.h"

#include "Game/Object.h"
#include "Game/Player.h"

#include "LTE/Bound.h"
#include "LTE/Math.h"
#include "LTE/Matrix.h"
#include "LTE/Vector.h"

bool ComponentDockable::CanDock(ObjectT* self, Object const& docker) {
  return docked >= capacity;
}

void ComponentDockable::Dock(ObjectT* self, Object const& docker) {
  LTE_ASSERT(!hangars.isEmpty());
  self->AddInterior(docker);

  Position newPos = hangars.random().Sample(RandV3());
  newPos.y -= docker->GetLocalBound().GetExpanded(docker->GetScale()).lower.y;

  docker->SetPos(newPos);
  docker->SetLook(Spherical(1, RandAngle(), 0));
  docker->SetUp(V3(0, 1, 0));

  Pointer<ComponentMotion> motion = docker->GetMotion();
  motion->velocity = 0;
  motion->velocityA = 0;
  motion->force = 0;
  motion->torque = 0;
}

void ComponentDockable::Undock(ObjectT* self, Object const& docker) {
  LTE_ASSERT(!ports.isEmpty());
  self->RemoveInterior(docker);
  self->GetContainer()->AddInterior(docker);

  float extent = docker->GetRadius() * Rand(1.25f, 5.0f);
  Position pos = self->GetTransform().TransformPoint(ports.random().GetCenter());
  V3 toPos = Normalize(docker->GetCenter() - self->GetCenter());
  pos += toPos * extent;
  docker->SetPos(pos);
  docker->SetLook(toPos);
  docker->SetUp(V3(0, 1, 0));
}
