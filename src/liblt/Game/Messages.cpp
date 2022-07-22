#include "Messages.h"

FreeFunction(MessageLink, MessageLink, "", Object, object) {
  return MessageLink(object);
}

FreeFunctionNoParams(MessageBoost, MessageBoost, "Use Boost Capacitor") {
  return MessageBoost();
}

FreeFunctionNoParams(MessageCruise, MessageCruise, "Engage Cruise") {
  return MessageCruise();
}

FreeFunctionNoParams(MessageFire, MessageFire, "Fire") {
  return MessageFire();
}

FreeFunctionNoParams(MessageReload, MessageReload, "Reload") {
  return MessageReload();
}

FreeFunction(MessageStartUsing, MessageStartUsing, "", Object, object) {
  return MessageStartUsing(object, nullptr);
}

FreeFunction(MessageStopUsing, MessageStopUsing, "", Object, object) {
  return MessageStopUsing(object);
}

FreeFunction(MessageTargetPosition, MessageTargetPosition,
  "Target 'position'",
  Position, position)
{
  return MessageTargetPosition(position);
}

FreeFunction(MessageThrustAngular, MessageThrustAngular,
  "Apply an angular thrust of 'magnitude' in the 'dir' local-space direction",
  V3, dir,
  float, magnitude)
{
  return MessageThrustAngular(dir, magnitude);
}

FreeFunction(MessageThrustLinear, MessageThrustLinear,
  "Apply a linear thrust in the 'dir' world-space direction",
  V3, dir)
{
  return MessageThrustLinear(dir);
}
