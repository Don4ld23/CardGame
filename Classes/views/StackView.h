#pragma once
#include "cocos2d.h"
//StackView 是下半区的“托盘 / 堆区”容器视图：
// 只负责占位（宽高、锚点），用来承载按钮（如“回退”）和被移动下来的卡牌节点；
// 本身不处理规则与动画。

/**
 * - 下半区“堆区/托盘”的视图容器（GameView 中位于下部）。
 * - 主要职责：提供一个固定尺寸与锚点的可见/命中范围，用于承载：
 *     1) UI（如回退按钮，由 GameView::setupInitialLayout 挂在这里）；
 *     2) 被从上半区 PlayFieldView 搬运下来的 CardView（见 playMoveToTrayAnimation）。
 * - 不包含业务逻辑、动画与输入处理；纯容器节点。
 */
class StackView : public cocos2d::Node {
public:
    CREATE_FUNC(StackView);
    bool init() override;
};
