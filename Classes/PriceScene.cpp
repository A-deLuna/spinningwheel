#include "PriceScene.h"
#include "2d/CCActionEase.h"
#include "2d/CCActionInterval.h"
#include "2d/CCParticleExamples.h"
#include "CCDirector.h"
#include "RouletteButton.h"
#include "RouletteScene.h"
#include "math/Vec2.h"
#include "ui/UIButton.h"

using cocos2d::Vec2;

namespace {
cocos2d::Spawn *appearAction() {
  float scaleDuration = .2;
  float translationDuration = .6;
  float scaleFactor = 2;

  auto vs = cocos2d::Director::getInstance()->getVisibleSize();
  auto center = Vec2(vs.width * .5, vs.height * .5);
  using easeOut = cocos2d::EaseQuadraticActionOut;
  auto scale =
      easeOut::create(cocos2d::ScaleBy::create(scaleDuration, scaleFactor));
  auto translate =
      easeOut::create(cocos2d::MoveTo::create(translationDuration, center));
  return cocos2d::Spawn::create(scale, translate, nullptr);
}

cocos2d::Sprite *itemHalo() {
  float rotationDurationInSeconds = 1;
  float rotationDegrees = 15;
  auto halo = cocos2d::Sprite::create("Assets/halo.png");
  halo->setPositionNormalized(Vec2(.5, .5));
  auto repeat = cocos2d::RepeatForever::create(
      cocos2d::RotateBy::create(rotationDurationInSeconds, rotationDegrees));
  halo->runAction(repeat);
  return halo;
}
} // namespace
PriceScene *PriceScene::createScene(Node *n) {
  auto s = PriceScene::create();
  if (s == nullptr) {
    return s;
  }
  s->setPrice(n);
  return s;
}

void PriceScene::setPrice(Node *n) {
  _price = n;
  _price->setRotation(0);
  this->addChild(_price, 1);

  _price->runAction(appearAction());
  _price->addChild(itemHalo(), -1);
}

bool PriceScene::init() {
  if (!Scene::init()) {
    return false;
  }
  auto vs = cocos2d::Director::getInstance()->getVisibleSize();
  auto button = RouletteButton::create("Claim");
  auto position = Vec2(vs.width * .5, vs.height * .1);
  button->setPosition(position);
  button->addTouchEventListener(
      [=](Ref *sender, cocos2d::ui::Widget::TouchEventType type) {
        cocos2d::Director::getInstance()->replaceScene(
            RouletteScene::createScene());
      });
  this->addChild(button, 2);
  return true;
}
