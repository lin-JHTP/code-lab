"""
Python NumPy 入门（AI必备基础）
================================
文件：numpy_intro.py
作者：你的名字
日期：2026

NumPy 是 Python AI/科学计算的基石！
本文件演示：
    1. 数组创建
    2. 数组运算（向量化，比循环快100x！）
    3. 数组索引与切片
    4. 矩阵运算（线性代数基础）
    5. 统计函数（均值、方差、相关性）
    6. 实际应用：传感器数据处理

安装：pip install numpy
运行：python numpy_intro.py

注意：如果没有安装 numpy，先运行：pip install numpy
"""

try:
    import numpy as np
    print("NumPy 已安装，版本:", np.__version__)
except ImportError:
    print("请先安装 NumPy: pip install numpy")
    exit(1)

# ========== 1. 数组创建 ==========
print("\n===== 1. 数组创建 =====")

# 从列表创建
arr1d = np.array([1, 2, 3, 4, 5])
arr2d = np.array([[1, 2, 3],
                  [4, 5, 6],
                  [7, 8, 9]])

print(f"一维数组: {arr1d}")
print(f"二维数组:\n{arr2d}")
print(f"形状 (shape): {arr2d.shape}")    # (行数, 列数)
print(f"维度 (ndim): {arr2d.ndim}")
print(f"元素总数: {arr2d.size}")
print(f"数据类型: {arr2d.dtype}")

# 常用创建函数
zeros = np.zeros((3, 4))            # 全0矩阵
ones = np.ones((2, 3), dtype=int)   # 全1矩阵（整数类型）
eye = np.eye(3)                      # 单位矩阵（对角线为1）
rng = np.arange(0, 10, 2)           # 等差数列（类似 range）
linspace = np.linspace(0, 1, 5)     # 均匀分布5个点

print(f"\nzeros(3x4):\n{zeros}")
print(f"eye(3):\n{eye}")
print(f"arange(0,10,2): {rng}")
print(f"linspace(0,1,5): {linspace}")

# ========== 2. 向量化运算（核心！）==========
print("\n===== 2. 向量化运算 =====")

# 传统 Python 循环 vs NumPy 向量化
import time

size = 1_000_000   # 一百万个元素

# 方式1：Python 循环（慢！）
python_list = list(range(size))
t0 = time.time()
python_result = [x * 2 + 1 for x in python_list]
t_python = time.time() - t0

# 方式2：NumPy 向量化（快！）
np_array = np.arange(size)
t0 = time.time()
np_result = np_array * 2 + 1    # 对整个数组的运算，一行搞定！
t_numpy = time.time() - t0

print(f"Python 循环耗时: {t_python*1000:.1f}ms")
print(f"NumPy  向量化:  {t_numpy*1000:.1f}ms")
print(f"NumPy 快了约: {t_python/t_numpy:.0f}x")

# 基本运算：对每个元素操作
a = np.array([1, 2, 3, 4])
b = np.array([10, 20, 30, 40])

print(f"\na + b = {a + b}")
print(f"a * b = {a * b}")        # 逐元素乘（不是矩阵乘！）
print(f"a ** 2 = {a ** 2}")      # 平方
print(f"np.sqrt(a) = {np.sqrt(a.astype(float)):.3f}")

# 广播（Broadcasting）：不同形状的数组运算
matrix = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
vector = np.array([1, 2, 3])
result = matrix + vector   # 每行都加 vector（广播！）
print(f"\n广播（矩阵+向量）:\n{result}")

# ========== 3. 索引与切片 ==========
print("\n===== 3. 索引与切片 =====")

arr = np.arange(20).reshape(4, 5)   # 创建4行5列的矩阵
print(f"4x5矩阵:\n{arr}")

print(f"第2行: {arr[1]}")            # 行索引
print(f"第3行第4列: {arr[2, 3]}")    # [行, 列]
print(f"前2行: \n{arr[:2]}")         # 切片
print(f"后2行后3列:\n{arr[-2:, -3:]}")

# 条件索引（布尔索引）- AI中非常常用！
data = np.array([10, -5, 20, -3, 15, -8, 25])
positives = data[data > 0]           # 筛选正数
print(f"\n原数据: {data}")
print(f"正数: {positives}")

# 将负数替换为0（信号处理中的 ReLU！）
data_clipped = np.where(data > 0, data, 0)
print(f"负数归零: {data_clipped}")

# ========== 4. 矩阵运算 ==========
print("\n===== 4. 矩阵运算 =====")

A = np.array([[1, 2], [3, 4]])
B = np.array([[5, 6], [7, 8]])

print(f"矩阵 A:\n{A}")
print(f"矩阵 B:\n{B}")
print(f"矩阵乘法 A@B:\n{A @ B}")         # @ 运算符表示矩阵乘（Python 3.5+）
print(f"矩阵转置 A.T:\n{A.T}")
print(f"行列式 det(A): {np.linalg.det(A):.1f}")
print(f"逆矩阵 inv(A):\n{np.linalg.inv(A)}")

# 解线性方程组 Ax = b（机器学习线性回归的核心！）
# 例：1x + 2y = 5
#     3x + 4y = 11
A_eq = np.array([[1, 2], [3, 4]])
b_eq = np.array([5, 11])
x = np.linalg.solve(A_eq, b_eq)
print(f"\n解方程组 Ax=b: x={x[0]:.1f}, y={x[1]:.1f}")

# ========== 5. 统计函数 ==========
print("\n===== 5. 统计函数 =====")

# 模拟充电桩一天的电压采样数据（正态分布模拟真实噪声）
np.random.seed(42)   # 固定随机种子，保证结果可重现（AI中必须这样做！）
voltage_samples = np.random.normal(loc=220.0, scale=2.0, size=1000)  # 均值220V，标准差2V

print(f"采样数量: {len(voltage_samples)}")
print(f"均值:     {np.mean(voltage_samples):.2f} V")
print(f"中位数:   {np.median(voltage_samples):.2f} V")
print(f"标准差:   {np.std(voltage_samples):.2f} V")
print(f"最小值:   {np.min(voltage_samples):.2f} V")
print(f"最大值:   {np.max(voltage_samples):.2f} V")
print(f"百分位数 [5%, 95%]: [{np.percentile(voltage_samples, 5):.1f}, {np.percentile(voltage_samples, 95):.1f}] V")

# 判断异常值（超出 ±3σ 范围）
mean = np.mean(voltage_samples)
std  = np.std(voltage_samples)
anomalies = voltage_samples[np.abs(voltage_samples - mean) > 3 * std]
print(f"异常电压点数量: {len(anomalies)}")

if __name__ == "__main__":
    print("\n✅ numpy_intro.py 运行完成！")
    print("🔥 下一步：学习 Matplotlib 可视化这些数据！")

"""
TODO 练习：

TODO-01: 手动实现矩阵乘法（不用 @），再与 np.dot 结果比较
    用三层 for 循环实现 C[i][j] = sum(A[i][k] * B[k][j])

TODO-02: 用 NumPy 实现 softmax 函数（神经网络输出层！）
    def softmax(x): return np.exp(x) / np.sum(np.exp(x))

TODO-03: 实现数据归一化（Min-Max Normalization）
    x_norm = (x - x_min) / (x_max - x_min)  → 值域 [0, 1]

TODO-04: 用 NumPy 分析充电桩电流数据
    生成模拟数据，计算：总电量(积分)、峰值、平均值、有效值(RMS)
    RMS = sqrt(mean(x^2))  交流电"有效值"的计算方式

TODO-05: 实现卷积操作（不用 scipy）
    对一维信号做滑动平均（均值滤波），用于去噪

TODO-06: 用 NumPy 实现简单线性回归（最小二乘法）
    给定 x 和 y 数据，求最佳拟合直线 y = kx + b
    公式：k = (n*Σxy - Σx*Σy) / (n*Σx² - (Σx)²)
"""
