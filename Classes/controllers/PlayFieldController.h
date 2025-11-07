#pragma once
#include "cocos2d.h"
#include "models/GameModel.h"
#include "models/UndoModel.h"
#include <vector>

class PlayFieldView;// 桌面区视图（上半区）
class GameView;// 顶层视图容器（提供stack/playfield访问 & 工具方法）
class CardView;// 单张牌视图
class LayoutManager;// 布局管理器（提供位置等）
struct LevelConfig; // 关卡配置

/**
 * - 负责“桌面区”的控制逻辑：
 *   1) initView：根据关卡与模型，把桌面牌实例化到 PlayFieldView；
 *   2) handleCardClick：处理点击桌面牌；依据 RuleService 判断是否能与“顶部牌(top)”相邻；
 *   3) replaceTrayWithPlayFieldCard：满足规则后，把桌面牌移入托盘顶部位（含 Undo 记录与模型更新）。
 * - 注意：不直接管理托盘区视图，但会调用 GameView/StackView 来实现跨父节点移动。
 */
class PlayFieldController {
public:
    // 构造：持有模型 / 视图 / 顶层视图 / 布局 / 撤销栈的引用
    PlayFieldController(GameModel& model,
        PlayFieldView& view,
        GameView& gameView,
        LayoutManager& layout,
        std::vector<UndoRecord>& undoStack);

    // 初始化桌面区视图：按关卡配置把_m.table中的卡牌渲染到PlayFieldView
    void initView(const LevelConfig& cfg);
    // 响应一次桌面牌点击（cardId = 牌的uid）
    void handleCardClick(int cardId);
    // 将桌面牌替换到托盘顶部：从playfield移到stack顶部位（并记录撤销）
    void replaceTrayWithPlayFieldCard(int cardId, const cocos2d::Vec2& fromPos);

private:
    // 引用外部数据/对象（Controller不拥有这些对象的生命周期）
    GameModel& _m;// 数据模型（cards、table、hand、top 等）
    PlayFieldView& _pfView; // 桌面区视图
    GameView& _gameView;// 顶层视图容器（提供findCardViewIn/stack/playfield）
    LayoutManager& _layout;// 布局（手牌槽位、顶部位等位置提供）
    std::vector<UndoRecord>& _undo;// 撤销栈（记录可回滚的信息）

    // 工具：在parent的直接子节点中按uid查找 CardView（非递归）
    CardView* findCardViewIn(cocos2d::Node* parent, int uid) const;
    // 工具：该牌是否还在桌面容器_m.table 中
    bool isOnTable(int cardId) const;
};
