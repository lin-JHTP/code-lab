"""
Python 面向对象编程（OOP）练习
================================
文件：oop.py
作者：你的名字
日期：2026

本文件演示：
    1. 类定义与实例化
    2. 魔术方法（__str__, __repr__, __eq__）
    3. 继承与多态
    4. 类方法与静态方法
    5. 属性装饰器（@property）
    6. 数据类（dataclass）- Python 3.7+
    7. 实际项目：充电桩OOP建模

运行：python oop.py
"""

from dataclasses import dataclass, field
from typing import List, Optional
from datetime import datetime
from enum import Enum, auto

# ========== 1. 枚举类 ==========
class GunStatus(Enum):
    """充电枪状态枚举（使用 Python 枚举，比C语言更安全）"""
    IDLE       = auto()   # 空闲
    CONNECTED  = auto()   # 已插枪
    CHARGING   = auto()   # 充电中
    FINISHED   = auto()   # 充电完成
    FAULT      = auto()   # 故障

# ========== 2. 基础类 ==========
class ChargingGun:
    """
    充电枪类 - 模拟你实际工作中的对象！
    
    Python OOP vs C 结构体：
    - C：struct + 函数（手动"模块化"）
    - Python：class 把数据和方法封装在一起
    """

    # 类变量（所有实例共享）
    gun_count: int = 0

    def __init__(self, gun_id: int, rated_power_w: int, location: str = "未知"):
        """
        构造函数（相当于C的初始化函数）
        self 相当于C里的 this 指针
        """
        # 实例变量（每个对象独立）
        self.gun_id       = gun_id
        self.rated_power_w = rated_power_w
        self.location     = location
        self.status       = GunStatus.IDLE
        self._voltage     = 0.0    # _ 前缀表示"私有"（约定，非强制）
        self._current     = 0.0
        self.error_code   = 0
        self.session_start: Optional[datetime] = None

        ChargingGun.gun_count += 1    # 类变量计数

    # ===== 魔术方法（Magic Methods）=====

    def __str__(self) -> str:
        """print(gun) 时调用"""
        return (f"充电枪[{self.gun_id}] @ {self.location} "
                f"| {self.status.name} | {self.rated_power_w}W")

    def __repr__(self) -> str:
        """调试时显示"""
        return f"ChargingGun(id={self.gun_id}, status={self.status})"

    def __eq__(self, other) -> bool:
        """gun1 == gun2 时调用"""
        if not isinstance(other, ChargingGun):
            return False
        return self.gun_id == other.gun_id

    # ===== 属性装饰器（@property）=====

    @property
    def power_w(self) -> float:
        """实时功率（只读属性，由电压电流计算）"""
        return self._voltage * self._current

    @property
    def is_available(self) -> bool:
        """是否可用（只读）"""
        return self.status == GunStatus.IDLE

    # ===== 实例方法 =====

    def start_charging(self, voltage: float, current: float) -> bool:
        """开始充电"""
        if self.status != GunStatus.CONNECTED:
            print(f"  [枪{self.gun_id}] 无法开始充电：当前状态 {self.status.name}")
            return False

        self._voltage = voltage
        self._current = current
        self.status = GunStatus.CHARGING
        self.session_start = datetime.now()
        print(f"  [枪{self.gun_id}] 充电开始: {voltage}V / {current}A = {self.power_w}W")
        return True

    def stop_charging(self) -> dict:
        """停止充电，返回本次充电摘要"""
        if self.status != GunStatus.CHARGING:
            return {}

        duration_s = (datetime.now() - self.session_start).total_seconds()
        energy_wh = self.power_w * (duration_s / 3600)

        self._voltage = 0.0
        self._current = 0.0
        self.status = GunStatus.FINISHED

        summary = {
            "gun_id":       self.gun_id,
            "duration_s":   round(duration_s, 1),
            "energy_wh":    round(energy_wh, 2),
        }
        print(f"  [枪{self.gun_id}] 充电结束: 时长={duration_s:.1f}s, 电量≈{energy_wh:.2f}Wh")
        return summary

    def connect(self):
        """插枪"""
        if self.status == GunStatus.IDLE:
            self.status = GunStatus.CONNECTED
            print(f"  [枪{self.gun_id}] 已插枪")

    def reset(self):
        """复位"""
        self.status = GunStatus.IDLE
        self._voltage = 0.0
        self._current = 0.0
        self.error_code = 0
        print(f"  [枪{self.gun_id}] 已复位")

    # ===== 类方法与静态方法 =====

    @classmethod
    def get_total_guns(cls) -> int:
        """类方法：访问类变量"""
        return cls.gun_count

    @staticmethod
    def validate_current(current: float) -> bool:
        """静态方法：不需要访问类或实例，只是工具函数"""
        return 0.0 < current <= 32.0    # 单相最大32A


# ========== 3. 继承 ==========

class ACChargingGun(ChargingGun):
    """
    交流充电枪（继承自 ChargingGun）
    你实际工作中使用的！
    """

    def __init__(self, gun_id: int, phases: int = 1, location: str = "未知"):
        """
        调用父类 __init__，再添加子类特有属性
        super() 相当于调用父类的方法
        """
        power_w = 3300 if phases == 1 else 7000   # 单相3.3kW，三相7kW
        super().__init__(gun_id, power_w, location)
        self.phases = phases              # 相数（1或3）
        self.mode = "Mode3"               # 交流充电模式

    def __str__(self) -> str:
        """重写父类的 __str__"""
        base = super().__str__()
        return f"{base} [{self.phases}相/{self.mode}]"

    def check_cp_signal(self) -> str:
        """
        CP信号检测（交流充电特有！Control Pilot）
        状态A：未连接（12V）
        状态B：已连接（9V）
        状态C：充电（6V）
        """
        cp_voltage_map = {
            GunStatus.IDLE:      12.0,
            GunStatus.CONNECTED: 9.0,
            GunStatus.CHARGING:  6.0,
        }
        voltage = cp_voltage_map.get(self.status, 0.0)
        state = {12.0: "A", 9.0: "B", 6.0: "C"}.get(voltage, "E")
        return f"CP信号: 状态{state}, {voltage}V"


class DCChargingGun(ChargingGun):
    """直流快充枪（扩展示例，用于将来学习）"""

    def __init__(self, gun_id: int, max_power_kw: float, location: str = "未知"):
        super().__init__(gun_id, int(max_power_kw * 1000), location)
        self.max_power_kw = max_power_kw
        self.protocol = "GB/T 27930"

    def __str__(self) -> str:
        return f"{super().__str__()} [DC/{self.protocol}/{self.max_power_kw}kW]"


# ========== 4. 数据类（dataclass）- Python 3.7+  ==========

@dataclass
class ChargingSession:
    """
    充电会话记录（用 dataclass 简化样板代码）
    dataclass 自动生成 __init__, __repr__, __eq__
    """
    session_id:   str
    gun_id:       int
    start_time:   datetime = field(default_factory=datetime.now)
    end_time:     Optional[datetime] = None
    energy_kwh:   float = 0.0
    cost_yuan:    float = 0.0
    user_id:      str = "anonymous"

    def duration_minutes(self) -> float:
        """计算充电时长（分钟）"""
        if self.end_time is None:
            return 0.0
        return (self.end_time - self.start_time).total_seconds() / 60

    def __str__(self) -> str:
        return (f"充电记录[{self.session_id}]: "
                f"枪{self.gun_id}, {self.energy_kwh:.2f}度, "
                f"费用{self.cost_yuan:.2f}元")


# ========== 5. 演示代码 ==========
def main():
    print("===== OOP 充电桩建模演示 =====\n")

    # 创建充电枪对象
    gun1 = ACChargingGun(gun_id=1, phases=1, location="A区-01")
    gun2 = ACChargingGun(gun_id=2, phases=3, location="A区-02")
    dc_gun = DCChargingGun(gun_id=3, max_power_kw=60.0, location="B区-01")

    print("创建的充电枪：")
    for gun in [gun1, gun2, dc_gun]:
        print(f"  {gun}")

    print(f"\n总充电枪数: {ChargingGun.get_total_guns()}")

    # 演示充电流程
    print("\n--- 充电流程演示 ---")
    gun1.connect()
    print(gun1.check_cp_signal())

    gun1.start_charging(220.0, 16.0)
    print(f"  实时功率: {gun1.power_w}W")

    import time
    time.sleep(0.01)    # 模拟充电0.01秒

    summary = gun1.stop_charging()
    gun1.reset()

    # 创建充电记录
    if summary:
        session = ChargingSession(
            session_id="S20260428001",
            gun_id=1,
            energy_kwh=summary["energy_wh"] / 1000,
            cost_yuan=summary["energy_wh"] / 1000 * 1.2,
            user_id="user_001"
        )
        print(f"\n{session}")

    # 验证功能
    print(f"\n电流验证 16A: {ChargingGun.validate_current(16.0)}")
    print(f"电流验证 40A: {ChargingGun.validate_current(40.0)}")


if __name__ == "__main__":
    main()

"""
TODO 练习：

TODO-01: 实现 ChargingStation 类，管理多个 ChargingGun
    - 属性：station_id, guns列表, max_guns
    - 方法：add_gun, remove_gun, get_available_guns, get_station_status

TODO-02: 添加 Observer 模式（观察者模式）
    - ChargingGun 状态变化时，通知所有注册的监听器
    - 用于：日志记录、UI更新、告警推送

TODO-03: 实现 __iter__ 和 __next__ 让 ChargingStation 可迭代
    for gun in station: print(gun)

TODO-04: 用 @property 加 setter 添加参数校验
    @voltage.setter
    def voltage(self, val):
        if not 0 <= val <= 260: raise ValueError(...)

TODO-05: 继承 ChargingGun，实现 SmartACGun（支持预约充电）
    - 新增属性：scheduled_start_time
    - 新增方法：schedule_charging, cancel_schedule

TODO-06: 实现 __lt__, __le__ 使充电枪支持排序（按功率）
    guns.sort()  # 自动按功率排序
"""
