#pragma once
#include "cocos2d.h"

/**
 * 下半区“堆区/托盘”的视图容器（GameView中位于下部）。
 * 主要职责：提供一个固定尺寸与锚点的可见/命中范围，用于承载：
 *   1) UI（如回退按钮，由GameView::setupInitialLayout挂在这里）；
 *   2) 被从上半区PlayFieldView搬运下来的CardView。
 * 不包含业务逻辑、动画与输入处理；纯容器节点。
 */
class StackView : public cocos2d::Node {
public:
    CREATE_FUNC(StackView);
    bool init() override;
};
