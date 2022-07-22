#ifndef Module_Sound_h__
#define Module_Sound_h__

#include "Common.h"
#include "LTE/Common.h"
#include "LTE/Reference.h"

struct SoundT : public RefCounted {
  virtual ~SoundT() {}

  virtual void Delete() = 0;

  virtual bool IsFinished() const = 0;
  virtual bool IsLooped() const = 0;

  /* Measured in ms. */
  virtual float GetDuration() const = 0;
  virtual float GetPan() const = 0;
  virtual float GetPitch() const = 0;
  virtual float GetVolume() const = 0;

  virtual void SetCursor(float position) = 0;
  virtual void SetPan(float pan) = 0;
  virtual void SetPlaying(bool playing) = 0;
  virtual void SetPitch(float pitch) = 0;
  virtual void SetVolume(float volume) = 0;
};

#endif
