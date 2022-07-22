#include "../Objects.h"

#include "Component/Nameable.h"
#include "Component/Orientation.h"
#include "Component/Resources.h"
#include "Component/Seeded.h"
#include "Component/Zoned.h"

#include "Game/Materials.h"

#include "LTE/DrawState.h"
#include "LTE/Math.h"
#include "LTE/Model.h"
#include "LTE/RNG.h"
#include "LTE/SDFs.h"
#include "LTE/SDFMesh.h"
#include "LTE/StackFrame.h"
#include "LTE/View.h"

const float kInitialSize = 8;
const uint kAsteroidCount = 24;
const float kCompression = 0.8f;
const uint kFieldLevels = 6;
const float kMaxScale = 10000;

namespace {
  ShaderInstance GetZoneMaterial() {
    static ShaderInstance shader;
    if (!shader) {
      shader = ShaderInstance_Create("npm.jsl", "material/lodfade.jsl");
      (*shader)(RenderStateSwitch_BlendModeAlpha);
      DrawState_Link(shader);
    }
    return shader;
  }
}

typedef ObjectWrapper
  < Component_Nameable
  < Component_Orientation
  < Component_Resources
  < Component_Seeded
  < Component_Zoned
  < ObjectWrapperTail<ObjectType_Zone>
  > > > > > >
  ZoneBaseT;

struct DynamicCell {
  Vector<Object> elements;
  float cellSize;
  float objectSize;
  int count;
  V3I currentCell;

  DynamicCell(float cellSize, float objectSize, int count) :
    cellSize(cellSize),
    objectSize(objectSize),
    count(count),
    currentCell(INT_MAX)
    {}

  void Update(ObjectT* parent, Position const& eye) {
    V3I cell = (V3I)Floor(eye / cellSize);
    if (currentCell == cell)
      return;
    
    currentCell = cell;

    for (size_t i = 0; i < elements.size(); ++i)
      elements[i]->Delete();
    elements.clear();

    ComponentZoned* zone = parent->GetZoned();

    for (int x = -1; x <= 1; ++x)
    for (int y = -1; y <= 1; ++y)
    for (int z = -1; z <= 1; ++z) {
      V3I thisCell = cell + V3I(x, y, z);
      Position cellCenter = (Position)thisCell * cellSize;

      HashT hash = Hash(thisCell) + Hash(cellSize);
      RNG rng = RNG_MTG((unsigned int)hash);

      for (int i = 0; i < count; ++i) {
        Position p = cellCenter + cellSize * rng->GetV3(-0.5f, 0.5f);
        V3 look = rng->GetDirection();
        V3 up = Normalize(Cross(look, rng->GetDirection()));
        V3 scale = rng->GetFloat(0.5f, 1.0f / 0.5f) * objectSize;
        uint seed = rng->GetInt();

        float probability = zone->GetContainment(parent, p);
        if (rng->GetFloat() < probability) {
          Object o = Object_Asteroid(seed);
          o->SetPos(p);
          o->SetLook(look);
          o->SetUp(up);
          o->SetScale(scale);
          elements.push(o);
          parent->AddChild(o);
        }
      }
    }
  }
};

AutoClassDerivedEmpty(Zone, ZoneBaseT)
  AutoPtr<DynamicCell> field[kFieldLevels];

  DERIVED_TYPE_EX(Zone)

  Zone() {
    for (size_t i = 0; i < kFieldLevels; ++i) {
      float objectSize = kInitialSize * Pow(2.0f, (float)i);
      float cellSize = 512.0f * Pow(objectSize, kCompression);
      field[i] = new DynamicCell(cellSize, objectSize, kAsteroidCount);
    }
  }

  void OnDraw(DrawState* state) {
    BaseType::OnDraw(state);
    for (size_t i = 0; i < kFieldLevels; ++i)
      field[i]->Update(this, state->view->transform.pos);
  }
};

DERIVED_IMPLEMENT(Zone)

float FVF(Position const& p, float s) {
  return 2.0f * Fractal(ValueNoise4D, V4(p, s), 4, 2) - 1.0f;
}

void Displace(Position& p, double g) {
  Position p2 = p / g;
  Position displace = Position(FVF(p2, 0), FVF(p2, 150), FVF(p2, 172));
  p += displace * g;
}

DefineFunction(Object_Zone) { AUTO_FRAME;
  Reference<Zone> self = new Zone;
  self->SetPos(args.position);
  self->SetScale(args.scale);
  self->Resources.elements = args.parent->GetResources()->elements;
  self->Seeded.seed = args.seed;
  self->Zoned.region = args.shape;
  self->Zoned.fogDensity = 1;

  /* TODO : Interesting resource variation. */
  RNG rng = RNG_MTG(self->GetSeed());
  Bound3 fieldBound = self->Zoned.region->GetBound();
  for (uint i = 0; i < args.statics; ++i) {
    Position center;
    while (true) {
      center = args.position + fieldBound.Sample(rng->GetV3()) * args.scale;
      if (self->Zoned.GetContainment(self, center) >= 1)
        break;
    }

    Object object;
    if (args.parent->GetResources()->elements.size()) {
      Item resource = args.parent->GetResources()->elements.sample(rng->GetFloat());
      Quantity value = (Quantity)(1000.0f * rng->GetExp());
      uint seed = rng->GetInt();
      Quantity quantity = value / resource->GetValue();
      object = Object_AsteroidRich(seed, resource, quantity);
    } else {
      object = Object_Asteroid(rng->GetInt());
    }

    float scale = Min(kMaxScale, 1 + args.scale.GetMax() * 0.01f * rng->GetExp());
    object->SetPos(center);
    object->SetScale(scale);

    V3 look = rng->GetDirection();
    V3 up = Normalize(Cross(look, rng->GetDirection()));
    object->SetLook(look);
    object->SetUp(up);
    args.parent->AddInterior(object);
  }

  return self;
}
