#pragma once
#include "configs/models/LevelConfig.h"
#include "models/GameModel.h"

/**
 *目的：把静态的关卡配置（LevelConfig）翻译为运行时可用的GameModel。即根据关卡配置数据构建游戏运行时模型
 *build(cfg, gm)：核心实现，清空并重建gm的 cards/table/hand/top。
 *generateGameModel(...)：给外部使用的别名（兼容你之前的命名）。
 */

class GameModelFromLevelGenerator {
public:
    // 将LevelConfig（静态配置）转换为GameModel（运行时数据）
    static void build(const LevelConfig& cfg, GameModel& gm);

    // 别名，与你之前的命名保持一致
    static inline void generateGameModel(const LevelConfig& cfg, GameModel& gm) {
        build(cfg, gm);
    }
};
