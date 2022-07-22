#include "ProgramLog.h"
#include "BuildMode.h"
#include "OS.h"
#include "StackFrame.h"
#include "Vector.h"
#include "Window.h"

#include <iostream>
#include <fstream>

const uint kMaxEntries = 4096;
const bool kLogToFile = true;
String const kLogFile = OS_GetUserDataPath() + "logErrors.txt";

const LogLevel::Enum level = LogLevel::Everything;

namespace {
  Vector<String>& GetEntryVector() {
    static Vector<String> entries;
    return entries;
  }

  std::ofstream& GetLogFile() {
    static std::ofstream logFile(kLogFile.c_str());
    return logFile;
  }

  static void DoLog(String const& entry) {
    if (GetEntryVector().size() < kMaxEntries) {
      std::stringstream stream;
      stream << entry;
      String buffer;
      while (getline(stream, buffer)) {
        GetEntryVector().push(buffer);
        std::cout << entry << '\n';
        std::cout << std::flush;
        if (kLogToFile)
          GetLogFile() << entry << '\n' << std::flush;
      }
    }
  }
}

namespace LTE {
  void Log_Critical(String const& entry) {
#ifdef BUILD_RELEASE
    GetWindow()->close();
#endif
    DoLog("[CRITICAL] (" + StackFrame_Get() + ") " + entry);
    DoLog("Shutting down due to critical error.");
    OS_MessageBox("ERROR", "Shutting down due to critical error:\n" + entry);
#ifdef BUILD_DEBUG
    LTE_ASSERT(false);
#endif
    exit(1);
  }

  void Log_Error(String const& entry) {
    if (level >= LogLevel::Errors)
      DoLog("[Error] (" + StackFrame_Get() + ") " + entry);
  }

  void Log_Event(String const& entry) {
    if (level >= LogLevel::Everything)
      DoLog("[Event] (" + StackFrame_Get() + ") " + entry);
  }

  void Log_Message(String const& entry) {
    if (level >= LogLevel::Everything)
      DoLog(entry);
  }

  void Log_Warning(String const& entry) {
    if (level >= LogLevel::Warnings)
      DoLog("[Warning] (" + StackFrame_Get() + ") " + entry);
  }

  size_t Log_GetEntries() {
    return GetEntryVector().size();
  }

  String const& Log_GetEntry(int index) {
    return GetEntryVector()[index];
  }
}
