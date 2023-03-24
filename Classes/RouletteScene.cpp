/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "RouletteScene.h"
#include "2d/CCAction.h"
#include "2d/CCActionEase.h"
#include "2d/CCActionInterval.h"
#include "RouletteButton.h"
#include "Wheel.h"
#include "ccMacros.h"
#include "ui/UIButton.h"
#include "ui/UIWidget.h"

USING_NS_CC;

Scene *RouletteScene::createScene() { return RouletteScene::create(); }

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char *filename) {
  printf("Error while loading: %s\n", filename);
  printf("Depending on how you compiled you might have to add 'Resources/' in "
         "front of filenames in RouletteScene.cpp\n");
}

bool RouletteScene::init() {
  if (!Scene::init()) {
    return false;
  }

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  auto center =
      Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

  auto wheel = Wheel::create();
  wheel->setPosition(center);
  this->addChild(wheel, 0);

  auto button = RouletteButton::create("Play On");
  button->setPosition(Vec2(visibleSize.width * .5, visibleSize.height * .2));

  auto debugButton = RouletteButton::create("Force Item (dbg)");
  button->addTouchEventListener(
      [=](Ref *sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
          button->setVisible(false);
          debugButton->setVisible(false);
          wheel->roll();
        }
      });
  this->addChild(button, 0);

  debugButton->setPosition(
      Vec2(visibleSize.width * .5, visibleSize.height * .85));
  debugButton->addTouchEventListener(
      [=](Ref *sender, ui::Widget::TouchEventType type) {
        if (type == ui::Widget::TouchEventType::ENDED) {
          debugButton->setVisible(false);
          button->setVisible(false);
          wheel->debugMenu();
        }
      });
#ifdef COCOS2D_DEBUG
  this->addChild(debugButton);
#endif

  return true;
}

void RouletteScene::menuCloseCallback(Ref *pSender) {
  // Close the cocos2d-x game scene and quit the application
  Director::getInstance()->end();

  /*To navigate back to native iOS screen(if present) without quitting the
   * application  ,do not use Director::getInstance()->end() as given
   * above,instead trigger a custom event created in RootViewController.mm as
   * below*/

  // EventCustom customEndEvent("game_scene_close_event");
  //_eventDispatcher->dispatchEvent(&customEndEvent);
}
