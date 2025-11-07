#pragma once
#include "cocos2d.h"

// 一条撤销记录
struct UndoRecord {
    enum Type { HAND_TO_TOP, TABLE_TO_TOP } type = TABLE_TO_TOP;

    int movedUid = -1;           // 本次移动的牌
    int prevTopUid = -1;           // 移动前的顶部牌
    int fromHandIndex = -1;          // 如果来自手牌，记录手牌槽序号
    cocos2d::Vec2 fromPos;           // 如果来自桌面，记录桌面原位置
};
