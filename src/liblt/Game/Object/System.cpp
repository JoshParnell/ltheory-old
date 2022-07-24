#include "../Objects.h"

#include "Component/Economy.h"
#include "Component/History.h"
#include "Component/Interior.h"
#include "Component/Nameable.h"
#include "Component/Orientation.h"
#include "Component/Queryable.h"
#include "Component/Resources.h"
#include "Component/Seeded.h"

#include "Game/Messages.h"
#include "Game/Renderables.h"
#include "Game/Universe.h"
#include "Game/Graphics/Generators.h"

#include "LTE/CubeMap.h"
#include "LTE/DrawState.h"
#include "LTE/Grammar.h"
#include "LTE/Math.h"
#include "LTE/Renderable.h"
#include "LTE/RNG.h"
#include "LTE/Script.h"
#include "LTE/StackFrame.h"
#include "LTE/View.h"

#include "LTE/Debug.h"
#include "LTE/Keyboard.h"
#include "LTE/Texture2D.h"
#include "LTE/Texture3D.h"

const uint kBaseStarCount = 100000;

const int kColorIterations = 4;
const int kColorPoints = 256;
const float kColorVariation = 0.02f;
const float kColorLacunarity = 0.6f;

namespace {
  V3 GenerateStarColor(RNG const& rng) {
    return V3(1.0f) - 0.5f * Log(rng->GetV3(0, 1)) * V3(1.0f, 0.5f, 1.0f);
  }
}

typedef ObjectWrapper
  < Attribute_Traits
  < Component_Economy
  < Component_History
  < Component_Interior
  < Component_Nameable
  < Component_Orientation
  < Component_Queryable
  < Component_Resources
  < Component_Seeded
  < ObjectWrapperTail<ObjectType_System>
  > > > > > > > > > >
  SystemBaseT;

AutoClassDerived(System, SystemBaseT,
  Object, star)
  DERIVED_TYPE_EX(System)
  POOLED_TYPE

  Renderable interior;
  Generic<CubeMap> envMap;
  Generic<CubeMap> envMapLF;
  Generic<CubeMap> irMap;

  Texture2D rCurve;
  Texture2D gCurve;
  Texture2D bCurve;
  Texture2D rCurve2;
  Texture2D gCurve2;
  Texture2D bCurve2;
  V2 rDir;
  V2 gDir;
  V2 bDir;

  System() {}

  void Initialize() {
    RNG rng = RNG_MTG(Seeded.seed);
    Interior.allowMovement = true;

    /* CubeMap. */ {
      Generator_Nebula_Args args;
      args.seed = rng->GetFloat(0, 1000); 
      args.roughness = rng->GetFloat();
      args.offset = rng->GetV4();

      MessageGetColor starColor;
      star->Send(starColor);
      args.color1 = starColor.color;
      args.color2 = Mix(args.color1, GenerateStarColor(rng), 0.5f);
      args.starDir = Normalize(star->GetPos());

      envMap = DiskCached(
        Generator_Nebula(args),
        Stringize() | "nebula_" | Seeded.seed);
    }

    /* Interior. */ {
      interior = Renderable_Starfield(
        Seeded.seed,
        Abs(kBaseStarCount + 2000 * rng->GetGaussian()));
    }

    envMapLF = DiskCached(
      Generator_Blur(envMap, 0.1f, 1024, 512),
      Stringize() | "sysbg_" | Seeded.seed | "_blurred");

    irMap = Generator_IRMap(envMap, 1024);

    RandomizeColors(RNG_MTG(rng->GetInt() + 3));
  }

  void RandomizeColors(RNG const& rng) {
    rCurve = CreateColorCurve(rng);
    gCurve = CreateColorCurve(rng);
    bCurve = CreateColorCurve(rng);
    rCurve2 = CreateColorCurve(rng);
    gCurve2 = CreateColorCurve(rng);
    bCurve2 = CreateColorCurve(rng);
    rDir = CreateColorDir(rng);
    gDir = CreateColorDir(rng);
    bDir = CreateColorDir(rng);
  }

  V2 CreateColorDir(RNG const& rng) {
    return rng->GetFloat() < 0.5f ? V2(1, 0) : V2(0, 1);
  }

  Texture2D CreateColorCurve(RNG const& rng) {
    Texture2D curve = Texture_Create(kColorPoints, 1, GL_TextureFormat::R8);
    curve->SetMagFilter(GL_TextureFilter::Linear);
    curve->SetMinFilter(GL_TextureFilterMip::Linear);
    curve->SetMaxLod(0);
    curve->SetMinLod(0);

    Vector<float> controlPoints;
    controlPoints.push(0);
    controlPoints.push(1);

    float variation = kColorVariation;
    for (int i = 0; i < kColorIterations; ++i) {
      Vector<float> newControlPoints;
      for (size_t j = 0; j + 1 < controlPoints.size(); ++j) {
        float v1 = controlPoints[j + 0];
        float v2 = controlPoints[j + 1];
        float v = (v1 + v2) / 2.0f;
        v += variation * rng->GetGaussian();
        newControlPoints.push(v1);
        newControlPoints.push(v);
      }

      newControlPoints.push(controlPoints.back());
      controlPoints = newControlPoints;
      variation *= kColorLacunarity;
    }

    Vector<float> points;
    for (size_t i = 0; i < kColorPoints; ++i) {
      float t = (float)i / (float)(kColorPoints - 1);
      Vector<float> interpolated = controlPoints;

      while (interpolated.size() > 1) {
        Vector<float> newInterpolated;
        for (size_t j = 0; j + 1 < interpolated.size(); ++j) {
          float v1 = interpolated[j + 0];
          float v2 = interpolated[j + 1];
          newInterpolated.push(Mix(v1, v2, t));
        }
        interpolated = newInterpolated;
      }

      points.push(interpolated[0]);
    }

    curve->SetData(
      0, 0, kColorPoints, 1,
      GL_PixelFormat::Red, GL_DataFormat::Float, points.data());
    return curve;
  }

  void BeginDrawInterior(DrawState* state) {
    if (Keyboard_Pressed(Key_F6))
      RandomizeColors(RNG_MTG(rand()));

    state->envMap.push(envMap());
    state->envMapLF.push(envMapLF());

    MessageGetColor starColor;
    star->Send(starColor);
    DrawState_Push("starColor", (V3)starColor.color);
    DrawState_Push("starPos", (V3)star->GetPos() - state->view->transform.pos);

    CubeMap const& irMap = this->irMap();
    DrawState_Push("irMap", irMap);

    DrawState_Push("rCurve", rCurve);
    DrawState_Push("gCurve", gCurve);
    DrawState_Push("bCurve", bCurve);
    DrawState_Push("rCurve2", rCurve2);
    DrawState_Push("gCurve2", gCurve2);
    DrawState_Push("bCurve2", bCurve2);
    DrawState_Push("rDir", rDir);
    DrawState_Push("gDir", gDir);
    DrawState_Push("bDir", bDir);
    DrawState_Push("colorPoints", (float)kColorPoints);
  }

  void OnDrawInterior(DrawState* state) {
    interior->Render(state);
  }

  void EndDrawInterior(DrawState* state) {
    DrawState_Pop("starColor");
    DrawState_Pop("starPos");

    DrawState_Pop("rCurve");
    DrawState_Pop("gCurve");
    DrawState_Pop("bCurve");
    DrawState_Pop("rCurve2");
    DrawState_Pop("gCurve2");
    DrawState_Pop("bCurve2");
    DrawState_Pop("rDir");
    DrawState_Pop("gDir");
    DrawState_Pop("bDir");
    DrawState_Pop("colorPoints");
    DrawState_Pop("irMap");

    state->envMap.pop();
    state->envMapLF.pop();
  }
};

DERIVED_IMPLEMENT(System)

DefineFunction(Object_System) { AUTO_FRAME;
  Reference<System> self = new System;

  RNG rng = RNG_MTG(args.seed);
  self->Seeded.seed = args.seed;
  self->SetPos(args.position);

  /* Create the central star. */ {
    self->star = Object_Star(GenerateStarColor(rng));
    self->star->SetPos(Spherical(60000000, 1.25f * kPi2, 0.0f));
    self->AddInterior(self->star);
    self->Initialize();
  }

  /* Create the dust. */ {
    self->AddInterior(Object_DustFlecks());
  }

  return self;
}
