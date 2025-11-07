#include "views/CardView.h"
#include "configs/models/CardResConfig.h"
#include "configs/models/CardSkinConfig.h"
USING_NS_CC;
//单张牌的可视化组件：掌管翻面、选中、高亮、可点击（触摸命中）等UI与交互。
//不做规则判断，把点击通过回调上抛。（只回传uid）

CardView* CardView::create(int uid) {
    //工厂创建：此处不指定贴图，外观由后续applyFace/applyFaceComposite决定
    auto v = new (std::nothrow) CardView();
    if (v && v->init()) {
        v->_uid = uid;
        v->autorelease();
        v->enableTouch(); // 注册触摸
        return v;
    }
    CC_SAFE_DELETE(v);
    return nullptr;
}

//void CardView::applyFace(bool up, const std::string& face, const std::string& back) {
//    setTexture(up ? face : back);
//}

//分层叠放（底板 + 数字 + 花色）
void CardView::applyFaceComposite(int rank, int suit, bool faceUp) {
    removeAllChildren();// 先清理所有子节点，避免重复调用导致叠层

    if (!faceUp) {
        // 背面朝上：设置背面贴图，不再叠加任何前景元素
        setTexture(CardResConfig::backTex());
        return;
    }

    // 底板：优先使用主题背景；若不存在则退回背面贴图
    const std::string bg = CardResConfig::backgroundTex();

    if (cocos2d::FileUtils::getInstance()->isFileExist(bg)) setTexture(bg);
    else setTexture(CardResConfig::backTex());
    // 以当前 Sprite 的 contentSize 为叠放布局基准
    const cocos2d::Size sz = getContentSize();

    // ====== 中央大数字（仅保留数字，不再叠加大花色）======
    // 资源由CardSkinConfig决定（可根据rank/suit切不同风格）
    if (auto num = Sprite::create(CardSkinConfig::bigNumberTex(rank, suit))) {
        num->setAnchorPoint({ 0.5f, 0.5f });
        num->setPosition({ sz.width * 0.5f, sz.height * 0.50f }); 
        float targetH = sz.height * CardResConfig::kBigNumHRate;
        float scale = targetH / num->getContentSize().height;
        num->setScale(scale);
        addChild(num, 1);
    }
    else {
        CCLOGERROR("missing big number texture, rank=%d suit=%d", rank, suit);// 资源缺失时给出日志，便于排查皮肤表配置
    }

    // ====== 左上角小数字 ======
    if (auto sNum = cocos2d::Sprite::create(CardSkinConfig::smallNumberTex(rank, suit))) {
        sNum->setAnchorPoint({ 0.f, 1.f });
        float padX = sz.width * CardResConfig::kCornerPadXRate;// 左/右边距比例
        float padY = sz.height * CardResConfig::kCornerPadYRate;// 上/下边距比例
        sNum->setPosition({ padX, sz.height - padY });
        float targetH = sz.height * CardResConfig::kSmallNumHRate; // 小数字目标高度比例
        float scale = targetH / sNum->getContentSize().height;
        sNum->setScale(scale);
        addChild(sNum, 2);// z-order=2，盖在大数字之上
    }
    else {
        CCLOGERROR("missing small number texture, rank=%d suit=%d", rank, suit);
    }

    // ====== 右上角超小花色（角标）======
    if (auto sSuit = cocos2d::Sprite::create(CardSkinConfig::suitTex(suit))) {
        sSuit->setAnchorPoint({ 1.f, 1.f });
        float padX = sz.width * CardResConfig::kCornerPadXRate;
        float padY = sz.height * CardResConfig::kCornerPadYRate;
        sSuit->setPosition({ sz.width - padX, sz.height - padY });
        float targetH = sz.height * CardResConfig::kSmallSuitHRate;// 花色角标目标高度比例
        float scale = targetH / sSuit->getContentSize().height;
        sSuit->setScale(scale);
        addChild(sSuit, 2);
    }
    else {
        CCLOGERROR("missing suit texture, suit=%d", suit);
    }
}


void CardView::enableTouch() {
    // 单点触摸监听；与项目现有输入模型一致Began命中即触发回调
    auto l = EventListenerTouchOneByOne::create();
    l->setSwallowTouches(true);// 吞掉事件，避免穿透到下一层
    l->onTouchBegan = [this](Touch* t, Event*) {
        // 若该节点不在场景树中（没父节点），不处理
        if (!getParent()) return false;
        Vec2 pLocal = getParent()->convertToNodeSpace(t->getLocation());// 将触摸点转换到“父节点坐标系”，与getBoundingBox()计算基准保持一致
        // 命中检测：使用自身在父坐标系下的包围盒
        if (getBoundingBox().containsPoint(pLocal)) {
            if (_onClicked) _onClicked(_uid);// 命中即上抛uid；外部根据uid处理匹配/回退等逻辑
            return true;
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(l, this);
}
