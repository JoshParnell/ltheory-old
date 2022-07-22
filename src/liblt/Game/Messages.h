#ifndef Game_Messages_h__
#define Game_Messages_h__

#include "Common.h"
#include "Game/Object.h"
#include "LTE/AutoClass.h"
#include "LTE/Color.h"

AutoClassEmpty(MessageBoost) MessageBoost() {} };
AutoClassEmpty(MessageCruise) MessageCruise() {} };
AutoClassEmpty(MessageFire) MessageFire() {} };
AutoClassEmpty(MessageLaunch) MessageLaunch() {} };
AutoClassEmpty(MessageReload) MessageReload() {} };

AutoClass(MessageCollectStars,
  Color, totalColor,
  Position, totalPosition,
  uint, count)

  MessageCollectStars() :
    totalColor(0),
    totalPosition(0),
    count(0)
    {}

  Color GetColor() const {
    return totalColor / count;
  }

  Position GetPosition() const {
    return totalPosition / count;
  }
};

AutoClassEmpty(MessageEjected)
  MessageEjected() {}
};

AutoClass(MessageGetColor,
  Color, color)
  MessageGetColor() {}
};

AutoClass(MessageLink,
  Object, object)
  MessageLink() {}
};

AutoClass(MessageStartUsing,
  Object, object,
  Object, target)
  MessageStartUsing() {}
};

AutoClass(MessageStopUsing,
  Object, object)
  MessageStopUsing() {}
};

AutoClass(MessageTargetPosition,
  Position, position)
  MessageTargetPosition() {}
};

AutoClass(MessageTargetObject,
  Object, object)
  MessageTargetObject() {}
};

AutoClass(MessageThrustAngular,
  V3, direction,
  float, amount)
  MessageThrustAngular() {}
};

AutoClass(MessageThrustLinear,
  V3, direction)
  MessageThrustLinear() {}
};

AutoClass(MessageUnlink,
  Object, object)
  MessageUnlink() {}
};

AutoClass(MessageWaypoint,
  Object, location)
  MessageWaypoint() {}
};

#endif
