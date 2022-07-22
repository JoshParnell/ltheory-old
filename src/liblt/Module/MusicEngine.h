#ifndef Module_MusicEngine_h__
#define Module_MusicEngine_h__

#include "Common.h"
#include "LTE/Module.h"
#include "LTE/String.h"

struct MusicEngine : public ModuleT {
  virtual ~MusicEngine() {}

  virtual MusicEngine* SetTension(GenericFloat const& tension) = 0;
  virtual MusicEngine* SetVolume(GenericFloat const& volume) = 0;
};

LT_API MusicEngine* CreateMusicEngine();
LT_API MusicEngine* GetMusicEngine();

#endif
