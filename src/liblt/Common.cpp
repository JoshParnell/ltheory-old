#include "Common.h"
#include "BuildMode.h"

#include "LTE/OS.h"
#include "LTE/StackFrame.h"
#include "LTE/Window.h"

#include <csignal>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef LIBLT_WINDOWS
  #include <windows.h>
  #undef MessageBox
#endif

void PrintAssert(
  std::ostream& stream,
  char const* file,
  int line,
  char const* statement)
{
  stream << std::flush << '\n';
  stream << "ASSERTION FAILURE.\n";
  stream << " Line : " << line << '\n';
  stream << " File : " << file << '\n';
  stream << " Statement : " << statement << '\n';
}

void LTE_ASSERT_FAILURE(const char* file, int line, const char* statement) {
  /* Write a log of assertions. */ {
    std::ofstream log((OS_GetUserDataPath() + "logAsserts.txt").c_str(), std::ios::app);
    PrintAssert(log, file, line, statement);
  }

  if (Window_Get())
    Window_Get()->Close();
  std::stringstream stream;

  /* Create a dump file. */ {
    String path = OS_GetUserDataPath() + "crashdumps/";
    OS_CreateDir(path.c_str());
    time_t t = time(0);
    path += ToString(t) + ".dmp";
    OS_WriteDump(path.c_str());
  }

  StackFrame_Print();

#ifdef BUILD_DEBUG
  PrintAssert(stream, file, line, statement);
  #ifdef LIBLT_WINDOWS
    stream << "CONTINUE to continue, CANCEL to exit, TRY AGAIN to break.\n";
    int result = MessageBoxA(NULL, stream.str().c_str(), "LT Engine Error", MB_CANCELTRYCONTINUE);
    if (result == IDCANCEL)
      exit(0);
    else if (result == IDTRYAGAIN)
      __debugbreak();
  #else
    std::cout << stream.str();
    raise(SIGABRT);
  #endif
#else

  stream << "The Limit Theory Engine has failed internally and cannot recover.\n";
  stream << "It would be greatly appreciated if you could report this occurence ";
  stream << "to us on the forums at http://forums.ltheory.com !\nIt would also help ";
  stream << "if you could include a description of what you were doing at the time of ";
  stream << "this crash.\n\nSorry for the inconvenience, and thanks in advance for your ";
  stream << "assistance in making the game more stable!";
  OS_MessageBox("LT Engine Error", stream.str().c_str());
  exit(0);

#endif
}

void LTE_DEBUG_BREAK(const char* file, int line) {
  LTE_PRINT_FILE(file, line);
#ifdef LIBLT_WINDOWS
  __debugbreak();
#else
  raise(SIGTRAP);
#endif
}

void LTE_PRINT_FILE(const char* file, int line) {
  std::cout << "[" << file << " @ " << line << "]\n";
  std::cout << std::flush;
}
