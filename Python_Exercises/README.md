# Python 练习区 🐍

> 目标：从零掌握 Python，为 AI/机器学习打下坚实基础。
> 适合人群：C语言工程师转 Python、AI 学习入门者。

---

## 📚 内容索引

| 编号 | 目录 | 主题 | 难度 |
|------|------|------|------|
| 01 | `01_basics/` | Python基础语法（变量/类型/控制流）| ⭐ |
| 02 | `02_data_structures/` | 列表、字典、元组、集合 | ⭐⭐ |
| 03 | `03_functions_modules/` | 函数、模块、包 | ⭐⭐ |
| 04 | `04_oop/` | 面向对象编程 | ⭐⭐⭐ |
| 05 | `05_file_io/` | 文件读写、JSON、CSV处理 | ⭐⭐ |
| 06 | `06_numpy_intro/` | NumPy 数组运算（AI 必备！）| ⭐⭐⭐ |
| 07 | `07_data_visualization/` | Matplotlib 数据可视化 | ⭐⭐⭐ |
| 08 | `08_web_requests/` | HTTP 请求、API 调用 | ⭐⭐ |

---

## 🎯 成长路线（针对C工程师）

```
第1阶段：适应 Python 思维（2-3周）
  └── 01~02：语法很像C，但更简洁，重点适应"一切皆对象"

第2阶段：Python 工程化（2周）
  └── 03~04：函数/模块/OOP，写出可维护的代码

第3阶段：数据处理基础（3周）
  └── 05~06：文件+NumPy，处理数据的基本功

第4阶段：AI 准备期（持续）
  └── 07~08：可视化+网络请求，为AI项目做工具准备
```

---

## 📝 练习建议（TODO 清单）

### 🌱 基础练习（转型Python必做）
- [ ] TODO-01：用列表推导式生成100以内的所有偶数
- [ ] TODO-02：实现一个函数，判断字符串是否为回文
- [ ] TODO-03：用字典统计一段文本中每个单词出现的次数
- [ ] TODO-04：实现快速排序（比较C和Python写法的差异）
- [ ] TODO-05：读取一个CSV文件，计算某列的平均值

### 🌿 中级练习（AI前置）
- [ ] TODO-06：用 NumPy 实现矩阵乘法，与手动实现比较速度
- [ ] TODO-07：用 Matplotlib 绘制正弦波图像
- [ ] TODO-08：写一个类，封装充电桩的数据模型（模拟你的工作）
- [ ] TODO-09：调用一个公开 API（如天气 API），解析 JSON 响应
- [ ] TODO-10：实现一个简单的命令行计算器（用正则表达式解析输入）

### 🌳 AI准备练习
- [ ] TODO-11：用 NumPy 计算一组数据的均值、方差、标准差
- [ ] TODO-12：用 Matplotlib 绘制数据分布直方图
- [ ] TODO-13：实现线性回归（手动推导公式 + NumPy 版）
- [ ] TODO-14：用 PIL/Pillow 读取图片，转成 NumPy 数组并打印形状

---

## ⚡ 环境配置

```bash
# 安装 Python（推荐 3.10+）
# 安装核心依赖
pip install numpy matplotlib requests pillow jupyter

# 运行脚本
python script.py

# 或使用 Jupyter Notebook（更适合学习）
jupyter notebook
```

---

## 💡 C工程师学Python的注意事项

| C 的写法 | Python 的等价写法 | 注意点 |
|---------|-----------------|-------|
| `int arr[10]` | `arr = [0]*10` | Python 列表动态大小 |
| `printf("%d", x)` | `print(x)` | print 自动换行 |
| `for(i=0;i<10;i++)` | `for i in range(10):` | 注意缩进！ |
| 函数指针 | 函数也是对象，可直接传递 | 更简单 |
| 手动 malloc/free | 自动垃圾回收 | 不用管内存 |
| `struct` | `class` 或 `dataclass` | 更强大 |

> 💡 最大陷阱：Python 用**缩进**表示代码块，必须严格对齐！
