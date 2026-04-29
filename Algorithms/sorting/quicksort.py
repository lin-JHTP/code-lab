"""
快速排序 - Python 版
=====================
文件：quicksort.py
作者：你的名字
日期：2026

Python 版快速排序，与 C 版对比学习。
特点：Python 代码更简洁，但性能不及C（解释型语言）。
AI 应用：排序是数据预处理的基础操作。
"""

import time
import random
from typing import List

# ===== 版本1：最简洁版（利用 Python 列表推导式）=====

def quicksort_simple(arr: List[int]) -> List[int]:
    """
    最 Python 风格的快排（简洁但创建了新数组，空间 O(n log n)）
    这版本虽然清晰，但不是原地排序
    """
    if len(arr) <= 1:
        return arr

    pivot = arr[len(arr) // 2]      # 取中间元素作为 pivot
    left  = [x for x in arr if x < pivot]
    mid   = [x for x in arr if x == pivot]
    right = [x for x in arr if x > pivot]

    return quicksort_simple(left) + mid + quicksort_simple(right)


# ===== 版本2：原地快排（节省内存）=====

def _partition(arr: List[int], low: int, high: int) -> int:
    """Lomuto 分区方案"""
    pivot = arr[high]
    i = low - 1

    for j in range(low, high):
        if arr[j] <= pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]   # Python 交换无需临时变量！

    arr[i + 1], arr[high] = arr[high], arr[i + 1]
    return i + 1


def quicksort_inplace(arr: List[int], low: int = 0, high: int = None) -> None:
    """原地快速排序（修改原数组）"""
    if high is None:
        high = len(arr) - 1

    if low < high:
        pi = _partition(arr, low, high)
        quicksort_inplace(arr, low, pi - 1)
        quicksort_inplace(arr, pi + 1, high)


# ===== 版本3：迭代版（避免递归栈溢出）=====

def quicksort_iterative(arr: List[int]) -> None:
    """
    用显式栈模拟递归，避免深度递归导致 Python 栈溢出
    Python 默认递归深度限制约 1000
    """
    if len(arr) <= 1:
        return

    stack = [(0, len(arr) - 1)]

    while stack:
        low, high = stack.pop()
        if low < high:
            pi = _partition(arr, low, high)
            stack.append((low, pi - 1))
            stack.append((pi + 1, high))


# ===== 对比 Python 内置排序 =====

def benchmark():
    """对比不同排序方法的速度"""
    n = 50000
    data = [random.randint(0, 10000) for _ in range(n)]

    print(f"测试规模: {n} 个元素\n")

    # 简洁版
    arr1 = data.copy()
    t0 = time.time()
    result1 = quicksort_simple(arr1)
    t1 = time.time() - t0
    print(f"简洁版快排:     {t1*1000:.1f}ms")

    # 原地版
    arr2 = data.copy()
    t0 = time.time()
    quicksort_inplace(arr2)
    t2 = time.time() - t0
    print(f"原地版快排:     {t2*1000:.1f}ms")

    # 迭代版
    arr3 = data.copy()
    t0 = time.time()
    quicksort_iterative(arr3)
    t3 = time.time() - t0
    print(f"迭代版快排:     {t3*1000:.1f}ms")

    # Python 内置（Timsort，混合归并+插入）
    arr4 = data.copy()
    t0 = time.time()
    arr4.sort()
    t4 = time.time() - t0
    print(f"Python sort():  {t4*1000:.1f}ms  ← 这是C实现，最快")

    # 验证正确性
    assert result1 == arr4, "简洁版排序错误！"
    assert arr2    == arr4, "原地版排序错误！"
    assert arr3    == arr4, "迭代版排序错误！"
    print("\n✅ 所有版本结果正确")


if __name__ == "__main__":
    # 小规模演示
    arr = [64, 34, 25, 12, 22, 11, 90, 7]
    print(f"原始: {arr}")
    sorted_arr = quicksort_simple(arr.copy())
    print(f"快排: {sorted_arr}")

    # 性能测试
    print("\n===== 性能对比 =====")
    benchmark()

"""
TODO 练习：

TODO-01: 实现归并排序（Python版）
    merge_sort(arr) 分治思想，结果正确且稳定

TODO-02: 用 heapq 实现堆排序
    import heapq; heapq.heapify(arr); ...

TODO-03: 实现基数排序（Radix Sort）
    先按个位排，再按十位排，... 最后得到有序数组

TODO-04: 可视化排序过程（用 matplotlib 动画！）
    每次交换后更新柱状图，视觉化理解排序

TODO-05: 实现归并排序计算逆序对数量
    对 [3, 1, 2] 而言，逆序对是 (3,1), (3,2)，共2个
"""
