/**
 * @file    main.c
 * @brief   TinyML on MCU - 边缘AI推理空白工程模板
 * @author  你的名字
 * @date    2026
 *
 * ============================================================
 * 项目概述：在 MCU（如 GD32/STM32）上运行神经网络推理
 * ============================================================
 *
 * 本模板演示如何在资源受限的MCU上运行轻量神经网络：
 *   - TensorFlow Lite Micro (TFLM) 框架
 *   - 量化模型（INT8）减少内存占用
 *   - 适合 GD32F450（512KB Flash, 256KB RAM）
 *
 * 典型应用场景：
 *   - 充电枪接口类型自动识别
 *   - 电流波形异常检测
 *   - 继电器动作声音分类
 *   - 加速度传感器手势识别
 *
 * 运行流程：
 *   传感器采集 → 数据预处理 → 模型推理 → 后处理 → 执行动作
 *
 * 依赖：
 *   - TensorFlow Lite Micro 源码（需另外下载）
 *   - GD32F450 HAL 库（或 STM32 HAL）
 *
 * 注意：
 *   本文件仅为框架模板，在PC上可以编译但无法真正运行推理。
 *   在真实MCU项目中，需要链接 TFLM 库。
 *
 * TODO 学习路线：
 *   1. 先在 PC 上用 Python/PyTorch 训练模型
 *   2. 导出为 TFLite 格式并量化为 INT8
 *   3. 将模型转为 C 数组（xxd 工具或 tf.lite 工具）
 *   4. 把本模板和 TFLM 库一起编译到 MCU 项目
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

/* ========== 模拟 TFLite Micro API（实际项目需包含 TFLM 头文件）========== */
/*
 * 真实项目中，应该包含：
 * #include "tensorflow/lite/micro/all_ops_resolver.h"
 * #include "tensorflow/lite/micro/micro_interpreter.h"
 * #include "tensorflow/lite/schema/schema_generated.h"
 * #include "model_data.h"   // 转换为 C 数组的模型文件
 */

/* ========== 模型元数据（占位符）========== */
#define MODEL_INPUT_SIZE    128    /* 输入特征数量 */
#define MODEL_OUTPUT_SIZE   4      /* 输出类别数量（例：4种故障类型）*/
#define TENSOR_ARENA_SIZE   (32 * 1024)  /* 推理用内存池：32KB */

/* 类别标签（根据实际模型定义）*/
static const char *class_labels[] = {
    "正常运行",      /* class 0 */
    "电流过高",      /* class 1 */
    "电压异常",      /* class 2 */
    "枪头松动",      /* class 3 */
};

/* 推理内存池（必须静态分配！嵌入式不能 malloc）*/
static uint8_t tensor_arena[TENSOR_ARENA_SIZE];

/* ========== 数据预处理 ========== */

/**
 * @brief 计算信号的均值
 */
static float calc_mean(const float *data, int n)
{
    float sum = 0.0f;
    for (int i = 0; i < n; i++) sum += data[i];
    return sum / n;
}

/**
 * @brief 计算信号的标准差
 */
static float calc_std(const float *data, int n)
{
    float mean = calc_mean(data, n);
    float var  = 0.0f;
    for (int i = 0; i < n; i++) {
        float diff = data[i] - mean;
        var += diff * diff;
    }
    return sqrtf(var / n);
}

/**
 * @brief 计算均方根值（RMS，交流电流有效值）
 */
static float calc_rms(const float *data, int n)
{
    float sum_sq = 0.0f;
    for (int i = 0; i < n; i++) sum_sq += data[i] * data[i];
    return sqrtf(sum_sq / n);
}

/**
 * @brief Z-Score 归一化（将数据标准化为均值0，方差1）
 * @note  归一化是神经网络推理前的必要步骤！
 */
static void normalize_zscore(float *data, int n)
{
    float mean = calc_mean(data, n);
    float std  = calc_std(data, n);

    if (std < 1e-6f) return;   /* 避免除零 */

    for (int i = 0; i < n; i++) {
        data[i] = (data[i] - mean) / std;
    }
}

/**
 * @brief 提取时域特征（用于简单分类器）
 * @param raw_data   原始采样数据（如电流波形）
 * @param n          采样点数量
 * @param features   输出特征数组
 * @param feat_size  特征数组大小
 */
static void extract_features(const float *raw_data, int n,
                               float *features, int feat_size)
{
    /* 时域特征（示例6个特征）*/
    if (feat_size < 6) return;

    features[0] = calc_mean(raw_data, n);    /* 均值 */
    features[1] = calc_std(raw_data, n);     /* 标准差 */
    features[2] = calc_rms(raw_data, n);     /* 均方根 */

    /* 最大值和最小值 */
    float min_v = raw_data[0], max_v = raw_data[0];
    for (int i = 1; i < n; i++) {
        if (raw_data[i] < min_v) min_v = raw_data[i];
        if (raw_data[i] > max_v) max_v = raw_data[i];
    }
    features[3] = max_v;                     /* 峰值 */
    features[4] = min_v;                     /* 谷值 */
    features[5] = max_v - min_v;             /* 峰峰值 */

    /* TODO: 可扩展更多特征
     * - 频域特征（FFT 后的主频、能量分布）
     * - 过零率（ZCR）
     * - 峭度（Kurtosis）- 对突发性故障敏感
     */
}

/* ========== 推理引擎接口（框架层）========== */

typedef struct {
    /* TODO: 在真实项目中替换为 TFLM 的解释器对象 */
    void    *interpreter;    /* tflite::MicroInterpreter* */
    int      input_size;
    int      output_size;
    uint8_t  initialized;
} TfLiteEngine_t;

static TfLiteEngine_t g_engine = {0};

/**
 * @brief 初始化推理引擎
 * @return 0 成功，-1 失败
 *
 * 真实代码示例（伪代码）：
 *   static tflite::AllOpsResolver resolver;
 *   static tflite::MicroInterpreter interpreter(
 *       model, resolver, tensor_arena, TENSOR_ARENA_SIZE);
 *   interpreter.AllocateTensors();
 *   g_input  = interpreter.input(0);
 *   g_output = interpreter.output(0);
 */
int tflite_engine_init(void)
{
    printf("[TFLite] 初始化推理引擎...\n");

    /* TODO: 加载模型，分配 tensor arena */
    /*
     * const tflite::Model* model = tflite::GetModel(g_model_data);
     * if (model->version() != TFLITE_SCHEMA_VERSION) { return -1; }
     */

    g_engine.input_size  = MODEL_INPUT_SIZE;
    g_engine.output_size = MODEL_OUTPUT_SIZE;
    g_engine.initialized = 1;

    printf("[TFLite] 初始化完成\n");
    printf("  Tensor Arena: %u KB\n", TENSOR_ARENA_SIZE / 1024);
    printf("  输入大小: %d\n", g_engine.input_size);
    printf("  输出类别: %d\n", g_engine.output_size);

    return 0;
}

/**
 * @brief 执行推理
 * @param input_data 输入数据（已归一化）
 * @param output     输出概率数组（长度 = MODEL_OUTPUT_SIZE）
 * @return 0 成功，-1 失败
 *
 * 真实代码（伪代码）：
 *   memcpy(interpreter.input(0)->data.f, input_data,
 *          MODEL_INPUT_SIZE * sizeof(float));
 *   interpreter.Invoke();
 *   memcpy(output, interpreter.output(0)->data.f,
 *          MODEL_OUTPUT_SIZE * sizeof(float));
 */
int tflite_engine_run(const float *input_data, float *output)
{
    if (!g_engine.initialized) return -1;

    /* TODO: 在真实项目中替换为实际推理调用 */

    /* 模拟推理输出（用于演示）*/
    output[0] = 0.85f;   /* 正常 */
    output[1] = 0.10f;   /* 电流过高 */
    output[2] = 0.03f;   /* 电压异常 */
    output[3] = 0.02f;   /* 枪头松动 */

    return 0;
}

/**
 * @brief softmax（将 logits 转换为概率）
 * @note  如果模型输出已经是概率（经过 softmax），则不需要此函数
 */
static void softmax(float *x, int n)
{
    float max_val = x[0];
    for (int i = 1; i < n; i++) {
        if (x[i] > max_val) max_val = x[i];
    }

    float sum = 0.0f;
    for (int i = 0; i < n; i++) {
        x[i] = expf(x[i] - max_val);   /* 减去最大值防止溢出 */
        sum += x[i];
    }
    for (int i = 0; i < n; i++) {
        x[i] /= sum;
    }
}

/* ========== 主程序（嵌入式 main 循环框架）========== */

/**
 * @brief 模拟传感器数据采集（真实项目中替换为 ADC 读取）
 */
static void acquire_sensor_data(float *buf, int n)
{
    /* 模拟 16A 交流电流波形（50Hz，采样率 1kHz）*/
    for (int i = 0; i < n; i++) {
        float t = (float)i / 1000.0f;                   /* 时间（秒）*/
        buf[i] = 16.0f * sinf(2.0f * 3.14159f * 50.0f * t);  /* 正弦波 */
        /* 真实项目：buf[i] = (float)adc_read() * VREF / 4096.0f; */
    }
}

int main(void)
{
    printf("============================================\n");
    printf("  TinyML on MCU - 边缘AI推理框架演示\n");
    printf("============================================\n\n");

    /* 1. 初始化硬件（真实项目中需要）*/
    /* hal_init(); */
    /* uart_init(115200); */
    /* adc_init(); */

    /* 2. 初始化推理引擎 */
    if (tflite_engine_init() != 0) {
        printf("推理引擎初始化失败！\n");
        return -1;
    }

    /* 3. 采集数据 */
    float raw_data[128];
    acquire_sensor_data(raw_data, 128);

    printf("\n[采样] 电流 RMS: %.2f A\n", calc_rms(raw_data, 128));
    printf("[采样] 峰值:     %.2f A\n", raw_data[0]);   /* 近似 */

    /* 4. 特征提取 */
    float features[6];
    extract_features(raw_data, 128, features, 6);
    printf("\n[特征] 均值=%.3f, 标准差=%.3f, RMS=%.3f\n",
           features[0], features[1], features[2]);
    printf("[特征] 峰值=%.3f, 谷值=%.3f, 峰峰值=%.3f\n",
           features[3], features[4], features[5]);

    /* 5. 数据归一化 */
    normalize_zscore(features, 6);

    /* 6. 执行推理 */
    float output[MODEL_OUTPUT_SIZE];
    if (tflite_engine_run(features, output) != 0) {
        printf("推理失败！\n");
        return -1;
    }

    /* 7. 后处理：找最大概率类别 */
    int   predicted_class = 0;
    float max_prob        = output[0];

    printf("\n[推理] 各类别概率:\n");
    for (int i = 0; i < MODEL_OUTPUT_SIZE; i++) {
        printf("  [%d] %-10s: %.1f%%\n",
               i, class_labels[i], output[i] * 100.0f);
        if (output[i] > max_prob) {
            max_prob = output[i];
            predicted_class = i;
        }
    }

    printf("\n[结果] 预测: %s (置信度: %.1f%%)\n",
           class_labels[predicted_class], max_prob * 100.0f);

    /* 8. 执行决策动作 */
    if (predicted_class != 0) {
        printf("[告警] 检测到异常！类型: %s\n",
               class_labels[predicted_class]);
        /* 真实项目：trigger_alarm(predicted_class); */
        /* 真实项目：report_to_cloud(predicted_class, max_prob); */
    } else {
        printf("[状态] 运行正常 ✓\n");
    }

    return 0;
}

/*
 * TODO 工程实践清单：
 *
 * TODO-01: 在 PC 端用 Python + scikit-learn 训练一个电流异常检测模型
 *   - 生成正常和异常的电流波形数据
 *   - 训练 Random Forest 或 SVM 分类器
 *   - 评估准确率（目标：>95%）
 *
 * TODO-02: 将模型转换为 TFLite 格式
 *   - 使用 PyTorch → ONNX → TFLite 转换链
 *   - 或直接用 TensorFlow/Keras 训练后导出
 *
 * TODO-03: INT8 量化模型
 *   converter.optimizations = [tf.lite.Optimize.DEFAULT]
 *   减小模型体积4倍，推理速度提升2-4倍！
 *
 * TODO-04: 将 .tflite 文件转为 C 数组
 *   xxd -i model.tflite > model_data.c
 *   (Linux/Mac) 或用 Python: open("model.tflite","rb").read()
 *
 * TODO-05: 在 GD32 上集成 TFLM
 *   - 下载 TFLite Micro 源码
 *   - 配置 Makefile/CMakeLists.txt
 *   - 确保 tensor arena 放在 SRAM（不是 Flash）
 *
 * TODO-06: 实现 FFT 频谱特征提取（用于电机振动分析）
 *   使用 CMSIS DSP 库的 arm_rfft_fast_f32()
 *
 * TODO-07: 添加模型版本管理
 *   Flash 中保存模型版本号，支持 OTA 更新模型
 *
 * TODO-08: 实现推理性能监测
 *   记录每次推理的时间（us）、内存使用，便于优化
 */
