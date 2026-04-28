"""
Python 函数与模块练习
====================
文件：functions_modules.py
作者：你的名字
日期：2026

本文件演示：
    1. 函数定义：位置参数、关键字参数、默认参数
    2. *args 和 **kwargs（可变参数）
    3. Lambda 表达式（匿名函数）
    4. 高阶函数：map、filter、sorted
    5. 装饰器（Decorator）- Python 高级特性
    6. 生成器（Generator）- 节省内存
    7. 模块化：import 和包结构

运行：python functions_modules.py
"""

import time        # 时间模块
import functools   # 高阶函数工具
from typing import List, Callable, Any

# ========== 1. 函数基础 ==========
print("===== 1. 函数基础 =====")

def calculate_power(kw: float, hours: float, price: float = 1.2) -> dict:
    """
    计算充电费用
    
    Args:
        kw:    充电功率（千瓦）
        hours: 充电时长（小时）
        price: 电价（元/度），默认 1.2 元
    
    Returns:
        dict: 包含电量和费用的字典
    """
    energy_kwh = kw * hours                  # 电量（度）
    cost = energy_kwh * price                 # 费用（元）
    return {
        "energy_kwh": round(energy_kwh, 2),
        "cost_yuan":  round(cost, 2)
    }

result = calculate_power(3.3, 2)
print(f"3.3kW充2小时: 电量={result['energy_kwh']}度, 费用={result['cost_yuan']}元")

result2 = calculate_power(kw=7.0, hours=1.5, price=0.9)   # 关键字参数
print(f"7kW充1.5小时（谷价）: 电量={result2['energy_kwh']}度, 费用={result2['cost_yuan']}元")

# ========== 2. *args 和 **kwargs ==========
print("\n===== 2. *args 和 **kwargs =====")

def log_event(*args, **kwargs):
    """
    通用事件日志函数
    *args:   位置参数（任意数量）
    **kwargs: 关键字参数（键值对）
    """
    print(f"[EVENT] {' '.join(str(a) for a in args)}")
    for key, value in kwargs.items():
        print(f"  {key}: {value}")

log_event("充电开始", gun_id=1, voltage=220, current=16)
log_event("故障告警", "E001", fault_code="OVP", severity="HIGH")

# ========== 3. Lambda + 高阶函数 ==========
print("\n===== 3. Lambda + 高阶函数 =====")

# lambda：简短的匿名函数
double = lambda x: x * 2
square = lambda x: x ** 2
print(f"double(5) = {double(5)}")

# map：对列表每个元素应用函数
numbers = [1, 2, 3, 4, 5]
doubled = list(map(lambda x: x * 2, numbers))
print(f"map double: {doubled}")

# filter：过滤列表
evens = list(filter(lambda x: x % 2 == 0, numbers))
print(f"filter even: {evens}")

# sorted：自定义排序键
chargers = [
    {"id": "A", "power": 7.0},
    {"id": "B", "power": 3.3},
    {"id": "C", "power": 11.0},
]
sorted_by_power = sorted(chargers, key=lambda c: c["power"], reverse=True)
print("按功率降序:", [f"{c['id']}:{c['power']}kW" for c in sorted_by_power])

# ========== 4. 装饰器（Decorator）==========
print("\n===== 4. 装饰器 =====")

def timer_decorator(func: Callable) -> Callable:
    """
    计时装饰器：测量函数执行时间
    这是一个高阶函数，接受函数，返回包装后的函数
    """
    @functools.wraps(func)   # 保留原函数的元信息（名称、文档等）
    def wrapper(*args, **kwargs):
        start = time.time()
        result = func(*args, **kwargs)
        elapsed = time.time() - start
        print(f"  [{func.__name__}] 执行时间: {elapsed*1000:.2f}ms")
        return result
    return wrapper

def retry_decorator(max_retries: int = 3):
    """
    重试装饰器工厂：失败时自动重试
    使用场景：网络请求、硬件通信等
    """
    def decorator(func: Callable) -> Callable:
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            for attempt in range(1, max_retries + 1):
                try:
                    return func(*args, **kwargs)
                except Exception as e:
                    print(f"  第{attempt}次尝试失败: {e}")
                    if attempt == max_retries:
                        raise
            return None
        return wrapper
    return decorator

@timer_decorator
def slow_computation(n: int) -> int:
    """模拟耗时计算"""
    total = sum(range(n))
    return total

@retry_decorator(max_retries=3)
def unstable_connection(host: str) -> bool:
    """模拟不稳定的网络连接（总是失败，用于演示重试）"""
    raise ConnectionError(f"无法连接到 {host}")

result = slow_computation(100000)
print(f"计算结果: {result}")

# 演示重试（会失败3次）
try:
    unstable_connection("192.168.1.1")
except ConnectionError as e:
    print(f"最终失败: {e}")

# ========== 5. 生成器（Generator）- 内存友好 ==========
print("\n===== 5. 生成器 =====")

def fibonacci_gen(limit: int):
    """
    斐波那契数列生成器
    用 yield 而不是 return，每次调用返回下一个值
    不会一次性把所有数据加载到内存（处理大数据集的关键！）
    """
    a, b = 0, 1
    while a <= limit:
        yield a              # 暂停并返回 a，下次从这里继续
        a, b = b, a + b

# 生成器不会立即计算，只在需要时才产生值
fib = fibonacci_gen(100)
print("100以内斐波那契:", list(fib))

# 生成器表达式（类似列表推导式，但用 ()）
gen_expr = (x**2 for x in range(10))   # 不立即计算
print("生成器表达式（惰性）:", sum(gen_expr))   # 求和时才计算

# 处理大数据文件的场景（不把整个文件加载到内存）
def read_sensor_data(filename: str):
    """
    逐行读取传感器数据文件（生成器版本）
    对于几GB的日志文件非常有用！
    """
    # 这里是演示，实际需要文件存在
    sample_data = ["22.5,0.95\n", "22.6,0.96\n", "22.4,0.94\n"]
    for line in sample_data:
        voltage, current = line.strip().split(',')
        yield {"voltage": float(voltage), "current": float(current)}

print("传感器数据:")
for reading in read_sensor_data("sensor.csv"):
    power = reading["voltage"] * reading["current"]
    print(f"  电压={reading['voltage']}V, 电流={reading['current']}A, 功率={power:.2f}W")

# ========== 6. 闭包（Closure）==========
print("\n===== 6. 闭包 =====")

def make_counter(start: int = 0):
    """
    计数器工厂：返回一个闭包（带状态的函数）
    类似C语言中的 static 局部变量，但更优雅
    """
    count = [start]   # 用列表包装，使内层函数可以修改

    def increment(step: int = 1) -> int:
        count[0] += step
        return count[0]

    def reset():
        count[0] = start

    return increment, reset

heartbeat, reset_heartbeat = make_counter(0)
print(f"心跳计数: {heartbeat()}, {heartbeat()}, {heartbeat(5)}")
reset_heartbeat()
print(f"重置后: {heartbeat()}")

# ========== main ==========
if __name__ == "__main__":
    print("\n✅ functions_modules.py 运行完成！")

"""
TODO 练习：

TODO-01: 实现一个 memoize 装饰器（缓存函数结果）
    @memoize
    def fib(n): ...
    fib(35) 第一次慢，第二次瞬间返回（因为缓存了）

TODO-02: 写一个日志装饰器，记录函数名、参数、返回值和时间戳到文件

TODO-03: 实现一个参数验证装饰器
    @validate(kw=(0, 100), hours=(0, 24))
    def calculate_power(kw, hours): ...
    当参数超出范围时抛出 ValueError

TODO-04: 用生成器实现一个无限序列（如无限计数器）
    def counter(): n=0; while True: yield n; n+=1

TODO-05: 实现柯里化（Currying）：
    add = lambda a: lambda b: a + b
    add5 = add(5)
    print(add5(3))  # 8
"""
