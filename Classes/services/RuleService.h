#pragma once
/**
 * - 封装与“出牌规则”相关的静态判断函数。
 * - 当前只提供相邻判断：adjacent(a, b)
 *   * 标准相邻：|a - b| == 1
 *   * 可选环接：A(1) 与 K(13) 也算相邻（由 CardResConfig::kWrapAk 控制）
 */
struct RuleService {
    /**
    * 判断两个点数是否相邻
    * @param a  点数（建议 1..13：A=1, J=11, Q=12, K=13）
    * @param b  点数（同上）
    * @return   true 相邻；false 不相邻或输入非法（<=0）
    *
    * 说明：
    * - 标准规则：|a - b| == 1
    * - 若 CardResConfig::kWrapAk 为 true，则 (1,13) 与 (13,1) 也视为相邻
    */
    static bool adjacent(int a, int b);
};
