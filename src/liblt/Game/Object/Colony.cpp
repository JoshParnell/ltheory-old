#include "../Objects.h"

#include "Component/Account.h"
#include "Component/Asset.h"
#include "Component/Attachable.h"
#include "Component/BoundingBox.h"
#include "Component/Cargo.h"
#include "Component/Dockable.h"
#include "Component/Drawable.h"
#include "Component/Interior.h"
#include "Component/Market.h"
#include "Component/MissionBoard.h"
#include "Component/Nameable.h"
#include "Component/Orientation.h"
#include "Component/Scriptable.h"
#include "Component/Storage.h"
#include "Component/Supertyped.h"
#include "Component/Tasks.h"
#include "Component/Zoned.h"

#include "Game/Materials.h"
#include "Game/Renderables.h"
#include "Game/ShadingModels.h"
#include "Game/Tasks.h"
#include "Game/Attribute/Traits.h"
#include "Game/Graphics/Generators.h"
#include "Game/Object/Planet.h"

#include "LTE/CubeMap.h"
#include "LTE/DrawState.h"
#include "LTE/Math.h"
#include "LTE/Meshes.h"
#include "LTE/Model.h"
#include "LTE/RNG.h"
#include "LTE/RenderStyle.h"
#include "LTE/Script.h"
#include "LTE/SDFs.h"
#include "LTE/ShaderInstance.h"
#include "LTE/Texture2D.h"

#include "UI/Widget.h"

const uint kTerrainQuality = 750;
const float kTerrainScale = 50000;
const float kTrees = 100;

inline float RidgedWorley(V2 const& uv) {
  return Abs(2.0f * WorleyNoise2D(uv) - 1.0f);
}

inline float TerrainFn(V2 const& uv) {
  float h = Abs(Fractal(RidgedWorley, uv * 3.0f, 10, 2.2f) - 0.5f);
  h = Min(h, Fractal(WorleyNoise2D, uv * 2.0f + 13.0f, 10, 2.0f));
  h *= h;
  return h - 0.01f;
}

inline float HeightFn(V2 const& uv) {
  return TerrainFn(uv) * 0.03f;
}

V3 SpatialFn(V2 const& uv) {
  return kTerrainScale * Normalize(V3(uv.x, 5.0f, uv.y)) * (1.0f + HeightFn(uv));
}

float TreeDensity(V2 const& uv) {
  float h = TerrainFn(uv);
  return h < 0 ? 0.0f : (1.0f - h) * Fractal(WorleyNoise2D, uv * 3.0f, 8, 2.0f);
}

float OcclusionFn(V2 const& uv) {
  return 1.0f;
}

void GenerateForests(Object const& container) {
  ShaderInstance shader = ShaderInstance_Create("billboard_axis.jsl", "tree.jsl");
  (*shader)
    (RenderStateSwitch_BlendModeAlpha)
    (RenderStateSwitch_ZWritableOff)
    ("atmoDensity", 50.0f)
    ("atmoTint", V3(1, 0.5f, 0.1f))
    ("axis", V3(0, 1, 0))
    ("size", 2.0f * V2(1, 2))
    ("texture", Texture_LoadFrom(Location_Texture("tree.png")));
  DrawState_Link(shader);

  const float kTreeRegion = 1.0f;
  const float kGridSize = kTreeRegion / 10.f;

  for (float x = -kTreeRegion; x < kTreeRegion; x += kGridSize)
  for (float y = -kTreeRegion; y < kTreeRegion; y += kGridSize) {
    V2 uv(x, y);
    if (Length(uv) > 1.0f)
      continue;

    Mesh m = Mesh_Create();
    for (uint i = 0; i < kTrees; ++i) {
      V2 uvp = uv + RandV2(0, kGridSize);
      if (Rand() > TreeDensity(uvp))
        continue;

      V3 p = SpatialFn(uvp) + V3(0, 1, 0);
      m->AddMesh(Mesh_Billboard(), Matrix::Translation(p));
    }

    container->AddInterior(
      Object_Static((Renderable)Model_Create()->Add(m, shader)));
  }
}

void GenerateCity(Object const& container) {
  Mesh m = Mesh_Create();
  Mesh box = Mesh_Box(2, true);
  box->SetU(1);
  V2 center = RandV2(-0.75f, 0.75f);
  for (uint i = 0; i < 100; ++i) {
    V2 uv = center + 0.25f * Squared(Rand()) * Polar(RandAngle());
    V3 p = SpatialFn(uv);
    V3 s = 10 * V3(Rand(1, 2), 20 * Squared(Rand()), Rand(1, 2));
    m->AddMesh(box, Matrix::Translation(p) * Matrix::Scale(s));
  }

  container->AddInterior(
    Object_Static((Renderable)Model_Create()->Add(m, Material_Metal())));
}

typedef ObjectWrapper
  < Attribute_Traits
  < Component_Account
  < Component_Asset
  < Component_Attachable
  < Component_BoundingBox
  < Component_Cargo
  < Component_Dockable
  < Component_Drawable
  < Component_Interior
  < Component_Market
  < Component_MissionBoard
  < Component_Nameable
  < Component_Orientation
  < Component_Scriptable
  < Component_Seeded
  < Component_Storage
  < Component_Supertyped
  < Component_Tasks
  < Component_Zoned
  < ObjectWrapperTail<ObjectType_Colony>
  > > > > > > > > > > > > > > > > > > > >
  ColonyBaseT;

AutoClassDerived(Colony, ColonyBaseT,
  Generic<CubeMap>, envMap,
  Generic<CubeMap>, envMapLF,
  Renderable, interior,
  Pointer<Planet>, planet,
  Quantity, population,
  Vector<Item>, localItems)

  DERIVED_TYPE_EX(Colony)
  POOLED_TYPE
  
  Colony() {}

  Colony(Object const& planet, Quantity const& population) :
    planet((Planet*)(ObjectT*)planet),
    population(population)
    {}

  void BeginDrawInterior(DrawState* state) {
    GetContainer()->BeginDrawInterior(state);
    DrawState_Push("fogDensity", 0.1f);
    // state->envMap.push(envMap());
    // state->envMapLF.push(envMapLF());
  }

  void EndDrawInterior(DrawState* state) {
    // state->envMap.pop();
    // state->envMapLF.pop();
    DrawState_Pop("fogDensity");
    GetContainer()->EndDrawInterior(state);
  }

  Signature GetSignature() const {
    return Signature(100, 2, 0.25f, 0.75f);
  }

  Widget GetWidget(Player const& self) {
    Widget widget;
    ScriptFunction_Load("Object/Widget/Colony:Create")
      ->Call(widget, self, (Object)this);
    return widget;
  }

  void Initialize() {
    this->envMap = Generator_PlanetSkybox(planet);
    this->envMapLF = Generator_Blur(envMap, 0.1f, 1024, 512);

    Cargo.capacity = FLT_MAX;
    Dockable.ports.push(Bound3(V3(0, 100, 0)));
    Dockable.hangars.push(Bound3(SpatialFn(0.5f) + V3(0, 100, 0)));
    Drawable.renderable = Renderable_Asteroid(1);
    Interior.allowMovement = false;
    Zoned.region = SDF_Sphere(0, 500);

    RNG rg = RNG_MTG(GetSeed());
    int itemCount = rg->GetInt(1, 5);
    for (int i = 0; i < itemCount; ++i) {

    }
  }

  void OnDrawInterior(DrawState* state) {
    if (!interior) {
      Mesh terrainMesh = Mesh_Plane(
        V3(-1, 0, -1),
        V3(2, 0, 0),
        V3(0, 0, 2), kTerrainQuality, kTerrainQuality);

      for (size_t i = 0; i < terrainMesh->vertices.size(); ++i) {
        Vertex& v = terrainMesh->vertices[i];
        V2 uv = v.p.GetXZ();
        v.p = SpatialFn(uv);
        v.u = OcclusionFn(uv);
        v.v = 0;
      }

      terrainMesh->ComputeNormals();

      Mesh waterMesh = Mesh_Plane(
        V3(-1, 0, -1),
        V3(2, 0, 0),
        V3(0, 0, 2), 100, 100);

      for (size_t i = 0; i < waterMesh->vertices.size(); ++i) {
        Vertex& v = waterMesh->vertices[i];
        v.p.y += 5.0f;
        v.p = Normalize(v.p);
        v.p *= kTerrainScale;
      }

      interior = (Renderable)Model_Create()
        ->Add(waterMesh, Material_Water())
        ->Add(terrainMesh, Material_Grass());
      //for (uint i = 0; i < 1; ++i) GenerateCity(this);
      // GenerateForests(this);
    }

    GetContainer()->OnDrawInterior(state);
    interior->Render(state);
  }

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);
  }
};

DERIVED_IMPLEMENT(Colony)

DefineFunction(Object_Colony) {
  Reference<Colony> self = new Colony(args.planet, args.population);
  self->Seeded.seed = args.seed;
  self->traits = args.type->GetTraits();
  self->SetSupertype(args.type);
  self->Initialize();
  self->PushTask(args.type->GetTask());
  ScriptFunction_Load("Object/Colony:Init")->VoidCall(0, DataRef((Object)self));
  return self;
}
