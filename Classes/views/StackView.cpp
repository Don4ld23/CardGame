#include "views/StackView.h"
#include "configs/models/CardResConfig.h"
USING_NS_CC;

bool StackView::init() {
    if (!Node::init()) return false;

    // 设定托盘区域的“内容尺寸”为设计宽 × 下半区高度
    // - 它决定了此容器的布局范围与命中区域（例如放按钮、承载卡牌）。
    setContentSize({ (float)CardResConfig::kDesignW, (float)CardResConfig::kStackH });
    setAnchorPoint({ 0.5f, 0.5f });
    return true;
}
