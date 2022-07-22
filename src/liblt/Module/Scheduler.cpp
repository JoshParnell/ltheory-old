#include "Scheduler.h"

#include "LTE/Config.h"
#include "LTE/GL.h"
#include "LTE/Job.h"
#include "LTE/Module.h"
#include "LTE/Pointer.h"
#include "LTE/Renderer.h"
#include "LTE/Texture2D.h"
#include "LTE/Thread.h"
#include "LTE/Timer.h"
#include "LTE/Tuple.h"
#include "LTE/ShaderInstance.h"
#include "LTE/StackFrame.h"
#include "LTE/Vector.h"

const float kMaxRunTime = 1.0f / 60.0f;

const bool kEnableFlushing = true;
const bool kEnableThreading = true;
const uint kMaxConcurrentThreads = 8;
const uint kMinJobSize = 1;
const uint kMaxMemoryUsage = 1024 * 1024;

namespace {
  AutoClass(ThreadedJob,
    Job, job,
    Thread, thread)
    ThreadedJob() {}
  };

  struct SchedulerImpl : public Scheduler {
    Vector<Job> serialJobs;
    Vector<ThreadedJob> threadedJobs;
    Vector<ThreadedJob> runningThreads;
    size_t memoryInUse;

    SchedulerImpl() : memoryInUse(0) {}

    ~SchedulerImpl() {
      for (size_t i = 0; i < runningThreads.size(); ++i)
        runningThreads[i].thread->Terminate();
    }

    void Add(Job const& job, bool threaded) {
      if (threaded && kEnableThreading)
        threadedJobs << ThreadedJob(job, nullptr);
      else
        serialJobs << job;
    }

    void Flush() {
      if (kEnableFlushing)
        while (HasJobs())
          Update();
    }

    Job GetBestJob(float remainingTime) {
      float bestPriority = 0;
      Job bestJob;

      for (size_t i = 0; i < serialJobs.size(); ++i) {
        if (serialJobs[i]->CanRun() &&
            (!bestJob || serialJobs[i]->GetPriority() > bestPriority))
        {
          bestPriority = serialJobs[i]->GetPriority();
          bestJob = serialJobs[i];
        }
      }

      return bestJob;
    }

    char const* GetName() const {
      return "Scheduler";
    }

    bool HasThreadedJobs() const {
      return threadedJobs.size() > 0 || runningThreads.size() > 0;
    }

    bool HasSerialJobs() const {
      return serialJobs.size() > 0;
    }

    void ManageThreads() {
      /* Clean up finished threads. */ {
        for (int i = 0; i < (int)runningThreads.size(); ++i) {
          ThreadedJob job = runningThreads[i];
          if (job.thread->IsFinished()) {
            size_t memory = job.job->GetMemoryUsage();
            runningThreads.eraseIndex(i);
            i--;
            memoryInUse -= memory;
          }
        }
      }

      /* Launch new threads. */ {
        while (
          memoryInUse < kMaxMemoryUsage &&
          runningThreads.size() < kMaxConcurrentThreads &&
          threadedJobs.size())
        {
          ThreadedJob job = threadedJobs.front();
          threadedJobs.eraseIndex(0);
          job.thread = Thread_Create(job.job);
          memoryInUse += job.job->GetMemoryUsage();
          runningThreads << job;
        }
      }
    }

    void Update() {
      ManageThreads();

      for (int i = 0; i < (int)serialJobs.size(); ++i) {
        Job const& job = serialJobs[i];
        if (job->Started() && job->IsFinished()) {
          job->OnEnd();
          serialJobs.removeIndex(i);
          i--;
        }
      }

      RendererBlendMode blendMode(BlendMode::Disabled);
      RendererCullMode cullMode(CullMode::Backface);
      RendererZBuffer zBuffer(false);
      Renderer_PushScissorOff();

      float remainingTime = kMaxRunTime;
      bool flushed = false;

      while (remainingTime > 0) {
        Job job = GetBestJob(remainingTime);
        if (!job)
          break;

        if (!flushed) {
          FRAME("Pre Flush")
            GL_Finish();
          flushed = true;
        }

        FRAME(job->GetName()) {
          uint jobSize = job->totalUnits > 0
            ? (uint)Floor(remainingTime / (job->totalTime / (float)job->totalUnits))
            : 0;
          jobSize = Max(jobSize, kMinJobSize);

          if (job->totalUnits == 0)
            job->OnBegin();

          Timer timer;
          job->OnRun(jobSize);
          GL_Finish();

          float elapsed = timer.GetElapsed();
          job->totalUnits += jobSize;
          job->totalTime += elapsed;
          remainingTime -= elapsed;

          if (job->IsFinished()) {
            job->OnEnd();
            serialJobs.erase(job);
          }
        }
      }

      Renderer_PopScissor();
    }
  };
}

void Scheduler_Add(Job const& job, bool threaded) {
  Scheduler_Get()->Add(job, threaded);
}

Scheduler* Scheduler_Get() {
  static Reference<SchedulerImpl> scheduler;
  if (!scheduler) {
    scheduler = new SchedulerImpl;
    Module_RegisterGlobal(scheduler);
  }
  return scheduler;
}
