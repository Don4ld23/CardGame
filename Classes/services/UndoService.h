#pragma once
#include "models/UndoModel.h"
#include <vector>

/**
 * 面向“撤销记录栈”的静态工具集合，直接操作外部传入的 std::vector<UndoRecord>。
 * 与 UndoManager 的区别：
 * - UndoManager 封装成一个类并持有内部栈；
 * - UndoService 则是“无状态”的静态工具，操作调用方传入的容器。
 *
 * 提供功能：
 * - push(st, r)：把一条撤销记录压栈（前进操作完成后调用）
 * - empty(st)  ：查询栈是否为空（用于判断能否撤销）
 * - pop(st)    ：弹出最近一条撤销记录并返回（执行撤销时调用）
 */
class UndoService {
public:
    // 压入一条撤销记录
    static void push(std::vector<UndoRecord>& st, const UndoRecord& r) {
        st.push_back(r);
    }
    // 栈是否为空
    static bool empty(const std::vector<UndoRecord>& st) {
        return st.empty();
    }
    // 弹出最近一条撤销记录（调用方负责据此回滚视图与模型）
    static UndoRecord pop(std::vector<UndoRecord>& st) {
        UndoRecord r = st.back();
        st.pop_back();
        return r;
    }
};

