#include "../Objects.h"

#include "Component/Attachable.h"
#include "Component/BoundingBox.h"
#include "Component/Collidable.h"
#include "Component/Cullable.h"
#include "Component/Damager.h"
#include "Component/Drawable.h"
#include "Component/Explodable.h"
#include "Component/Integrity.h"
#include "Component/Orientation.h"
#include "Component/Supertyped.h"

#include "Module/SoundEngine.h"

#include "LTE/DrawState.h"
#include "LTE/Geom.h"
#include "LTE/Loader.h"
#include "LTE/Math.h"
#include "LTE/Matrix.h"
#include "LTE/Mesh.h"
#include "LTE/Model.h"
#include "LTE/Pool.h"
#include "LTE/RenderStyle.h"
#include "LTE/ShaderInstance.h"
#include "LTE/Tuple.h"
#include "LTE/Vector.h"
#include "LTE/VectorMap.h"

const size_t kMaxHits = 16;
const float kMaxAge = 1;
const float kChargeTime = 60;
const float kRestoreFraction = .25f;

namespace {
  /* CRITICAL : Seriously..? That's going to cause problems. */
  VectorMap<size_t, Mesh> gMeshCache;

  static Shader gShader;
  static ShaderInstance gShaderInstance;

  void OnLoad() {
    gShader = Shader_Create("npm.jsl", "shield.jsl");
    gShaderInstance = ShaderInstance_Create(gShader);
    (*gShaderInstance)
      (RenderStateSwitch_BlendModeAdditive)
      (RenderStateSwitch_CullModeDisabled)
      (RenderStateSwitch_ZWritableOff);
  } static bool l = RegisterLoader(OnLoad);
}

typedef ObjectWrapper
  < Component_Attachable
  < Component_BoundingBox
  < Component_Collidable
  < Component_Cullable
  < Component_Drawable
  < Component_Explodable
  < Component_Integrity
  < Component_Orientation
  < Component_Supertyped
  < ObjectWrapperTail<ObjectType_Shield>
  > > > > > > > > > >
  ShieldBaseT;

AutoClassDerived(Shield, ShieldBaseT,
  float, time,
  float, recharge)

  Mesh mesh;
  std::vector<V3> hitPosition;
  std::vector<float> hitAge;

  DERIVED_TYPE_EX(Shield)
  POOLED_TYPE

  struct RenderComponent : public RenderableT {
    DERIVED_TYPE(RenderComponent, RenderableT)
    Shield* self;

    RenderComponent(Shield* self) :
      self(self)
      {}

    Bound3 GetBound() const {
      return self->mesh ? self->mesh->GetBound() : Bound3(0);
    }

    Mesh GetCollisionMesh() const {
      return self->mesh ? self->mesh->Clone() : nullptr;
    }

    size_t GetHash() const {
      return (size_t)self->mesh;
    }

    short GetVersion() const {
      return self->mesh ? self->mesh->GetVersion() : (short)-1;
    }

    void Render(DrawState* state) const {
      if (!self->mesh)
        return;

      static std::vector<float> ages;
      static std::vector<V3> positions;
      ages.clear();
      positions.clear();

      for (size_t i = 0; i < self->hitAge.size(); ++i) {
        float thisAge = self->hitAge[i];
        if (thisAge > kMaxAge)
          continue;
        ages.push_back(thisAge);
        positions.push_back(self->hitPosition[i]);
      }

      /* Only draw the shield if there are active hits on it. */
      if (ages.size()) {
        gShader->SetFloatArray("hitAge", ages.data(), ages.size());
        gShader->SetFloat3Array("hitPos", positions.data(), positions.size());
        gShader->SetInt("activeHits", ages.size());
        RenderStyle const& style = RenderStyle_Get();
        style->SetShader(gShaderInstance);
        style->Render(self->mesh);
      }
    }

    V3 Sample() const {
      return self->mesh ? self->mesh->Sample() : V3(0);
    }
  };

  Shield() :
    time(Rand(0, 10000)),
    recharge(1)
  {
    Drawable.renderable = (Renderable)(new RenderComponent(this));
    Explodable.explosionType = ExplosionType_Plasma;
    hitAge.resize(kMaxHits, 1e10f);
    hitPosition.resize(kMaxHits, 0);
  }

  float GetCooldown() const {
    return recharge;
  }

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);

    /* Keep the shield's relative orientation fixed. */
    /* TODO : Why? Shouldn't it stay fixed automatically?? */
    Attachable.transform = Transform_Identity();
    Attachable.moved = true;

    if (!mesh) {
      size_t id = parent->GetDrawable()->renderable()->GetHash();
      if (!gMeshCache[id]) {
        Mesh source = parent->GetDrawable()->renderable()->GetCollisionMesh();
        /* TODO : Thread this, since SmoothHull is expensive. */
        if (source && source->GetTriangles())
          gMeshCache[id] = Geom_SmoothHull(source, 5, 5);
      }
      mesh = gMeshCache[id];
    }

    time += state.dt;
    for (size_t i = 0; i < hitAge.size(); ++i)
      hitAge[i] += state.dt;

    float chargeUnit = parent->GetPowerFraction() * (state.dt / kChargeTime);
    if (!IsAlive()) {
      if ((recharge -= chargeUnit) <= 0) {
        Explodable.exploded = false;
        Integrity.health = kRestoreFraction * Integrity.maxHealth;
        recharge = 1;
      }
    } else {
      Integrity.health = Min(
        Integrity.maxHealth,
        (Health)(Integrity.health + chargeUnit * Integrity.maxHealth));
    }

    SetScale(parent->GetScale());
  }

  bool CanCollide(ObjectT const* other) const {
    ComponentDamager const* d = other->GetDamager();
    if (!d)
      return false;
    return IsAlive() &&
           d->source &&
           d->source->GetRoot() != this->GetRoot();
  }

  void OnCollide(
    ObjectT* self,
    ObjectT* other,
    Position const& pSelf,
    Position const& pOther)
  {
    if (hitAge.size() >= kMaxHits) {
      hitAge.erase(hitAge.begin());
      hitPosition.erase(hitPosition.begin());
    }

    hitAge.push_back(0);
    hitPosition.push_back(pSelf);
    Sound_Play3D("shield/hit.ogg", self,
      self->GetTransform().InversePoint(pSelf), 0.25f);
  }
};

DERIVED_IMPLEMENT(Shield)

DefineFunction(Object_Shield) {
  LTE_ASSERT(args.type->GetType() == ItemType_ShieldType);
  Reference<Shield> self = new Shield;
  self->SetSupertype(args.type);
  return self;
}
