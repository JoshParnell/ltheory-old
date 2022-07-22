#include "Patch.h"
#include "Array.h"
#include "AutoPtr.h"
#include "Diff.h"
#include "Location.h"
#include "ProgramLog.h"

namespace {
  struct PatchEntry {
    Location loc;
    AutoPtr<Diff> diff;

    bool Apply() const {
      AutoPtr<Array<uchar> > srcData = loc->Exists()
        ? loc->Read()
        : AutoPtr<Array<uchar> >(new Array<uchar>);
      AutoPtr<Array<uchar> > dstData = diff->Inflate(*srcData);
      if (!dstData) {
        Log_Error("Source " + loc->ToString() + " not found");
        return false;
      }
      return loc->Write(*dstData);
    }
  };

  struct PatchImpl : public Patch {
    Vector<PatchEntry> entries;

    void Add(Location const& target, Location const& patchFile) {
      AutoPtr<Array<uchar> > srcData = target->Exists()
        ? target->Read()
        : AutoPtr<Array<uchar> >(new Array<uchar>);
      AutoPtr<Array<uchar> > dstData = patchFile->Read();

      PatchEntry e;
      e.loc = target->Clone();
      e.diff = Diff_Create(*srcData, *dstData);
      entries << e;
    }

    bool Apply() const {
      Array< AutoPtr<Array<uchar> > > backups(entries.size());

      for (size_t i = 0; i < entries.size(); ++i) {
        backups[i] = entries[i].loc->Read();
        if (!entries[i].Apply()) {
          Log_Warning("Patch application failed, attempting to restore");
          for (size_t j = 0; j < i; ++j) {
            if (backups[j]) {
              if (entries[j].loc->Write(*backups[j]))
                Log_Warning("Successfully restored " + entries[j].loc->ToString());
              else
                Log_Warning("Failed to restore file " + entries[j].loc->ToString());
            }
          }
          return false;
        }
      }
      return true;
    }
  };
}

namespace LTE {
  Patch* Patch_Create() {
    return new PatchImpl;
  }
}
