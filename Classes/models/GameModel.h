#pragma once
#include "CardModel.h"
#include <unordered_map>
#include <vector>

enum class AreaType { TABLE, HAND_SLOT, TOP };

class GameModel {
public:
    // 全部卡牌数据（uid → CardModel）
    std::unordered_map<int, CardModel> cards;

    // 桌面牌：存 uid
    std::vector<int> table;

    // 手牌槽：每个槽是一个 vector（顶端是 back()）
    std::vector<std::vector<int>> hand;

    // 顶部牌 uid（-1 表示无）
    int top = -1;

    // 生成唯一 uid
    int genUid() { return _nextUid++; }

private:
    int _nextUid = 1;
};
