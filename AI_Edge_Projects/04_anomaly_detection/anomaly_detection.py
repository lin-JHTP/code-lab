"""
充电桩电流异常检测
==================
文件：anomaly_detection.py
作者：你的名字
日期：2026

这是最接近你实际工作的 AI 项目！
用机器学习方法检测充电桩运行中的电流异常。

技术栈：
    - NumPy：数据处理
    - scikit-learn：机器学习模型
    - Matplotlib：数据可视化

安装依赖：
    pip install numpy scikit-learn matplotlib

运行：python anomaly_detection.py

学习价值：
    ★★★★★ 和你工作直接相关，能应用到真实场景
    - 理解"特征工程"：从原始信号中提取有用信息
    - 理解"无监督学习"：不需要标注数据的异常检测
    - 理解"模型评估"：如何判断模型好不好
"""

import numpy as np
import matplotlib.pyplot as plt

# 尝试导入 sklearn，给出友好提示
try:
    from sklearn.ensemble import IsolationForest
    from sklearn.preprocessing import StandardScaler
    from sklearn.model_selection import train_test_split
    from sklearn.metrics import classification_report, confusion_matrix
    SKLEARN_AVAILABLE = True
except ImportError:
    print("提示：scikit-learn 未安装，将跳过 ML 部分")
    print("安装命令：pip install scikit-learn")
    SKLEARN_AVAILABLE = False


# ===== 1. 数据生成（模拟真实充电数据）=====

def generate_normal_charging(n_samples: int = 1000, seed: int = 42) -> np.ndarray:
    """
    生成正常充电数据
    正常情况：16A交流，电流稳定，轻微噪声
    """
    np.random.seed(seed)

    # 每个样本是一段时间窗口内的统计特征
    # 特征：[均值, 标准差, RMS, 峰值, 谷值, 峰峰值]
    data = np.column_stack([
        np.random.normal(16.0, 0.3, n_samples),    # 均值 ~16A
        np.random.normal(0.5,  0.1, n_samples),    # 标准差（小，说明稳定）
        np.random.normal(11.3, 0.2, n_samples),    # RMS ≈ 16/√2
        np.random.normal(22.8, 0.5, n_samples),    # 峰值 ≈ 16√2
        np.random.normal(-22.8, 0.5, n_samples),   # 谷值
        np.random.normal(45.6, 1.0, n_samples),    # 峰峰值 ≈ 2*16√2
    ])
    return data


def generate_anomaly_data(n_samples: int = 100, seed: int = 99) -> np.ndarray:
    """
    生成异常数据（多种故障类型混合）
    故障1：电流过高（33%比例）
    故障2：电流波动（33%比例）
    故障3：枪头接触不良（33%比例）
    """
    np.random.seed(seed)
    n3 = n_samples // 3

    # 故障1：电流过高（过载）
    fault1 = np.column_stack([
        np.random.normal(25.0, 1.0, n3),    # 均值25A（超过额定）
        np.random.normal(0.8,  0.2, n3),
        np.random.normal(17.7, 0.5, n3),
        np.random.normal(35.0, 1.0, n3),
        np.random.normal(-35.0, 1.0, n3),
        np.random.normal(70.0, 2.0, n3),
    ])

    # 故障2：电流剧烈波动
    fault2 = np.column_stack([
        np.random.normal(16.0, 3.0, n3),    # 均值正常但标准差大
        np.random.normal(5.0,  1.0, n3),    # 标准差很大（波动剧烈）
        np.random.normal(11.5, 2.0, n3),
        np.random.normal(28.0, 3.0, n3),
        np.random.normal(-28.0, 3.0, n3),
        np.random.normal(56.0, 6.0, n3),
    ])

    # 故障3：间歇性接触不良（电流时断时续）
    n_rest = n_samples - 2 * n3
    fault3 = np.column_stack([
        np.random.normal(8.0, 5.0, n_rest),   # 均值低，波动大
        np.random.normal(7.0, 2.0, n_rest),   # 标准差极大
        np.random.normal(6.0, 3.0, n_rest),
        np.random.normal(18.0, 5.0, n_rest),
        np.random.normal(-5.0, 5.0, n_rest),
        np.random.normal(23.0, 8.0, n_rest),
    ])

    return np.vstack([fault1, fault2, fault3])


# ===== 2. 特征工程 =====

def extract_features_from_timeseries(timeseries: np.ndarray) -> np.ndarray:
    """
    从时间序列数据中提取特征
    输入：一维时间序列（如1000个采样点的电流波形）
    输出：特征向量（用于机器学习模型）
    """
    mean     = np.mean(timeseries)
    std      = np.std(timeseries)
    rms      = np.sqrt(np.mean(timeseries ** 2))
    peak     = np.max(timeseries)
    valley   = np.min(timeseries)
    peak2peak = peak - valley

    # 可选：频域特征（FFT）
    fft_result  = np.abs(np.fft.rfft(timeseries))
    fft_energy  = np.sum(fft_result ** 2)
    dominant_freq_idx = np.argmax(fft_result[1:]) + 1   # 跳过直流分量

    return np.array([
        mean, std, rms, peak, valley, peak2peak,
        fft_energy, dominant_freq_idx
    ])


# ===== 3. 机器学习模型 =====

def train_anomaly_detector(normal_data: np.ndarray) -> 'IsolationForest':
    """
    训练 Isolation Forest 异常检测器
    
    Isolation Forest 是无监督学习算法，不需要标注"这是正常/异常"。
    它通过随机构建决策树，孤立点（异常）更容易被分离出来。
    
    非常适合：没有大量异常样本标注的工程场景！
    """
    if not SKLEARN_AVAILABLE:
        return None

    # 数据标准化（均值0，方差1）
    scaler = StandardScaler()
    normal_scaled = scaler.fit_transform(normal_data)

    # 训练 Isolation Forest
    # contamination：预期的异常比例（调参！实际场景一般设 0.01~0.1）
    model = IsolationForest(
        n_estimators=100,      # 树的数量
        contamination=0.05,    # 预期5%的数据是异常
        random_state=42,
        n_jobs=-1              # 使用所有CPU核心
    )
    model.fit(normal_scaled)

    return model, scaler


def evaluate_model(model, scaler, normal_data, anomaly_data):
    """评估模型效果"""
    if not SKLEARN_AVAILABLE or model is None:
        return

    # 预测（-1=异常，1=正常）
    normal_scaled  = scaler.transform(normal_data)
    anomaly_scaled = scaler.transform(anomaly_data)

    normal_pred  = model.predict(normal_scaled)
    anomaly_pred = model.predict(anomaly_scaled)

    # 统计
    normal_tp  = np.sum(normal_pred == 1)     # 正常判为正常（True Negative）
    normal_fp  = np.sum(normal_pred == -1)    # 正常误判为异常（False Positive）
    anomaly_tp = np.sum(anomaly_pred == -1)   # 异常判为异常（True Positive）
    anomaly_fn = np.sum(anomaly_pred == 1)    # 异常漏检（False Negative）

    print("\n===== 模型评估结果 =====")
    print(f"正常样本：{len(normal_data)} 个")
    print(f"  正确判断（无虚警）: {normal_tp} ({normal_tp/len(normal_data)*100:.1f}%)")
    print(f"  误报（虚警）:       {normal_fp} ({normal_fp/len(normal_data)*100:.1f}%)")
    print(f"\n异常样本：{len(anomaly_data)} 个")
    print(f"  正确检出（检出率）: {anomaly_tp} ({anomaly_tp/len(anomaly_data)*100:.1f}%)")
    print(f"  漏检（漏报率）:     {anomaly_fn} ({anomaly_fn/len(anomaly_data)*100:.1f}%)")

    precision = anomaly_tp / (anomaly_tp + normal_fp + 1e-9)
    recall    = anomaly_tp / (anomaly_tp + anomaly_fn + 1e-9)
    f1 = 2 * precision * recall / (precision + recall + 1e-9)
    print(f"\n精确率 (Precision): {precision:.3f}")
    print(f"召回率 (Recall):    {recall:.3f}")
    print(f"F1 Score:           {f1:.3f}")


# ===== 4. 可视化 =====

def visualize_data(normal_data: np.ndarray, anomaly_data: np.ndarray):
    """可视化正常与异常数据的分布"""
    feature_names = ["均值", "标准差", "RMS", "峰值", "谷值", "峰峰值"]

    plt.figure(figsize=(15, 8))
    plt.suptitle("充电桩电流特征分布：正常 vs 异常", fontsize=14)

    for i in range(min(6, normal_data.shape[1])):
        plt.subplot(2, 3, i + 1)
        plt.hist(normal_data[:, i],  bins=50, alpha=0.7,
                 label="正常", color="green", density=True)
        plt.hist(anomaly_data[:, i], bins=20, alpha=0.7,
                 label="异常", color="red",   density=True)
        plt.title(feature_names[i])
        plt.legend(fontsize=8)
        plt.grid(True, alpha=0.3)

    plt.tight_layout()

    # 保存图片（而不是弹出窗口）
    output_path = "/tmp/anomaly_detection_visualization.png"
    plt.savefig(output_path, dpi=100, bbox_inches='tight')
    print(f"\n📊 可视化图片已保存到: {output_path}")
    plt.close()


# ===== 5. 实时检测模拟 =====

def simulate_realtime_detection(model, scaler):
    """模拟实时检测场景"""
    if not SKLEARN_AVAILABLE or model is None:
        print("\n[模拟] 无法运行（sklearn 未安装）")
        return

    print("\n===== 模拟实时异常检测 =====")
    print("（模拟一段时间内的充电数据，包含1个异常事件）\n")

    # 模拟30个时间窗口（每个窗口1秒）
    normal_windows   = generate_normal_charging(28, seed=1)
    anomaly_windows  = generate_anomaly_data(2, seed=2)

    import random
    all_windows = list(normal_windows)
    # 在第15秒和第20秒插入异常
    all_windows.insert(15, anomaly_windows[0])
    all_windows.insert(20, anomaly_windows[1])
    all_windows = np.array(all_windows)

    labels = ["正常"] * len(all_windows)
    labels[15] = "⚠️ 异常"
    labels[20] = "⚠️ 异常"

    for i, (window, label) in enumerate(zip(all_windows, labels)):
        window_scaled = scaler.transform(window.reshape(1, -1))
        pred = model.predict(window_scaled)[0]
        status = "🔴 告警！" if pred == -1 else "🟢 正常"
        print(f"  t={i:2d}s  真实: {label:8s}  检测: {status}")


# ===== main =====

if __name__ == "__main__":
    print("="*50)
    print("充电桩电流异常检测系统")
    print("="*50)

    # 1. 生成数据
    print("\n[1] 生成模拟数据...")
    normal_data  = generate_normal_charging(n_samples=1000)
    anomaly_data = generate_anomaly_data(n_samples=100)
    print(f"    正常数据: {normal_data.shape}")
    print(f"    异常数据: {anomaly_data.shape}")

    # 2. 可视化
    print("\n[2] 生成数据分布可视化...")
    visualize_data(normal_data, anomaly_data)

    # 3. 训练模型
    if SKLEARN_AVAILABLE:
        print("\n[3] 训练 Isolation Forest 模型...")
        model, scaler = train_anomaly_detector(normal_data)
        print("    模型训练完成！")

        # 4. 评估
        evaluate_model(model, scaler, normal_data, anomaly_data)

        # 5. 实时检测模拟
        simulate_realtime_detection(model, scaler)

    print("\n✅ 完成！")

"""
TODO 练习：

TODO-01: 用真实充电桩数据替换模拟数据
    从你的工作中（脱敏后）获取真实的电压/电流时间序列数据

TODO-02: 添加更多特征
    - 过零率（Zero Crossing Rate）
    - 峭度（Kurtosis）：对突发性脉冲故障非常敏感
    - 偏度（Skewness）：信号不对称性
    - FFT 前10个频谱分量

TODO-03: 尝试其他异常检测算法，比较效果
    - One-Class SVM
    - AutoEncoder（神经网络版）
    - LOF（Local Outlier Factor）

TODO-04: 将模型部署为 REST API
    用 Flask 包装，接受 JSON 输入，返回异常判断
    curl -X POST http://localhost:5000/detect -d '{"features": [...]}'

TODO-05: 将训练好的模型导出为 TFLite 并部署到 GD32（完整闭环！）
    见 01_tinyml_template/
"""
