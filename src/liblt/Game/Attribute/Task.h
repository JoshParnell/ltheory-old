#ifndef Attribute_Task_h__
#define Attribute_Task_h__

#include "Common.h"

template <class T>
struct Attribute_Task : public T {
  typedef Attribute_Task SelfType;
  ATTRIBUTE_COMMON(task)
  Task task;

  Attribute_Task() {}

  Task const& GetTask() const {
    return task;
  }

  bool HasTask() const {
    return true;
  }
};

#endif
