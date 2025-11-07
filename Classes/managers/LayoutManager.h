#pragma once
#include "cocos2d.h"
#include "configs/models/CardResConfig.h"

/**
 * 负责提供视图中用到的关键“位置点”的计算/封装：
 * - topPos()   ：托盘区“顶部位”的坐标（用于显示/移动顶牌）
 * - handPos(i)：第 i 个手牌槽位的坐标（i 从 0 开始）
 *
 * 设计思路：
 * - 将具体数值（坐标、间距）集中在 CardResConfig 中配置，
 *   LayoutManager 只做“读配置并计算坐标”的薄封装，便于后续全局调参。
 */

class LayoutManager {
public:
    // 顶部位坐标（通常用于托盘区最上方的一张牌）
    cocos2d::Vec2 topPos() const { 
        return cocos2d::Vec2(CardResConfig::kTopX, CardResConfig::kTopY); 
    }

    // 第 i 个手牌槽位坐标（等间距排布）
    // handStartX + i * handGap 为横坐标，纵坐标固定在 handY
    cocos2d::Vec2 handPos(int i) const {
        return cocos2d::Vec2(CardResConfig::kHandStartX + i * CardResConfig::kHandGap,
            CardResConfig::kHandY);
    }
};

