#include "../Tasks.h"

#include "Component/Interior.h"
#include "Component/Mineable.h"
#include "Component/MotionControl.h"
#include "Component/Resources.h"
#include "Component/Sockets.h"
#include "Component/Storage.h"

#include "Game/Conditions.h"
#include "Game/Events.h"
#include "Game/Messages.h"
#include "Game/Object.h"

#include "LTE/Math.h"
#include "LTE/Pool.h"
#include "LTE/SDFs.h"
#include "LTE/StackFrame.h"
#include "LTE/Smooth.h"
#include "LTE/Vector.h"

#include "LTE/Debug.h"

const uint kAmount = 10;
const Distance kMineDistance = 500.0;

namespace {
  AutoClass(TaskMineInstance,
    Smooth<V3>, angle,
    Object, target,
    float, timer,
    Vector< Smooth<V3> >, offsets)

    TaskMineInstance() :
      angle(V3(1, 0, 0)),
      timer(0)
      {}
  };

  AutoClassDerived(TaskMine, TaskT,
    Task_Mine_Args, args)
    DERIVED_TYPE_EX(TaskMine)
    POOLED_TYPE

    TaskMine() {}

    bool CanMine(Object const& self) const {
      return self->GetFreeCapacity() > kAmount;
    }

    /* TODO : Estimate of travel time. */
    float GetDuration() const {
      return 1.0f;
    }

    String GetName() const {
      return "Mine";
    }

    String GetNoun() const {
      return "Mining Barge";
    }

    void GetOutputs(Vector<ItemDelta>& outputs) const {
      Pointer<ComponentResources> resources = args.zone->GetResources();
      for (size_t i = 0; i < resources->elements.size(); ++i)
        outputs.push(ItemDelta(resources->elements.entries[i].key, nullptr, 1));
    }

    Capability GetRateFactor() const {
      return Capability_Transference(1);
    }

    void OnBegin(Object const& self, Data& data) {
      data = TaskMineInstance();
    }

    void OnUpdate(Object const& self, float dt, Data& data) { AUTO_FRAME;
      TaskMineInstance& it = data.Convert<TaskMineInstance>();
      it.timer -= dt;

      if (it.timer <= 0) {
        it.angle.target = RandDirection();
        it.timer = Rand(1, 5);
        for (size_t i = 0; i < it.offsets.size(); ++i)
          it.offsets[i].target = SampleSphere();
      }

      it.angle.Update(0.4f * dt);
      it.angle.value = Normalize(it.angle.value);
      for (size_t i = 0; i < it.offsets.size(); ++i)
        it.offsets[i].Update(0.5f * dt);

      if (it.target &&
          it.target->GetMineable() &&
          it.target->GetMineable()->quantity >= 10)
      {
        if (Condition_Nearby(self, it.target, kMineDistance)) {
          Pointer<ComponentMotionControl> control = self->GetMotionControl();
          float targetRadius = it.target->GetRadius();
          float r = targetRadius + self->GetRadius();
          control->elements.push(SDF_Sphere(
            it.target->GetCenter() + r * it.angle.value, 1));

          Pointer<ComponentSockets> sockets = self->GetSockets();
          uint index = 0;
          for (size_t i = 0; i < sockets->instances.size(); ++i) {
            Object const& o = sockets->instances[i];
            if (!o || o->GetCapability().Transference <= 0)
              continue;

            index++;
            while (it.offsets.size() < index)
              it.offsets.push(Smooth<V3>(RandDirection()));

            o->Broadcast(MessageFire());
            o->Broadcast(MessageTargetPosition(
              (0.25f * targetRadius * it.offsets[index].value + it.target->GetCenter())));
          }
        } else
          self->PushTask(Task_Goto(it.target, 0.9 * kMineDistance));
      }

      /* TODO : Better target selection. */
      else {
        ComponentInterior* interior = args.zone->GetContainer()->GetInterior();
        it.target = interior->objects[RandI(interior->objects.size())];
      }
    }
  };
}

DefineFunction(Task_Mine) {
  return new TaskMine(args);
}
