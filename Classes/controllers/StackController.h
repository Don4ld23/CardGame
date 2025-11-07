#pragma once
#include "cocos2d.h"
#include "configs/models/LevelConfig.h"
#include "models/GameModel.h"
#include "models/UndoModel.h"
#include <vector>

class StackView;// 下半区托盘/手牌视图容器
class GameView;// 顶层视图容器（可访问playfield/stack）
class CardView; // 单张牌的视图
class LayoutManager;// 布局（手牌槽、顶部位坐标等）

/**
 * - 负责“托盘/手牌区”的控制逻辑：
 *   * initView：把手牌各列的“顶牌”和顶部位渲染到StackView；
 *   * handleHandClick：响应用户点击某个手牌槽顶牌；
 *   * replaceTopWithHandCard：将手牌顶牌替换到“顶部位”，记录撤销并更新模型。
 * - 仅处理视图与模型的协调，不包含规则判断（相邻等在PlayFieldController/RuleService）。
 */
class StackController {
public:
    // 构造：持有模型/视图/顶层视图/布局/撤销栈的引用
    StackController(GameModel& model,
        StackView& stackView,
        GameView& gameView,
        LayoutManager& layout,
        std::vector<UndoRecord>& undoStack)
        : _m(model), _stackV(stackView), _gameV(gameView), _layout(layout), _undo(undoStack) {
    }

    // 初始化渲染：手牌各槽的顶牌 + 顶部位
    void initView(const LevelConfig& cfg);

    // 用户点击第handIndex个手牌槽顶牌
    void handleHandClick(int handIndex);

private:
    GameModel& _m;// 数据模型（hand/top/cards等）
    StackView& _stackV;// 托盘/手牌视图容器
    GameView& _gameV;// 顶层视图容器
    LayoutManager& _layout;// 提供手牌槽/顶部位坐标
    std::vector<UndoRecord>& _undo;// 撤销栈

    // 工具：在parent的直接子节点里按uid查找CardView
    CardView* findCardViewIn(cocos2d::Node* parent, int uid) const;
    // 执行“用手牌顶牌替换顶部位”的流程（含补间、撤销记录、模型更新）
    void replaceTopWithHandCard(int handIndex);
};
