#include "controllers/PlayFieldController.h"
#include "views/PlayFieldView.h"
#include "views/GameView.h"
#include "views/CardView.h"
#include "views/StackView.h"
#include "configs/models/CardResConfig.h"
#include "managers/LayoutManager.h"
#include "services/RuleService.h"// 规则：是否相邻（adjacent）
#include "services/UndoService.h" // 撤销：push 记录
#include "configs/models/LevelConfig.h"

USING_NS_CC;

PlayFieldController::PlayFieldController(GameModel& model,
    PlayFieldView& view,
    GameView& gameView,
    LayoutManager& layout,
    std::vector<UndoRecord>& undoStack)
    : _m(model), _pfView(view), _gameView(gameView), _layout(layout), _undo(undoStack) {
    // 将桌面区视图的“点击桌面牌回调”绑定到本控制器
    _pfView.setOnCardClickCallback([this](int cardId) { handleCardClick(cardId); });
}

void PlayFieldController::initView(const LevelConfig& cfg) {
    // 桌面区初始渲染：按照cfg.playfield的位置，把 _m.table 中前n张牌实例化为CardView
    size_t n = std::min(_m.table.size(), cfg.playfield.size());
    for (size_t i = 0; i < n; ++i) {
        int uid = _m.table[i];
        const CardModel& cm = _m.cards.at(uid);

        CardView* cv = CardView::create(uid);
        if (!cv) { CCLOGERROR("Create CardView failed uid=%d", uid); continue; }

        cv->setOnClicked([this](int clickedId) {
            this->handleCardClick(clickedId);
            });

        _pfView.addChild(cv);
        cv->setPosition(cfg.playfield[i].Position);// 摆到关卡配置指定的位置
        cv->applyFaceComposite(cm.rank, cm.suit, true); // 正面朝上：底板+大数字+角标花色
    }
}

CardView* PlayFieldController::findCardViewIn(Node* parent, int uid) const {
    // 在parent的直接子节点里找uid匹配的CardView（非递归）
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

bool PlayFieldController::isOnTable(int cardId) const {
    // 该uid是否仍在“桌面容器”中（未被移走）
    return std::find(_m.table.begin(), _m.table.end(), cardId) != _m.table.end();
}

void PlayFieldController::handleCardClick(int cardId) {
    //牌必须还在桌面上（避免重复点击/已移动）
    if (!isOnTable(cardId)) return;

    //顶部位必须存在一张“参考牌”（_m.top != -1）
    if (_m.top == -1) return;

    //取点击牌与顶部牌的模型信息
    const CardModel& c = _m.cards.at(cardId);
    const CardModel& top = _m.cards.at(_m.top);

    //规则判断：是否点数相邻
    if (!RuleService::adjacent(c.rank, top.rank)) return;

    //若满足规则：找到这张牌在桌面视图中的实例，并执行“移入托盘顶部”的动作
    CardView* cv = findCardViewIn(&_pfView, cardId);
    if (cv) {
        replaceTrayWithPlayFieldCard(cardId, cv->getPosition());
    }
}

void PlayFieldController::replaceTrayWithPlayFieldCard(int cardId, const Vec2& fromPos) {
    //先创建一条撤销记录，并压入撤销栈
    UndoRecord rec; 
    rec.type = UndoRecord::TABLE_TO_TOP;// 类型：从“桌面 → 顶部”
    rec.movedUid = cardId; // 本次移动的牌
    rec.prevTopUid = _m.top;// 移动前顶部是哪个uid（用于撤销恢复） 
    rec.fromPos = fromPos;// 该牌在桌面视图中的原始位置（撤销时回到这）
    UndoService::push(_undo, rec);

    //从模型的桌面容器中移除这张牌
    std::vector<int>::iterator it = std::find(_m.table.begin(), _m.table.end(), cardId);
    if (it != _m.table.end()) _m.table.erase(it);

    //找到桌面视图里的CardView指针（准备做动画）
    CardView* cv = findCardViewIn(&_pfView, cardId);
    if (!cv) return;

    //如果顶部目前已有牌，先把旧顶牌视图“隐藏”（仍留在托盘层，便于撤销恢复）
    if (_m.top != -1) {
        CardView* oldV = _gameView.findCardViewIn(_gameView.stackView(), _m.top);
        if (oldV) oldV->setVisible(false);
    }

    //执行动画：把这张桌面牌从桌面PlayFieldView搬到StackView的顶部位
    // PlayFieldView::playMoveToTrayAnimation内部已做：
    // 坐标转换(本地->世界->托盘局部)、跨父节点搬运、Tween 平移
    _pfView.playMoveToTrayAnimation(
        cv, fromPos, _gameView.stackView(), _layout.topPos(),
        [this, cardId] { _m.top = cardId; }// 动画完成后，更新模型的顶部uid
    );
}

