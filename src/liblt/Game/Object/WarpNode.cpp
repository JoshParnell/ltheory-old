#include "../Objects.h"

#include "Component/BoundingBox.h"
#include "Component/Cullable.h"
#include "Component/Detectable.h"
#include "Component/Drawable.h"
#include "Component/Motion.h"
#include "Component/Navigable.h"
#include "Component/Nameable.h"
#include "Component/Orientation.h"
#include "Component/Scriptable.h"

#include "Game/Materials.h"
#include "Game/Messages.h"

#include "Module/SoundEngine.h"

#include "LTE/DrawState.h"
#include "LTE/Meshes.h"
#include "LTE/Model.h"

#include "LTE/Debug.h"

const float kPadding = 5;
const float kRailForce = 2.0f * 2048;
const size_t kSplitPrecision = 16;
const int kInitialPartitions = 4;
const float kMinRadius = 200;
const float kMaxRadius = 5000;

namespace {
  typedef Reference<struct RailSlotT> RailSlot;
  typedef Reference<struct WarpNodeControllerT> WarpNodeController;

  Renderable GetModel() {
#if 0
    static Renderable model;
    if (!model) {
      ShaderInstance ss = ShaderInstance_Create("npm.jsl", "wormhole.jsl");
      (*ss)(RenderStateSwitch_BlendModeAdditive);
      DrawState_Link(ss);
      model = (Renderable)Model_Create()
        ->Add(Mesh_BoxSphere(5, true)
            ->ReverseWinding(), ss);
    }
    return model;
#else
    static Model model;
    if (!model)
      model = Model_Create()->Add(
        Mesh_BoxSphere(16, true)->SetU(1),
        Material_Ice());
    return model;
#endif
  }

  AutoClass(Passenger,
    Object, object,
    float, radius,
    RailSlot, slot,
    Object, prevNode,
    Object, nextNode,
    Object, target,
    V3, dir)
    Passenger() {}
  };

  AutoClassDerived(RailSlotT, RefCounted,
    Pointer<RailSlotT>, parent,
    Vector<RailSlot>, children,
    float, angle,
    float, angleSize,
    float, radius,
    float, radiusSize,
    float, innerRadius,
    float, innerPosition,
    bool, allocated)
    RailSlotT() {}

    RailSlotT(
        float angle,
        float angleSize,
        float radius,
        float radiusSize) :
      angle(angle),
      angleSize(angleSize),
      radius(radius),
      radiusSize(radiusSize),
      allocated(false)
    {
      ComputeInnerRadius();
    }

    void ComputeInnerRadius() {
      const int iterations = 8;
      const float angleFactor = Sin(kTau * angleSize / 2.0f);

      innerRadius = 0;

      for (int i = 0; i < iterations; ++i) {
        float t = (float)(i + 1) / (float)(iterations + 1);
        float h = radius + t * radiusSize;
        float inner = Min(h - radius, (radius + radiusSize) - h);
        inner = Min(inner, h * angleFactor);
        if (inner > innerRadius) {
          innerRadius = inner;
          innerPosition = h;
        }
      }
    }

    Position GetCenter(Object const& object) {
      float t = kTau * angle;
      V3 const& up = object->GetUp();
      V3 const& right = object->GetRight();
      V3 offset = innerPosition * (Cos(t) * up + Sin(t) * right);
      return object->GetPos() + offset;
    }
  };

  AutoClassDerived(WarpNodeControllerT, RefCounted,
    Vector<Passenger>, passengers,
    Vector<RailSlot>, slots)

    WarpNodeControllerT() {}

    void PrintSlot(RailSlot const& slot) {
      dbg
        | "(" | slot->radius
        | " - " | (slot->radius + slot->radiusSize)
        | ") (" | slot->angle
        | " - " | (slot->angle + slot->angleSize)
        | ")  " | slot->radiusSize
        | " x " | slot->angleSize
        | endl;
    }

    RailSlot Allocate(float radius) {
      /* Find the best free slot to use. */
      RailSlot slot;
      float minRadius = 0;
      for (size_t i = 0; i < slots.size(); ++i) {
        RailSlot const& thisSlot = slots[i];
        if (thisSlot->allocated ||
            thisSlot->innerRadius < radius)
          continue;

        if (!slot || thisSlot->radius < minRadius) {
          slot = thisSlot;
          minRadius = thisSlot->radius;
        }
      }

      if (!slot)
        return nullptr;

      slot->allocated = true;

      float angleLower = 0;
      float angleUpper = slot->angleSize;
      float radiusLower = 0;
      float radiusUpper = slot->radiusSize;

      /* Binary search the best split position. */
      for (size_t i = 0; i < kSplitPrecision; ++i) {
        /* Angular step. */ {
          float thisAngle = (angleLower + angleUpper) / 2.0f;
          RailSlotT newSlot(slot->angle, thisAngle, slot->radius, radiusUpper);
          if (newSlot.innerRadius >= radius)
            angleUpper = thisAngle;
          else
            angleLower = thisAngle;
        }

        /* Radial step. */ {
          float thisRadius = (radiusLower + radiusUpper) / 2.0f;
          RailSlotT newSlot(slot->angle, angleUpper, slot->radius, thisRadius);
          if (newSlot.innerRadius >= radius)
            radiusUpper = thisRadius;
          else
            radiusLower = thisRadius;
        }
      }

      /* Split slot into 4 pieces based on minimal radial & angular sizes. */
      RailSlot split1 = new RailSlotT(
        slot->angle,
        angleUpper,
        slot->radius,
        radiusUpper);

      RailSlot split2 = new RailSlotT(
        slot->angle + angleUpper,
        slot->angleSize - angleUpper,
        slot->radius,
        radiusUpper);

      RailSlot split3 = new RailSlotT(
        slot->angle,
        angleUpper,
        slot->radius + radiusUpper,
        slot->radiusSize - radiusUpper);

      RailSlot split4 = new RailSlotT(
        slot->angle + angleUpper,
        slot->angleSize - angleUpper,
        slot->radius + radiusUpper,
        slot->radiusSize - radiusUpper);

      split1->parent = slot;
      split2->parent = slot;
      split3->parent = slot;
      split4->parent = slot;
      slot->children.push(split1);
      slot->children.push(split2);
      slot->children.push(split3);
      slot->children.push(split4);
      slots.push(split1);
      slots.push(split2);
      slots.push(split3);
      slots.push(split4);
      split1->allocated = true;

#if 0
      PrintSlot(slot);
      PrintSlot(split1);
      PrintSlot(split2);
      PrintSlot(split3);
      PrintSlot(split4);
      dbg | " ------- " | endl;
      dbg | waitkey;
#endif
      return split1;
    }

    RailSlot Enter(
      Object const& object,
      Object const& node,
      Object const& target)
    {
      float radius = object->GetRadius() + kPadding;
      RailSlot slot = Allocate(radius);
      if (slot) {
        V3 dir = Normalize(node->GetPos() - object->GetPos());
        passengers.push(Passenger(object, radius, slot, node, node, target, dir));
      }
      return slot;
    }

    void Exit(Passenger const& passenger) {
      passenger.object->Send(MessageEjected());
      static ScriptFunction ejectPassenger =
        ScriptFunction_Load("Object/WarpNode:EjectPassenger");
      ejectPassenger->VoidCall(0, DataRef(passenger.object));
      Free(passenger.slot);
    }

    void Free(RailSlot const& slot) {
      slot->allocated = false;

      /* Check if siblings can be merged. */
      RailSlot parent = slot->parent.t;
      if (parent) {
        bool childAllocated = false;
        for (size_t i = 0; i < parent->children.size(); ++i) {
          if (parent->children[i]->allocated) {
            childAllocated = true;
            break;
          }
        }

        /* Merge. */
        if (!childAllocated) {
          for (size_t i = 0; i < parent->children.size(); ++i)
            slots.remove(parent->children[i]);
          Free(parent);
        }
      }
    }

    bool HasPassenger(Object const& object) {
      for (size_t i = 0; i < passengers.size(); ++i)
        if (passengers[i].object == object)
          return true;
      return false;
    }

    void Initialize() {
      for (int i = 0; i < kInitialPartitions; ++i) {
        RailSlot slot = new RailSlotT;
        slot->angle = (float)i / (float)kInitialPartitions;
        slot->angleSize = 1.0f / (float)kInitialPartitions;
        slot->radius = kMinRadius;
        slot->radiusSize = kMaxRadius - kMinRadius;
        slot->allocated = false;
        slot->ComputeInnerRadius();
        slots.push(slot);
      }
    }

    void Update(float dt) {
      static ScriptFunction updatePassenger =
        ScriptFunction_Load("Object/WarpNode:UpdatePassenger");

      /* Apply forces to passengers. */
      for (int i = 0; i < (int)passengers.size(); ++i) {
        Passenger& passenger = passengers[i];
        Object const& object = passenger.object;
        Position pos = object->GetPos();
        Position targetPos = passenger.slot->GetCenter(passenger.nextNode);
        V3 look = object->GetLook();
        V3 toTarget = targetPos - pos;
        V3 dir = Normalize(toTarget);

        updatePassenger->VoidCall(0, DataRef(object));

        /* Apply force toward next node. */
        float railForce = kRailForce;
        float radiusNorm = (passenger.slot->radius - kMinRadius) / (kMaxRadius - kMinRadius);
        railForce *= Mix(2.0f, 1.0f, radiusNorm);

        float mag = (passenger.prevNode ? railForce : 256.0f) * object->GetMass();
        object->ApplyForce(mag * dir);

        V3 torque = object->GetTransform().InverseDir(Cross(look, dir));
        torque -= object->GetVelocityA() / kAngularDrag;
        torque *= object->GetMotion()->inertia;
        object->ApplyTorque(torque);

        /* Apply lane-locking force. */
        if (passenger.prevNode != passenger.nextNode) {
          Position lastTarget = passenger.slot->GetCenter(passenger.prevNode);
          V3 laneVector = Normalize(targetPos - lastTarget);
          V3 toTargetComp = targetPos - (pos + object->GetVelocity() / kLinearDrag);
          V3 toLane = toTargetComp - laneVector * Dot(toTargetComp, laneVector);
          // object->SetPos(object->GetPos() + 0.25f * toLane);
          object->ApplyForce(object->GetMass() * toLane);
        }

        if (Length(toTarget) < 16.0f || Dot(passenger.dir, dir) < 0) {
          ComponentNavigable* nav = passenger.nextNode->GetNavigable();
          Object nextNode;

          for (size_t j = 0; j < nav->nodes.size(); ++j) {
            Object const& link = nav->nodes[j].dest;
            if (link != passenger.prevNode) {
              nextNode = link;
              if (link == passenger.target)
                break;
            }
          }

          if (nextNode) {
            passenger.prevNode = passenger.nextNode;
            passenger.nextNode = nextNode;
            passenger.dir = Normalize(nextNode->GetPos() - pos);
            object->Send(MessageWaypoint(passenger.nextNode));
          } else {
            Object lastNode = passenger.nextNode;
            Exit(passenger);
            passengers.removeIndex(i);
            object->Send(MessageWaypoint(lastNode));
            i--;
            continue;
          }
        }
      }
    }
  };
}

typedef ObjectWrapper
  < Component_BoundingBox
  < Component_Cullable
  < Component_Detectable
  < Component_Drawable
  < Component_Navigable
  < Component_Nameable
  < Component_Orientation
  < Component_Scriptable
  < ObjectWrapperTail<ObjectType_WarpNode>
  > > > > > > > > >
  WarpNodeBaseT;

AutoClassDerived(WarpNode, WarpNodeBaseT,
  WarpNodeController, controller,
  bool, owner)
  DERIVED_TYPE_EX(WarpNode)

  WarpNode() {
    Drawable.renderable = GetModel;
  }

  void Initialize() {
    owner = false;
  }

  void OnMessage(Data& m) {
    BaseType::OnMessage(m);
    if (m.type == Type_Get<MessageStartUsing>()) {
      MessageStartUsing const& message = m.Convert<MessageStartUsing>();
      if (!controller->HasPassenger(message.object)) {
        Object target = message.target;
        if (!target)
          target = Navigable.nodes[0].dest;
        RailSlot slot = controller->Enter(message.object, this, target);
#if 0
        if (!slot)
          debugprint;
#endif
      }
    }

    else if (m.type == Type_Get<MessageStopUsing>()) {
      MessageStopUsing const& message = m.Convert<MessageStopUsing>();
      for (size_t i = 0; i < controller->passengers.size(); ++i) {
        if (controller->passengers[i].object == message.object) {
          controller->Exit(controller->passengers[i]);
          controller->passengers.removeIndex(i);
          return;
        }
      }
    }

    /* Link this node into a new network. */
    if (m.type == Type_Get<MessageLink>()) {
      MessageLink const& message = m.Convert<MessageLink>();
      // Distance distance = Length(message.object->GetPos() - GetPos());
      // float cost = distance / kRailForce;
      Navigable.nodes.push(NavigableNode(message.object, 0));
      WarpNode& other = *(WarpNode*)message.object.t;

      if (other.controller)
        controller = other.controller;
      else {
        controller = new WarpNodeControllerT;
        controller->Initialize();
        owner = true;
        other.controller = controller;
      }
    }
  }

  void OnUpdate(UpdateState& state) {
    BaseType::OnUpdate(state);
    if (owner)
      controller->Update(state.dt);
  }

}; DERIVED_IMPLEMENT(WarpNode)

DefineFunction(Object_WarpNode) {
  Reference<WarpNode> self = new WarpNode;
  self->Initialize();
  self->SetName("Warp Node");
  ScriptFunction_Load("Object/WarpNode:Init")->VoidCall(0, DataRef((Object)self));
  return self;
}
