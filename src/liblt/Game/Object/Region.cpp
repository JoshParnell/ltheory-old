#include "../Objects.h"

#include "Component/Interior.h"
#include "Component/Nameable.h"
#include "Component/Orientation.h"
#include "Component/Resources.h"
#include "Component/Seeded.h"

#include "Game/Attribute/Traits.h"

#include "LTE/AutoPtr.h"
#include "LTE/Grammar.h"
#include "LTE/Graph.h"
#include "LTE/Hash.h"
#include "LTE/Pool.h"
#include "LTE/RNG.h"
#include "LTE/Script.h"
#include "LTE/Tuple.h"

const uint kMaxChildren = 1;//4;
const uint kPlanetCount = 1;

typedef ObjectWrapper
  < Attribute_Traits
  < Component_Interior
  < Component_Nameable
  < Component_Orientation
  < Component_Resources
  < Component_Seeded
  < ObjectWrapperTail<ObjectType_Region>
  > > > > > > >
  RegionBaseT;

AutoClassDerived(Region, RegionBaseT,
  uint, level)

  DERIVED_TYPE_EX(Region)
  POOLED_TYPE

  Region() {}
};

DERIVED_IMPLEMENT(Region)

/* TODO : Could we extend this to connect in-system POIs?? */
ObjectT* GetNearest(ObjectT* parent, Position const& target) {
  if (parent->GetType() == ObjectType_System)
    return parent;

  Distance bestDist = FLT_MAX;
  ObjectT* bestObject = nullptr;

  for (InteriorIterator it = Object_GetInteriorObjects(parent);
       it.HasMore(); it.Advance())
  {
    Distance thisDist = LengthSquared(it.Get()->GetPos() - target);
    if (thisDist < bestDist) {
      bestDist = thisDist;
      bestObject = it.Get();
    }
  }

  return GetNearest(bestObject, target);
}

void Connect(ObjectT* src, ObjectT* dst) {
  ObjectT* e1 = GetNearest(src, dst->GetPos());
  ObjectT* e2 = GetNearest(dst, src->GetPos());
  Object_Wormholes(e1, e2);
}

DefineFunction(Object_Region) {
  Reference<Region> self = new Region;
  self->level = args.level;
  self->Resources.elements = args.resources;
  self->Seeded.seed = args.seed;
  self->SetPos(args.pos);
  self->SetScale(args.radius);

  RNG rng = RNG_MTG(args.seed);
  self->Nameable.name = String_Capital(Grammar_Get()->Generate(rng, "$system", ""));

  struct ChildElement {
    Position pos;
    float radius;
  };

  const uint kMaxAttempts = 1000;
  const float kPadding = 0;

  float maxRadius = 0.7f * args.radius;
  float minRadius = 0.3f * args.radius;
  float maxOverlap = 0.1f * args.radius;

  uint childCount = rng->GetInt(1, kMaxChildren);
  childCount += (uint)(5.0f * rng->GetExp());

  Vector<ChildElement> children;

  for (uint i = 0; i < childCount; ++i) {
    for (uint j = 0; j < kMaxAttempts; ++j) {
      V2 co = rng->GetCircle();
      V3 thisOffset = (1 - kPadding) * args.radius * (V3(co.x, 0, co.y));
      Position thisPos = args.pos + thisOffset;
      float thisRadius = Min(maxRadius, args.radius - Length(thisOffset));

      for (size_t k = 0; k < children.size(); ++k) {
        V3 toOther = (V3)(thisPos - children[k].pos);
        float thisMax = Length(toOther) - children[k].radius + maxOverlap;
        thisRadius = Min(thisRadius, thisMax);
        if (thisRadius < minRadius)
          break;
      }

      if (thisRadius < minRadius)
        continue;

      ChildElement ce;
      ce.pos = thisPos;
      ce.radius = thisRadius;
      children << ce;
      break;
    }
  }

  Vector<ObjectT*> childObjects;

  /* Generate the children. */ {
    for (size_t i = 0; i < children.size(); ++i) {
      ChildElement& child = children[i];
      Object object;

      /* Vary the natural resource distribution. */
      /* TODO : Factor generating algorithm. */
      Distribution<Item> resources = self->Resources.elements;
      for (size_t j = 0; j < resources.size(); ++j)
        resources.getIndexValue(j) += rng->GetFloat(-0.1f, 0.1f);

      if (args.level > 0) {
        RegionType thisType;
        thisType.level = args.level - 1;
        thisType.pos = child.pos;
        thisType.radius = child.radius;
        thisType.resources = resources;
        thisType.seed = rng->GetInt();
        object = Object_Region(thisType);
      } else {
        SystemType thisType;
        thisType.position = child.pos;
        thisType.seed = rng->GetInt();
        object = Object_System(thisType);
        object->GetResources()->elements = resources;
        ScriptFunction_Load("Object/System:Init")->VoidCall(0, object);
      }

      object->SetPos(child.pos);
      object->SetScale(child.radius);
      self->AddInterior(object);
      childObjects << object;
    }
  }

  /* Connect the children using multiple spanning trees. */ {
    Vector< Tuple2<size_t, size_t> > connections;
    const size_t kConnectivity = 2;
    for (size_t i = 0; i < kConnectivity; ++i) {
      AutoPtr<Graph<ObjectT*>> graph = new Graph<ObjectT*>;
      for (size_t j = 0; j < childObjects.size(); ++j) {
        graph->Add(childObjects[j]);
        V3 p1 = childObjects[j]->GetPos();
        for (size_t k = j + 1; k < childObjects.size(); ++k) {
          V3 p2 = childObjects[k]->GetPos();
          graph->Add(j, k, Length(p1 - p2) * (1.0f + 0.5f * rng->GetExp()));
        }
      }

      AutoPtr<Graph<ObjectT*>> mst = graph->ComputeMST();
      for (size_t j = 0; j < mst->edges.size(); ++j) {
        size_t i1 = mst->edges[j].src;
        size_t i2 = mst->edges[j].dst;
        Tuple2<size_t, size_t> t = Tuple(Min(i1, i2), Max(i1, i2));
        if (!connections.contains(t)) {
          Connect(mst->nodes[i1].data, mst->nodes[i2].data);
          connections << t;
        }
      }
    }
  }

  return self;
}
