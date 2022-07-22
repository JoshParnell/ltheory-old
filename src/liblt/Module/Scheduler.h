#ifndef Module_Scheduler_h__
#define Module_Scheduler_h__

#include "LTE/Common.h"
#include "LTE/Module.h"
#include "LTE/String.h"
#include "LTE/Thread.h"

struct Scheduler : public ModuleT {
  virtual void Add(Job const& job, bool threaded = false) = 0;
  virtual void Flush() = 0;
  virtual bool HasSerialJobs() const = 0;
  virtual bool HasThreadedJobs() const = 0;

  bool HasJobs() const {
    return HasSerialJobs() || HasThreadedJobs();
  }
};

LT_API void Scheduler_Add(Job const& job, bool threaded);
LT_API Scheduler* Scheduler_Get();

#endif
