#pragma once
#include "configs/models/LevelConfig.h"// 关卡配置的数据结构声明
#include <string>

/**
 * 负责把磁盘上的关卡JSON（静态配置）读取并反序列化成运行时结构LevelConfig。
 * - 只做“加载与解析”，不涉及任何游戏逻辑（符合“Services/Configs分层”）。
 */

class LevelConfigLoader {
public:
    static bool loadFromJson(const std::string& path, LevelConfig& out);

    // 按关卡号读取：Resources/config/level{levelId}.json
    static inline bool loadLevelConfig(int levelId, LevelConfig& out) {
        return loadFromJson(
            cocos2d::StringUtils::format("config/level%d.json", levelId),
            out
        );
    }
};
