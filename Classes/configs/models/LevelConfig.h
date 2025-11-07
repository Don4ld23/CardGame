#pragma once
#include "cocos2d.h"
#include <vector>
//定义“静态关卡配置”的数据结构。
//与LevelConfigLoader配套：后者把JSON读入并解析为本结构；
//然后GameModelFromLevelGenerator再把它转成运行时GameModel。

/**
 * 关卡文件中单张卡的静态配置：
 *  - CardFace：点数（0..13 或 1..13，与你的项目枚举/约定一致即可）
 *  - CardSuit：花色（0..3）
 *  - Position：该卡初始显示位置（像素坐标，参考 CardResConfig 的设计分辨率）
 */
struct LevelCardCfg {
    // 卡牌数字CardFace: 0..12 → rank: 1..13（在 generator 中 +1）
    int CardFace = 0;
    // 卡牌花色CardSuit: 0..3（0=♣,1=♦,2=♥,3=♠）
    int CardSuit = 0;
    cocos2d::Vec2 Position;// 在“主牌区”或“堆区”内的绝对像素坐标
};

/**
 * 整个关卡的静态配置：
 *  - designW/designH：设计分辨率
 *  - playfieldH/stackH：上下分区高度
 *  - playfield：主牌区卡牌列表
 *  - stack：堆区初始牌（备用/手牌/顶部等，具体解释由生成器决定）
 */
struct LevelConfig {
    // 桌面（主牌区）布局
    std::vector<LevelCardCfg> playfield;
    // 堆牌（手牌槽 + 顶部：数组最后一张为初始顶部牌）
    std::vector<LevelCardCfg> stack;

    // 设计分辨率
    int  designW = 1080;
    int  designH = 2080;
    int  playfieldH = 1500;
    int  stackH = 580;
};
