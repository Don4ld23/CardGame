#pragma once
#include "cocos2d.h"
#include "configs/models/CardResConfig.h"

/**
 * 极简动画封装：目前只提供“MoveTo + 结束回调”。
 * 设计目的：
 * - 统一动画时长：使用 CardResConfig::kMoveDuration 作为全局配置；
 * - 统一回调触发：动画结束后若提供了 done() 则调用（空则忽略）；
 * - 降低控制器里直接拼接 Sequence/CallFunc 的重复代码。
 *
 * 使用示例：
 *   TweenService::moveTo(cardView, layout.topPos(), []{
 *       // 动画完成后的状态更新...
 *   });
 */
// 轻量动画封装：仅封装 MoveTo + 回调，便于统一动画时长
class TweenService {
public:
    /**
    * 将结点 n 平移到 dst，并在完成后调用 done（若非空）
    * @param n     需要移动的结点（Node*）；若为 nullptr，直接同步调用 done 并返回
    * @param dst   目标坐标（父坐标系下）
    * @param done  可选的完成回调（缺省为空）
    */
    static void moveTo(cocos2d::Node* n,
        const cocos2d::Vec2& dst,
        const std::function<void()>& done = std::function<void()>())
    {
        using namespace cocos2d;

        // 判空保护：没有节点时不做动画，直接触发回调（若有）
        if (!n) { if (done) done(); return; }
        // 运行一个 MoveTo，然后在结束时调用 done
        n->runAction(Sequence::create(
            MoveTo::create(CardResConfig::kMoveDuration, dst),// 统一持续时间
            CallFunc::create([done] { if (done) done(); }),// 结束回调（可空）
            nullptr
        ));
    }
};
