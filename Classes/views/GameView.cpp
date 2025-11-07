#include "views/GameView.h"
#include "views/PlayFieldView.h"
#include "views/StackView.h"
#include "views/CardView.h"
#include "configs/models/CardResConfig.h"
USING_NS_CC;

bool GameView::init() {
    if (!Node::init()) return false;
    // 1) 先构建上下分区的背景色块（仅用于视觉分层）
    buildBackgroundSections();

    // 2) 创建并摆放上半区（玩法区/牌桌区）
    _playfield = PlayFieldView::create();
    _playfield->setPosition(Vec2(
        CardResConfig::kDesignW * 0.5f,// 居中
        CardResConfig::kStackH + CardResConfig::kPlayfieldH * 0.5f// 放在下区之上
    ));
    addChild(_playfield, 0);

    // 3) 创建并摆放下半区（堆区/发牌区）
    _stack = StackView::create();
    _stack->setPosition(Vec2(
        CardResConfig::kDesignW * 0.5f,// 居中
        CardResConfig::kStackH * 0.5f// 下区居中
    ));
    addChild(_stack, 1);

    return true;
}

void GameView::buildBackgroundSections() {
    // 上半区背景：一般作为“玩法/牌桌区”的底色
    auto pfLayer = LayerColor::create(
        Color4B(175, 128, 78, 255),
        CardResConfig::kDesignW, CardResConfig::kPlayfieldH);
    pfLayer->setIgnoreAnchorPointForPosition(false);
    pfLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    pfLayer->setPosition(Vec2(
        CardResConfig::kDesignW * 0.5f,
        CardResConfig::kStackH + CardResConfig::kPlayfieldH * 0.5f));
    addChild(pfLayer, -10);// 背景放到更低层（z=-10）

    // 下半区背景：一般作为“堆区/发牌区”的底色
    auto stLayer = LayerColor::create(
        Color4B(128, 32, 128, 255),
        CardResConfig::kDesignW, CardResConfig::kStackH);
    stLayer->setIgnoreAnchorPointForPosition(false);
    stLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    stLayer->setPosition(Vec2(
        CardResConfig::kDesignW * 0.5f,
        CardResConfig::kStackH * 0.5f));
    addChild(stLayer, -10);// 同样放在底层
}

void GameView::setupInitialLayout(const LayoutManager&, const LevelConfig&) {
    // UI 根菜单，挂在底部堆区节点上，保证层级在牌面上面
    auto menu = Menu::create();
    menu->setName("UIRoot");
    menu->setPosition(Vec2::ZERO);
    _stack->addChild(menu, 100); // z=100，压过所有卡牌

    // 文本：优先用中文 TTF，没放字体时自动回退英文
    MenuItemLabel* undoBtn = nullptr;
    auto labelCN = Label::createWithTTF(u8"回退", "fonts/msyh.ttc", 40);
    if (labelCN) {
        undoBtn = MenuItemLabel::create(labelCN, nullptr);
    }
    else {
        auto labelEN = Label::createWithSystemFont("Undo", "Arial", 38);
        undoBtn = MenuItemLabel::create(labelEN, nullptr);
    }
    undoBtn->setName("UndoButton");
    undoBtn->setAnchorPoint(Vec2(1.f, 0.5f));

    // 将按钮放到下半区右侧内边距处
    const float pad = 70.f; // 右侧内边距
    undoBtn->setPosition(Vec2(CardResConfig::kDesignW - pad, CardResConfig::kStackH * 0.5f));

    menu->addChild(undoBtn);
}

void GameView::playEnterAnimation(const std::function<void()>& onDone) {
    // 整体淡入动画：先设置透明，再用0.2s 淡入；结束后执行回调
    setOpacity(0);
    runAction(Sequence::create(
        FadeIn::create(0.2f),
        CallFunc::create([onDone] { if (onDone) onDone(); }),
        nullptr));
}

//findCardViewIn 实现
//工具：在指定父节点下，按uid查找CardView
//仅遍历 parent 的“直接子节点”
CardView* GameView::findCardViewIn(Node* parent, int uid) const {
    if (!parent) return nullptr;
    const auto& children = parent->getChildren();
    for (size_t i = 0; i < children.size(); ++i) {
        Node* n = children.at(i);
        if (auto* cv = dynamic_cast<CardView*>(n)) {
            if (cv->uid() == uid) return cv;
        }
    }
    return nullptr;
}

