#pragma once
#include "cocos2d.h"
#include <functional>

/*
 * - 继承自 Sprite：方便直接作为“整张牌”的根节点参与布局/点击命中。
 * - 通过 uid 唯一标识（由外部生成并传入），点击时仅上抛 uid。
 * - 提供两种绘制方式：
 *    1) applyFace(up, face, back) —— 旧接口：单图贴图切换（face/back 两张）
 *    2) applyFaceComposite(rank, suit, faceUp) —— 新接口：三件套（底板 + 大数字/字母 + 花色）
 * - 触摸事件在 enableTouch() 中注册：命中自身 contentSize 后上抛 _onClicked(uid)
 */
class CardView : public cocos2d::Sprite {
public:
    /**
     * 工厂方法
     * @param uid 这张牌的唯一 ID（由外部逻辑分配）
     * @return 已 autorelease 的 CardView*
     */
    static CardView* create(int uid);
    int uid() const { return _uid; }

    // 外部注册点击回调（把 uid 传回去）
    /**
     * 外部注册点击回调
     * @param cb 形如 void(int uid) 的函数对象；命中点击时回传 uid
     */
    void setOnClicked(std::function<void(int)> cb) { _onClicked = std::move(cb); }

    //void applyFace(bool up, const std::string& face, const std::string& back);

    // 新接口：三件套叠放（底板 + 大数字 + 花色）
    /*
     * @param rank     点数（建议：A=1, 2..10, J=11, Q=12, K=13；Joker 可用 0 或 14）
     * @param suit     花色（建议：0=♠,1=♥,2=♣,3=♦,4=Joker）
     * @param faceUp   是否正面朝上；false 时仅显示背面底板
     * 注意：
     * - 该接口会清理旧的三件套子节点，并重新创建所需子节点。
     * - 背面样式可在实现中通过常量资源名或纯色底板控制。
     */
    void applyFaceComposite(int rank, int suit, bool faceUp);

private:
    int _uid = -1;// 唯一 ID
    std::function<void(int)> _onClicked;// 点击回调（仅传回 uid）

    // 启用触摸：注册单点触摸监听，命中自身时触发 _onClicked(uid)
    void enableTouch();
};
