#pragma once
#include "cocos2d.h"
// An EaseBackInOut implementation that allows callers to
// configure the overshoot parameter.
class OvershootEaseBackInOut : public cocos2d::ActionEase {
  CC_CONSTRUCTOR_ACCESS : virtual ~OvershootEaseBackInOut() {}
  OvershootEaseBackInOut() = default;
  OvershootEaseBackInOut(float);

public:
  static OvershootEaseBackInOut *create(ActionInterval *action);
  static OvershootEaseBackInOut *create(ActionInterval *action,
                                        float overshoot);
  virtual OvershootEaseBackInOut *clone() const override;
  virtual void update(float time) override;
  virtual cocos2d::ActionEase *reverse() const override;

private:
  float _overshoot;
  CC_DISALLOW_COPY_AND_ASSIGN(OvershootEaseBackInOut);
};
