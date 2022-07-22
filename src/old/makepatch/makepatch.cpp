#define LTE_CONSOLE
#include "LTE/LTE.h"
#include <iostream>
#include <fstream>

const char* kSourceDirectory = "C:\\Program Files (x86)\\Procedural Reality\\LT Prototype\\";
const char* kTargetDirectory = "C:\\Program Files (x86)\\Procedural Reality\\LT Prototype\\v1.1\\";

void SaveBinaryToHeader(const Array<uchar>& data, const String& file) {
  std::ofstream stream(file.c_str());
  stream << "{ ";

  for (size_t i = 0; i < data.size(); ++i) {
    stream << std::hex << std::showbase;
    stream << (int)((unsigned char)data[i]) << ',';
    if (i % 13 == 12)
      stream << "\n  ";
  }
  stream << std::noshowbase;
  stream << "};";
}

int main(int argc, char const* argv[]) {
#if 0
  String workingDir = OS_GetWorkingDir();
  OS_ChangeDir(kSourceDirectory);
  std::vector<String> files = OS_ListDir(kTargetDirectory);
  AutoPtr<Patch> patch = Patch_Create();

  for (size_t i = 0; i < files.size(); ++i) {
    if (Location_File(kTargetDirectory + files[i])->Exists()) {
      if (Location_File(files[i])->Exists()) {
        AutoPtr<Array<uchar> > srcData = Location_File(files[i])->Read();
        AutoPtr<Array<uchar> > dstData = Location_File(kTargetDirectory + files[i])->Read();
        if (*srcData == *dstData)
          continue;
      }

      std::cout << "Adding <" << files[i] << "> ...";
      patch->Add(Location_File(files[i]), Location_File(kTargetDirectory + files[i]));
      std::cout << "done\n";
    }
  }

  LocationMemory dest;
  SaveTo(patch, dest, 0);
  OS_ChangeDir(workingDir);
  SaveBinaryToHeader(*dest.memory, "../src/updater/patchdata.h");
#endif
  return 0;
}
