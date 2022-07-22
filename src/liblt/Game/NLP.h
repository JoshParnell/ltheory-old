#ifndef Game_NLP
#define Game_NLP

#include "Common.h"
#include "LTE/String.h"

LT_API String AngularVelocityToString(float av);

LT_API String DistanceToString(float distance);

LT_API String DurationToString(float duration);

inline String DurationToString(Time duration) {
  return DurationToString((float)duration / (float)kTimeScale);
}

LT_API String MassToString(Mass mass);

LT_API String RofToString(float rof);

LT_API String PhraseToSentence(String const& phrase);

LT_API String VelocityToString(float velocity);

LT_API String QuantityToString(Quantity quantity);

LT_API String QuantityToString(float quantity);

LT_API String QuantityToStringExact(Quantity quantity);

LT_API String ScaleToClass(float scale);

LT_API String String_Time(Time time);

#endif
