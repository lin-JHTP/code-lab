"""
Python 基础语法练习
===================
文件：basics.py
作者：你的名字
日期：2026

本文件演示（专为有C基础的工程师设计）：
    1. 变量与动态类型
    2. 字符串（f-string 格式化）
    3. 列表、元组、字典、集合
    4. 控制流（if/for/while）
    5. 列表推导式（Python 特色！）
    6. 类型提示（Type Hints，AI代码规范）

运行：python basics.py
"""

# ========== 1. 变量与类型 ==========
# Python 不需要声明类型，解释器自动推断
print("===== 1. 变量与类型 =====")

age = 25                   # int
height = 1.75              # float
name = "嵌入式工程师"       # str（Python默认Unicode，完美支持中文！）
is_working = True          # bool（注意：True/False 首字母大写）
nothing = None             # None（相当于C的NULL）

print(f"姓名: {name}")                   # f-string：Python 3.6+ 推荐格式化方式
print(f"年龄: {age}, 类型: {type(age)}") # type() 查看类型
print(f"身高: {height:.2f}m")            # :.2f 保留2位小数
print(f"is None: {nothing is None}")     # is 判断对象同一性

# 动态类型：变量可以随时改变类型（C里做不到！）
x = 10
print(f"x = {x}, 类型 = {type(x).__name__}")
x = "现在是字符串了"
print(f"x = {x}, 类型 = {type(x).__name__}")

# ========== 2. 字符串操作 ==========
print("\n===== 2. 字符串操作 =====")

s = "Hello, Python!"
print(f"原始: {s}")
print(f"大写: {s.upper()}")
print(f"小写: {s.lower()}")
print(f"长度: {len(s)}")
print(f"切片 [0:5]: {s[0:5]}")     # 切片：[起始:结束]，不含结束
print(f"反转: {s[::-1]}")          # 步长-1 = 反转（Python 神操作！）
print(f"分割: {s.split(', ')}")    # 按逗号+空格分割，返回列表
print(f"替换: {s.replace('Python', 'World')}")
print(f"去空格: {'  空格  '.strip()}")

# 多行字符串
multi_line = """
这是
多行
字符串
"""
print(f"多行: {multi_line.strip()}")

# ========== 3. 列表（类似C数组，但更强大）==========
print("\n===== 3. 列表 =====")

# 创建列表
nums = [3, 1, 4, 1, 5, 9, 2, 6]
fruits = ["苹果", "香蕉", "橙子"]
mixed = [1, "two", 3.0, True]    # 列表可以混合类型！

print(f"数字列表: {nums}")
print(f"第一个: {nums[0]}")       # 下标访问
print(f"最后一个: {nums[-1]}")    # 负数下标：-1 是最后一个
print(f"切片 [1:4]: {nums[1:4]}")

# 列表方法
nums.append(7)               # 末尾追加
nums.insert(0, 0)            # 在位置0插入
nums.remove(1)               # 删除第一个值为1的元素
popped = nums.pop()          # 弹出最后一个
nums.sort()                  # 原地排序
print(f"操作后: {nums}")

# 遍历
print("遍历:", end=" ")
for fruit in fruits:
    print(fruit, end=" ")
print()

# 带索引遍历（相当于 for i, for i<len, 同时获取下标和值）
for i, fruit in enumerate(fruits):
    print(f"  [{i}] {fruit}")

# ========== 4. 字典（键值对，比C结构体更灵活）==========
print("\n===== 4. 字典 =====")

# 相当于充电桩配置
charger_config = {
    "station_id": "CS001",
    "gun_count": 2,
    "rated_power_w": 3300,
    "location": "停车场A区",
    "is_online": True
}

print(f"充电站ID: {charger_config['station_id']}")
print(f"枪数量: {charger_config.get('gun_count', 0)}")  # .get() 带默认值，更安全

# 添加/修改键值
charger_config["firmware_version"] = "1.2.3"
charger_config["gun_count"] = 4

# 遍历字典
print("所有配置:")
for key, value in charger_config.items():
    print(f"  {key}: {value}")

# 字典推导式
squares = {x: x**2 for x in range(1, 6)}   # {1:1, 2:4, 3:9, 4:16, 5:25}
print(f"平方字典: {squares}")

# ========== 5. 控制流 ==========
print("\n===== 5. 控制流 =====")

# if-elif-else（注意缩进，不用括号！）
score = 85
if score >= 90:
    grade = "A"
elif score >= 80:
    grade = "B"
elif score >= 60:
    grade = "C"
else:
    grade = "F"
print(f"分数 {score} → 等级 {grade}")

# for + range
print("for range:", [i**2 for i in range(1, 6)])  # 列表推导式！

# while 循环
n = 1
powers_of_2 = []
while n <= 100:
    powers_of_2.append(n)
    n *= 2
print(f"100以内2的幂: {powers_of_2}")

# ========== 6. 列表推导式（Python 特色！）==========
print("\n===== 6. 列表推导式 =====")

# 基础：[表达式 for 变量 in 可迭代对象]
evens = [x for x in range(20) if x % 2 == 0]
print(f"偶数: {evens}")

# 嵌套推导式（矩阵）
matrix = [[i * 3 + j for j in range(3)] for i in range(3)]
print(f"矩阵: {matrix}")

# 字符串处理
words = ["hello", "world", "python"]
upper_words = [w.upper() for w in words]
print(f"大写: {upper_words}")

# 过滤
data = [1, -2, 3, -4, 5, -6]
positives = [x for x in data if x > 0]
print(f"正数: {positives}")

# ========== 7. 类型提示（Type Hints）==========
print("\n===== 7. 类型提示（AI代码规范）=====")

def add(a: int, b: int) -> int:
    """两数相加（带类型提示的写法，AI/大型项目推荐）"""
    return a + b

def greet(name: str, times: int = 1) -> str:
    """带默认参数的函数"""
    return f"你好, {name}! " * times

result: int = add(3, 5)
message: str = greet("工程师", 2)
print(f"add(3,5) = {result}")
print(f"greet: {message}")

# ========== 运行提示 ==========
if __name__ == "__main__":
    print("\n✅ basics.py 运行完成！")
    print("下一步：尝试完成文件末尾的 TODO 练习")

"""
TODO 练习：

TODO-01: 用列表推导式找出100以内所有能被3或5整除的数，并求和
    hint: sum([x for x in range(1, 101) if x % 3 == 0 or x % 5 == 0])

TODO-02: 写一个函数，接受一个字符串，返回其中字符出现频率的字典
    例如: count_chars("hello") → {'h':1, 'e':1, 'l':2, 'o':1}
    hint: 用字典推导式或 collections.Counter

TODO-03: 模拟充电桩数据
    创建一个包含10个充电枪状态的列表（字典），
    用列表推导式筛选出"充电中"的枪

TODO-04: 实现 FizzBuzz（经典）
    1~50 中：3的倍数→"Fizz"，5的倍数→"Buzz"，都是→"FizzBuzz"，其他→数字
    用列表推导式一行搞定！

TODO-05: 从嵌套字典中提取所有值（深度遍历）
    data = {"a": 1, "b": {"c": 2, "d": {"e": 3}}}
    期望得到 [1, 2, 3]（递归实现）
"""
