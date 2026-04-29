/**
 * @file    structs.c
 * @brief   C 语言结构体、枚举、联合体
 * @author  你的名字
 * @date    2026
 *
 * 本文件演示：
 *   1. 结构体定义与初始化
 *   2. 结构体数组
 *   3. 结构体指针（->运算符）
 *   4. 嵌套结构体
 *   5. 枚举（enum）
 *   6. 联合体（union）：多类型共享内存
 *   7. 位域（bit fields）：嵌入式寄存器映射！
 *   8. typedef 简化类型名
 *
 * 编译命令：gcc -Wall -std=c99 -o structs structs.c
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* ========== 1. 结构体定义 ========== */

/* 用 typedef 定义结构体类型（推荐！嵌入式工程普遍使用）*/
typedef struct {
    char     name[32];    /* 姓名 */
    int      age;         /* 年龄 */
    float    salary;      /* 薪资 */
} Employee_t;

/* ========== 2. 枚举类型 ========== */

typedef enum {
    STATUS_OK       = 0,   /* 正常 */
    STATUS_ERROR    = 1,   /* 通用错误 */
    STATUS_TIMEOUT  = 2,   /* 超时 */
    STATUS_BUSY     = 3,   /* 忙碌 */
    STATUS_MAX             /* 枚举数量（用于边界检查）*/
} Status_t;

/* 枚举状态转字符串（嵌入式调试常用）*/
const char *status_to_str(Status_t s)
{
    switch (s) {
        case STATUS_OK:      return "OK";
        case STATUS_ERROR:   return "ERROR";
        case STATUS_TIMEOUT: return "TIMEOUT";
        case STATUS_BUSY:    return "BUSY";
        default:             return "UNKNOWN";
    }
}

/* ========== 3. 充电桩配置结构体（贴近实际业务）========== */

typedef struct {
    uint8_t  gun_id;          /* 充电枪编号（0~7）*/
    uint16_t rated_power_w;   /* 额定功率（瓦）*/
    uint8_t  max_current_a;   /* 最大电流（安培）*/
    uint8_t  is_enabled;      /* 是否启用（1=启用，0=禁用）*/
    char     location[32];    /* 安装位置描述 */
} GunConfig_t;

typedef struct {
    char        station_id[16];   /* 充电站ID */
    GunConfig_t guns[4];          /* 最多4个充电枪（嵌套结构体数组）*/
    uint8_t     gun_count;        /* 实际枪数量 */
    Status_t    status;           /* 整站状态 */
} ChargingStation_t;

void demo_structs(void)
{
    printf("===== 结构体示例 =====\n");

    /* 初始化方式一：逐字段赋值 */
    Employee_t emp1;
    strncpy(emp1.name, "张三", sizeof(emp1.name) - 1);
    emp1.age = 28;
    emp1.salary = 12000.0f;

    /* 初始化方式二：初始化列表（更简洁，推荐）*/
    Employee_t emp2 = {"李四", 32, 18000.0f};

    printf("员工1: 姓名=%s, 年龄=%d, 薪资=%.0f\n",
           emp1.name, emp1.age, emp1.salary);
    printf("员工2: 姓名=%s, 年龄=%d, 薪资=%.0f\n",
           emp2.name, emp2.age, emp2.salary);
    printf("结构体大小: %zu 字节\n", sizeof(Employee_t));

    /* 充电桩配置 */
    ChargingStation_t station = {
        .station_id = "CS001",
        .gun_count = 2,
        .status = STATUS_OK,
        .guns = {
            {1, 3300, 16, 1, "A区-01号位"},
            {2, 3300, 16, 1, "A区-02号位"},
        }
    };

    printf("\n充电站 %s 状态: %s\n", station.station_id,
           status_to_str(station.status));
    for (int i = 0; i < station.gun_count; i++) {
        GunConfig_t *gun = &station.guns[i];   /* 结构体指针 */
        printf("  枪%d: 功率=%dW, 电流=%dA, 位置=%s, 状态=%s\n",
               gun->gun_id, gun->rated_power_w, gun->max_current_a,
               gun->location, gun->is_enabled ? "启用" : "禁用");
    }
}

/* ========== 4. 联合体（union）：内存共享 ========== */

typedef union {
    uint32_t word;        /* 以32位整数访问 */
    uint16_t half[2];     /* 以两个16位整数访问 */
    uint8_t  bytes[4];    /* 以4个字节访问 */
} DataUnion_t;

void demo_union(void)
{
    printf("\n===== 联合体示例（内存共享）=====\n");

    DataUnion_t data;
    data.word = 0x12345678;

    printf("word  = 0x%08X\n", data.word);
    printf("half[0] = 0x%04X, half[1] = 0x%04X\n",
           data.half[0], data.half[1]);
    printf("bytes: [0]=0x%02X [1]=0x%02X [2]=0x%02X [3]=0x%02X\n",
           data.bytes[0], data.bytes[1], data.bytes[2], data.bytes[3]);

    /* 判断大端/小端 */
    if (data.bytes[0] == 0x78) {
        printf("本机是小端字节序（低字节在低地址）\n");
    } else {
        printf("本机是大端字节序（高字节在低地址）\n");
    }
}

/* ========== 5. 位域（Bit Fields）：寄存器位操作 ========== */

/* 模拟一个充电状态寄存器（16位）*/
typedef struct {
    uint16_t gun_connected   : 1;   /* bit0: 枪头已连接 */
    uint16_t car_connected   : 1;   /* bit1: 车辆已连接 */
    uint16_t charging        : 1;   /* bit2: 充电中 */
    uint16_t fault           : 1;   /* bit3: 故障 */
    uint16_t fault_code      : 4;   /* bit4-7: 故障码（0~15）*/
    uint16_t current_percent : 7;   /* bit8-14: 电流百分比（0~100）*/
    uint16_t reserved        : 1;   /* bit15: 保留 */
} ChargeStatus_Reg_t;

void demo_bitfields(void)
{
    printf("\n===== 位域示例（寄存器模拟）=====\n");

    ChargeStatus_Reg_t reg = {0};   /* 全部初始化为0 */

    reg.gun_connected  = 1;
    reg.car_connected  = 1;
    reg.charging       = 1;
    reg.current_percent = 75;       /* 75% 电流 */

    printf("枪头连接: %d\n", reg.gun_connected);
    printf("车辆连接: %d\n", reg.car_connected);
    printf("充电中:   %d\n", reg.charging);
    printf("故障:     %d\n", reg.fault);
    printf("电流:     %d%%\n", reg.current_percent);
    printf("寄存器大小: %zu 字节\n", sizeof(ChargeStatus_Reg_t));

    /* 作为整数读取原始寄存器值 */
    uint16_t *raw = (uint16_t *)&reg;
    printf("原始寄存器值: 0x%04X\n", *raw);
}

int main(void)
{
    demo_structs();
    demo_union();
    demo_bitfields();
    return 0;
}

/*
 * TODO 练习：
 *
 * TODO-01: 定义一个学生结构体，包含姓名、学号、3门课成绩
 *   实现函数：计算平均分、打印成绩单
 *
 * TODO-02: 用结构体数组实现一个简单的通讯录（增、删、查）
 *
 * TODO-03: 实现一个链表节点结构体（含数据和 next 指针），
 *   实现链表的创建、追加、打印、释放
 *
 * TODO-04: 用联合体实现一个消息结构体
 *   typedef union { int i; float f; char s[8]; } MsgData;
 *   typedef struct { uint8_t type; MsgData data; } Message;
 *
 * TODO-05: 用位域模拟 GD32 的 GPIO 模式寄存器
 *   每2位控制一个引脚的模式（00=输入，01=输出，10=复用，11=模拟）
 *
 * TODO-06: 实现一个基于结构体的栈（Stack）
 *   typedef struct { int data[64]; int top; } Stack_t;
 *   实现 push、pop、peek、is_empty、is_full
 */
