#pragma once
#include "cocos2d.h"
#include <functional>

class CardView;
/*
 * - “桌面牌区”的视图容器（位于上半区），承载若干CardView作为子节点。
 * - 提供点击回调：将命中的卡牌uid上抛给外部逻辑（不在此处做规则判断）。
 * - 提供将桌面牌“移入托盘”的动画接口（跨父节点搬运 + 平移动画）。
 */
class PlayFieldView : public cocos2d::Node {
public:
    CREATE_FUNC(PlayFieldView);
    bool init() override;

    // 点击桌面牌回调（把卡牌uid传回去）
    void setOnCardClickCallback(std::function<void(int)> cb) { // 设置回调函数
        _onCardClick = std::move(cb); //保存回调
    } 

    // 桌面牌移入托盘（顶部）动画：把节点从本层搬到目标父节点并平移动画
    void playMoveToTrayAnimation(CardView* cv,
        const cocos2d::Vec2& fromPos,
        cocos2d::Node* trayParent,
        const cocos2d::Vec2& trayLocalPos,
        const std::function<void()>& onDone);

private:
    std::function<void(int)> _onCardClick;// 声明点击回调  点击回调：仅传回卡牌uid
    void enableTouch();// 启用单点触摸监听：负责从顶层子节点开始命中测试并上抛uid
};
