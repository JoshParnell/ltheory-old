#ifndef LTE_Thread_h__
#define LTE_Thread_h__

#include "DeclareFunction.h"
#include "Reference.h"

struct ThreadT : public RefCounted {
  virtual ~ThreadT() {}

  virtual Job GetJob() const = 0;
  virtual bool IsFinished() const = 0;
  virtual void Terminate() = 0;
  virtual void Wait() = 0;
};

LT_API Thread Thread_Create(Job const& job);

DeclareFunction(Thread_SleepMS, void, uint, ms)
DeclareFunction(Thread_SleepUS, void, uint, us)

#endif
