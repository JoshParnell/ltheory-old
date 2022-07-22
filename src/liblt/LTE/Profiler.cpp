#include "Profiler.h"
#include "Array.h"
#include "HashMap.h"
#include "Job.h"
#include "Lock.h"
#include "Map.h"
#include "Math.h"
#include "Module.h"
#include "Pointer.h"
#include "ProgramLog.h"
#include "Renderer.h"
#include "Stack.h"
#include "String.h"
#include "Timer.h"
#include "Thread.h"
#include "V3.h"

#include "Module/Settings.h"

// #define USE_TELEMETRY

#ifdef LIBLT_LINUX
  #ifdef USE_TELEMETRY
    #define linux
    #include "telemetry/telemetry.h"
    #define TELEMETRY
  #else
    #include <iostream>
  #endif
#endif

#include <algorithm>
#include <cstring>
#include <iostream>

const bool kDefaultFlush = false;

/* Number of microseconds to wait between each interrupt. */
const uint kSampleInterval = 1;

struct StackFrame {
  Array<char const*> segments;

  StackFrame(Stack<char const*> const& frame) :
    segments(frame.size())
  {
    for (size_t i = 0; i < frame.size(); ++i)
      segments[i] = frame[i];
  }

  String ToString() const {
    Stringize str;
    for (size_t i = 0; i < segments.size(); ++i) {
      if (i)
        str | " . ";
      str | segments[i];
    }
    return str;
  }
};

namespace {
#ifdef TELEMETRY
  HTELEMETRY g_context;

  struct ProfilingModule : public ModuleT {
    TmU8* arena;

    ProfilingModule() {
      tmLoadTelemetry(TM_LOAD_CHECKED_LIBRARY);
      tmStartup();         // Only call this once

      const unsigned int ARENA_SIZE = 2 * 1024 * 1024; // How much memory you want Telemetry to use
      arena = (TmU8*) malloc( ARENA_SIZE );
      tmInitializeContext( &g_context, (void*)arena, ARENA_SIZE );

      if (tmOpen(
            g_context,
            "Limit Theory",
            __DATE__ " " __TIME__,
            "localhost",
            TMCT_TCP,
            TELEMETRY_DEFAULT_PORT,
            TMOF_DEFAULT, 1000) != TM_OK )
        Log_Warning("Could not connect to telemetry server");
    }

    ~ProfilingModule() {
      tmClose(g_context);
      tmShutdownContext(g_context);
      tmShutdown();
      free(arena);
    }

    void Auto(float duration) {}

    void Flush() {}

    char const* GetName() const {
      return "Telemetry Profiler";
    }

    void Pop() {
      tmLeave(g_context);
    }

    void Push(char const* name) {
      tmEnter(g_context, TMZF_NONE, name, "");
    }

    void SetFlushes(bool flushes) {}
    
    void Start() {}
    
    void Stop() {}

    void Update() {
      tmTick(g_context);
    }
  };

#else
  AutoClass(FrameSamples,
    StackFrame*, frame,
    uint, samples)

    FrameSamples() {}

    friend bool operator<(FrameSamples const& a, FrameSamples const& b) {
      return a.samples > b.samples;
    }
  };

  struct ProfilingModule : public ModuleT {
    StackFrame* currentFrame;
    Stack<char const*> segments;
    HashMap<size_t, StackFrame*> frameMap;

    bool active;
    bool flushes;

    size_t totalFrames;
    size_t totalSamples;
    Timer timer;
    Map<StackFrame*, uint> samples;
    float maxTime;

    Thread samplingThread;
    Lock samplesLock;

    struct SamplerJob : public JobT {
      ProfilingModule* profiler;

      SamplerJob(ProfilingModule* profiler) :
        profiler(profiler)
        {}

      char const* GetName() const {
        return "Sampler";
      }

      void OnRun(uint) {
        while (true) {
          if (profiler->active) {
            {
              ScopedLock lock(profiler->samplesLock);
              StackFrame* current = profiler->currentFrame;
              if (current)
                profiler->samples[current]++;
              profiler->totalSamples++;
            }
            Thread_SleepUS(1 + rand() % kSampleInterval);
          } else
            Thread_SleepMS(16);
        }
      }
    };

    ProfilingModule() :
      currentFrame(nullptr),
      active(false),
      flushes(kDefaultFlush),
      maxTime(-1),
      samplesLock(Lock_Create())
    {
      samplingThread = Thread_Create(new SamplerJob(this));
    }

    void Auto(float duration) {
      maxTime = duration;
      Start();
    }

    bool CanDelete() const {
      return false;
    }

    void Flush() {
      if (active)
        Renderer_Flush();
    }

    StackFrame* GetFrame() {
      HashT hash = Hash(
        (void const*)segments.data(), segments.size() * sizeof(char const*));
      StackFrame*& thisFrame = frameMap[hash];
      if (!thisFrame)
        thisFrame = new StackFrame(segments);
      return thisFrame;
    }

    char const* GetName() const {
      return "Profiler";
    }

    void Push(char const* segment) {
      segments.push(segment);
      if (active)
        currentFrame = GetFrame();
    }

    void Pop() {
      if (active && flushes)
        Renderer_Flush();
      segments.pop();
      if (active)
        currentFrame = GetFrame();
    }

    void SetFlushes(bool flushes) {
      this->flushes = flushes;
    }

    void Start() {
      ScopedLock lock(samplesLock);
      samples.clear();
      totalFrames = 1;
      totalSamples = 0;
      timer.Reset();
      active = true;
    }

    void Stop() {
      active = false;
      ScopedLock lock(samplesLock);
      float elapsed = timer.GetElapsed();
      maxTime = -1;

      Vector<FrameSamples> sorted;
      for (Map<StackFrame*, uint>::iterator it = samples.begin();
           it != samples.end(); ++it)
        sorted.push(FrameSamples(it->first, it->second));

      std::sort(sorted.begin(), sorted.end());
      std::cout
        << totalSamples << " samples collected over "
        << totalFrames << " frames"
        << std::endl;
      std::cout << "% ----- %C ---- # ----- MS ---- MST --- Name " << std::endl;
      float total = 0;
      float msTotal = 0;

      for (size_t i = 0; i < sorted.size(); ++i) {
        FrameSamples& f = sorted[i];
        double fraction = (double)f.samples / totalSamples;

        if (total >= 0.9375f)
          break;

        float prevTotal = total;
        total += fraction;
        std::cout << ToString(0.1 * Round(1000.0 * fraction)) << "% \t";
        std::cout << ToString(Round(100.0 * total)) << "% \t";
        std::cout << ToString(f.samples) << " \t";

        float ms = (float)(fraction * 1000.0 * elapsed / totalFrames);
        msTotal += ms;
        std::cout << ToString(0.01 * Round(100.0 * ms)) << " \t";
        std::cout << ToString(0.01 * Round(100.0 * msTotal)) << " \t";
        std::cout << f.frame->ToString() << " " << std::endl;

        if (prevTotal < 0.5f && total >= 0.5f)
          std::cout << "---------------- 50% (1x) -----------------" << std::endl;
        if (prevTotal < 0.75f && total >= 0.75f)
          std::cout << "---------------- 75% (2x) -----------------" << std::endl;
        if (prevTotal < 0.875f && total >= 0.875f)
          std::cout << "---------------- 87.5% (4x) ---------------" << std::endl;
      }
      std::cout << "---------------- 93.75% (8x) --------------" << std::endl;
      std::cout << std::flush;
    }

    void Update() {
      if (active) {
        totalFrames++;
        if (maxTime > 0 && timer.GetElapsed() >= maxTime)
          Stop();
      }
    }
  };
#endif

  ProfilingModule* GetProfiler() {
    static Reference<ProfilingModule> profiler;
    if (!profiler) {
      profiler = new ProfilingModule;
      Module_RegisterGlobal(profiler);
    }
    return profiler;
  }
}

#ifdef ENABLE_PROFILING

void Profiler_Flush() {
  GetProfiler()->Flush();
}

void Profiler_Pop() {
  GetProfiler()->Pop();
}

void Profiler_Push(char const* name) {
  GetProfiler()->Push(name);
}

#endif

void Profiler_SetFlushes(bool flushes) {
  GetProfiler()->SetFlushes(flushes);
}

DefineFunction(Profiler_Auto) {
  GetProfiler()->Auto(args.duration);
}

DefineFunction(Profiler_Start) {
  GetProfiler()->Start();
}

DefineFunction(Profiler_Stop) {
  GetProfiler()->Stop();
}
