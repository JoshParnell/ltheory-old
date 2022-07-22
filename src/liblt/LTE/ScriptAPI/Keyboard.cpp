#include "LTE/Function.h"
#include "LTE/Keyboard.h"
#include "LTE/Vector.h"

namespace Priv1 {
  #define X(x)                                                                   \
    FreeFunctionNoParams(Key, Key_##x,                                           \
      "Return the " #x " key")                                                   \
    {                                                                            \
      return Key_##x;                                                            \
    }
  KEY_X
  #undef X
}

FreeFunctionNoParams(bool, Key_Alt,
  "Return whether an alt key is down")
{
  return Keyboard_Alt();
}

FreeFunctionNoParams(bool, Key_Control,
  "Return whether a control key is down")
{
  return Keyboard_Control();
}

FreeFunction(bool, Key_Down,
  "Return whether 'key' is currently down",
  Key, key)
{
  return Keyboard_Down(key);
} FunctionAlias(Key_Down, Down);

FreeFunctionNoParams(Vector<Key>, Keyboard_GetPressed,
  "Return a list of all keys pressed this frame")
{
  return Keyboard_GetKeysPressed();
}

VoidFreeFunction(Keyboard_ModifyString,
  "Modify the 's' according to keys pressed this frame",
  String, s,
  int, cursor)
{
  Keyboard_ModifyString(Mutable(s), Mutable(cursor));
}

FreeFunction(bool, Key_Pressed,
  "Return whether 'key' was pressed this frame",
  Key, key)
{
  return Keyboard_Pressed(key);
} FunctionAlias(Key_Pressed, Pressed);

FreeFunction(bool, Key_Released,
  "Return whether 'key' was released this frame",
  Key, key)
{
  return Keyboard_Released(key);
} FunctionAlias(Key_Released, Released);

FreeFunctionNoParams(bool, Key_Shift,
  "Return whether a shift key is down")
{
  return Keyboard_Shift();
}
