#pragma once
#include "cocos2d.h"

class PlayFieldView;// 牌桌/玩法区（上半区）
class StackView;// 堆区/发牌区（下半区）
class CardView; // 单张牌的视图
struct LevelConfig; // 关卡配置（数据层）
class LayoutManager;// 布局管理器（用于计算/组织初始布局）

/*
 * - 作为视图层顶级容器，将PlayFieldView（上半区）和StackView（下半区）摆放到屏幕上。
 * - 负责建立上下分区背景色块（仅视觉分区，不含逻辑）。
 * - setupInitialLayout在下半区挂载回退按钮，确保层级在卡牌之上。
 * - 提供playEnterAnimation：进入场景时的淡入动画。
 * - 提供findCardViewIn：在指定父节点下，按uid搜索CardView（单层遍历）。
 * - 这里不包含游戏规则与匹配逻辑，仅负责视图搭建与简单过场。
 */
class GameView : public cocos2d::Node {
public:
    CREATE_FUNC(GameView);
    bool init() override;// 生命周期初始化：构建背景区块、创建并摆放子视图

    // 访问子视图（便于外部控制或查询）
    PlayFieldView* playfieldView() const { return _playfield; }// 上半区
    StackView* stackView()     const { return _stack; }// 下半区

    void setupInitialLayout(const LayoutManager& layout, const LevelConfig& cfg);
    void playEnterAnimation(const std::function<void()>& onDone);//入场动画：整棵GameView做一个淡入，结束后回调onDone

    // 查找某父节点下的CardView（按uid）
    CardView* findCardViewIn(cocos2d::Node* parent, int uid) const;

private:
    void buildBackgroundSections();   // 上下分区背景

    // 子视图指针（由init中创建并addChild，GameView析构时由场景树统一释放）
    PlayFieldView* _playfield = nullptr; // 上半区：玩法/牌桌
    StackView* _stack = nullptr;// 下半区：牌堆/发牌区
};
