#include "services/RuleService.h"
#include "configs/models/CardResConfig.h"
#include <cstdlib> // std::abs

bool RuleService::adjacent(int a, int b) {
    // 基本合法性：点数必须为正（通常是 1..13）
    if (a <= 0 || b <= 0) return false;
    // 标准相邻：差值为 1
    if (std::abs(a - b) == 1) return true;
    // 可选：A-K 环接（例如允许从 A 走到 K，或从 K 走到 A）
    if (CardResConfig::kWrapAk) {
        if ((a == 1 && b == 13) || (a == 13 && b == 1)) return true;
    }
    // 其余情况不相邻
    return false;
}
