#define LTE_CONSOLE
#include "LTE/LTE.h"
#include <iostream>

const char* kRegistryPath = "SOFTWARE\\Procedural Reality\\LT Prototype";
const char* kOldVersion = "1.0.0";
const char* kNewVersion = "1.1.0";

const unsigned char kPatchData[] =
#include "patchdata.h"

int main(int argc, char const* argv[]) {
#if 0
  String version = OS_GetRegistryString(kRegistryPath, "Version");
  if (version == kNewVersion) {
    OS_MessageBox("Error", "LT v1.1 is already installed!");
    return 1;
  } else if (version != kOldVersion) {
    OS_MessageBox("Error", "Patch requires LT v1.0");
    return 1;
  } else
    Log_Message("Updater found correct version");

  String path = OS_GetRegistryString(kRegistryPath, "Path");
  if (!path.size()) {
    OS_MessageBox("Error", "Unable to read LT directory registry key");
    return 1;
  }

  Log_Message("Updater found target directory at <" + path + ">");
  OS_ChangeDir(path);

  AutoPtr<Patch> patch = Patch_Create();
  Location memory;
  memory.memory->resize(sizeof(kPatchData));
  memcpy(memory.memory->data(), kPatchData, sizeof(kPatchData));

  if (!LoadFrom(patch, memory, 0)) {
    OS_MessageBox("Error", "Failed to read patch data");
    return 1;
  }

  std::cout << "Patching to LT v1.1 ...\n";

  if (!patch->Apply()) {
    OS_MessageBox("Error", "Failed to apply patch. \
Please try running the updeter with admin privileges. \
If this does not solve the problem, please report this issue on the LT forums!");
    return 1;
  } else {
    OS_MessageBox("Success", "Patch Successful");
    OS_SetRegistryString(kRegistryPath, "Version", kNewVersion);
  }
#endif
  return 0;
}
