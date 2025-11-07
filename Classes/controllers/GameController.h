#pragma once
#include "cocos2d.h"
#include "views/GameView.h"
#include "models/GameModel.h"
#include "managers/LayoutManager.h"
#include "managers/UndoManager.h"
#include "controllers/PlayFieldController.h"
#include "controllers/StackController.h"
#include <memory>

/**
 * GameController（场景级控制器）
 * 整局游戏的生命周期总控（数据装配、视图搭建、控制器绑定、回退处理）。
 * 继承自cocos2d::Scene：可直接runWithScene() 作为场景进入。
 * startGame(levelId)：按关卡ID装配GameModel，创建GameView，构建子控制器。
 * undoOnce()：从UndoManager中弹出一步记录，执行视图/模型的逆操作。
 */
class GameController : public cocos2d::Scene {
public:
    CREATE_FUNC(GameController);
    bool init() override;
    ~GameController() override;

    // 开始一局（按关卡ID载入配置并初始化MVC）
    void startGame(int levelId);
    // 执行一次撤销（若有记录）
    void undoOnce();

private:
    GameView* _gameView = nullptr;// 顶级视图容器（拥有PlayFieldView/StackView）
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
