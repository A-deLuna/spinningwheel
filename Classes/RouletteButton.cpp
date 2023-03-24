#include "RouletteButton.h"
#include "2d/CCLabel.h"
#include "CCPlatformMacros.h"

namespace RouletteButton {
cocos2d::ui::Button *create(const char *label) {
  auto button = cocos2d::ui::Button::create("Assets/spin_button.png");
  cocos2d::TTFConfig labelConfig;
  labelConfig.fontFilePath = "fonts/arial.ttf";
  labelConfig.fontSize = 12;
  labelConfig.glyphs = cocos2d::GlyphCollection::DYNAMIC;
  labelConfig.outlineSize = 1;
  auto labelButton = cocos2d::Label::createWithTTF(labelConfig, label);
  button->setTitleLabel(labelButton);
  return button;
}
} // namespace RouletteButton
