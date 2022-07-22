#include "Lock.h"

#include "SFML/System/Mutex.hpp"

namespace {
  struct LockImpl : public LockT {
    sf::Mutex mutex;

    void Acquire() {
      mutex.lock();
    }

    void Release() {
      mutex.unlock();
    }
  };
}

Lock Lock_Create() {
  return new LockImpl;
}
