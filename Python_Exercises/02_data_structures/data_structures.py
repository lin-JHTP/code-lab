"""
Python 数据结构练习（针对AI学习）
===================================
文件：data_structures.py
作者：你的名字
日期：2026

本文件演示 Python 内置数据结构的高级用法：
    1. 列表进阶（切片、排序、双端队列）
    2. 字典进阶（defaultdict、OrderedDict、Counter）
    3. 集合操作（并集、交集、差集）
    4. 元组和命名元组
    5. 栈和队列（用列表和 deque 实现）
    6. 堆（heapq）- 优先队列

运行：python data_structures.py
"""

from collections import defaultdict, OrderedDict, Counter, deque
from typing import List, Tuple
import heapq

# ========== 1. 列表进阶 ==========
print("===== 1. 列表进阶 =====")

# 切片技巧
nums = list(range(1, 11))   # [1, 2, 3, ..., 10]
print(f"原列表: {nums}")
print(f"前3个: {nums[:3]}")
print(f"后3个: {nums[-3:]}")
print(f"奇数位: {nums[::2]}")       # 步长2
print(f"逆序: {nums[::-1]}")        # 步长-1

# 列表排序
students = [("张三", 85), ("李四", 92), ("王五", 78), ("赵六", 92)]
by_score = sorted(students, key=lambda x: x[1], reverse=True)
print(f"\n按成绩降序: {by_score}")

# 用 zip 合并
names = ["Alice", "Bob", "Charlie"]
scores = [90, 85, 88]
paired = list(zip(names, scores))
print(f"zip合并: {paired}")

# ========== 2. 字典进阶 ==========
print("\n===== 2. 字典进阶 =====")

# defaultdict：访问不存在的键时自动创建默认值
# 场景：统计充电桩各故障码出现次数
fault_log = ["E001", "E002", "E001", "E003", "E001", "E002"]
fault_count = defaultdict(int)   # 默认值为 0
for code in fault_log:
    fault_count[code] += 1
print(f"故障统计: {dict(fault_count)}")

# Counter：专门用于计数
counter = Counter(fault_log)
print(f"Counter: {counter}")
print(f"最常见的2种故障: {counter.most_common(2)}")

# 字典推导式（嵌套）
gun_data = {
    "gun1": {"voltage": 220.5, "current": 16.0, "status": "charging"},
    "gun2": {"voltage": 0.0,   "current": 0.0,  "status": "idle"},
    "gun3": {"voltage": 220.1, "current": 14.5, "status": "charging"},
}
# 提取所有正在充电的枪的功率
charging_powers = {
    gid: round(d["voltage"] * d["current"], 1)
    for gid, d in gun_data.items()
    if d["status"] == "charging"
}
print(f"充电中的功率: {charging_powers}")

# ========== 3. 集合操作 ==========
print("\n===== 3. 集合操作 =====")

online_stations = {"CS001", "CS002", "CS003", "CS005"}
fault_stations  = {"CS002", "CS004", "CS005"}

print(f"在线站点: {online_stations}")
print(f"故障站点: {fault_stations}")
print(f"在线且故障（需处理）: {online_stations & fault_stations}")   # 交集
print(f"在线且正常: {online_stations - fault_stations}")              # 差集
print(f"所有站点: {online_stations | fault_stations}")                # 并集
print(f"只在一个集合中: {online_stations ^ fault_stations}")          # 对称差集

# ========== 4. 命名元组（结构体替代品）==========
print("\n===== 4. 命名元组 =====")

from collections import namedtuple

# 创建命名元组类型（不可变的轻量级数据类）
MeterReading = namedtuple("MeterReading", ["timestamp", "voltage", "current", "power_factor"])

reading1 = MeterReading(
    timestamp="2026-04-28 10:00:00",
    voltage=220.5,
    current=16.0,
    power_factor=0.95
)

print(f"电表读数: {reading1}")
print(f"电压: {reading1.voltage}V")           # 按名称访问（比元组下标更直观）
print(f"功率: {reading1.voltage * reading1.current * reading1.power_factor:.1f}W")

# ========== 5. 栈和队列 ==========
print("\n===== 5. 栈与队列 =====")

# 栈（LIFO）：用列表的 append/pop
print("--- 栈（后进先出）---")
stack = []
for item in ["帧头", "长度", "数据", "校验"]:
    stack.append(item)
    print(f"  入栈: {item}, 栈内: {stack}")

while stack:
    print(f"  出栈: {stack.pop()}")

# 队列（FIFO）：用 deque（比列表 pop(0) 效率高！）
print("\n--- 队列（先进先出）---")
queue = deque()
for cmd in ["启动充电", "查询状态", "停止充电"]:
    queue.append(cmd)
    print(f"  入队: {cmd}")

while queue:
    cmd = queue.popleft()   # O(1)，列表 pop(0) 是 O(n)
    print(f"  处理命令: {cmd}")

# 双端队列（deque）特有：两端都能高效操作
dq = deque([1, 2, 3], maxlen=5)   # maxlen：限制最大长度，超出时自动丢弃旧的
dq.appendleft(0)    # 左侧插入
dq.append(4)        # 右侧插入
print(f"\n双端队列: {list(dq)}")

# ========== 6. 堆（优先队列）==========
print("\n===== 6. 堆（优先队列）=====")

# 场景：充电请求按优先级处理
# 格式：(优先级, 请求ID, 描述)  数字越小优先级越高
requests = []
heapq.heappush(requests, (3, "R003", "普通充电"))
heapq.heappush(requests, (1, "R001", "紧急充电-救护车"))
heapq.heappush(requests, (2, "R002", "VIP用户充电"))
heapq.heappush(requests, (1, "R004", "紧急充电-消防车"))

print("按优先级处理充电请求:")
while requests:
    priority, req_id, desc = heapq.heappop(requests)
    print(f"  优先级{priority}: [{req_id}] {desc}")

if __name__ == "__main__":
    print("\n✅ data_structures.py 运行完成！")

"""
TODO 练习：

TODO-01: 用 defaultdict(list) 实现一个分组函数
    group_by(chargers, key=lambda c: c["location"])
    → {"A区": [gun1, gun3], "B区": [gun2]}

TODO-02: 实现一个 LRU 缓存（用 OrderedDict）
    最近最少使用：get时移到末尾，超过容量时删除头部

TODO-03: 用 Counter 分析文本，找出出现最多的10个词

TODO-04: 实现一个线程安全的任务队列（用 deque + threading.Lock）

TODO-05: 用堆实现 TopK 问题：找出海量数据中最大的K个数
    hint: 维护一个大小为K的小顶堆

TODO-06: 用字典实现 Trie 树（前缀树）
    用于快速查找以某前缀开头的所有命令（CLI 自动补全）
"""
