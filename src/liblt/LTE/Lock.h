#ifndef LTE_Lock_h__
#define LTE_Lock_h__

#include "Reference.h"

struct LockT : public RefCounted {
  virtual ~LockT() {}
  virtual void Acquire() = 0;
  virtual void Release() = 0;
};

struct ScopedLock {
  Lock lock;

  ScopedLock(Lock const& lock) :
    lock(lock)
  {
    lock->Acquire();
  }

  ~ScopedLock() {
    lock->Release();
  }
};

LT_API Lock Lock_Create();

#endif
