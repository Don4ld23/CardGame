#include "configs/loaders/LevelConfigLoader.h"
#include "cocos2d.h"
#include "json/document.h"

using namespace cocos2d;

//把JSON数组写入到vector<LevelCardCfg> 即解析JSON数组，输出关卡(卡片)配置列表
static void parseArray(const rapidjson::Value& arr, std::vector<LevelCardCfg>& out) {
    out.clear();
    for (auto& v : arr.GetArray()) {// 遍历JSON数组中的每个元素
        LevelCardCfg c;// 创建临时卡片配置对象
        // 读取点数/花色
        c.CardFace = v["CardFace"].GetInt();
        c.CardSuit = v["CardSuit"].GetInt();
        // 位置坐标
        c.Position.x = v["Position"]["x"].GetFloat();
        c.Position.y = v["Position"]["y"].GetFloat();
        out.push_back(c);// 将解析好的卡片配置加入结果列表
    }
}

//从JSON文件加载游戏(关卡)配置
bool LevelConfigLoader::loadFromJson(const std::string& path, LevelConfig& out) {
    //读取文本
    std::string s = FileUtils::getInstance()->getStringFromFile(path);//读取文件内容到字符串
    if (s.empty()) {
        CCLOGERROR("LevelConfigLoader: file not found or empty: %s", path.c_str());
        return false;
    }

    //解析JSON
    rapidjson::Document d; 
    d.Parse(s.c_str());
    if (!d.IsObject()) {
        CCLOGERROR("LevelConfigLoader: invalid json: %s", path.c_str());
        return false;
    }

    //Playfield / Stack 两个数组
    //检查JSON中必须包含"Playfield"数组
    if (!d.HasMember("Playfield") || !d["Playfield"].IsArray()) {
        CCLOGERROR("LevelConfigLoader: missing Playfield in %s", path.c_str());
        return false;
    }
    //检查JSON中必须包含"Stack"数组
    if (!d.HasMember("Stack") || !d["Stack"].IsArray()) {
        CCLOGERROR("LevelConfigLoader: missing Stack in %s", path.c_str());
        return false;
    }

    // 5) 解析到 LevelConfig
    parseArray(d["Playfield"], out.playfield);
    parseArray(d["Stack"], out.stack);
    return true;
}

