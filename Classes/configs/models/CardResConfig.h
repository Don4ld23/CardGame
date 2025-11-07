#pragma once
#include <string>

/**
 * 与“皮肤资源”无关的全局常量与公共贴图名。
 * - 设计分辨率与区域高度（GameView 布局、上下分区着色）
 * - 手牌与顶部牌的摆放参数
 * - 卡面元素（大数字/小数字/小花色）缩放与内边距比例
 * - 牌面底板/牌背文件名（通常由一张card_general.png 复用）
 * 不负责红/黑大数字路径；那是CardSkinConfig的职责。
 */

struct CardResConfig {
    // --------- 画布/分区尺寸（与AppDelegate的setDesignResolutionSize保持一致） ---------
    static constexpr int  kDesignW = 1080;
    static constexpr int  kDesignH = 2080;
    static constexpr int  kPlayfieldH = 1500;// 上半主牌区高度
    static constexpr int  kStackH = 580;// 下半堆区高度

    // --------- 规则/交互相关常量 ----------
    static constexpr bool  kWrapAk = false;//A与K是否相邻
    static constexpr float kMoveDuration = 0.18f;// 简单MoveTo动画时长（秒）

    // --------- 堆区（底部）摆放 ----------
    static constexpr float kHandGap = 150.f;// 手牌之间的水平间距
    static constexpr float kHandStartX = 270.f;// 左侧第一张手牌的X
    static constexpr float kHandY = kStackH * 0.5f;// 手牌Y
    static constexpr float kTopX = 750.f;// 右侧顶部牌的X
    static constexpr float kTopY = kStackH * 0.5f;// 顶部牌Y

    // --------- 卡面元素缩放（按牌高比例） ----------
    // 这些比例在 CardView::applyFaceComposite() 中用于缩放各元素
    // 角标与中心元素的布局（按牌高比例）
    static constexpr float kBigNumHRate = 0.55f; // 中央大数字占牌高
    static constexpr float kSmallNumHRate = 0.14f;// 左上角小数字占牌高
    static constexpr float kSmallSuitHRate = 0.14f; // 右上角小花色占牌高

    // --------- 角标内边距（相对牌宽/高） ----------
    static constexpr float kCornerPadXRate = 0.07f; // 左右边距 = 牌宽 * 0.07
    static constexpr float kCornerPadYRate = 0.06f; // 上边距   = 牌高 * 0.06

    // --------- 底板/牌背贴图 ----------
    // 与皮肤目录无关：把此文件放在Resources根目录即可
    static std::string backgroundTex() { return "card_general.png"; }
    static std::string faceTex(int, int) { return backgroundTex(); }
    static std::string backTex() { return backgroundTex(); }
};

