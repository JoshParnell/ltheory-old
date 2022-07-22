#include "../Tasks.h"

#include "Component/Economy.h"
#include "Component/Market.h"
#include "Component/Projects.h"

#include "Game/Project.h"
#include "Game/Items.h"
#include "Game/NLP.h"

#include "LTE/Grammar.h"
#include "LTE/Hash.h"
#include "LTE/Map.h"
#include "LTE/Math.h"
#include "LTE/Pool.h"
#include "LTE/RNG.h"
#include "LTE/StackFrame.h"
#include "LTE/Vector.h"

#include "LTE/Debug.h"

const uint kMaxProjects = 1;
const Quantity kMinProfit = 1;

const size_t kMaxUnits = 16;
const Quantity kDiscount = 100;
const Quantity kMinShipCost = 100000 / kDiscount;

/* --- PLAYING LIMIT THEORY ---
 *
 * 1) Choose a set of operations that maximize gain given current capabilities
 * 2) Optimally allocate resources to chosen operations
 * 3) TODO Use income to create expansion projects
 * 4) TODO Use income to create competition projects
 * 4) TODO Use income to create personal projects
 *
 */

/* Global Meta-Projects
 * 1) Expansion
 * 2) Maintenance
 * 3) Monopolize
 * 4) Research
 * 5) Security
 */

namespace {
  Item GetShipType(Object const& self, Quantity value) {
    value = (Quantity)Pow(10.0, Floor(Log((double)value, 10.0)));
    value *= kDiscount;
    static uint counter = 0;
    static Map<Quantity, Item> types;
    Item& type = types[value];
    if (!type)
      type = Item_ShipType(value, counter++);
    self->RemoveCredits(value / kDiscount);
    return type;
  }

  AutoClassDerived(TaskPlay, TaskT, Task_Play_Args, args,
    Item, weaponType,
    Item, transferType)
    DERIVED_TYPE_EX(TaskPlay)
    POOLED_TYPE

    TaskPlay() {}

    TaskPlay(Task_Play_Args const& args) :
      args(args)
    {
      RNG rng = RNG_MTG(args.player->GetHash());
      args.player->SetName(String_Capital(Grammar_Get()->Generate(rng, "$faction", "")));
      weaponType = Item_WeaponType(rng->GetInt());
      transferType = Item_TransferUnitType(1, rng->GetInt(), 1, 1, 1, 1);
    }

    void AllocateFreeAsset(Object const& asset) {
      Pointer<ComponentProjects> projects = args.player->GetProjects();

      Project bestProject;
      double bestAllocation = 0.0;
      for (size_t i = 0; i < projects->elements.size(); ++i) {
        Project const& project = projects->elements[i];

        /* TODO : Real allocation evaluation */
        double thisAllocation = 1.0;
        if (thisAllocation > bestAllocation) {
          bestAllocation = thisAllocation;
          bestProject = project;
        }
      }

      if (bestProject) {
        asset->SetName(Stringize()
          | args.player->GetName() | " "
          | bestProject->task->GetNoun()
          | " #" | (bestProject->allocated.size() + 1));
        Project_Allocate(bestProject, asset);
      }
    }

    void ConfigureProject(Project const& project) {
      project->management.manager = (ObjectT*)args.player.t;
      project->management.task = TaskInstance(Task_Manage(project));
      project->settings.feedback = 0.9f;
    }

    void ConsiderProject(Task const& task) {
      Vector<Project>& projects = args.player->GetProjects()->elements;
      Project project = Project_Create(args.player, task);
      project->UpdateGross();

      if (project->gross < kMinProfit) {
        //dbg | "  >  " | project->gross | " (@ " | project->grossRate | ") not sufficient" | endl;
        return;
      }

      ConfigureProject(project);

      if (projects.size() < kMaxProjects) {
        projects.push(project);
        
        dbg | "Add Project " | project->task->GetName()
            | " (" | project->grossRate | ") " | endl;
      }

      /* CRITICAL : Project switching removed. */
#if 0
      /* Search for projects to replace. */
      else {
        for (size_t i = 0; i < projects.size(); ++i) {
          Project& oldProject = projects[i];

          if (oldProject->grossRate < project->grossRate) {
            oldProject->UpdateGross();

            if (oldProject->grossRate < project->grossRate) {
              oldProject->Stop();

              if (oldProject->inProgress == 0)
                oldProject = project;
#if 0
              dbg | "Switch Project " | projects[i]->task->GetName()
                  | " (" | projects[i]->grossRate | ") "
                  | " -> " | project->task->GetName()
                  | " (" | grossRate | ")" | endl;
#endif
            }
          }
        }
      }
#endif
    }

    void FindProject() {
      Pointer<ObjectT> container = args.player->GetContainer();
      Pointer<ComponentEconomy> economy = container->GetEconomy();

      while (!economy && container->GetContainer()) {
        container = container->GetContainer();
        economy = container->GetEconomy();
      }

      if (!economy || !economy->tasks.size() || !economy->nodes.size())
        return;

      Task task = economy->tasks.random();
      if (!task)
        return;

      /* Lawful players won't even consider 'bad' tasks. */
      float alignment = task->GetAlignment();
      if (alignment < 0 && args.player->traits.Lawless < -alignment)
        return;

      /* Bind inputs and outputs. */ {
        static Vector<ItemDelta> inputs;
        static Vector<ItemDelta> outputs;
        inputs.clear();
        outputs.clear();
        task->GetInputs(inputs);
        task->GetOutputs(outputs);
        Quantity scale = 10;

        for (size_t i = 0; i < inputs.size(); ++i) {
          ItemDelta const& input = inputs[i];
          if (!input.node) {
            Object location = economy->nodes.random();
            task = Task_Buy(location, input.item, scale * input.delta, task);
          }
        }

        for (size_t i = 0; i < outputs.size(); ++i) {
          ItemDelta const& output = outputs[i];
          if (!output.node) {
            Object location = economy->nodes.random();
            task = Task_Sell(location, output.item, scale * output.delta, task);
          }
        }
      }

      ConsiderProject(task);
    }

    String GetName() const {
      return "Play";
    }

    void ManageProject(Project const& project, float dt) {
       if (project->management.manager)
         project->management.task.OnUpdate(project->management.manager, dt);
    }

    void SpawnNew() {
      while (
        args.player->GetAssets()->elements.size() < kMaxUnits &&
        args.player->GetCredits() > kMinShipCost)
      {
        Quantity value = Pow(Rand(), 8.0f) * args.player->GetCredits();
        value = Max(value, kMinShipCost);
        Object const& piloting = args.player->piloting;
        Object ship = GetShipType(args.player, value)->Instantiate();
        ship->SetPos(piloting->GetPos() + 1000 * RandDirection());

        for (uint i = 0, n = RandI(1, 4); i < n; ++i)
          ship->Plug(transferType);
        while (ship->Plug(weaponType)) {}

        args.player->AddAsset(ship);
        piloting->GetContainer()->AddInterior(ship);
      }
    }

    void OnUpdate(Object const& self, float dt, Data& data) { AUTO_FRAME;
      /* TODO : Global capital allocation. */
      FindProject();

      /* Allocate Free Assets. */ {
        Pointer<ComponentAssets> assets = args.player->GetAssets();
        for (size_t i = 0; i < assets->elements.size(); ++i) {
          /* CRITICAL : Disable asset allocation. */
#if 1
          Object const& asset = assets->elements[i];
          Pointer<ComponentTasks> tasks = asset->GetTasks();
          if (tasks && !tasks->project)
            AllocateFreeAsset(asset);
#endif
        }
      }

      /* Manage Projects. */ {
        Pointer<ComponentProjects> projects = args.player->GetProjects();
        for (size_t i = 0; i < projects->elements.size(); ++i)
          ManageProject(projects->elements[i], dt);
      }

      SpawnNew();
    }
  };
}

DefineFunction(Task_Play) {
  return new TaskPlay(args);
}
