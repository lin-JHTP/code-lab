# TinyML on MCU - 边缘AI推理模板

## 项目概述

在资源受限的 MCU（如 GD32F450）上部署轻量神经网络，实现电气设备的智能监控。

## 项目结构

```
01_tinyml_template/
├── main.c              # 主程序（推理框架演示）
├── model_data.h        # TODO: 放你的 .tflite 转 C 数组的模型
├── model_data.c        # TODO: 模型数据（通过 xxd 生成）
└── README.md           # 本文件
```

## 快速开始（PC 端模拟）

```bash
# 编译运行（PC端模拟，无需真实MCU）
gcc -Wall -std=c99 -lm -o tinyml_demo main.c
./tinyml_demo
```

## 真实 MCU 部署步骤

### Step 1：Python 端训练模型

```python
# 见 ../../Python_Exercises/ 和 ../04_anomaly_detection/
import tensorflow as tf
# ... 训练代码 ...
model.save("anomaly_detector.h5")
```

### Step 2：转换为 TFLite（含量化）

```python
converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
tflite_model = converter.convert()
with open("model.tflite", "wb") as f:
    f.write(tflite_model)
print(f"模型大小: {len(tflite_model)/1024:.1f} KB")
```

### Step 3：转换为 C 数组

```bash
xxd -i model.tflite > model_data.c
# 或用 Python：
python3 -c "
data = open('model.tflite','rb').read()
print(f'const unsigned char g_model_data[] = {{')
print(', '.join([f'0x{b:02x}' for b in data]))
print(f'}};')
print(f'const int g_model_data_len = {len(data)};')
" > model_data.c
```

### Step 4：集成到 GD32 项目

1. 下载 TensorFlow Lite Micro 源码
2. 将 `tensorflow/` 目录加入项目
3. 编译时链接 CMSIS DSP（用于 FFT、矩阵运算加速）
4. 设置 Linker 脚本，将 tensor arena 放到 SRAM

## 内存预估（GD32F450）

| 组件 | Flash | RAM |
|------|-------|-----|
| TFLM 运行时 | ~30KB | - |
| INT8 量化模型 | ~10-50KB | - |
| Tensor Arena | - | 32KB |
| 其他 | ~100KB | ~20KB |
| **合计** | **~180KB** | **~52KB** |

> GD32F450 有 512KB Flash + 256KB RAM，足够运行中等规模模型

## TODO

- [ ] 实现 FFT 特征提取（接 CMSIS DSP）
- [ ] 添加模型热更新（OTA 更新神经网络）
- [ ] 支持多模型切换（白天/夜间不同模型）
- [ ] 添加推理性能统计（时间、内存用量）
