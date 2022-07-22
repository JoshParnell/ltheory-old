#ifndef Component_MissionBoard_h__
#define Component_MissionBoard_h__

#include "Common.h"
#include "Game/Mission.h"
#include "LTE/Vector.h"

AutoClass(ComponentMissionBoard,
  Vector<Mission>, elements)

  ComponentMissionBoard() {}

  void Run(ObjectT*, UpdateState&) {
    for (int i = 0; i < (int)elements.size(); ++i) {
      if (elements[i]->deleted) {
        elements.removeIndex(i);
        i--;
      }
    }
  }
};

AutoComponent(MissionBoard)
  void OnUpdate(UpdateState& s) {
    MissionBoard.Run(this, s);
    BaseT::OnUpdate(s);
  }

  void AddMissionListing(Mission const& mission) {
    MissionBoard.elements.push(mission);
  }

  void RemoveMissionListing(Mission const& mission) {
    MissionBoard.elements.remove(mission);
  }
};

#endif
