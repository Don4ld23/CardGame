#pragma once
#include <string>
//把“点数 + 花色”映射为具体资源路径：大数字、小数字、花色图标。

/**
 * 皮肤资源映射层：根据rank/suit返回具体PNG路径（相对Resources/）。
 * - 红黑映射规则：梅花/黑桃 -> black；方块/红桃 -> red
 * - 只返回路径字符串，不做createSprite；由View决定如何加载与缩放
 */
enum CardSuitType {
    CST_NONE = -1,
    CST_CLUBS = 0,     // 梅花 ♣
    CST_DIAMONDS = 1,  // 方块 ♦
    CST_HEARTS = 2,    // 红桃 ♥
    CST_SPADES = 3,    // 黑桃 ♠
    CST_NUM_CARD_SUIT_TYPES
};

enum CardFaceType {
    CFT_NONE = -1,
    CFT_ACE = 0,  
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};

struct CardSkinConfig {
    // 把 rank（0..12 或 1..13）统一映射成 "A","2",...,"10","J","Q","K"
    static std::string rankToken(int rank) {
        int r = rank;
        if (r == 0) r = 1;          // 0 -> 1（ACE）
        if (r < 1 || r > 13) r = 1; // 容错与兜底
        switch (r) {
        case 1:  return "A";
        case 2:  return "2";
        case 3:  return "3";
        case 4:  return "4";
        case 5:  return "5";
        case 6:  return "6";
        case 7:  return "7";
        case 8:  return "8";
        case 9:  return "9";
        case 10: return "10";
        case 11: return "J";
        case 12: return "Q";
        case 13: return "K";
        default: return "A";
        }
    }

    // 梅花/黑桃 -> black；方块/红桃 -> red
    static const char* colorPrefix(int suit) {
        if (suit == CST_CLUBS || suit == CST_SPADES)   return "black";
        if (suit == CST_DIAMONDS || suit == CST_HEARTS) return "red";
        return "black"; // 兜底
    }

    // 中央大数字
    static std::string bigNumberTex(int rank, int suit) {
        return "number/big_" + std::string(colorPrefix(suit)) + "_" + rankToken(rank) + ".png";
    }

    // 左上角小数字
    static std::string smallNumberTex(int rank, int suit) {
        return "number/small_" + std::string(colorPrefix(suit)) + "_" + rankToken(rank) + ".png";
    }

    // 花色图标
    static std::string suitTex(int suit) {
        switch (suit) {
        case CST_CLUBS:    return "suits/club.png";
        case CST_DIAMONDS: return "suits/diamond.png";
        case CST_HEARTS:   return "suits/heart.png";
        case CST_SPADES:   return "suits/spade.png";
        default:           return "suits/club.png";
        }
    }
};

