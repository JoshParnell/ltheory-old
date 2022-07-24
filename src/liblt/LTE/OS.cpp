#include "OS.h"
#include "Profiler.h"
#include "ProgramLog.h"
#include "StackFrame.h"
#include "String.h"

#include <csignal>
#include <cstring>
#include <iostream>

#include <sys/stat.h>

#ifdef LIBLT_WINDOWS

  #include "process.h"
  #include "Shlobj.h"
  #include "DbgHelp.h"
  #include "windirent.h"
  #include "windows.h"
  #include "Tchar.h"
  #include "Direct.h"

  #undef CreateDirectory
  #undef MessageBox

#else

  #include <dirent.h>
  #include <spawn.h>
  #include <unistd.h>

  extern char **environ;

#endif

void IntHandler(int value) {
  StackFrame_Print();
  exit(0);
}

void SegHandler(int value) {
  error("Access Violation");
  StackFrame_Print();
  exit(0);
}

#ifdef LIBLT_WINDOWS
  LONG _stdcall Win32SegHandler(EXCEPTION_POINTERS* pExcept) {
    SegHandler(0);
    return 0;
  }
#endif

bool OS_ChangeDir(const String& path) {
#ifdef LIBLT_WINDOWS
  return SetCurrentDirectoryA(path) != 0;
#else
  return chdir(path) == 0;
#endif
}

void OS_ConfigureSignalHandlers() {
#ifndef LIBLT_WINDOWS
  signal(SIGABRT, IntHandler);
  signal(SIGINT, IntHandler);
  signal(SIGSEGV, SegHandler);
#endif
}

bool OS_CreateDir(const String& path) {
#ifdef LIBLT_WINDOWS
  return CreateDirectoryA(path, NULL) != 0;
#else
  return mkdir(path, 0777) == 0;
#endif
}

void OS_CreatePath(const String& path) {
  if (!path.size())
    return;

  std::stringstream stream(path);
  String buf;
  String partialPath;
  std::vector<String> partials;
  while (getline(stream, buf, '/'))
    partials.push_back(buf);
  for (size_t i = 0; i+ 1 < partials.size(); ++i) {
    partialPath += partials[i] + '/';
    OS_CreateDir(partialPath);
  }

  if (path.back() == '/')
    OS_CreateDir(path);
}

bool OS_FileExists(String const& path) {
  struct stat s;
  return stat(path, &s) == 0;
}

String OS_GetAppDir() {
#ifdef LIBLT_WINDOWS
  char path[MAX_PATH];
  SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, path);
  return path;
#else
  // TODO
  return "/usr/bin";
#endif
}

String OS_GetDocumentsDir() {
#ifdef LIBLT_WINDOWS
  char path[MAX_PATH];
  SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, path);
  return path;
#else
  return "./data";
#endif
}

String OS_GetUserDataPath() {
  static bool created = false;
  if (!created) {
    created = true;
    OS_CreateDir("./cache/");
  }
  return "./cache/";
}

String OS_GetWorkingDir() {
  char path[1024];
#ifdef LIBLT_WINDOWS
  if (!_getcwd(path, sizeof(path)))
    return "";
#else
  if (!getcwd(path, sizeof(path)))
    return "";
#endif
  return path;
}

bool OS_IsDir(String const& path) {
  struct stat s;
  return stat(path, &s) == 0 && (s.st_mode & S_IFDIR);
}

bool OS_IsFile(String const& path) {
  struct stat s;
  return stat(path, &s) == 0 && (s.st_mode & S_IFREG);
}

Vector<String> OS_ListDir(String const& path) {
  Vector<String> result;
  DIR* dir = opendir(path);
  if (!dir)
    return result;

  dirent* entry;
  while ((entry = readdir(dir)))
    result.push(entry->d_name);
  closedir(dir);
  return result;
}

void OS_MessageBox(String const& title, String const& message) {
#ifdef LIBLT_WINDOWS
  MessageBoxA(NULL, message, title, MB_OK);
#else
  std::cout << "[" << title << "] : " << message << '\n';
  std::cerr << "[" << title << "] : " << message << '\n';
#endif
}

bool OS_Spawn(String const& path) {
  char* const argv[] = {strdup(path), NULL};
#ifdef LIBLT_WINDOWS
  STARTUPINFO info={sizeof(info)};
  PROCESS_INFORMATION processInfo;
  return CreateProcess(
    path, "", NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo)
    != 0;
#else
  pid_t pid;
  return posix_spawn(&pid, path, NULL, NULL, argv, environ) == 0;
#endif
}

void OS_WriteDump(String const& path) {
#ifdef LIBLT_WINDOWS
  typedef BOOL (_stdcall *pDumpFn)
    (HANDLE hProcess, DWORD ProcessId, HANDLE hFile, MINIDUMP_TYPE DumpType,
     PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
     PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
     PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

  HANDLE hFile = CreateFileA(path,
    GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
    FILE_ATTRIBUTE_NORMAL, NULL);

  HMODULE h = LoadLibrary(_T("DbgHelp.dll"));
  LTE_ASSERT(h);
  if (!h)
    return;

  pDumpFn dumpFn = (pDumpFn)GetProcAddress(h, "MiniDumpWriteDump");
  if (!dumpFn)
    return;

  if(hFile && hFile != INVALID_HANDLE_VALUE) {
    BOOL rv = (*dumpFn)(GetCurrentProcess(), GetCurrentProcessId(),
                        hFile, MiniDumpNormal, 0, 0, 0);
    CloseHandle(hFile);
  }
  FreeLibrary(h);
#else
  /* TODO. */
#endif
}
