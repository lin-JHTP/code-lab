# 图像识别入门项目 📷

## 项目目标

学习用 Python + PyTorch/TensorFlow 实现图像分类，
为将来的"充电枪接口类型识别"等实际应用打基础。

## 学习路线

```
阶段1：理解卷积神经网络（CNN）原理
  └── 什么是卷积？池化？激活函数？

阶段2：用 MNIST 手写数字识别入门（经典任务）
  └── 训练一个 >99% 准确率的模型

阶段3：迁移学习（Transfer Learning）
  └── 用预训练的 ResNet/MobileNet 做充电枪识别

阶段4：模型压缩部署
  └── INT8 量化 → 部署到边缘设备
```

## 快速开始

```bash
pip install torch torchvision matplotlib pillow
python mnist_cnn.py
```

## 文件清单

| 文件 | 说明 | 状态 |
|------|------|------|
| `mnist_cnn.py` | MNIST手写数字识别（入门）| TODO：待创建 |
| `transfer_learning.py` | 迁移学习（充电枪分类）| TODO：待创建 |
| `data_augmentation.py` | 数据增强技巧 | TODO：待创建 |
| `inference_demo.py` | 推理演示 | TODO：待创建 |

## TODO 练习

- [ ] TODO-01：理解 CNN 结构图（卷积层→池化层→全连接层）
- [ ] TODO-02：用 MNIST 训练第一个 CNN 模型（目标：>98%准确率）
- [ ] TODO-03：可视化卷积核（理解 CNN 学到了什么）
- [ ] TODO-04：用 Grad-CAM 可视化模型关注区域（热力图）
- [ ] TODO-05：收集10张充电枪接口照片，用迁移学习训练分类器
- [ ] TODO-06：将模型量化为 INT8，对比精度损失
- [ ] TODO-07：用 ONNX Runtime 在 Python 中做推理
- [ ] TODO-08：用 OpenCV 读取摄像头实时推理

## 充电桩相关应用构想

1. **充电枪接口类型识别**：拍照判断 Type1/Type2/CCS/CHAdeMO
2. **停车位占用检测**：判断充电车位是否有车
3. **枪头异物检测**：检测充电口是否被堵塞
4. **车辆类型识别**：普通轿车/SUV/卡车（用于功率推荐）
