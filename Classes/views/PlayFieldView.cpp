#include "views/PlayFieldView.h"
#include "views/CardView.h"
#include "services/TweenService.h"
#include "configs/models/CardResConfig.h"
USING_NS_CC;

bool PlayFieldView::init() {
    if (!Node::init()) return false;
    // 设定自身的内容尺寸为上半区大小，便于统一命中/布局
    setContentSize({ (float)CardResConfig::kDesignW, (float)CardResConfig::kPlayfieldH });
    setAnchorPoint({ 0.5f, 0.5f });
    // 注册触摸监听（内部负责把命中的CardView的uid回调出去）
    enableTouch();
    return true;
}

void PlayFieldView::enableTouch() {
    auto l = EventListenerTouchOneByOne::create();
    l->setSwallowTouches(true);
    l->onTouchBegan = [this](Touch* t, Event*) {
        // 将触摸点转换到本视图的局部坐标系，即PlayFieldView 的本地坐标
        Vec2 lp = convertToNodeSpace(t->getLocation());
        // 从上往下命中，保证最上层牌优先
        // 从上往下命中，保证最上层（更高 z-order）的牌优先被点到
        for (int i = (int)getChildrenCount() - 1; i >= 0; --i) {
            if (auto* cv = dynamic_cast<CardView*>(getChildren().at(i))) {
                // 使用每张卡牌在本视图坐标系，即PlayFieldView 的本地坐标下的包围盒做命中（getBoundingBox()返回的是在父坐标系中。
                // CardView 的父亲就是 PlayFieldView）
                if (cv->getBoundingBox().containsPoint(lp)) {//判断点lp是否落在这个矩形内（同一坐标系下）
                    // 命中即上抛该牌uid，外部根据uid处理匹配/回退等逻辑
                    if (_onCardClick) _onCardClick(cv->uid());//调用保存的回调函数
                    return true;
                }
            }
        }
        return false;// 未命中任何卡牌
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(l, this);
}

void PlayFieldView::playMoveToTrayAnimation(CardView* cv,
    const Vec2& fromPos,
    Node* trayParent,
    const Vec2& trayLocalPos,
    const std::function<void()>& onDone) {
    Vec2 world = convertToWorldSpace(fromPos);
    Vec2 localInTray = trayParent->convertToNodeSpace(world);

    // 跨父节点搬运：
    cv->retain();
    cv->removeFromParent();
    trayParent->addChild(cv);
    cv->release();

    // 3) 在新父节点下，先落在“转换后的起点”再做位移动画到目标位置
    cv->setPosition(localInTray);
    // 4) 使用 TweenService 执行平移动画，完成后回调onDone
    TweenService::moveTo(cv, trayLocalPos, onDone);
}
