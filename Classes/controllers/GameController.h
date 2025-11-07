#pragma once
#include "cocos2d.h"
#include "views/GameView.h"
#include "models/GameModel.h"
#include "managers/LayoutManager.h"
#include "managers/UndoManager.h"
#include "controllers/PlayFieldController.h"
#include "controllers/StackController.h"
#include <memory>

//GameController 继承 Scene，是整局游戏的总控制器：
// 负责按关卡载入数据 → 生成模型 → 构建视图 → 组装子控制器（桌面区/托盘区）→ 绑定回退按钮 → 播放入场动画。
//内部维护：
//数据：GameModel（桌面、手牌、顶部牌等状态）；
//视图：GameView（顶级视图容器）；
//控制：PlayFieldController（上半区）与 StackController（下半区）；
//布局：LayoutManager；
//回退：UndoManager（提供 UndoStack）。
//undoOnce() 根据 UndoManager 记录把最近一步操作复原，包含跨父节点搬运与位置补间，同时回填 GameModel 状态。


/**
 * GameController（场景级控制器）
 * 角色：整局游戏的生命周期总控（数据装配、视图搭建、控制器绑定、回退处理）。
 * - 继承自 cocos2d::Scene：可直接 runWithScene() 作为场景进入。
 * - startGame(levelId)：按关卡 ID 装配 GameModel，创建 GameView，构建子控制器。
 * - undoOnce()：从 UndoManager 中弹出一步记录，执行视图/模型的逆操作。
 */
class GameController : public cocos2d::Scene {
public:
    CREATE_FUNC(GameController);
    bool init() override;
    ~GameController() override;          // ★ 只声明，不内联定义

    // 开始一局（按关卡 ID 载入配置并初始化 MVC）
    void startGame(int levelId);
    // 执行一次撤销（若有记录）
    void undoOnce();

private:
    GameView* _gameView = nullptr;// 顶级视图容器（拥有 PlayFieldView/StackView）
    GameModel _gameModel;// 数据模型（桌面、手牌、顶部等状态）

    // 子控制器：上半区（牌桌/玩法区）与下半区（托盘/堆区）
    std::unique_ptr<PlayFieldController> _pfCtl;
    std::unique_ptr<StackController>     _stCtl;

    LayoutManager _layout;// 计算布局位置（如手牌槽位、顶部牌位）
    UndoManager   _undo;// 回退管理（内部有一个栈）

    // 绑定 UI“回退”按钮到 undoOnce()
    void hookUndoButton();
    // 清理当前局（视图/控制器/模型/回退栈复位）
    void destroyRunningGame();
};