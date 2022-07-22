#include "Cursor.h"
#include "LTE/AutoClass.h"
#include "LTE/Vector.h"

namespace {
  AutoClass(Cursor,
    V2, pos,
    V2, last)
    Cursor() {}
  };

  Vector<Cursor>& GetStack() {
    static Vector<Cursor> stack;
    return stack;
  }
}

DefineFunction(Cursor_Get) {
  return GetStack().back().pos;
}

DefineFunction(Cursor_GetDelta) {
  Cursor const& cursor = GetStack().back();
  return cursor.pos - cursor.last;
}

DefineFunction(Cursor_GetLast) {
  return GetStack().back().last;
}

DefineFunction(Cursor_Pop) {
  GetStack().pop();
}

DefineFunction(Cursor_Push) {
  GetStack().push(Cursor(args.pos, args.posLast));
}
