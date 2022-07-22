#include "Thread.h"

#include "AutoPtr.h"
#include "Job.h"
#include "Lock.h"

#include "SFML/System.hpp"

namespace {
  Lock GetThreadLock() {
    /* NOTE : Locks cannot be static destructed. */
    static Lock* lock = new Lock(Lock_Create());
    return *lock;
  }

  struct ThreadImpl : public ThreadT {
    Job job;
    AutoPtr<sf::Thread> thread;
    bool finished;

    ThreadImpl(Job const& job) :
      job(job),
      finished(false)
    {
      ScopedLock lock(GetThreadLock());
      job->OnBegin();
      thread = new sf::Thread(&ThreadImpl::Run, this);
      thread->launch();
    }

    ~ThreadImpl() {
      if (!finished)
        Terminate();
      job->OnEnd();
    }

    Job GetJob() const {
      return job;
    }

    bool IsFinished() const {
      return finished;
    }

    void Run() {
      job->OnRun(UINT_MAX);
      finished = true;
    }

    void Terminate() {
      ScopedLock lock(GetThreadLock());
      thread->terminate();
    }

    void Wait() {
      thread->wait();
    }
  };
}

Thread Thread_Create(Job const& job) {
  return new ThreadImpl(job);
}

DefineFunction(Thread_SleepMS) {
  sf::sleep(sf::milliseconds(args.ms));
}

DefineFunction(Thread_SleepUS) {
  sf::sleep(sf::microseconds(args.us));
}
