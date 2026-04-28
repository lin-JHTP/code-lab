# code-lab 🧪

> **代码练习场 | C · Python · 算法 · AI**
>
> 这是一个为嵌入式工程师转型"边缘AI复合人才"设计的代码学习仓库。
> 从基础语法到实战项目，体现 **初学 → 进阶 → 实战** 的完整成长曲线。

---

## 🗺️ 仓库结构

```
code-lab/
├── C_Exercises/          # C 语言练习（嵌入式基础）
├── Python_Exercises/     # Python 练习（AI 工具语言）
├── Algorithms/           # 算法与数据结构
└── AI_Edge_Projects/     # 边缘AI项目（你的差异化优势！）
```

---

## 📚 各区块一览

### 🔧 C_Exercises — C 语言从入门到嵌入式实战

| 目录 | 内容 | 状态 |
|------|------|------|
| `01_hello_world/` | 基础语法、编译流程 | ✅ |
| `02_data_types/` | 数据类型、位运算 | ✅ |
| `03_control_flow/` | 条件、循环、状态机 | ✅ |
| `04_functions/` | 函数、递归、函数指针 | ✅ |
| `05_pointers/` | 指针（嵌入式核心！）| ✅ |
| `06_arrays_strings/` | 数组、字符串、帧解析 | ✅ |
| `07_structs/` | 结构体、枚举、位域 | ✅ |
| `08_file_io/` | 文件读写、参数存储 | ✅ |
| `09_dynamic_memory/` | malloc/free、内存池 | ✅ |
| `10_embedded_patterns/` | FSM、循环队列、定时器 | ✅ |

### 🐍 Python_Exercises — Python 从零到 AI 前置

| 目录 | 内容 | 状态 |
|------|------|------|
| `01_basics/` | 变量、列表、字典、推导式 | ✅ |
| `02_data_structures/` | 高级数据结构、队列、堆 | ✅ |
| `03_functions_modules/` | 函数、装饰器、生成器 | ✅ |
| `04_oop/` | 面向对象（充电桩建模）| ✅ |
| `05_file_io/` | 文件、JSON、CSV、日志 | ✅ |
| `06_numpy_intro/` | NumPy 向量化运算（AI必备）| ✅ |
| `07_data_visualization/` | Matplotlib 可视化 | 🔜 |
| `08_web_requests/` | HTTP请求、API调用 | 🔜 |

### 🧮 Algorithms — 算法与数据结构

| 目录 | 内容 | 状态 |
|------|------|------|
| `sorting/` | 快速排序（C+Python）| ✅ |
| `searching/` | 二分搜索、ADC校准表 | ✅ |
| `data_structures/` | 链表、栈、队列 | ✅ |
| `fsm/` | 有限状态机完整框架 | ✅ |
| `dynamic_programming/` | DP经典题模板 | ✅ |
| `graph/` | 图算法（BFS/DFS）| 🔜 |

### 🤖 AI_Edge_Projects — 边缘AI实战项目

| 目录 | 内容 | 状态 |
|------|------|------|
| `01_tinyml_template/` | MCU 神经网络推理框架 | ✅ |
| `02_image_recognition/` | 图像识别（CNN）| 🔜 |
| `03_chatbot_nlp/` | 本地对话机器人 | 🔜 |
| `04_anomaly_detection/` | 充电桩异常检测 ⭐ | ✅ |
| `05_gesture_recognition/` | 手势识别（IMU）| 🔜 |

> ✅ = 已有代码  🔜 = 规划中（有 README 和 TODO）

---

## 🎮 学习进度追踪（像玩 RPG 一样升级！）

```
当前等级：Lv.1 见习开发者
┌─────────────────────────────────────────────────────────┐
│  技能树                                                  │
│  C 语言     ████████░░  80%  [10个练习文件已就绪]         │
│  Python     ██████░░░░  60%  [6个练习文件已就绪]          │
│  算法       █████░░░░░  50%  [5个模板已就绪]              │
│  边缘AI     ███░░░░░░░  30%  [框架已就绪，等你填充！]      │
└─────────────────────────────────────────────────────────┘

下一个里程碑：
  □ 完成 C_Exercises 所有 TODO 练习
  □ 安装 Python 环境，运行 01_basics/basics.py
  □ 运行 04_anomaly_detection/anomaly_detection.py（你的第一个AI项目！）
```

---

## ⚡ 快速开始

### C 语言练习

```bash
# 编译并运行示例
cd C_Exercises/01_hello_world
gcc -Wall -Wextra -std=c99 -o hello hello_world.c
./hello
```

### Python 练习

```bash
# 安装依赖
pip install numpy matplotlib scikit-learn

# 运行基础练习
cd Python_Exercises/01_basics
python basics.py

# 运行 AI 项目（最有趣的！）
cd AI_Edge_Projects/04_anomaly_detection
python anomaly_detection.py
```

---

## 🏆 成就系统

完成以下里程碑将解锁"成就"：

| 成就 | 解锁条件 | 奖励 |
|------|----------|------|
| 🌱 第一步 | 成功运行 hello_world.c | 打好 C 基础 |
| 🔑 解锁 Python | 运行 basics.py | 双语言能力 |
| 🤖 初探 AI | 运行 anomaly_detection.py | AI 入门证书 |
| ⚡ 嵌入式 AI | TFLite 模型跑在 MCU 上 | 边缘 AI 工程师 |
| 💎 满级 | 完成所有 TODO | 全栈嵌入式AI工程师 |

---

## 📅 建议学习计划（每周5小时）

| 周次 | 任务 | 目标 |
|------|------|------|
| 第1-2周 | C_Exercises 01~04 | 回顾/巩固C基础 |
| 第3-4周 | C_Exercises 05~07 | 掌握指针/结构体 |
| 第5-6周 | Python_Exercises 01~03 | Python 入门 |
| 第7-8周 | Python_Exercises 04~06 | OOP + NumPy |
| 第9-10周 | Algorithms 排序/搜索 | 算法基础 |
| 第11-12周 | AI_Edge 04（异常检测）| 第一个AI项目！ |

---

## 🔗 关联仓库

- `tech-notes`：知识库文档（MkDocs 网站）
- `learning-roadmap`：详细学习路线图

---

> 💡 **座右铭**：不积跬步无以至千里。每天写一点代码，一年后回头看，你会感谢今天的自己。
