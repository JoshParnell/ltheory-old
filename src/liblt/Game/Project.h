#ifndef Game_Project_h__
#define Game_Project_h__

#include "Task.h"
#include "Object.h"
#include "Order.h"

#include "LTE/AutoClass.h"
#include "LTE/DeclareFunction.h"
#include "LTE/Pool.h"
#include "LTE/Vector.h"

AutoClass(ProjectAllocation,
  Object, object,
  Quantity, completed,
  Quantity, inProgress)

  ProjectAllocation() {}
};

AutoClass(ProjectManagement,
  Object, manager,
  TaskInstance, task)

  ProjectManagement() {}
};

AutoClass(ProjectMetrics,
  Time, duration,
  Quantity, cycles,
  Quantity, expense,
  Quantity, revenue,
  Quantity, losses)

  ProjectMetrics() :
    duration(0),
    cycles(0),
    expense(0),
    revenue(0),
    losses(0)
    {}

  Quantity GetNet() const {
    return revenue - (expense + losses);
  }
};

AutoClass(ProjectSettings,
  float, feedback)

  ProjectSettings() :
    feedback(0)
    {}
};

AutoClass(ProjectWallet,
  Quantity, credits,
  double, accumulator)

  ProjectWallet() : 
    credits(0),
    accumulator(0)
    {}
};

AutoClassDerived(ProjectT, RefCounted,
  String, name,
  Object, owner,
  Task, task,
  Vector<Project>, children,
  ProjectManagement, management,
  ProjectMetrics, metrics,
  ProjectSettings, settings,
  ProjectWallet, wallet,
  Vector<Order>, buys,
  Vector<Order>, sells,
  Vector<ProjectAllocation>, allocated,
  Quantity, gross,
  double, grossRate,
  Quantity, inProgress,
  bool, active)
  
  POOLED_TYPE

  ProjectT() :
    gross(0),
    grossRate(0),
    inProgress(0),
    active(true)
    {}

  ~ProjectT() {
    Stop();
  }

  LT_API void Stop();

  LT_API void Update(UpdateState&);

  LT_API void UpdateGross();

  LT_API void UpdateMarketOrders();
};

DeclareFunction(Project_Allocate, void,
  Project, project,
  Object, asset)

DeclareFunction(Project_Deallocate, void,
  Project, project,
  Object, asset)

DeclareFunction(Project_Create, Project,
  Object, owner,
  Task, task)

#endif
