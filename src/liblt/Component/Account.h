#ifndef Component_Account_h__
#define Component_Account_h__

#include "Common.h"
#include "LTE/AutoClass.h"

AutoClass(ComponentAccount,
  Quantity, credits)

  ComponentAccount() :
    credits(0)
    {}
};

AutoComponent(Account)
  void AddCredits(Quantity count) {
    Account.credits += count;
  }

  Quantity GetCredits() const {
    return Account.credits;
  }

  bool RemoveCredits(Quantity count) {
    if (Account.credits < count)
      return false;
    Account.credits -= count;
    return true;
  }
};

#endif
