// Classes/configs/models/CardResConfig.h
//提供与皮肤无关的全局 UI / 布局常量与“牌面底板 / 牌背”的文件名。
// 控制 GameView 上下分区高度、手牌与顶部牌位置、卡面元素比例、移动动画时长等。
#pragma once
#include <string>

/**
 * CardResConfig
 * -------------
 * 与“皮肤资源”无关的全局常量与公共贴图名。
 * - 设计分辨率与区域高度（GameView 布局、上下分区着色）
 * - 手牌与顶部牌的摆放参数
 * - 卡面元素（大数字/小数字/小花色）缩放与内边距比例
 * - 牌面底板/牌背文件名（通常由一张 card_general.png 复用）
 *
 * 不负责红/黑大数字路径；那是 CardSkinConfig 的职责。
 */

struct CardResConfig {
    // --------- 画布/分区尺寸（与 AppDelegate 的 setDesignResolutionSize 保持一致） ---------
    static constexpr int  kDesignW = 1080;
    static constexpr int  kDesignH = 2080;
    static constexpr int  kPlayfieldH = 1500;// 上半主牌区高度
    static constexpr int  kStackH = 580;// 下半堆区高度

    // --------- 规则/交互相关常量 ----------
    static constexpr bool  kWrapAk = false;// A 与 K 是否相邻（玩法开关）
    static constexpr float kMoveDuration = 0.18f;// 简单 MoveTo 动画时长（秒）

    // --------- 堆区（底部）摆放 ----------
    static constexpr float kHandGap = 150.f;// 手牌之间的水平间距
    static constexpr float kHandStartX = 270.f;// 左侧第一张手牌的 X
    static constexpr float kHandY = kStackH * 0.5f;// 手牌 Y
    static constexpr float kTopX = 750.f;// 右侧顶部牌的 X
    static constexpr float kTopY = kStackH * 0.5f;// 顶部牌 Y

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
    // 与皮肤目录无关：把此文件放在 Resources 根目录即可
    static std::string backgroundTex() { return "card_general.png"; }
    static std::string faceTex(int, int) { return backgroundTex(); } // 兼容旧接口
    static std::string backTex() { return backgroundTex(); }
};

