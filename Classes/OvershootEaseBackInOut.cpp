#include "OvershootEaseBackInOut.h"

namespace {
float backEaseInOut(float time, float overshoot) {
  time = time * 2;
  if (time < 1) {
    return (time * time * ((overshoot + 1) * time - overshoot)) / 2;
  } else {
    time = time - 2;
    return (time * time * ((overshoot + 1) * time + overshoot)) / 2 + 1;
  }
}
} // namespace

OvershootEaseBackInOut::OvershootEaseBackInOut(float overshoot)
    : _overshoot(overshoot) {}

OvershootEaseBackInOut *OvershootEaseBackInOut::create(ActionInterval *action) {
  return create(action, 0.52158f);
}

OvershootEaseBackInOut *OvershootEaseBackInOut::create(ActionInterval *action,
                                                       float overshoot) {
  OvershootEaseBackInOut *ease =
      new (std::nothrow) OvershootEaseBackInOut(overshoot);
  if (ease) {
    if (ease->initWithAction(action))
      ease->autorelease();
    else
      CC_SAFE_RELEASE_NULL(ease);
  }
  return ease;
}

OvershootEaseBackInOut *OvershootEaseBackInOut::clone() const {
  if (_inner)
    return OvershootEaseBackInOut::create(_inner->clone());
  return nullptr;
}

void OvershootEaseBackInOut::update(float time) {
  _inner->update(backEaseInOut(time, _overshoot));
}

cocos2d::ActionEase *OvershootEaseBackInOut::reverse() const {
  return OvershootEaseBackInOut::create(_inner->reverse(), _overshoot);
}
