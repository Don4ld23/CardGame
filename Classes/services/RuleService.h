#pragma once
/**
 * - 封装与“出牌规则”相关的静态判断函数。
 * - 当前只提供相邻判断：adjacent(a, b)
 *   * 标准相邻：|a - b| == 1
 *   * 可选环接：A(1) 与 K(13) 也算相邻（由CardResConfig::kWrapAk控制）
 */
struct RuleService {
    static bool adjacent(int a, int b);
};
