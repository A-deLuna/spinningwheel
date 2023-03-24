#include "Wheel.h"
#include "2d/CCActionEase.h"
#include "2d/CCActionInterval.h"
#include "2d/CCDrawNode.h"
#include "2d/CCLabel.h"
#include "2d/CCTransition.h"
#include "OvershootEaseBackInOut.h"
#include "PriceScene.h"
#include "ccTypes.h"
#include "cocos2d.h"
#include "math/Vec2.h"
#include "ui/UIButton.h"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

using cocos2d::Vec2;

namespace {

constexpr float PI = 3.14159265;

struct WheelItem {
  std::string filename;
  std::string label;
  float dropchance;
  std::string debugmenulabel;
  WheelItem(std::string _filename, std::string _label, float _dropchance,
            std::string _debugmenulabel)
      : filename(std::move(_filename)), label(std::move(_label)),
        dropchance(_dropchance), debugmenulabel(std::move(_debugmenulabel)) {}
};

std::vector<WheelItem> items = {
    WheelItem("Assets/hammer.png", "X1", .20, "hammer x1"),
    WheelItem("Assets/gem.png", "X75", .05, "gem x75"),
    WheelItem("Assets/brush.png", "X1", .20, "brush x1"),
    WheelItem("Assets/coin.png", "X750", .05, "coin x750"),
    WheelItem("Assets/hammer.png", "X3", .10, "hammer x3"),
    WheelItem("Assets/gem.png", "X35", .10, "gem x35"),
    WheelItem("Assets/brush.png", "X3", .10, "brush x3"),
    WheelItem("Assets/heart.png", "30", .20, "heart x30"),
};

cocos2d::Sprite *loadItem(const WheelItem &item) {
  auto sprite = cocos2d::Sprite::create(item.filename);

  cocos2d::TTFConfig labelConfig;
  labelConfig.fontFilePath = "fonts/arial.ttf";
  labelConfig.fontSize = 12;
  labelConfig.glyphs = cocos2d::GlyphCollection::DYNAMIC;
  labelConfig.outlineSize = 1;
  auto label = cocos2d::Label::createWithTTF(labelConfig, item.label,
                                             cocos2d::TextHAlignment::RIGHT);
  label->setPositionNormalized(Vec2(.7, 0));
  sprite->addChild(label);
  return sprite;
}

float rand_0_1() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);
  return dis(gen);
}

int pickWinner() {
  float rand = rand_0_1();
  float prob = 0;
  for (int i = 0; i < items.size(); i++) {
    prob += items[i].dropchance;
    if (rand < prob) {
      return i;
    }
  }
  return items.size() - 1;
}

void simulateRolls() {
  std::fstream fout("results.txt", fout.binary | fout.trunc | fout.out);
  int results[8] = {};
  int totalTests = 1000;
  for (int i = 0; i < totalTests; i++) {
    results[pickWinner()]++;
  }
  fout << "emulation results:\n";
  fout << "total tests: " << totalTests << '\n';
  for (int i = 0; i < items.size(); i++) {
    fout << "count: " << std::setw(3) << results[i] << " | "
         << "ratio: " << std::setprecision(2) << std::fixed << 1.0 * results[i] / totalTests << " | "
         << "probability: " << std::setprecision(2) << std::fixed <<  items[i].dropchance << " | "
         << items[i].debugmenulabel << '\n';
  }
  fout.flush();
  fout.close();
}
} // namespace

void Wheel::debugMenu() {
#ifdef COCOS2D_DEBUG
  auto bg = cocos2d::DrawNode::create();
  cocos2d::Vector<cocos2d::MenuItem *> menuItems;
  for (int i = 0; i < items.size(); i++) {
    auto &&item = items[i];
    auto label = cocos2d::Label::createWithTTF(item.debugmenulabel,
                                               "fonts/Marker Felt.ttf", 12);
    auto menuLabel = cocos2d::MenuItemLabel::create(label, [=](cocos2d::Ref *) {
      bg->removeFromParent();
      this->rollToWinner(i);
    });
    menuItems.pushBack(menuLabel);
  }
  auto menu = cocos2d::Menu::createWithArray(menuItems);
  menu->alignItemsVerticallyWithPadding(8);
  menu->setPositionNormalized(Vec2(.5, .5));

  auto box = menu->getContentSize();
  bg->drawSolidRect(Vec2(-box.width, -box.height), Vec2(box.width, box.height),
                    cocos2d::Color4F::BLACK);
  bg->addChild(menu);
  bg->setPositionNormalized(Vec2(0, 0));
  this->addChild(bg, 3);
#endif
}
bool Wheel::init() {
  if (!Node::init()) {
    return false;
  }
  auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
  Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

  auto border = cocos2d::Sprite::create("Assets/wheel_border.png");
  border->setPositionNormalized(Vec2(.5, .5));
  this->addChild(border, 1);

  auto arrow = cocos2d::Sprite::create("Assets/wheel_arrow.png");
  arrow->setPositionNormalized(Vec2(.5, .96));
  border->addChild(arrow, 2);

  const char *wheel_str = "Assets/wheel_sections_8.png";
  _wheel = cocos2d::Sprite::create(wheel_str);
  _wheel->setPositionNormalized(Vec2(.5, .5));
  border->addChild(_wheel, -1);

  std::vector<Node *> v;
  for (auto &&item : items) {
    auto node = loadItem(item);
    v.push_back(node);
    _wheel->addChild(node);
  }

  // Arrange them counter clockwise starting from 12 o'clock.
  float step = 2 * PI / items.size();
  float angle = 2 * PI - step / 2;
  for (int i = 0; i < v.size(); i++, angle -= step) {
    v[i]->setRotation(angle * 180 / PI);
    auto rot = Vec2(sin(angle), cos(angle));
    auto scaled = rot * .65;
    auto translated = scaled + Vec2(1, 1);
    auto pos = translated / 2;
    v[i]->setPositionNormalized(pos);
  }

  return true;
}

void Wheel::rollToWinner(int winnerIndex) {
  float rotations = 5;
  float spinDurationInSeconds = 6;
  float delayAfterResultInSeconds = .5;

  // The strategy for spinnning the wheel is to spin a fixed amount of
  // times plus the offset from the center to the winning item.
  // A back in out easing function makes the movement feel better.
  float slice = 360.0 / items.size();
  float angle = slice * winnerIndex + 360 * rotations + slice / 2;
  auto position = cocos2d::RotateBy::create(spinDurationInSeconds, angle);
  auto easeOut = OvershootEaseBackInOut::create(position, 0.52158f);

  auto delay = cocos2d::DelayTime::create(delayAfterResultInSeconds);
  auto cb = cocos2d::CallFunc::create([=]() {
    // We are going to reuse the winning item on the next scene. Take
    // it out of it's parent while preserving it's current position.
    auto item = this->_wheel->getChildren().at(winnerIndex);
    auto localPosition = item->getPosition();
    auto scenePosition = item->getParent()->convertToWorldSpace(localPosition);
    item->setPosition(scenePosition);
    // removeFromParendAndCleanup frees the item even if cleanup=false.
    // call retain to increase the reference count.
    item->retain();
    item->removeFromParentAndCleanup(false);
    auto scene = PriceScene::createScene(item);
    cocos2d::Director::getInstance()->replaceScene(scene);
  });
  auto action = cocos2d::Sequence::create(easeOut, delay, cb, nullptr);
  this->_wheel->runAction(action);
}
void Wheel::roll() {
  //simulateRolls();
  int winnerIndex = pickWinner();
  rollToWinner(winnerIndex);
}
