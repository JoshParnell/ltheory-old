#ifndef LTE_Job_h__
#define LTE_Job_h__

#include "Reference.h"
#include "String.h"

namespace LTE {
  struct JobT : public RefCounted {
    typedef void ReturnType;

    float totalTime;
    uint totalUnits;

    JobT() :
      totalTime(0),
      totalUnits(0)
      {}

    virtual ~JobT() {}
    
    virtual bool CanRun() const {
      return true;
    }

    virtual uint GetMemoryUsage() const {
      return 0;
    }

    virtual char const* GetName() const = 0;

    virtual float GetPriority() const {
      return 0.5f;
    };

    virtual bool IsFinished() const {
      return true;
    }

    virtual void OnBegin() {}
    virtual void OnEnd() {}
    virtual void OnRun(uint units) = 0;

    bool Started() const {
      return totalUnits > 0;
    }
  };
}

#endif
