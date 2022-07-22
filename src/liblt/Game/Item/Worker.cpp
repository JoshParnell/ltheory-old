#include "../Items.h"

#include "Game/Attribute/Icon.h"
#include "Game/Attribute/Name.h"
#include "Game/Attribute/Value.h"

/* Engineer - Production
 * Miner - Mining
 * Pilot - Transportation
 * Researcher - Research
 */

typedef
    Attribute_Icon
  < Attribute_Name
  < Attribute_Value
  < ItemWrapper<ItemType_Worker>
  > > >
  WorkerBase;

AutoClassDerived(WorkerEngineer, WorkerBase,
  uint, level,
  Item, nextLevel)
  DERIVED_TYPE_EX(WorkerEngineer)

  WorkerEngineer() {}

  uint GetSkillEngineering() const {
    return level;
  }
};

AutoClassDerived(WorkerMiner, WorkerBase,
  uint, level,
  Item, nextLevel)
  DERIVED_TYPE_EX(WorkerMiner)

  WorkerMiner() {}

  uint GetSkillMiner() const {
    return level;
  }
};

AutoClassDerived(WorkerPilot, WorkerBase,
  uint, level,
  Item, nextLevel)
  DERIVED_TYPE_EX(WorkerPilot)

  WorkerPilot() {}

  uint GetSkillPiloting() const {
    return level;
  }
};

DefineFunction(Item_Worker_Engineer) {
  return new WorkerEngineer(args.level, args.nextLevel);
}

DefineFunction(Item_Worker_Miner) {
  return new WorkerMiner(args.level, args.nextLevel);
}

DefineFunction(Item_Worker_Pilot) {
  return new WorkerPilot(args.level, args.nextLevel);
}
