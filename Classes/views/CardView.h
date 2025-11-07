#pragma once
#include "cocos2d.h"
#include <functional>

/*
 * - 继承自Sprite：方便直接作为“整张牌”的根节点参与布局/点击命中。
 * - 通过uid唯一标识（由外部生成并传入），点击时仅上抛uid。
 * - 触摸事件在 enableTouch() 中注册：命中自身contentSize 后上抛 _onClicked(uid)
 */
class CardView : public cocos2d::Sprite {
public:
    static CardView* create(int uid);
    int uid() const { return _uid; }

    void setOnClicked(std::function<void(int)> cb) { _onClicked = std::move(cb); }

    //void applyFace(bool up, const std::string& face, const std::string& back);

    // 三件套叠放（底板 + 大数字 + 花色）
    void applyFaceComposite(int rank, int suit, bool faceUp);

private:
    int _uid = -1;// 唯一ID
    std::function<void(int)> _onClicked;// 点击回调（仅传回uid）

    // 启用触摸：注册单点触摸监听，命中自身时触发 _onClicked(uid)
    void enableTouch();
};
