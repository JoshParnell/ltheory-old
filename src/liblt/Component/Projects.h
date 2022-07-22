#ifndef Component_Projects_h__
#define Component_Projects_h__

#include "Common.h"
#include "Game/Project.h"
#include "LTE/AutoClass.h"
#include "LTE/Vector.h"

AutoClass(ComponentProjects,
  Vector<Project>, elements)

  ComponentProjects() {}
  
  void Run(ObjectT* self, UpdateState& state) {
    for (size_t i = 0; i < elements.size(); ++i)
      elements[i]->Update(state);
  }
};

AutoComponent(Projects)
  void OnUpdate(UpdateState& s) {
    Projects.Run(this, s);
    BaseT::OnUpdate(s);
  }
};

#endif
