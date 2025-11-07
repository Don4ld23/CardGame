#pragma once

struct CardModel {
    int uid = -1;  // 唯一ID
    int rank = 1;   // 1..13（A..K）
    int suit = 0;   // 0..3（0=♣,1=♦,2=♥,3=♠）
    bool faceUp = true; // 是否翻开
};
