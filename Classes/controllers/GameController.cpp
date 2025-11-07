#include "controllers/GameController.h"
#include "controllers/PlayFieldController.h"
#include "controllers/StackController.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "services/GameModelFromLevelGenerator.h"
#include "configs/models/CardResConfig.h"
#include "configs/models/LevelConfig.h"
#include "views/CardView.h"
#include "views/PlayFieldView.h"
#include "views/StackView.h"

USING_NS_CC;

GameController::~GameController() {}

bool GameController::init() {
    if (!Scene::init()) return false;
    // 默认启动关卡 1
    startGame(1);
    return true;
}

/**
 * 清理当前正在运行的一局：
 * - 移除并释放 GameView
 * - 释放子控制器
 * - 重置撤销栈
 * - 重置 GameModel
 */
void GameController::destroyRunningGame() {
    if (_gameView) { //检查游戏视图是否存在
        _gameView->removeFromParent(); //从父节点移除视图
        _gameView = nullptr;
    }
    _pfCtl.reset();
    _stCtl.reset();
    _undo.init();// 清空回退栈
    _gameModel = GameModel{};// 模型回到初始状态
}

/**
 * 启动一局：按关卡ID装配MVC
 * 1) 读LevelConfig（关卡参数/初始布局等）
 * 2) 用GameModelFromLevelGenerator生成GameModel（牌面/手牌/顶部等初始状态）
 * 3) 创建GameView，挂到场景，初始化UI（回退按钮）
 * 4) 创建子控制器PlayFieldController/StackController，并各自initView(cfg)
 * 5) 绑定回退按钮回调，播放入场动画
 */
void GameController::startGame(int levelId) {
    destroyRunningGame();

    LevelConfig cfg;
    if (!LevelConfigLoader::loadLevelConfig(levelId, cfg)) {
        CCLOGERROR("Load level %d failed.", levelId);
        return;
    }
    // 将关卡配置“翻译”为可用的GameModel（牌堆/桌面/手牌等）
    GameModelFromLevelGenerator::generateGameModel(cfg, _gameModel);

    // 创建顶层视图容器并加入场景
    _gameView = GameView::create();
    addChild(_gameView, 0);

    // 让视图层创建 UI（如回退按钮）；_layout 提供位置常量
    _gameView->setupInitialLayout(_layout, cfg);

    // 依据 MVC 拆分控制：上半区（牌桌）/ 下半区（托盘）
    _pfCtl.reset(new PlayFieldController(_gameModel, *_gameView->playfieldView(), *_gameView, _layout, _undo.stack()));
    _stCtl.reset(new StackController(_gameModel, *_gameView->stackView(), *_gameView, _layout, _undo.stack()));

    // 先初始化托盘区再初始化牌桌区，保证下方接牌位置准备好
    _stCtl->initView(cfg);
    _pfCtl->initView(cfg);

    // 绑定“回退”按钮行为
    hookUndoButton();
    // 入场淡入动画（onDone 此处传空）
    _gameView->playEnterAnimation(nullptr);
}

/**
 * 在 StackView（下半区）里查找名为 "UndoButton" 的菜单项，并绑定点击回调到 undoOnce()
 * - GameView::setupInitialLayout 会把 Menu/Undo 挂在 StackView 上（z=100）
 */
void GameController::hookUndoButton() {
    // 绑定“回退”按钮
    StackView* sv = _gameView->stackView();
    const auto& children = sv->getChildren();
    for (size_t i = 0; i < children.size(); ++i) {
        cocos2d::Node* n = children.at(i);
        if (auto* menu = dynamic_cast<Menu*>(n)) {
            auto* btn = menu->getChildByName<MenuItemLabel*>("UndoButton");
            if (btn) {
                // 注意 MenuItem 的回调签名：void(Ref*)，这里用 lambda 捕获 this
                btn->setCallback([this](cocos2d::Ref*) { this->undoOnce(); });  // ✅ 带 Ref* 参数
            }
        }
    }
}

/**
 * 执行一次撤销（Undo）
 * - 从 UndoManager::stack 弹出一条 UndoRecord
 * - 按记录类型分两种回滚路径：
 *   1) HAND_TO_TOP：把牌从“顶部区”撤回到“某个手牌列”；
 *   2) TABLE_TO_TOP：把牌从“顶部区”撤回到“桌面原位置”（可能涉及跨父节点搬运）。
 * - 同步更新 GameModel（例如 top、hand、table 等容器的元素）。
 */
void GameController::undoOnce() {
    if (_undo.empty()) return;
    UndoRecord rec = _undo.pop();

    // 还原“顶部牌”之前的可见/位置状态（prevTopUid = 撤销前顶牌是谁）
    if (rec.prevTopUid != -1) {
        CardView* pv = _gameView->findCardViewIn(_gameView->stackView(), rec.prevTopUid);
        if (pv) {
            pv->setVisible(true);
            pv->setPosition(_layout.topPos());// 放回顶部位
        }
        _gameModel.top = rec.prevTopUid;// 同步模型
    }
    else {
        _gameModel.top = -1;// 顶部为空
    }

    // 找到“本次被移动的牌”（movedUid）在 StackView 中的视图节点
    CardView* mv = _gameView->findCardViewIn(_gameView->stackView(), rec.movedUid);
    if (mv) {
        if (rec.type == UndoRecord::HAND_TO_TOP) {
            // 路径1：从“顶部”撤回到“手牌列 fromHandIndex”
            cocos2d::Vec2 dst = _layout.handPos(rec.fromHandIndex);
            mv->stopAllActions();
            mv->runAction(Sequence::create(
                MoveTo::create(CardResConfig::kMoveDuration, dst), // 平移回手牌槽位
                CallFunc::create([this, rec] { _gameModel.hand[rec.fromHandIndex].push_back(rec.movedUid); }),// 模型回填：该手牌列重新拥有此牌
                nullptr));
        }
        else {
            // 路径2：从“顶部”撤回到“桌面原坐标 rec.fromPos”
            // - 需要把 mv 从 StackView 搬回 PlayFieldView（跨父节点）
            // - 位置转换：先把 mv 在 StackView 下的位置转成世界，再转回 PlayFieldView 的局部
            cocos2d::Vec2 world = _gameView->stackView()->convertToWorldSpace(mv->getPosition());
            cocos2d::Vec2 localInPlay = _gameView->playfieldView()->convertToNodeSpace(world);

            mv->retain(); 
            mv->removeFromParent(); 
            _gameView->playfieldView()->addChild(mv); 
            mv->release();

            // 先落地到转换位置，再补间回到“撤销记录的 fromPos”
            mv->setPosition(localInPlay);
            mv->runAction(Sequence::create(
                MoveTo::create(CardResConfig::kMoveDuration, rec.fromPos),
                CallFunc::create([this, rec] {
                    _gameModel.table.push_back(rec.movedUid); // 模型回填：该牌回到桌面容器
                    }),
                nullptr));
        }
    }
}

