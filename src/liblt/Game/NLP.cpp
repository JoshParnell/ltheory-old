#include "NLP.h"

#include "LTE/Math.h"

const float kDistanceMult = 2;

namespace {
  float Round(float f, float place) {
    return Floor(f * place) / place;
  }
}

String AngularVelocityToString(float av) {
  av *= 180 / kPi;
  return Stringize() | Round(av) | " deg/s";
}

String DistanceToString(float distance) {
  distance *= kDistanceMult;
  if (distance > 1000)
    return Stringize() | Round(distance / 1000, 10) | " km";
  return Stringize() | Round(distance, 10) | " m";
}

String DurationToString(float duration) {
  if (duration < 0.01f)
    return Stringize() | Round(duration * 1000, 10) | " ms";
  else if (duration < 60)
    return Stringize() | Round(duration, 10) | " s";
  else if (duration < 3600)
    return Stringize() | Round(duration / 60, 10) | " m";
  else
    return Stringize() | Round(duration / 3600, 10) | " hr";
}

String MassToString(Mass mass) {
  if (mass > 10000000)
    return Stringize() | mass / 1000000 | " Mt";
  else if (mass > 10000)
    return Stringize() | mass / 1000 | " kt";
  else
    return Stringize() | mass | " t";
}

String RofToString(float rof) {
  if (rof < 1)
    return Stringize() | rof * 60 | " rpm";
  else
    return Stringize() | rof | " rps";
}

String PhraseToSentence(const String& phrase) {
  String result = phrase;
  if (result.size())
    result.front() = (char)toupper(result.front());
  result +=  ".";
  return result;
}

String VelocityToString(float velocity) {
  return DistanceToString(velocity) + "/s";
}

String QuantityToString(Quantity quantity) {
  if (quantity > 10000000000)
    return Stringize() | quantity / 1000000000 | "B";
  if (quantity > 10000000)
    return Stringize() | quantity / 1000000 | "M";
  if (quantity > 10000)
    return Stringize() | quantity / 1000 | "K";
  return Stringize() | quantity;
}

String QuantityToString(float quantity) {
  if (quantity >= 10000000)
    return Stringize() | quantity / 1000000 | "M";
  if (quantity >= 10000)
    return Stringize() | quantity / 1000 | "K";
  return Stringize() | quantity;
}

String QuantityToStringExact(Quantity quantity) {
  String str;
  size_t index = 0;
  while (quantity >= 10) {
    if ((index++ % 3) == 0 && index > 1)
      str = ',' + str;
    str = ToString(quantity % 10) + str;
    quantity /= 10;
  }
  // TODO : Lazy.
  if ((index++ % 3) == 0 && index > 1)
    str = ',' + str;
  return ToString(quantity) + str;
}

String ScaleToClass(float scale) {
  scale /= 10.0f;
  if (scale <= 1)
    return "Fighter";
  else if (scale <= 2)
    return "Bomber";
  else if (scale <= 5)
    return "Corvette";
  else if (scale <= 10)
    return "Frigate";
  else if (scale <= 20)
    return "Destroyer";
  else if (scale <= 40)
    return "Cruiser";
  else if (scale <= 80)
    return "Battleship";
  else if (scale <= 100)
    return "Dreadnaught";
  else
    return "Colossus";
}

String String_Time(Time time) {
  if (time > 3600000)
    return Stringize() | (time / 3600000) | " hr";
  else if (time > 60000)
    return Stringize() | (time / 60000) | " m";
  else if (time > 1000)
    return Stringize() | (time / 1000) | " s";
  else
    return Stringize() | time | " ms";
}
