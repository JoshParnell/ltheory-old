#include "Object.h"

#include "AI/Traits.h"

#include "Component/Attachable.h"
#include "Component/Asset.h"
#include "Component/BoundingBox.h"
#include "Component/Cargo.h"
#include "Component/Drawable.h"
#include "Component/Integrity.h"
#include "Component/Interior.h"
#include "Component/Motion.h"
#include "Component/Nameable.h"
#include "Component/Orientation.h"
#include "Component/Pilotable.h"
#include "Component/Sockets.h"

#include "LTE/Iterator.h"
#include "LTE/Matrix.h"
#include "LTE/Math.h"
#include "LTE/StackFrame.h"
#include "LTE/Static.h"

#include "UI/Glyphs.h"
#include "UI/Widget.h"

const float kDefaultSimulationFrequency = 60;
const float kDislikeThreshold = -0.5f;
const float kLikeThreshold = 0.5f;
const float kTorqueRatio = 1.0f / 32.0f;

ObjectT::ObjectT() : deleted(false) {
  Data& nextObjectID = Static_Get("nextObjectID");
  if (!nextObjectID)
    nextObjectID = ObjectID(0);
  id = nextObjectID.Convert<ObjectID>()++;
}

ObjectT::~ObjectT() {
}

bool ObjectT::CanMove() const {
  return GetContainer()->GetInterior()->allowMovement;
}

void ObjectT::Delete() {
  if (!deleted) {
    deleted = true;
    LIST_ITERATE(Object, children, nextSibling) {
      it->Delete();
      it->parent = nullptr;
      it.Remove();
    }
  }
}

Capability ObjectT::GetCapability() const {
  Pointer<ItemT> type = GetSupertype();
  return type ? type->GetCapability() : Capability();
}

Icon ObjectT::GetIcon() const {
  static Icon icon = Icon_Create()
    ->Add(Glyph_Circle(0, 1, 1, 1));
  return icon;
}

Widget ObjectT::GetWidget(Player const& self) {
  return nullptr;
}

Pointer<ObjectT> ObjectT::GetRoot() {
  ObjectT* o = this;
  while (o && o->parent && !o->GetAsset())
    o = o->parent;
  return o;
}

Pointer<ObjectT const> ObjectT::GetRoot() const {
  return (ObjectT const*)(((ObjectT*)this)->GetRoot());
}

Traits const& ObjectT::GetTraits() const {
  static Traits traits;
  return traits;
}

Quantity ObjectT::GetValue() const {
  Pointer<ItemT> type = GetSupertype();
  return type ? type->GetValue() : 0;
}

void ObjectT::OnCreate() {
  LIST_ITERATE(Object, children, nextSibling)
    it->OnCreate();
}

void ObjectT::OnDeath() {
  LIST_ITERATE(Object, children, nextSibling)
    it->OnDeath();
}

void ObjectT::OnDestroy() {
  LIST_ITERATE(Object, children, nextSibling)
    it->OnDestroy();
}

void ObjectT::Update(UpdateState& state) {
  OnUpdate(state);

  LIST_ITERATE(Object, children, nextSibling) {
    if (!it->IsDeleted())
      FRAME(it->GetTypeString())
        it->Update(state);

    if (it->IsDeleted())
       it.Remove();
  }
}

Player const& ObjectT::GetOwner() const {
  static Player nullPlayer;
  return parent ? parent->GetOwner() : nullPlayer;
}

Position ObjectT::GetCenter() const {
  return GetTransform().TransformPoint(GetLocalBound().GetCenter());
}

V3 ObjectT::GetExtent() const {
  return GetLocalBound().GetSideLengths() * GetScale();
}

float ObjectT::GetRadius() const {
  return 0.5f * Length(GetExtent());
}

Position ObjectT::GetDockLocation(Object const& docker) const {
  Position center = docker->GetCenter();
  Transform const& transform = GetTransform();
  Vector<Bound3> const* docks = GetSupertype()->GetDocks();
  LTE_ASSERT(docks);

  Position closestPoint = transform.TransformPoint(docks->get(0).GetCenter());
  Distance closestDistance = Length(closestPoint - center);
  for (size_t i = 1; i < docks->size(); ++i) {
    Distance d = Length(transform.TransformPoint(docks->get(i).GetCenter()) - center);
    Position point = transform.TransformPoint(docks->get(i).GetCenter());
    if (d < closestDistance) {
      closestDistance = d;
      closestPoint = point;
    }
  }
  return closestPoint;
}

Bound3 ObjectT::GetLocalBound() const {
  ComponentDrawable const* d = GetDrawable();
  if (!d)
    return Bound3(1);

  if (!d->renderable)
    return Bound3(0);

  /* CRITICAL - Will force loading of assets in other systems :/ */
  return d->renderable()->GetBound();
}

Bound3D ObjectT::GetGlobalBound() const {
  ComponentBoundingBox const* bb = GetBoundingBox();
  LTE_ASSERT(bb);
  ((ComponentBoundingBox*)bb)->Recompute(this);
  return bb->worldBox;
}

Renderable ObjectT::GetRenderable() const {
  return nullptr;
}

float ObjectT::GetMinRange() const {
  ComponentSockets const* socks = GetSockets();
  if (!socks)
    return 0;

  float range = 0;
  for (size_t i = 0; i < socks->instances.size(); ++i) {
    if (socks->instances[i]) {
      float thisRange = socks->instances[i]->GetRange();
      if (thisRange > 0)
        range = Min(range, thisRange);
    }
  }
  return range;
}

float ObjectT::GetMaxRange() const {
  ComponentSockets const* socks = GetSockets();
  if (!socks)
    return 0;

  float range = 0;
  for (size_t i = 0; i < socks->instances.size(); ++i)
    if (socks->instances[i])
      range = Max(range, socks->instances[i]->GetRange());
  return range;
}

Pointer<ObjectT> ObjectT::GetContainerRoot() const {
  ObjectT* context = GetContainer();
  return context ? context->GetContainerRoot().t : (ObjectT*)this;
}

Pointer<ObjectT const> ObjectT::GetRegion() const {
  ObjectT* context = GetContainer();
  while (context && context->GetType() != ObjectType_Region)
    context = context->GetContainer();
  return context ? context : nullptr;
}

Pointer<ObjectT const> ObjectT::GetSystem() const {
  ObjectT* context = GetContainer();
  while (context && context->GetType() != ObjectType_System)
    context = context->GetContainer();
  return context ? context : nullptr;
}

Pointer<Universe const> ObjectT::GetUniverse() const {
  ObjectT* context = GetContainer();
  while (context && context->GetType() != ObjectType_Universe)
    context = context->GetContainer();
  return context ? (Universe const*)context : nullptr;
}

Damage ObjectT::ApplyDamage(Damage damage) {
  Damage remaining = damage;
  ComponentSockets* socks = GetSockets();

  if (socks) {
    /* First, damage the shields. */
    for (size_t i = 0; i < socks->instances.size(); ++i)
      if (socks->instances[i] &&
          socks->instances[i]->GetType() == ObjectType_Shield)
        remaining = socks->instances[i]->ApplyDamage(remaining);

    /* TODO : Small chance of sockets absorbing some damage? */
  }

  ComponentIntegrity* integrity = GetIntegrity();
  if (integrity && damage > 0)
    remaining = integrity->ApplyDamage(this, remaining);

  /* We return the amount of damage that is left undone. */
  return remaining;
}

Health ObjectT::GetTotalHealth() const {
  Health health = 0;
  ComponentIntegrity const* integrity = GetIntegrity();
  health += integrity ? integrity->health : 0;

  ComponentSockets const* socks = GetSockets();
  if (socks)
    for (size_t i = 0; i < socks->instances.size(); ++i)
      if (socks->instances[i])
        health += socks->instances[i]->GetTotalHealth();
  return health;
}

/* TODO : Automate the act of recursively summing an attribute over all
 *        child nodes. */
Health ObjectT::GetTotalMaxHealth() const {
  Health health = 0;
  ComponentIntegrity const* integrity = GetIntegrity();
  health += integrity ? integrity->maxHealth : 0;

  /* TODO : How to clean up type dependency? */
  ComponentSockets const* socks = GetSockets();
  if (socks)
    for (size_t i = 0; i < socks->instances.size(); ++i)
      if (socks->instances[i] &&
          socks->instances[i]->GetType() == ObjectType_Shield)
        health += socks->instances[i]->GetTotalMaxHealth();
  return health;
}

void ObjectT::Broadcast(Data& message) {
  Send(message);
  LIST_ITERATE(Object, children, nextSibling)
    it->Broadcast(message);
}

float ObjectT::GetImpactTime(
  ObjectT const* source,
  float speed,
  Position& hitPoint) const
{
  return ComputeImpact(
    source->GetCenter(),
    GetCenter(),
    source->GetRoot()->GetVelocity(),
    GetRoot()->GetVelocity(),
    speed,
    &hitPoint);
}

Transform const& ObjectT::GetLocalTransform() const {
  static Transform identity = Transform_Identity();
  ComponentAttachable const* at = GetAttachable();
  ComponentOrientation const* ori = GetOrientation();
  return at ? at->transform : ori ? ori->GetTransform() : identity;
}

void ObjectT::AddChild(Object const& child) {
  child->parent = this;
  LIST_INSERT(children, nextSibling, child);
}

void ObjectT::Attach(Object const& child, Transform const& transform) {
  AddChild(child);
  child->SetLocalTransform(transform);
}

void ObjectT::RemoveChild(Object const& child) {
  child->parent = nullptr;
  LIST_REMOVE(Object, children, nextSibling, child)
}

Transform const& ObjectT::GetTransform() const {
  static Transform identity = Transform_Identity();
  return identity;
}

V3 ObjectT::GetLook() const {
  return GetTransform().look;
}

V3 ObjectT::GetUp() const {
  return GetTransform().up;
}

V3 ObjectT::GetRight() const {
  return GetTransform().right;
}

Position ObjectT::GetPos() const {
  return GetTransform().pos;
}

V3 ObjectT::GetScale() const {
  return GetTransform().scale;
}

bool ObjectT::Dislikes(Object const& o) const {
  return GetOpinion(o) <= kDislikeThreshold;
}

bool ObjectT::Likes(Object const& o) const {
  return GetOpinion(o) >= kLikeThreshold;
}

float ObjectT::GetMaxTorque() const {
  return GetMaxThrust() * kTorqueRatio;
}

bool ObjectT::InRange(
  ObjectT const* other,
  Distance distance,
  bool precise) const
{
  Position p1 = precise ? GetCenter() : GetPos();
  Position p2 = precise ? other->GetCenter() : other->GetPos();
  Distance d2 = LengthSquared(p2 - p1);
  if (precise) {
    Distance d = Sqrt(d2) - (GetRadius() + other->GetRadius());
    return d <= distance;
  } else {
    return d2 <= distance*distance;
  }
}
