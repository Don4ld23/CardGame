#pragma once
#include "models/UndoModel.h"
#include <vector>

/**
 * 撤销管理器：维护一个“撤销记录”的栈（后进先出 LIFO）。
 * - init()   ：清空撤销栈（新局或重置时调用）
 * - push(r)  ：压入一条撤销记录（每次有效操作后调用）
 * - empty()  ：是否为空（用于判断能否撤销）
 * - pop()    ：弹出最近一条撤销记录并返回（执行撤销时调用）
 * - stack()  ：暴露底层容器引用，供需要直接访问的服务/控制器使用
 *
 * 说明：
 * - UndoRecord 的结构定义在 models/UndoModel.h 中，通常包含：
 *   * type（操作类型，如 HAND_TO_TOP / TABLE_TO_TOP）
 *   * movedUid / prevTopUid / fromHandIndex / fromPos 等
 */

class UndoManager {
public:
    // 重置撤销栈（开新局或切关卡时）
    void init() { _stack.clear(); }
    // 推入一条撤销记录（完成一次前进操作后调用）
    void push(const UndoRecord& r) { _stack.push_back(r); }
    // 撤销栈是否为空（为空则不能撤销）
    bool empty() const { return _stack.empty(); }
    // 弹出最近一次操作记录（调用方据此执行回滚）
    UndoRecord pop() { 
        UndoRecord r = _stack.back();
        _stack.pop_back(); 
        return r; 
    }
    // 直接访问底层容器（例如传给控制器/服务进行更灵活的操作）
    std::vector<UndoRecord>& stack() { return _stack; }
private:
    std::vector<UndoRecord> _stack;// 撤销记录栈（LIFO）
};
