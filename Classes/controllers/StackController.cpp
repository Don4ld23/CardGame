#include "controllers/StackController.h"
#include "views/StackView.h"
#include "views/GameView.h"
#include "views/CardView.h"
#include "configs/models/CardResConfig.h"
#include "managers/LayoutManager.h"
#include "services/TweenService.h"// 平移动画
#include "services/UndoService.h" // 撤销记录入栈


USING_NS_CC;

CardView* StackController::findCardViewIn(Node* parent, int uid) const {
    // 在 parent 的直接子节点中查找 uid 匹配的 CardView（非递归遍历）
    const auto& children = parent->getChildren();
    for (size_t i = 0; i < children.size(); ++i) {
        Node* n = children.at(i);
        CardView* cv = dynamic_cast<CardView*>(n);
        if (cv) {
            if (cv->uid() == uid) return cv;
        }
    }
    return nullptr;
}

void StackController::initView(const LevelConfig& cfg) {
    const int n = (int)cfg.stack.size();
    // 1) 手牌槽渲染：对每个手牌槽，取“顶牌”（back()）画到对应槽位
    for (int i = 0; i < n - 1; ++i) {// 注意：cfg.stack 的最后一格通常用于“顶部位”
        if (_m.hand.size() <= (size_t)i || _m.hand[i].empty()) continue;
        int uid = _m.hand[i].back();// 顶牌 uid
        const CardModel& cm = _m.cards.at(uid);// 点数/花色

        CardView* cv = CardView::create(uid);
        _stackV.addChild(cv);
        cv->setPosition(_layout.handPos(i));// 摆到第 i 个手牌槽位
        cv->applyFaceComposite(cm.rank, cm.suit, true);
        // 给该顶牌注册点击：点击后由本控制器处理“替换到顶部位”
        cv->setOnClicked([this, i](int) { handleHandClick(i); });
    }
    
    // 2) 顶部位渲染：若模型中已有 top，则画出它
    if (_m.top != -1) {
        int uid = _m.top;
        const CardModel& cm = _m.cards.at(uid);
        CardView* topView = CardView::create(uid);
        _stackV.addChild(topView);
        topView->setPosition(_layout.topPos());// 顶部位坐标
        topView->applyFaceComposite(cm.rank, cm.suit, true);
    }

    // 3) 撤销按钮回调清空（真正绑定在 GameController::hookUndoButton 中）
    const auto& children = _stackV.getChildren();
    for (size_t i = 0; i < children.size(); ++i) {
        Node* n = children.at(i);
        if (auto* menu = dynamic_cast<Menu*>(n)) {
            auto* btn = menu->getChildByName<MenuItemLabel*>("UndoButton");
            if (btn) { btn->setCallback(nullptr); }
        }
    }
}

void StackController::handleHandClick(int handIndex) {
    // 点击某个手牌槽的顶牌：若索引有效且该槽非空，则执行替换
    if (handIndex < 0 || handIndex >= (int)_m.hand.size()) return;
    if (_m.hand[handIndex].empty()) return;
    replaceTopWithHandCard(handIndex);
}

void StackController::replaceTopWithHandCard(int handIndex) {
    // 1) 从模型的手牌列中弹出该列顶牌 uid
    int uid = _m.hand[handIndex].back();
    _m.hand[handIndex].pop_back();

    // 2) 生成撤销记录并入栈
    UndoRecord r;
    r.type = UndoRecord::HAND_TO_TOP;// 类型：手牌 -> 顶部
    r.movedUid = uid; // 移动的牌
    r.prevTopUid = _m.top;// 原先的顶部牌（撤销时需恢复）
    r.fromHandIndex = handIndex; // 来自哪一列手牌
    r.fromPos = _layout.handPos(handIndex);// 该牌的出发位置（撤销归位用）
    UndoService::push(_undo, r);

    // 3) 视图更新：
    // 3.1 若顶部已存在牌：找到其视图并“隐藏”（保留以便撤销时恢复）
    if (_m.top != -1) {
        CardView* oldV = findCardViewIn(&_stackV, _m.top);
        if (oldV) oldV->setVisible(false);
    }
    //3.2 找到这张手牌的视图
    CardView* v = findCardViewIn(&_stackV, uid);
    if (v) {
        // 已存在视图：直接从手牌槽位 Tween 到顶部位
        TweenService::moveTo(v, _layout.topPos(), [this, uid] { _m.top = uid; });
    }
    else {
        // 没有现成视图（例如首次渲染）：创建一个再 Tween 到顶部位
        const CardModel& cm = _m.cards.at(uid);
        CardView* cv = CardView::create(uid);
        _stackV.addChild(cv);
        cv->setPosition(_layout.handPos(handIndex)); // 先放在手牌槽起点
        cv->applyFaceComposite(cm.rank, cm.suit, true);
        TweenService::moveTo(cv, _layout.topPos(), [this, uid] { _m.top = uid; });
    }
}
