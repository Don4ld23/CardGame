#pragma once
#include "models/UndoModel.h"
#include <vector>

/**
 * 面向“撤销记录栈”的静态工具集合，直接操作外部传入的std::vector<UndoRecord>。
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

