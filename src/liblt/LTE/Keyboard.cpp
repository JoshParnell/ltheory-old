#include "Keyboard.h"

#include "String.h"
#include "Thread.h"
#include "Vector.h"

#include "SFML/Window.hpp"

#define KEY_X2                                                                 \
  X(A)                                                                         \
  X(B)                                                                         \
  X(C)                                                                         \
  X(D)                                                                         \
  X(E)                                                                         \
  X(F)                                                                         \
  X(G)                                                                         \
  X(H)                                                                         \
  X(I)                                                                         \
  X(J)                                                                         \
  X(K)                                                                         \
  X(L)                                                                         \
  X(M)                                                                         \
  X(N)                                                                         \
  X(O)                                                                         \
  X(P)                                                                         \
  X(Q)                                                                         \
  X(R)                                                                         \
  X(S)                                                                         \
  X(T)                                                                         \
  X(U)                                                                         \
  X(V)                                                                         \
  X(W)                                                                         \
  X(X)                                                                         \
  X(Y)                                                                         \
  X(Z)                                                                         \
  X(F1)                                                                        \
  X(F2)                                                                        \
  X(F3)                                                                        \
  X(F4)                                                                        \
  X(F5)                                                                        \
  X(F6)                                                                        \
  X(F7)                                                                        \
  X(F8)                                                                        \
  X(F9)                                                                        \
  X(F10)                                                                       \
  X(F11)                                                                       \
  X(F12)                                                                       \
  X(F13)                                                                       \
  X(F14)                                                                       \
  X(F15)

#define KEY_MAP_XY                                                             \
  XY(N0, Num0)                                                                 \
  XY(N1, Num1)                                                                 \
  XY(N2, Num2)                                                                 \
  XY(N3, Num3)                                                                 \
  XY(N4, Num4)                                                                 \
  XY(N5, Num5)                                                                 \
  XY(N6, Num6)                                                                 \
  XY(N7, Num7)                                                                 \
  XY(N8, Num8)                                                                 \
  XY(N9, Num9)                                                                 \
  XY(NP0, Numpad0)                                                             \
  XY(NP1, Numpad1)                                                             \
  XY(NP2, Numpad2)                                                             \
  XY(NP3, Numpad3)                                                             \
  XY(NP4, Numpad4)                                                             \
  XY(NP5, Numpad5)                                                             \
  XY(NP6, Numpad6)                                                             \
  XY(NP7, Numpad7)                                                             \
  XY(NP8, Numpad8)                                                             \
  XY(NP9, Numpad9)                                                             \
  XY(Add, Add)                                                                 \
  XY(BackSpace, BackSpace)                                                     \
  XY(BackSlash, BackSlash)                                                     \
  XY(Comma, Comma)                                                             \
  XY(Dash, Dash)                                                               \
  XY(Delete, Delete)                                                           \
  XY(Divide, Divide)                                                           \
  XY(Down, Down)                                                               \
  XY(End, End)                                                                 \
  XY(Equal, Equal)                                                             \
  XY(Escape, Escape)                                                           \
  XY(Home, Home)                                                               \
  XY(Insert, Insert)                                                           \
  XY(LBracket, LBracket)                                                       \
  XY(Left, Left)                                                               \
  XY(Menu, Menu)                                                               \
  XY(Multiply, Multiply)                                                       \
  XY(PageDown, PageDown)                                                       \
  XY(PageUp, PageUp)                                                           \
  XY(Pause, Pause)                                                             \
  XY(Period, Period)                                                           \
  XY(Quote, Quote)                                                             \
  XY(RBracket, RBracket)                                                       \
  XY(Return, Return)                                                           \
  XY(Right, Right)                                                             \
  XY(SemiColon, SemiColon)                                                     \
  XY(Slash, Slash)                                                             \
  XY(Space, Space)                                                             \
  XY(Subtract, Subtract)                                                       \
  XY(Tab, Tab)                                                                 \
  XY(Tilde, Tilde)                                                             \
  XY(Up, Up)                                                                   \
  XY(LAlt, LAlt)                                                               \
  XY(RAlt, RAlt)                                                               \
  XY(LControl, LControl)                                                       \
  XY(RControl, RControl)                                                       \
  XY(LShift, LShift)                                                           \
  XY(RShift, RShift)                                                           \
  XY(LSystem, LSystem)                                                         \
  XY(RSystem, RSystem)

namespace {
  sf::Keyboard::Key Key_LTE_to_SFML(Key key);
  Key Key_SFML_to_LTE(sf::Keyboard::Key key);
  uchar GetKeyChar(Key key, bool shift);

  struct Keyboard {
    bool down[Key_SIZE];
    bool downLast[Key_SIZE];
    Vector<uchar> chars;
    Vector<Key> pressed;
    bool blocked;
    bool blockedLast;

    Keyboard() :
      blocked(false),
      blockedLast(false)
    {
      for (Key key = 0; key < Key_SIZE; ++key) {
        down[key] = downLast[key] = false;
      }
    }

  } gKeyboard;
}

namespace LTE {
  void Keyboard_AddDown(int key) {
    Key thisKey = Key_SFML_to_LTE((sf::Keyboard::Key)key);
    gKeyboard.down[thisKey] = true;
    gKeyboard.pressed.push(thisKey);
  }

  void Keyboard_AddText(uchar c) {
    gKeyboard.chars.push(c);
  }

  void Keyboard_Block() {
    gKeyboard.blockedLast = gKeyboard.blocked = true;
  }

  bool Keyboard_Down(Key key) {
    return gKeyboard.down[key];
  }

  Vector<Key> const& Keyboard_GetKeysPressed() {
    return gKeyboard.pressed;
  }

  bool Keyboard_IsBlocked() {
    return gKeyboard.blockedLast;
  }

  void Keyboard_ModifyString(String& str, int& cursor) {
    for (size_t i = 0; i < gKeyboard.chars.size(); ++i)
      str.insert(str.begin() + (cursor++), gKeyboard.chars[i]);

    if (Keyboard_Pressed(Key_BackSpace)) {
      if (Keyboard_Control()) 
        while (cursor && str[(size_t)(cursor - 1)] != ' ')
          str.erase((cursor--) - 1, 1);

      if (str.size())
        str.erase((cursor--) - 1, 1);
    }
  }

  bool Keyboard_Pressed(Key key) {
    if (gKeyboard.blockedLast)
      return false;
    return gKeyboard.down[key] && !gKeyboard.downLast[key];
  }

  bool Keyboard_Released(Key key) {
    if (gKeyboard.blockedLast)
      return false;
    return !gKeyboard.down[key] && gKeyboard.downLast[key];
  }

  void Keyboard_Update(bool hasFocus) {
    gKeyboard.blockedLast = gKeyboard.blocked;
    gKeyboard.blocked = false;

    for (Key key = 0; key < Key_SIZE; ++key) {
      gKeyboard.downLast[key] = gKeyboard.down[key];
      
      if (gKeyboard.downLast[key])
        gKeyboard.down[key] = hasFocus &&
          sf::Keyboard::isKeyPressed(Key_LTE_to_SFML(key));
    }

    gKeyboard.chars.clear();
    gKeyboard.pressed.clear();
  }

  const char* Key_Name(Key key) {
    switch (key) {
      #define X(x) case Key_##x: return #x;
      KEY_X2
      #undef X
      case Key_N1:         return "1";
      case Key_N2:         return "2";
      case Key_N3:         return "3";
      case Key_N4:         return "4";
      case Key_N5:         return "5";
      case Key_N6:         return "6";
      case Key_N7:         return "7";
      case Key_N8:         return "8";
      case Key_N9:         return "9";
      case Key_N0:         return "0";
      case Key_NP1:        return "Numpad 1";
      case Key_NP2:        return "Numpad 2";
      case Key_NP3:        return "Numpad 3";
      case Key_NP4:        return "Numpad 4";
      case Key_NP5:        return "Numpad 5";
      case Key_NP6:        return "Numpad 6";
      case Key_NP7:        return "Numpad 7";
      case Key_NP8:        return "Numpad 8";
      case Key_NP9:        return "Numpad 9";
      case Key_NP0:        return "Numpad 0";
      case Key_Add:        return "Add";
      case Key_BackSpace:  return "Backspace";
      case Key_BackSlash:  return "Backslash";
      case Key_Comma:      return "Comma";
      case Key_Dash:       return "Dash";
      case Key_Delete:     return "Delete";
      case Key_Divide:     return "Divide";
      case Key_Down:       return "Down";
      case Key_End:        return "End";
      case Key_Equal:      return "Equal";
      case Key_Escape:     return "Escape";
      case Key_Home:       return "Home";
      case Key_Insert:     return "Insert";
      case Key_LBracket:   return "Left Bracket";
      case Key_Left:       return "Left";
      case Key_Menu:       return "Menu";
      case Key_Multiply:   return "Multiply";
      case Key_PageDown:   return "PageDown";
      case Key_PageUp:     return "PageUp";
      case Key_Pause:      return "Pause";
      case Key_Period:     return "Period";
      case Key_Quote:      return "Quote";
      case Key_RBracket:   return "Right Bracket";
      case Key_Return:     return "Return";
      case Key_Right:      return "Right";
      case Key_SemiColon:  return "Semicolon";
      case Key_Slash:      return "Slash";
      case Key_Space:      return "Space";
      case Key_Subtract:   return "Subtract";
      case Key_Tab:        return "Tab";
      case Key_Tilde:      return "Tilde";
      case Key_Up:         return "Up";

      case Key_LAlt:       return "Left Alt";
      case Key_LControl:   return "Left Control";
      case Key_LShift:     return "Left Shift";
      case Key_LSystem:    return "Left System";
      case Key_RAlt:       return "Right Alt";
      case Key_RControl:   return "Right Control";
      case Key_RShift:     return "Right Shift";
      case Key_RSystem:    return "Right System";

      default:              return "Unknown";
    }
  }
}

namespace {
  sf::Keyboard::Key Key_LTE_to_SFML(Key key) {
    switch (key) {
      #define X(x) case Key_##x: return sf::Keyboard::x;
      KEY_X2
      #undef X

      #define XY(x, y) case Key_##x: return sf::Keyboard::y;
      KEY_MAP_XY
      #undef XY
      default: return sf::Keyboard::Unknown;
    };
  }

  Key Key_SFML_to_LTE(sf::Keyboard::Key key) {
    switch (key) {
      #define X(x) case sf::Keyboard::x: return Key_##x;
      KEY_X2
      #undef X

      #define XY(x, y) case sf::Keyboard::y: return Key_##x;
      KEY_MAP_XY
      #undef XY
      default: return Key_SIZE;
    };
  }

  uchar GetKeyChar(Key key, bool shift) {
    if (shift) {
      switch (key) {
        case Key_A: return 'A';
        case Key_B: return 'B';
        case Key_C: return 'C';
        case Key_D: return 'D';
        case Key_E: return 'E';
        case Key_F: return 'F';
        case Key_G: return 'G';
        case Key_H: return 'H';
        case Key_I: return 'I';
        case Key_J: return 'J';
        case Key_K: return 'K';
        case Key_L: return 'L';
        case Key_M: return 'M';
        case Key_N: return 'N';
        case Key_O: return 'O';
        case Key_P: return 'P';
        case Key_Q: return 'Q';
        case Key_R: return 'R';
        case Key_S: return 'S';
        case Key_T: return 'T';
        case Key_U: return 'U';
        case Key_V: return 'V';
        case Key_W: return 'W';
        case Key_X: return 'X';
        case Key_Y: return 'Y';
        case Key_Z: return 'Z';
        case Key_N1: return '!';
        case Key_N2: return '@';
        case Key_N3: return '#';
        case Key_N4: return '$';
        case Key_N5: return '%';
        case Key_N6: return '^';
        case Key_N7: return '&';
        case Key_N8: return '*';
        case Key_N9: return '(';
        case Key_N0: return ')';
        case Key_NP1: return '1';
        case Key_NP2: return '2';
        case Key_NP3: return '3';
        case Key_NP4: return '4';
        case Key_NP5: return '5';
        case Key_NP6: return '6';
        case Key_NP7: return '7';
        case Key_NP8: return '8';
        case Key_NP9: return '9';
        case Key_NP0: return '0';
        case Key_BackSlash: return '|';
        case Key_Comma: return '<';
        case Key_Dash: return '_';
        case Key_Equal: return '+';
        case Key_LBracket: return '{';
        case Key_Period: return '>';
        case Key_Quote: return '"';
        case Key_RBracket: return '}';
        case Key_SemiColon: return ':';
        case Key_Slash: return '?';
        case Key_Space: return ' ';
        case Key_Tilde: return '~';
        default: return '\0';
      }
    } else {
      switch (key) {
        case Key_A: return 'a';
        case Key_B: return 'b';
        case Key_C: return 'c';
        case Key_D: return 'd';
        case Key_E: return 'e';
        case Key_F: return 'f';
        case Key_G: return 'g';
        case Key_H: return 'h';
        case Key_I: return 'i';
        case Key_J: return 'j';
        case Key_K: return 'k';
        case Key_L: return 'l';
        case Key_M: return 'm';
        case Key_N: return 'n';
        case Key_O: return 'o';
        case Key_P: return 'p';
        case Key_Q: return 'q';
        case Key_R: return 'r';
        case Key_S: return 's';
        case Key_T: return 't';
        case Key_U: return 'u';
        case Key_V: return 'v';
        case Key_W: return 'w';
        case Key_X: return 'x';
        case Key_Y: return 'y';
        case Key_Z: return 'z';
        case Key_N1: return '1';
        case Key_N2: return '2';
        case Key_N3: return '3';
        case Key_N4: return '4';
        case Key_N5: return '5';
        case Key_N6: return '6';
        case Key_N7: return '7';
        case Key_N8: return '8';
        case Key_N9: return '9';
        case Key_N0: return '0';
        case Key_NP1: return '1';
        case Key_NP2: return '2';
        case Key_NP3: return '3';
        case Key_NP4: return '4';
        case Key_NP5: return '5';
        case Key_NP6: return '6';
        case Key_NP7: return '7';
        case Key_NP8: return '8';
        case Key_NP9: return '9';
        case Key_NP0: return '0';
        case Key_Add: return '+';
        case Key_BackSlash: return '\\';
        case Key_Comma: return ',';
        case Key_Dash:  return '-';
        case Key_Divide: return '/';
        case Key_Equal: return '=';
        case Key_LBracket: return '[';
        case Key_Multiply: return '*';
        case Key_Period: return '.';
        case Key_Quote: return '\'';
        case Key_RBracket: return ']';
        case Key_SemiColon: return ';';
        case Key_Slash: return '/';
        case Key_Space: return ' ';
        case Key_Subtract: return '-';
        case Key_Tab: return '\t';
        case Key_Tilde: return '`';
        default: return '\0';
      }
    }
  }
}
