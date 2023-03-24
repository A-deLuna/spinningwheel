#pragma once
#include "2d/CCNode.h"
#include "2d/CCSprite.h"

class Wheel : public cocos2d::Node {
public:
  bool init() override;
  void roll();
  CREATE_FUNC(Wheel);
  void debugMenu();

private:
  void rollToWinner(int winnerIndex);
  cocos2d::Sprite *_wheel;
};
