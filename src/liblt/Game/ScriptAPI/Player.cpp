#include "Game/Player.h"

#include "LTE/Function.h"

TypeAlias(Reference<PlayerT>, Player);

DefineConversion(player_to_object, Player, Object) {
  dest = (Object)src;
}

FreeFunction(Object, Player_GetPiloting,
  "Return the object that 'player' is currently piloting'",
  Player, player)
{
  return player->piloting;
} FunctionAlias(Player_GetPiloting, GetPiloting);

VoidFreeFunction(Player_Pilot,
  "Move 'player' into 'object's pilot seat",
  Player, player,
  Object, object)
{
  return player->Pilot(object);
} FunctionAlias(Player_Pilot, Pilot);
