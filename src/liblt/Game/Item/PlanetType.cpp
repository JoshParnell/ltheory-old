#include "../Items.h"

#include "Game/Objects.h"
#include "Game/Object/Planet.h"
#include "Game/Attribute/Docks.h"
#include "Game/Attribute/Icon.h"
#include "Game/Attribute/Name.h"
#include "Game/Attribute/Renderable.h"
#include "Game/Attribute/Scale.h"
#include "Game/Attribute/Seed.h"
#include "Game/Graphics/Generators.h"

#include "LTE/CubeMap.h"
#include "LTE/DrawState.h"
#include "LTE/Meshes.h"
#include "LTE/Model.h"
#include "LTE/RNG.h"
#include "LTE/Script.h"
#include "LTE/ShaderInstance.h"
#include "LTE/StackFrame.h"
#include "LTE/Texture2D.h"

#include "UI/Glyphs.h"

const float kOuterScale = 1.025f;
const uint kMeshQuality = 50;

typedef
    Attribute_Docks
  < Attribute_Icon
  < Attribute_Name
  < Attribute_Renderable
  < Attribute_Scale
  < Attribute_Seed
  < ItemWrapper<ItemType_PlanetType>
  > > > > > >
  PlanetTypeBase;

AutoClassDerived(PlanetType, PlanetTypeBase,
  float, atmoDensity,
  V3, atmoTint,
  float, cloudLevel,
  Color, color1,
  Color, color2,
  V3, wavelength)

  DERIVED_TYPE_EX(PlanetType)
  PlanetType() {}

  Object Instantiate(ObjectT* parent) {
    return Object_Planet(this);
  }
};

DERIVED_IMPLEMENT(PlanetType)

Renderable Generate(PlanetType const& type) {
  static Mesh planetMesh;
  static Mesh atmoMesh;
  static Mesh ringMesh;
  static Shader planetShader = Shader_Create("npm.jsl", "planet.jsl");
  static Shader atmoShader = Shader_Create("npm.jsl", "atmosphere.jsl");
  static Shader ringShader = Shader_Create("npm.jsl", "planetring.jsl");

  if (!planetMesh) {
    planetMesh = Mesh_BoxSphere(kMeshQuality, true)->SetU(1);
    atmoMesh = Mesh_BoxSphere(kMeshQuality, true)
      ->Scale(kOuterScale)
      ->ReverseWinding()
      ->SetU(1);
    ringMesh = Mesh_Quad()
      ->Rotate(V3(0, kPi2, 0))
      ->Scale(3.0f);
  }

  uint seed = type.GetSeed();
  RNG rg = RNG_MTG(seed);
  Model model = Model_Create();

  /* Planet. */ {
    ShaderInstance planetShaderInstance = ShaderInstance_Create(planetShader);
    float heightMult = 1;
    float oceanLevel = Pow(rg->GetFloat(), 1.5f);

    (*planetShaderInstance)
      ("atmoDensity", type.atmoDensity)
      ("atmoTint", type.atmoTint)
      ("cloudLevel", type.cloudLevel)
      ("color1", type.color1)
      ("color2", rg->GetV3(0.5f, 0.75f))
      ("color3", rg->GetV3(0.5f, 0.75f))
      ("color4", type.color2)
      ("colorSeed", rg->GetFloat(1, 1000))
      ("heightMult", heightMult)
      ("oceanLevel", oceanLevel)
      ("planetMap",
        DiskCached(Generator_PlanetSurface(seed), Stringize() | "planetsurface_" | seed))
      ("wavelength", type.wavelength);
    DrawState_Link(planetShaderInstance);
    model->Add(planetMesh, planetShaderInstance);
  }

  /* Atmosphere. */ {
    ShaderInstance atmoShaderInstance = ShaderInstance_Create(atmoShader);
    (*atmoShaderInstance)
      (RenderStateSwitch_BlendModeAdditive)
      ("atmoDensity", type.atmoDensity)
      ("atmoTint", type.atmoTint)
      ("wavelength", type.wavelength);
    DrawState_Link(atmoShaderInstance);
    model->Add(atmoMesh, atmoShaderInstance, false);
  }

  /* Rings. */ {
    static Shader generate = Shader_Create("identity.jsl", "gen/planetring.jsl");
    (*generate)("seed", rg->GetFloat());

    Texture2D ringTexture = Texture_Create(1024, 1, GL_TextureFormat::R32F);
    Texture_Generate(ringTexture, generate);

    ShaderInstance ringShaderInstance = ShaderInstance_Create(ringShader);
    (*ringShaderInstance)
      (RenderStateSwitch_BlendModeAlpha)
      (RenderStateSwitch_CullModeDisabled)
      ("rings", ringTexture);
    DrawState_Link(ringShaderInstance);
    model->Add(ringMesh, ringShaderInstance, false);
  }

  return model;
}

DefineFunction(Item_PlanetType) { AUTO_FRAME;
  RNG rg = RNG_MTG(args.seed);

  Reference<PlanetType> self = new PlanetType;
  self->docks.push(Bound3(V3(-1), V3(1)));
  self->dockCapacity = -1;
  ScriptFunction_Load("Icons:Planet")->Call(self->icon);
  self->name = "Planet";
  self->scale = 100000;
  self->seed = args.seed;
  self->atmoDensity = rg->GetFloat(1.0f, 1.0f);
  self->atmoTint = V3(1);
  self->cloudLevel = rg->GetFloat(-0.2f, 0.15f);

  float desat = rg->GetFloat(0.75f, 1);
  self->color1 = Desaturate(rg->GetV3(0, 0.25f), desat);
  self->color2 = Desaturate(rg->GetV3(0.25f, 0.5f), desat);
  self->wavelength = V3(1) / Pow(V3(0.66f, 0.53f, 0.4f) + rg->GetV3(0, -0.1f), 4.0f);

  self->renderable = Generate(*self);
  return self;
}
