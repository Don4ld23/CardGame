#pragma once
#include "cocos2d.h"
#include "configs/models/CardResConfig.h"

/**
 * 极简动画封装：目前只提供“MoveTo+结束回调”。
 * 设计目的：
 * - 统一动画时长：使用CardResConfig::kMoveDuration作为全局配置；
 * - 统一回调触发：动画结束后若提供了done()则调用（空则忽略）；
 * - 降低控制器里直接拼接Sequence/CallFunc的重复代码。
 */
class TweenService {
public:
    static void moveTo(cocos2d::Node* n,
        const cocos2d::Vec2& dst,
        const std::function<void()>& done = std::function<void()>())
    {
        using namespace cocos2d;

        // 判空保护：没有节点时不做动画，直接触发回调（若有）
        if (!n) { if (done) done(); return; }
        // 运行一个MoveTo，然后在结束时调用done
        n->runAction(Sequence::create(
            MoveTo::create(CardResConfig::kMoveDuration, dst),// 统一持续时间
            CallFunc::create([done] { if (done) done(); }),// 结束回调（可空）
            nullptr
        ));
    }
};
