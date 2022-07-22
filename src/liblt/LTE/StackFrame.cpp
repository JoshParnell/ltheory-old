#include "StackFrame.h"

#include "AutoClass.h"
#include "Profiler.h"
#include "ProgramLog.h"

const size_t kMaxStackDepth = 256;

namespace {
  AutoClass(StackFrameT,
    char const*, function,
    char const*, file,
    int,         line,
    char const*, annotation)
    StackFrameT() {}
  };

  StackFrameT frames[kMaxStackDepth];
  size_t depth = 0;
}

String StackFrame_Get() {
  String str;
  for (size_t i = 0; i < depth; ++i) {
    if (i)
      str += ".";
    str += frames[i].annotation;
  }
  return str;
}

void StackFrame_Print() {
  Log_Message("Stack Trace");

  for (size_t i = 0; i < depth; ++i) {
    StackFrameT& frame = frames[depth - (i + 1)];
    Log_Message(Stringize() | "  "
      | frame.annotation | " \t"
      | frame.file | " \t"
      | frame.line | " \t");
  }
}

void StackFrame_Push(
  char const* func,
  char const* file,
  int line,
  char const* annotation)
{
  LTE_ASSERT(depth + 1 < kMaxStackDepth);
  frames[depth++] = StackFrameT(func, file, line, annotation);
  Profiler_Push(annotation);
}

void StackFrame_Pop() {
  LTE_ASSERT(depth > 0);
  depth--;
  Profiler_Pop();
}
