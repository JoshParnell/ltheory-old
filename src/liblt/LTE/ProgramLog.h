#ifndef LTE_ProgramLog_h__
#define LTE_ProgramLog_h__

#include "String.h"

namespace LTE {
  namespace LogLevel {
    enum Enum {
      Nothing,
      Errors,
      Warnings,
      Everything
    };
  }

  LT_API void Log_Critical(String const& entry);
  LT_API void Log_Error(String const& entry);
  LT_API void Log_Event(String const& entry);
  LT_API void Log_Message(String const& entry);
  LT_API void Log_Warning(String const& entry);

  LT_API size_t Log_GetEntries();
  LT_API String const& Log_GetEntry(int index);
}

#endif
