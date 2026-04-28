# 手势识别项目（加速度传感器）⌨️

## 项目概述

用 IMU（惯性测量单元，如 MPU6050）的加速度和陀螺仪数据，
识别手势动作（上下左右、旋转等）。

**为什么选这个项目？**
- MCU + 传感器是你的强项！
- 数据量小，适合入门 TinyML
- 有完整的 Edge Impulse 免费工具链支持

## 技术路线

```
MPU6050 → GD32/STM32 (I²C) → 数据采集
    ↓
PC 端 Python → 特征提取 → 模型训练（sklearn/TFLite）
    ↓
模型部署回 MCU → 实时手势识别
```

## 文件清单

| 文件 | 说明 | 状态 |
|------|------|------|
| `data_collector.c` | MCU 端数据采集（GD32/STM32）| TODO：待创建 |
| `feature_extraction.py` | 特征提取（Python）| TODO：待创建 |
| `train_classifier.py` | 训练手势分类器 | TODO：待创建 |
| `deploy_to_mcu.c` | MCU 端推理代码 | TODO：待创建 |

## 快速上手（仅用 PC 模拟）

```python
# 无需硬件，用合成数据练手
python feature_extraction.py
python train_classifier.py
```

## TODO 练习

- [ ] TODO-01：了解 MPU6050 的 I²C 通信协议，在 GD32 上读取原始数据
- [ ] TODO-02：采集5种手势各50组数据，存为 CSV 文件
- [ ] TODO-03：用 Python 提取时域/频域特征（见 anomaly_detection 的方法）
- [ ] TODO-04：训练 Random Forest 分类器，目标准确率 >90%
- [ ] TODO-05：用 Edge Impulse 平台完成完整的 TinyML 流程（免费！）
  访问：https://edgeimpulse.com/
- [ ] TODO-06：将训练好的模型部署到 GD32，实现实时手势识别
- [ ] TODO-07：扩展应用：用手势控制充电桩（挥手启动/停止充电）

## 推荐工具

- **Edge Impulse**：最友好的嵌入式AI开发平台（免费，支持大多数MCU）
- **Arduino/Platform IO**：快速验证硬件
- **Serial Plot**：实时绘制串口数据波形
