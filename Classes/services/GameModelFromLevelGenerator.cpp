#include "services/GameModelFromLevelGenerator.h"

//CardFace(0..12) → rank(1..13)
//说明：关卡里的CardFace通常从0开始（0..12），而GameModel使用1..13（A..K）。
//因此这里做一个简单的“面值 → 点数”映射。
static inline int faceToRank(int faceType) { 
    return faceType + 1; 
}

void GameModelFromLevelGenerator::build(const LevelConfig& cfg, GameModel& gm) {
    //重置运行时模型
    gm.cards.clear();// uid → CardModel的字典
    gm.table.clear();// 桌面区（Playfield）中牌的uid列表
    gm.hand.clear();// 手牌列（vector< vector<uid> >）
    gm.top = -1;// 顶部牌 uid，-1表示无

    //生成桌面区（Playfield）
    //遍历关卡配置里的playfield槽位，为每一格生成一张牌：
    //分配新的uid（gm.genUid()）
    //由 CardFace / CardSuit生成CardModel（点数rank、花色suit）
    //记到gm.cards，并把uid推入gm.table
    for (size_t i = 0; i < cfg.playfield.size(); ++i) {
        const LevelCardCfg& c = cfg.playfield[i];

        CardModel m;
        m.uid = gm.genUid();// 全局唯一ID
        m.rank = faceToRank(c.CardFace);// 0..12 → 1..13
        m.suit = c.CardSuit;// 花色（与关卡枚举保持一致）
        m.faceUp = true;// 初始全部正面朝上

        gm.cards[m.uid] = m; // 记录到“uid→牌模型”的表
        gm.table.push_back(m.uid); // 放到桌面容器
    }

    //生成堆牌区（Stack）
    //cfg.stack的前n-1个用于“手牌槽”；最后1个用于“初始顶部牌(top)”
    const int n = (int)cfg.stack.size();
    if (n > 0) {
        gm.hand.resize(n - 1);// 手牌列数量 = n - 1

        for (int i = 0; i < n; ++i) {
            const LevelCardCfg& s = cfg.stack[i];
            CardModel m;
            m.uid = gm.genUid();
            m.rank = faceToRank(s.CardFace);
            m.suit = s.CardSuit;
            m.faceUp = true;
            gm.cards[m.uid] = m;

            if (i == n - 1) { //最后一格 → 初始顶部牌
                gm.top = m.uid; 
            }
            else {//其余每个手牌槽一个顶牌（push_back到对应列）
                gm.hand[i].push_back(m.uid); // 每个手牌槽一个
            }
        }
    }
}

