#include "../Tasks.h"

#include "Component/Market.h"

#include "Game/Events.h"
#include "Game/Icons.h"
#include "Game/Mission.h"
#include "Game/Object.h"
#include "Game/ItemPropertys.h"
#include "Game/Template.h"

#include "LTE/Math.h"
#include "LTE/Pool.h"
#include "LTE/StackArray.h"
#include "LTE/StackFrame.h"

#include "LTE/Debug.h"

namespace {
#define XTYPE MetaprojectType
#define XLIST                                                                  \
  X(Develop)                                                                   \
  X(Expand)                                                                    \
  X(Monopolize)                                                                \
  X(Secure)

  #include "LTE/XEnum.h"
#undef XTYPE
#undef XLIST

  AutoClass(Metaproject,
    float, allocation,
    Quantity, wallet,
    double, walletAccumulator)

    Metaproject() :
      allocation(0),
      wallet(0),
      walletAccumulator(0)
      {}
  };

  typedef StackArray<Metaproject, MetaprojectType_SIZE> MetaprojectsT;

  AutoClass(TaskManageInstance,
    MetaprojectsT, metaprojects,
    Quantity, wallet)

    TaskManageInstance() :
      wallet(0)
      {}
  };

  AutoClassDerived(TaskManage, TaskT,
    Pointer<ProjectT>, project,
    Mission, missionMonopolize,
    Template, templateExpansion)
    DERIVED_TYPE_EX(TaskManage)
    POOLED_TYPE

    TaskManage() {}

    TaskManage(Project const& project) :
      project(project.t)
      {}

    ~TaskManage() {
      if (missionMonopolize)
        missionMonopolize->deleted = true;
    }

    String GetName() const {
      return "Manage";
    }

    String GetNoun() const {
      return "Manager";
    }

    /* Development Factor : output / # units. */
    void ManageDevelop(Metaproject& mp) {
      /* TODO. */
    }

    /* Expansion Factor : output. */
    void ManageExpand(Metaproject& mp) {
      /* TODO. */
      if (!templateExpansion) {
        templateExpansion = new TemplateT;
      }
    }

    /* Monopolization Factor : my supply / total supply. */
    void ManageMonopolize(Metaproject& mp) {
      if (missionMonopolize) {
        missionMonopolize->pool += mp.wallet;
        mp.wallet = 0;
        return;
      }

      if (!project->sells.size())
        return;

      Order const& order = project->sells.random();
      if (!order->node)
        return;

      Pointer<ComponentMarket> market = order->node->GetMarket();
      MarketData const& data = market->elements[order->item->GetHash()];
      for (size_t i = 0; i < data.asks.size(); ++i) {
        Order const& other = data.asks[i];
        if (project->owner != other->owner) {
          missionMonopolize = Mission_Create(project->owner);
          missionMonopolize->constraints.push(MissionConstraint_Equality(
            ItemProperty_Owner(), order->owner));
          missionMonopolize->name = "Damage the Enemy";
          missionMonopolize->description = "Do it.  Do it now.";
          missionMonopolize->price = 1.0;
          missionMonopolize->type = ItemType_DataDamaged;

          missionMonopolize->pool = mp.wallet;
          mp.wallet = 0;

          /* NOTE : Assuming market node has mission board..not necessarily true. */
          order->node->AddMissionListing(missionMonopolize);
        }
      }
    }

    /* Security Factor : asset loss / revenue */
    void ManageSecure(Metaproject& mp) {
      /* TODO. */
    }

    void OnBegin(Object const& self, Data& data) {
      data = TaskManageInstance();
      TaskManageInstance& it = data.Convert<TaskManageInstance>();
      /* TODO : Capital allocation based on personality. */
      it.metaprojects[MetaprojectType_Develop].allocation = 0.2f;
      it.metaprojects[MetaprojectType_Expand].allocation = 0.3f;
      it.metaprojects[MetaprojectType_Monopolize].allocation = 0.2f;
      it.metaprojects[MetaprojectType_Secure].allocation = 0.3f;
    }

    void OnUpdate(Object const& self, float dt, Data& data) { AUTO_FRAME;
      TaskManageInstance& it = data.Convert<TaskManageInstance>();

      /* Split the project wallet among metaprojects. */ {
        if (project->wallet.credits > 0) {
          it.wallet += project->wallet.credits;

          for (int i = 0; i < MetaprojectType_SIZE; ++i) {
            Metaproject& mp = it.metaprojects[i];
            mp.walletAccumulator += (double)project->wallet.credits * mp.allocation;

            if (mp.walletAccumulator >= 1.0) {
              Quantity amount = (Quantity)Floor(mp.walletAccumulator);
              amount = Min(amount, it.wallet);
              it.wallet -= amount;
              mp.wallet += amount;
              mp.walletAccumulator -= (double)amount;
            }
          }

          project->wallet.credits = 0;
        }
      }

      for (int i = 0; i < MetaprojectType_SIZE; ++i) {
        Metaproject& mp = it.metaprojects[i];
        if (mp.wallet > 0) {
          if (i == MetaprojectType_Develop)
            ManageDevelop(mp);
          else if (i == MetaprojectType_Expand)
            ManageExpand(mp);
          else if (i == MetaprojectType_Monopolize)
            ManageMonopolize(mp);
          else if (i == MetaprojectType_Secure)
            ManageSecure(mp);
        }
      }
    }

    void OnEnd(Object const& self, Data& data) {
      /* Return any remaining wallet contents to the project owner. */
      TaskManageInstance& it = data.Convert<TaskManageInstance>();
      if (it.wallet)
        project->owner->AddCredits(it.wallet);
      for (int i = 0; i < MetaprojectType_SIZE; ++i)
        if (it.metaprojects[i].wallet)
          project->owner->AddCredits(it.metaprojects[i].wallet);
    }
  };
}

DefineFunction(Task_Manage) {
  return new TaskManage(args.project);
}
