# AI 边缘项目区 🤖

> **你的核心差异化优势：嵌入式 + AI = 边缘智能工程师**
>
> 这是 2026 年最热门的复合型人才方向之一。
> 你已有的嵌入式硬件背景，是别人无法快速复制的壁垒！

---

## 📚 内容索引

| 编号 | 目录 | 项目内容 | 所需技术 | 难度 |
|------|------|----------|----------|------|
| 01 | `01_tinyml_template/` | MCU 上运行神经网络的空白工程 | C/TensorFlow Lite Micro | ⭐⭐⭐⭐ |
| 02 | `02_image_recognition/` | 图像识别项目（PC端学习）| Python/PyTorch | ⭐⭐⭐ |
| 03 | `03_chatbot_nlp/` | 本地智能对话机器人 | Python/LangChain | ⭐⭐⭐ |
| 04 | `04_anomaly_detection/` | 充电桩异常检测（最接近工作！）| Python/sklearn | ⭐⭐⭐ |
| 05 | `05_gesture_recognition/` | 手势识别（加速度传感器）| C/Python | ⭐⭐⭐⭐ |

---

## 🎯 你的专属学习路线（嵌入式工程师转边缘AI）

```
Phase 0：AI 基础知识（2个月）
  ├── Python 基础（见 Python_Exercises/）
  ├── NumPy / Matplotlib
  └── 机器学习概念（监督/无监督/强化学习）

Phase 1：ML 入门（1个月）
  ├── 线性回归、逻辑回归（手动实现）
  ├── sklearn 工具库
  └── 项目：充电桩电流异常检测 ← 从你最熟悉的领域入手！

Phase 2：深度学习（2个月）
  ├── PyTorch 基础（张量、自动微分）
  ├── CNN 图像分类
  └── 项目：枪头识别（图像识别）

Phase 3：边缘AI部署（2个月）
  ├── 模型量化与压缩（INT8、知识蒸馏）
  ├── TensorFlow Lite / ONNX
  └── 项目：MCU 上运行 TinyML 模型！

Phase 4：综合实战（持续）
  └── 充电桩+AI：预测性维护、异常检测、用户行为分析
```

---

## 📝 项目 TODO 清单（10+ 建议）

### 🌱 入门项目（可以现在开始！）
- [ ] TODO-01：用 sklearn 对充电桩电流数据做异常检测（Isolation Forest）
- [ ] TODO-02：用线性回归预测用户充电时长
- [ ] TODO-03：用 K-Means 对充电行为分组（早晚高峰分析）
- [ ] TODO-04：用 Python 绘制充电桩一天的负荷曲线

### 🌿 中级项目（3-6个月后）
- [ ] TODO-05：训练一个简单 CNN 识别充电枪接口类型（Type1/Type2/CCS）
- [ ] TODO-06：用 LSTM 预测明天的充电需求峰值
- [ ] TODO-07：本地运行 LLM（如 Ollama + llama3），做充电运维助手
- [ ] TODO-08：用麦克风+FFT+ML 实现继电器故障声音检测

### 🌳 进阶/实战项目（6-12个月后）
- [ ] TODO-09：将训练好的模型用 TFLite Micro 部署到 GD32
- [ ] TODO-10：实现一个"会学习"的充电调度算法（强化学习入门）
- [ ] TODO-11：用摄像头+YOLO实现停车位占用检测
- [ ] TODO-12：Edge AI 完整 Pipeline：数据采集 → 标注 → 训练 → 部署 → 监控

---

## 💡 边缘AI关键技术栈

| 层次 | 技术 | 说明 |
|------|------|------|
| 硬件 | GD32 / STM32 / ESP32-S3 | 你的本行 |
| 运行时 | TFLite Micro / CMSIS-NN | MCU上的AI推理 |
| 模型格式 | .tflite / .onnx | 标准格式 |
| 量化工具 | TFLite Converter / onnxruntime | 模型压缩 |
| 训练框架 | PyTorch / TensorFlow | PC端训练 |
| 数据处理 | NumPy / Pandas | 数据科学基础 |

---

## 🔗 推荐资源

- TensorFlow Lite Micro 官方文档
- Edge Impulse（嵌入式AI开发平台，免费，强烈推荐！）
  https://edgeimpulse.com/
- 《TinyML》书籍 —— Pete Warden 著
- STM32Cube.AI（ST官方AI部署工具）
