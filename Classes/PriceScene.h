#pragma once
#include "2d/CCScene.h"

class PriceScene : public cocos2d::Scene {
public:
  bool init() override;
  CREATE_FUNC(PriceScene);
  static PriceScene *createScene(cocos2d::Node *n);
  void setPrice(cocos2d::Node *price);

private:
  cocos2d::Node *_price;
};
