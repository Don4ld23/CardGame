#pragma once
#include "cocos2d.h"
#include <functional>

//PlayFieldView 是上半区的“桌面 / 牌桌视图容器”：
// 负责承载桌面上的所有 CardView、处理点击命中并把卡牌uid上抛，
// 以及把一张桌面牌移动到托盘（下方堆区）的过渡动画（含跨父节点搬运与位移补间）。

class CardView;
/*
 * - “桌面牌区”的视图容器（位于上半区），承载若干 CardView 作为子节点。
 * - 提供点击回调：将命中的卡牌 uid 上抛给外部逻辑（不在此处做规则判断）。
 * - 提供将桌面牌“移入托盘”的动画接口（跨父节点搬运 + 平移动画）。
 */
class PlayFieldView : public cocos2d::Node {
public:
    CREATE_FUNC(PlayFieldView);
    bool init() override;

    // 点击桌面牌回调（把卡牌 uid 传回去）
    // 说明：enableTouch 中从最上层子节点往下做命中测试，命中即回调。
    void setOnCardClickCallback(std::function<void(int)> cb) { // 设置回调函数
        _onCardClick = std::move(cb); // 保存回调
    } 

    /**
     * 桌面牌移入托盘（顶部）动画：
     * - 典型用于“桌面牌与手牌匹配成功后”将该牌移动到下方托盘的过渡。
     * - 过程：
     *   1) 将 fromPos（本节点局部坐标）先转为世界坐标，再转到托盘父节点的局部坐标；
     *   2) 把 CardView 从当前父节点移除、加到托盘父节点；
     *   3) 先设置到转换后的起点位置，再做 TweenService 的位移动画到 trayLocalPos；
     *   4) 动画结束后调用 onDone。
     * @param cv            将要移动的卡牌视图（已存在于本视图或其子层）
     * @param fromPos       移动起点（本视图的局部坐标系）
     * @param trayParent    目标托盘的父节点（例如 StackView 内的某层容器）
     * @param trayLocalPos  在 trayParent 坐标系下的目标位置
     * @param onDone        动画完成回调（可为空）
     */
    // 桌面牌移入托盘（顶部）动画：把节点从本层搬到目标父节点并平移动画
    void playMoveToTrayAnimation(CardView* cv,
        const cocos2d::Vec2& fromPos,
        cocos2d::Node* trayParent,
        const cocos2d::Vec2& trayLocalPos,
        const std::function<void()>& onDone);

private:
    std::function<void(int)> _onCardClick;// 声明点击回调  点击回调：仅传回卡牌 uid
    void enableTouch();// 启用单点触摸监听：负责从顶层子节点开始命中测试并上抛uid
};
