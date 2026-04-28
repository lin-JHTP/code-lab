"""
Python 文件读写与数据格式处理
================================
文件：file_io.py
作者：你的名字
日期：2026

本文件演示：
    1. 文本文件读写
    2. CSV 文件处理（充电记录日志）
    3. JSON 文件（配置文件、API数据）
    4. 上下文管理器（with 语句）
    5. 路径操作（pathlib）
    6. 日志记录（logging 模块）

运行：python file_io.py
"""

import json
import csv
import logging
import os
from pathlib import Path
from datetime import datetime

# ========== 配置日志 ==========
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s',
    datefmt='%H:%M:%S'
)
logger = logging.getLogger(__name__)

# ========== 1. 文本文件 ==========
print("===== 1. 文本文件读写 =====")

# 使用 with 语句（上下文管理器）自动关闭文件，即使出错也安全
test_file = "/tmp/test_charger_log.txt"

# 写入
with open(test_file, "w", encoding="utf-8") as f:
    f.write("充电桩日志系统 v1.0\n")
    f.write(f"生成时间: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
    for i in range(5):
        f.write(f"[{datetime.now().strftime('%H:%M:%S')}] 枪{i+1}: 正常运行\n")

print(f"写入文件: {test_file}")

# 读取所有内容
with open(test_file, "r", encoding="utf-8") as f:
    content = f.read()
print(f"文件内容:\n{content}")

# 逐行读取（适合大文件）
with open(test_file, "r", encoding="utf-8") as f:
    for i, line in enumerate(f, start=1):
        print(f"  第{i}行: {line.rstrip()}")

# ========== 2. CSV 文件（充电记录）==========
print("\n===== 2. CSV 文件处理 =====")

csv_file = "/tmp/charging_records.csv"

# 生成模拟充电记录
records = [
    ["session_id", "gun_id", "start_time",           "duration_min", "energy_kwh", "cost_yuan"],
    ["S001",       1,        "2026-04-28 08:00:00",  120,            7.92,          9.50],
    ["S002",       2,        "2026-04-28 09:30:00",  45,             2.48,          2.97],
    ["S003",       1,        "2026-04-28 11:00:00",  90,             4.95,          5.94],
    ["S004",       3,        "2026-04-28 14:00:00",  60,             3.30,          3.96],
    ["S005",       2,        "2026-04-28 15:30:00",  180,            9.90,          11.88],
]

# 写入 CSV
with open(csv_file, "w", newline="", encoding="utf-8") as f:
    writer = csv.writer(f)
    writer.writerows(records)
print(f"CSV 已写入: {csv_file}")

# 读取并分析 CSV
total_energy = 0.0
total_cost = 0.0
with open(csv_file, "r", encoding="utf-8") as f:
    reader = csv.DictReader(f)   # DictReader 自动用第一行作为键
    print("充电记录分析:")
    for row in reader:
        energy = float(row["energy_kwh"])
        cost   = float(row["cost_yuan"])
        total_energy += energy
        total_cost   += cost
        print(f"  {row['session_id']}: 枪{row['gun_id']}, "
              f"{row['duration_min']}分钟, {energy}度, {cost}元")

print(f"今日合计: 总电量={total_energy:.2f}度, 总收入={total_cost:.2f}元")

# ========== 3. JSON 文件（配置文件）==========
print("\n===== 3. JSON 文件处理 =====")

config_file = "/tmp/charger_config.json"

# 充电站配置（Python 字典）
config = {
    "station": {
        "id":       "CS001",
        "name":     "停车场A区充电站",
        "location": {"lat": 30.5728, "lng": 104.0668},
        "operator": "某充电运营商"
    },
    "guns": [
        {
            "id":          1,
            "type":        "AC",
            "rated_kw":    3.3,
            "connector":   "Type2",
            "is_enabled":  True
        },
        {
            "id":          2,
            "type":        "AC",
            "rated_kw":    7.0,
            "connector":   "Type2",
            "is_enabled":  True
        }
    ],
    "pricing": {
        "peak_yuan_kwh":    1.5,
        "valley_yuan_kwh":  0.5,
        "flat_yuan_kwh":    1.2
    },
    "updated_at": datetime.now().isoformat()
}

# 写入 JSON
with open(config_file, "w", encoding="utf-8") as f:
    json.dump(config, f, ensure_ascii=False, indent=2)
    # ensure_ascii=False：支持中文
    # indent=2：格式化缩进，易读
print(f"JSON 已写入: {config_file}")

# 读取 JSON
with open(config_file, "r", encoding="utf-8") as f:
    loaded_config = json.load(f)

print(f"读取站点名称: {loaded_config['station']['name']}")
print(f"枪数量: {len(loaded_config['guns'])}")
print(f"峰值电价: {loaded_config['pricing']['peak_yuan_kwh']} 元/度")

# JSON 字符串转换（API 响应常用）
json_str = json.dumps({"status": "OK", "code": 200}, ensure_ascii=False)
print(f"JSON字符串: {json_str}")
parsed = json.loads(json_str)
print(f"解析后状态: {parsed['status']}")

# ========== 4. pathlib（现代路径操作）==========
print("\n===== 4. pathlib 路径操作 =====")

p = Path("/tmp")
print(f"路径: {p}")
print(f"是否存在: {p.exists()}")
print(f"是否是目录: {p.is_dir()}")

# 列出 /tmp 下的文件
tmp_files = list(p.glob("*.txt"))[:5]   # 最多显示5个
print(f"/tmp 下的 txt 文件（前5个）: {[f.name for f in tmp_files]}")

# 构建路径（跨平台！）
project_dir = Path("/tmp") / "my_project" / "data"
project_dir.mkdir(parents=True, exist_ok=True)   # 创建多级目录
print(f"创建目录: {project_dir}")

# ========== 5. logging 模块（生产级日志）==========
print("\n===== 5. 日志记录 =====")

# 在真实项目中，用 logging 而不是 print！
logger.info("充电站系统启动")
logger.warning("枪3 电压异常: 225V（超过告警阈值）")
logger.error("枪1 过温保护触发: 85°C")
logger.debug("调试信息（默认不显示，需设置 level=DEBUG）")

if __name__ == "__main__":
    print("\n✅ file_io.py 运行完成！")
    print(f"生成的文件在 /tmp/ 目录下")

"""
TODO 练习：

TODO-01: 实现一个充电记录查询器
    读取 CSV 文件，按日期、枪号、金额范围筛选记录

TODO-02: 实现一个配置文件管理器（Config Manager）
    - 读取/写入 JSON 配置
    - 支持 get(key, default)、set(key, value)、save() 方法
    - 配置变更时自动写入文件

TODO-03: 实现一个日志文件分析器
    解析形如 "[2026-04-28 10:00:00] [ERROR] 枪1 故障..." 的日志
    统计每种日志级别的数量，提取所有错误信息

TODO-04: 实现文件监控器
    定时检查文件是否被修改（用 os.path.getmtime 和 time.sleep）
    文件变更时重新加载配置（模拟热更新）

TODO-05: 读取二进制文件（struct 模块）
    将充电记录结构体序列化为二进制文件（类似嵌入式EEPROM存储）
    struct.pack("IHff", session_id, gun_id, energy, cost)
"""
