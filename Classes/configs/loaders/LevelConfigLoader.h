#pragma once
#include "configs/models/LevelConfig.h"// 关卡配置的数据结构声明
#include <string>

/**
 * 负责把磁盘上的关卡JSON（静态配置）读取并反序列化成运行时结构LevelConfig。
 * - 只做“加载与解析”，不涉及任何游戏逻辑（符合“Services/Configs分层”）。
 * - 供GameController在startGame(levelId)时调用。
 * 依赖：
 *  - Cocos2d-x FileUtils（读文件）
 *  - rapidjson（解析 JSON）
 */

class LevelConfigLoader {
public:
    /**
     * 从相对Resources/的路径读取JSON并填充out。
     * @param path  例如 "config/level1.json"
     * @param out   输出的 LevelConfig（调用前不需要初始化）
     * @return      解析成功返回true；否则false
     */
    static bool loadFromJson(const std::string& path, LevelConfig& out);

    // 按关卡号读取：Resources/config/level{levelId}.json
    static inline bool loadLevelConfig(int levelId, LevelConfig& out) {
        return loadFromJson(
            cocos2d::StringUtils::format("config/level%d.json", levelId),
            out
        );
    }
};