#ifndef Game_Object_h__
#define Game_Object_h__

#include "Capability.h"
#include "Signature.h"
#include "UpdateState.h"

#include "AI/Types.h"

#include "LTE/BaseType.h"
#include "LTE/Data.h"
#include "LTE/Pointer.h"
#include "LTE/Reference.h"
#include "LTE/V3.h"

#include "UI/Common.h"

struct ObjectT : public RefCounted {
  BASE_TYPE_EX(ObjectT)

  ObjectID id;
  Pointer<ObjectT> parent;
  Pointer<ObjectT> container;
  Object nextSibling;
  Object children;
  bool deleted;

  LT_API ObjectT();
  LT_API virtual ~ObjectT();

  /* Common Operations. */
  LT_API bool CanMove() const;

  LT_API void Delete();

  LT_API virtual Capability GetCapability() const;

  LT_API Pointer<ObjectT> GetRoot();

  LT_API Pointer<ObjectT const> GetRoot() const;

  /* Pure. */
  virtual ObjectType GetType() const = 0;

  /* Inline. */
  Pointer<ObjectT> GetContainer() const {
    if (container)
      return container;
    return parent ? parent->GetContainer() : nullptr;
  }

  HashT GetHash() const {
    return (HashT)(id % HASHT_MAX);
  }

  ObjectID GetID() const {
    return id;
  }

  char const* GetTypeString() const {
    return ObjectType_String[GetType()];
  }

  bool IsDeleted() const {
    return deleted;
  }

  /* Non-Component Related. */
  virtual float GetCooldown() const {
    return -1;
  }

  virtual Quantity GetMaxUses() const {
    return 0;
  }

  virtual float GetRange() const {
    return 0;
  }

  virtual uint GetSeed() const {
    return 0;
  }

  virtual Signature GetSignature() const {
    return Signature(0, 0, 0, 0);
  }

  virtual Quantity GetUses() const {
    return 0;
  }

  LT_API virtual Icon GetIcon() const;

  LT_API virtual Widget GetWidget(Player const& self);

  LT_API float GetMaxRange() const;

  LT_API float GetMinRange() const;

  LT_API virtual Traits const& GetTraits() const;

  LT_API virtual Quantity GetValue() const;

  LT_API virtual void OnCreate();

  LT_API virtual void OnDeath();

  LT_API virtual void OnDestroy();

  virtual void OnUpdate(UpdateState& state) {}

  LT_API void Update(UpdateState& state);

  /* Drawing. */
  virtual void OnDraw(DrawState* state) {}
  virtual void OnDrawInterior(DrawState* state) {}
  virtual void BeginDrawInterior(DrawState* state) {}
  virtual void EndDrawInterior(DrawState* state) {}

  /* Account. */
  virtual void AddCredits(Quantity count) {
    NOT_IMPLEMENTED
  }

  virtual Quantity GetCredits() const {
    return 0;
  }

  virtual bool RemoveCredits(Quantity count) {
    NOT_IMPLEMENTED
    return false;
  }

  /* Asset. */
  LT_API virtual Player const& GetOwner() const;

  virtual void SetOwner(Player const&) {
    NOT_IMPLEMENTED
  }

  /* Assets. */
  virtual void AddAsset(Object const& asset) {
    NOT_IMPLEMENTED
  }

  virtual void RemoveAsset(Object const& asset) {
    NOT_IMPLEMENTED
  }

  /* Attachable. */
  LT_API virtual Transform const& GetLocalTransform() const;

  virtual void SetLocalTransform(Transform const& transform) {
    NOT_IMPLEMENTED
  }

  /* BoundingBox. */
  LT_API Position GetCenter() const;
  LT_API V3 GetExtent() const;
  LT_API float GetRadius() const;

  /* Cargo. */
  virtual bool AddItem(
    Item const& item,
    Quantity quantity,
    bool force = false)
  {
    return false;
  }

  bool RemoveItem(Item const& item, Quantity quantity) {
    return AddItem(item, -quantity);
  }

  Mass GetFreeCapacity() const {
    return GetCapability().Storage - GetUsedCapacity();
  }

  virtual Mass GetUsedCapacity() const {
    return 0;
  }

  virtual Quantity GetItemCount(Item const& item) const {
    return 0;
  }

  /* Collidable. */
  virtual bool CanCollide(ObjectT const* object) const {
    return true;
  }

  virtual void OnCollide(
    ObjectT* self,
    ObjectT* other,
    Position const& selfLocation,
    Position const& otherLocation) {}

  /* Container. */
  virtual void AddInterior(Object const& child) {
    NOT_IMPLEMENTED
  }

  virtual void RemoveInterior(Object const& child) {
    NOT_IMPLEMENTED
  }

  /* Cullable. */
  virtual float GetCullDistanceMult() const {
    return 1.0f;
  }

  LT_API Position GetDockLocation(Object const& docker) const;

  /* Dockable. */
  virtual bool CanDock(Object const& docker) {
    return false;
  }

  virtual void Dock(Object const& docker) {
    NOT_IMPLEMENTED
  }

  virtual void Undock(Object const& docker) {
    NOT_IMPLEMENTED
  }

  /* Drawable. */
  LT_API Bound3 GetLocalBound() const;
  LT_API Bound3D GetGlobalBound() const;

  LT_API virtual Renderable GetRenderable() const;

  LT_API Pointer<ObjectT> GetContainerRoot() const;
  LT_API Pointer<ObjectT const> GetRegion() const;
  LT_API Pointer<ObjectT const> GetSystem() const;
  LT_API Pointer<Universe const> GetUniverse() const;

  /* Integrity. */
  virtual ItemT* GetDataDamaged() const {
    return nullptr;
  }

  virtual ItemT* GetDataDestroyed() const {
    return nullptr;
  }

  virtual Health GetHealth() const {
    return 0;
  }

  virtual Health GetMaxHealth() const {
    return 0;
  }

  virtual float GetHealthNormalized() const {
    Health maxHealth = GetMaxHealth();
    return maxHealth > 0 ? (float)GetHealth() / (float)maxHealth : 0;
  }

  bool IsAlive() const {
    return GetHealth() > 0;
  }

  Health GetDamage() const {
    return GetMaxHealth() - GetHealth();
  }

  Health GetTotalDamage() const {
    return GetTotalMaxHealth() - GetTotalHealth();
  }

  LT_API Damage ApplyDamage(Damage damage);
  LT_API Health GetTotalHealth() const;
  LT_API Health GetTotalMaxHealth() const;

  /* Log. */
  virtual void AddLogMessage(String const& message, float importance = 0) {
    NOT_IMPLEMENTED
  }

  /* Market. */
  virtual void AddMarketAsk(Order const&) {
    NOT_IMPLEMENTED
  }

  virtual void AddMarketBid(Order const&) {
    NOT_IMPLEMENTED
  }

  virtual void RemoveMarketAsk(Order const&) {
    NOT_IMPLEMENTED
  }

  virtual void RemoveMarketBid(Order const&) {
    NOT_IMPLEMENTED
  }

  /* Messaging. */
  LT_API virtual void Broadcast(Data& message);

  virtual void OnMessage(Data& message) {}

  template <class T>
  void Broadcast(T const& value) {
    Data message(value);
    Broadcast(message);
    Mutable(value) = message.Convert<T>();
  }

  template <class T>
  void Send(T const& value) {
    Data message(value);
    Send(message);
    Mutable(value) = message.Convert<T>();
  }

  void Send(Data& message) {
    OnMessage(message);
  }

  /* MissionBoard. */
  virtual void AddMissionListing(Mission const&) {
    NOT_IMPLEMENTED
  }

  virtual void RemoveMissionListing(Mission const&) {
    NOT_IMPLEMENTED
  }

  /* Missions. */
  virtual void AddMission(Mission const&) {
    NOT_IMPLEMENTED
  }

  virtual void RemoveMission(Mission const&) {
    NOT_IMPLEMENTED
  }

  /* Motion. */
  virtual void ApplyForce(V3 const&) {
    NOT_IMPLEMENTED
  }

  virtual void ApplyTorque(V3 const&) {
    NOT_IMPLEMENTED
  }

  virtual V3 GetAngularVelocity() const {
    return 0;
  }

  virtual Mass GetMass() const {
    return 1000000;
  }

  virtual float GetSpeed() const {
    return 0;
  }

  virtual float GetTopSpeed() const {
    return 0;
  }

  virtual V3 GetVelocity() const {
    return 0;
  }

  virtual V3 GetVelocityA() const {
    return 0;
  }

  LT_API float GetImpactTime(
    ObjectT const* source,
    float speed,
    Position& hitPoint) const;

  virtual bool IsMovable() const {
    return false;
  }

  /* Nameable. */
  virtual String GetName() const {
    return GetTypeString();
  }

  virtual void SetName(String const& name) {
    NOT_IMPLEMENTED
  }

  /* Parent. */
  LT_API void AddChild(Object const& child);

  LT_API void Attach(Object const& child, Transform const& transform);

  LT_API void RemoveChild(Object const& child);

  /* Pluggable. */
  virtual float GetPowerFraction() const {
    return 0;
  }

  /* Supertyped. */
  virtual ItemT* GetSupertype() const {
    return nullptr;
  }

  virtual void SetSupertype(Item const&) {}

  /* Opinion. */
  virtual float GetOpinion(Object const&) const {
    return 0.0f;
  }

  virtual void ModOpinion(Object const&, float) {}

  LT_API bool Dislikes(Object const&) const;

  LT_API bool Likes(Object const&) const;

  /* Orientation. */
  LT_API V3 GetLook() const;

  LT_API V3 GetUp() const;

  LT_API V3 GetRight() const;

  LT_API Position GetPos() const;

  LT_API V3 GetScale() const;

  LT_API virtual Transform const& GetTransform() const;

  virtual void SetLook(V3 const& look) {
    NOT_IMPLEMENTED
  }

  virtual void SetPos(Position const& position) {
    NOT_IMPLEMENTED
  }

  virtual void SetScale(V3 const& scale) {
    NOT_IMPLEMENTED
  }

  virtual void SetUp(V3 const& up) {
    NOT_IMPLEMENTED
  }

  /* Queryable. */
  virtual void QueryInterior(Bound3D const& box, Vector<ObjectT*>& objects) {}

  virtual ObjectT* QueryInterior(
    WorldRay const& ray,
    float& t,
    float tMax,
    V3* normalOut = nullptr,
    bool accelerate = false,
    bool (*check)(ObjectT const*, void*) = nullptr,
    void* aux = nullptr)
  {
    return nullptr;
  }

  /* Scriptable. */
  virtual void AddScript(Data const& object) {
    NOT_IMPLEMENTED;
  }

  /* Sockets. */
  virtual bool Plug(Item const& item) {
    NOT_IMPLEMENTED
    return false;
  }

  virtual bool Plug(Object const& object) {
    NOT_IMPLEMENTED
    return false;
  }

  /* Storage. */
  virtual Object GetStorageLocker(Object const& owner) {
    return nullptr;
  }

  /* Tasks. */
  virtual void ClearTasks() {
    NOT_IMPLEMENTED
  }

  virtual TaskInstance const* GetCurrentTask() const {
    return nullptr;
  }

  virtual void PushTask(Task const&) {
    NOT_IMPLEMENTED
  }

  /* Thrustable. */
  float GetMaxThrust() const {
    return GetCapability().Motion;
  }

  LT_API float GetMaxTorque() const;

  LT_API bool InRange(ObjectT const* other,
                      Distance distance,
                      bool precise = false) const;

  /* Component Access. */
  bool HasComponent(ComponentType type) const {
    switch(type) {
      #define X(x) case ComponentType_##x: return Get##x() != nullptr;
      COMPONENT_X
      #undef X

      default: return false;
    }
  }

  #define X(x) virtual Pointer<Component##x> Get##x() {                        \
    return nullptr;                                                            \
  }
  COMPONENT_X
  #undef X

  #define X(x) Pointer<const Component##x> Get##x() const {                    \
    return ((ObjectT*)this)->Get##x().t;                                       \
  }
  COMPONENT_X
  #undef X

  FIELDS {
    MAPFIELD(id)
    MAPFIELD(parent)
    MAPFIELD(container)
    MAPFIELD(nextSibling)
    MAPFIELD(children)
    MAPFIELD(deleted)
  }
};

#endif
