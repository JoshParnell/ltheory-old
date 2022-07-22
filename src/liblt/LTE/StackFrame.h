#ifndef LTE_StackFrame_h__
#define LTE_StackFrame_h__

#include "Common.h"
#include "BuildMode.h"

#ifdef LIBLT_WINDOWS
#define _FNAME_ __FUNCTION__
#else
#define _FNAME_ __func__
#endif

LT_API String StackFrame_Get();

LT_API void StackFrame_Print();

LT_API void StackFrame_Pop();

LT_API void StackFrame_Push(
  char const* func,
  char const* file,
  int line,
  char const* annotation);

struct ScopedFrame {
  int i;

  ScopedFrame(
      char const* func,
      char const* file,
      int line, 
      char const* annotation = "") :
    i(0)
  {
    StackFrame_Push(func, file, line, annotation);
  }

  ~ScopedFrame() {
    StackFrame_Pop();
  }
};

#ifdef BUILD_DEBUG

#define AUTO_FRAME ScopedFrame __sframe(_FNAME_, __FILE__, __LINE__, _FNAME_)
#define SFRAME(x) ScopedFrame __sframe(_FNAME_, __FILE__, __LINE__, x)
#define FRAME(x)                                                       \
  for (ScopedFrame __f(_FNAME_, __FILE__, __LINE__, x); __f.i < 1; __f.i++)

#else

#define AUTO_FRAME
#define SFRAME(x)
#define FRAME(x)

#endif

#endif
