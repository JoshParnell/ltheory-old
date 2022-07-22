#include "LTE/Function.h"
#include "LTE/Location.h"
#include "LTE/OS.h"

FreeFunction(Vector<String>, Directory_List,
  "Return a list of all files and directories in 'path'",
  String, path)
{
  Vector<String> files = OS_ListDir(path);
  files.remove(".");
  files.remove("..");
  return files;
} FunctionAlias(Directory_List, ListDir);

FreeFunction(bool, OS_IsDirectory,
  "Return whether 'path' is a directory",
  String, path)
{
  return OS_IsDir(path);
} FunctionAlias(OS_IsDirectory, IsDirectory);

FreeFunction(bool, OS_IsFile,
  "Return whether 'path' is a file",
  String, path)
{
  return OS_IsFile(path);
} FunctionAlias(OS_IsFile, IsFile);

FreeFunction(String, File_Read,
  "Return the contents of the file located at 'path'",
  String, path)
{
  Location location = Location_File(path);
  if (!location->Exists())
    return "";
  return location->ReadAscii();
}
