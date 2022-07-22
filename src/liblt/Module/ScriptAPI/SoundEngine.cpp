#include "LTE/Function.h"
#include "LTE/Math.h"
#include "Game/Object.h"
#include "Module/SoundEngine.h"

TypeAlias(Reference<SoundT>, Sound);

VoidFreeFunction(Sound_Delete,
  "Mark 'sound' for deletion",
  Sound, sound)
{
  sound->Delete();
} FunctionAlias(Sound_Delete, Delete);

FreeFunction(float, Sound_GetDuration,
  "Return the duration of 'sound' in ms",
  Sound, sound)
{
  return sound->GetDuration();
} FunctionAlias(Sound_GetDuration, GetDuration);

FreeFunction(bool, Sound_IsFinished,
  "Return whether 'sound' has finished playing",
  Sound, sound)
{
  return sound->IsFinished();
} FunctionAlias(Sound_IsFinished, IsFinished);

FreeFunctionNoParams(Sound, Sound_None, "Return a null sound") {
  return nullptr;
}

FreeFunction(Sound, Sound_Play2D,
  "Create and play 'sound' in 2D at 'volume' (0 = silent, 1 = max volume)",
  String, sound,
  float, volume)
{
  return Sound_Play2D(sound, volume, false);
} FunctionAlias(Sound_Play2D, Sound_Play);

FreeFunction(Sound, Sound_Play3D,
  "Create and play 'sound' in 3D attached to 'object' with 'offset', 'volume', and 'size'",
  String, sound,
  Object, object,
  V3, offset,
  float, volume,
  float, size)
{
  return Sound_Play3D(sound, object, offset, volume, size, false);
} FunctionAlias(Sound_Play3D, Sound_Play);

FreeFunction(Sound, Sound_Play2DLooped,
  "Create and play 'sound' continuously in 2D at 'volume' (0 = silent, 1 = max volume)",
  String, sound,
  float, volume)
{
  return Sound_Play2D(sound, volume, true);
} FunctionAlias(Sound_Play2DLooped, Sound_PlayLooped);

FreeFunction(Sound, Sound_Play3DLooped,
  "Create and play 'sound' continuously in 3D attached to 'object' with 'offset', 'volume', and 'size'",
  String, sound,
  Object, object,
  V3, offset,
  float, volume,
  float, size)
{
  return Sound_Play3D(sound, object, offset, volume, size, true);
} FunctionAlias(Sound_Play3DLooped, Sound_PlayLooped);

VoidFreeFunction(Sound_RandomizePosition,
  "Set the play position of 'sound' to a random point within the sound",
  Sound, sound)
{
  return sound->SetCursor(Rand(0.0f, sound->GetDuration()));
} FunctionAlias(Sound_RandomizePosition, RandomizePosition);

VoidFreeFunction(Sound_SetCursor,
  "Set the play position of 'sound' to 'cursor' in ms",
  Sound, sound,
  float, cursor)
{
  return sound->SetCursor(cursor);
} FunctionAlias(Sound_SetCursor, SetCursor);

VoidFreeFunction(Sound_SetPitch,
  "Set the play rate of 'sound' to 'pitch' (1 = normal speed)",
  Sound, sound,
  float, pitch)
{
  sound->SetPitch(pitch);
} FunctionAlias(Sound_SetPitch, SetPitch);

VoidFreeFunction(Sound_SetVolume,
  "Set the volume of 'sound' to 'volume' (0 = silent, 1 = max volume)",
  Sound, sound,
  float, volume)
{
  sound->SetVolume(volume);
} FunctionAlias(Sound_SetVolume, SetVolume);
