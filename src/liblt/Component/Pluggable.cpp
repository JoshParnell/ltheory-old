#include "Pluggable.h"
#include "Attachable.h"
#include "Sockets.h"

Socket const& ComponentPluggable::GetSocket(ObjectT const* self) const {
  return self->parent->GetSockets()->sockets[index];
}

void ComponentPluggable::UpdateJoint(ObjectT* self) const {
  Socket const& socket = GetSocket(self);
  Pointer<ComponentAttachable> at = self->GetAttachable();
  Transform& transform = at->transform;
  at->moved = true;

  if (socket.joint == JointType::AxisX) {
    transform.look = OrthoProjection(transform.look, socket.transform.right);
    transform.up = OrthoProjection(transform.up, socket.transform.right);
  } else if (socket.joint == JointType::AxisY) {
    transform.look = OrthoProjection(transform.look, socket.transform.up);
    transform.up = socket.transform.up;
  } else if (socket.joint == JointType::AxisZ) {
    transform.look = socket.transform.look;
    transform.up = OrthoProjection(transform.up, socket.transform.look);
  } else if (socket.joint == JointType::Fixed)
    transform = socket.transform;
  
  at->UpdateTransform(self);

  if (Dot(at->transform.up, transform.up) < 0)
    at->transform.up *= -1;
}
