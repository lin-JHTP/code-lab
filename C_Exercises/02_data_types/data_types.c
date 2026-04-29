/**
 * @file    data_types.c
 * @brief   C 语言数据类型、变量与运算符详解
 * @author  你的名字
 * @date    2026
 *
 * 本文件演示：
 *   1. 基本数据类型（int, float, double, char）
 *   2. 类型范围与 limits.h
 *   3. 类型转换（隐式 & 显式）
 *   4. 位运算（嵌入式必备！）
 *   5. 常量与宏定义
 *
 * 编译命令：gcc -Wall -std=c99 -o data_types data_types.c
 *
 * TODO 练习：
 *   TODO-01: 实验 int 溢出：让 INT_MAX + 1，观察结果
 *   TODO-02: 用 sizeof 打印所有类型的字节数
 *   TODO-03: 用位运算实现"交换两个变量的值"（不使用临时变量）
 */

#include <stdio.h>
#include <limits.h>   /* INT_MAX, INT_MIN 等 */
#include <float.h>    /* FLT_MAX, DBL_MAX 等 */
#include <stdint.h>   /* uint8_t, int32_t 等（嵌入式常用！） */

int main(void)
{
    /* ========== 1. 整数类型 ========== */
    printf("===== 整数类型 =====\n");

    char   c  = 'A';          /* 8位，-128 ~ 127 */
    short  s  = 1000;         /* 16位，-32768 ~ 32767 */
    int    i  = 100000;       /* 通常32位 */
    long   l  = 1000000L;     /* 至少32位，L后缀 */

    printf("char   大小: %zu 字节, 范围: %d ~ %d\n", sizeof(char), CHAR_MIN, CHAR_MAX);
    printf("short  大小: %zu 字节, 范围: %d ~ %d\n", sizeof(short), SHRT_MIN, SHRT_MAX);
    printf("int    大小: %zu 字节, 范围: %d ~ %d\n", sizeof(int), INT_MIN, INT_MAX);
    printf("long   大小: %zu 字节\n", sizeof(long));

    /* 嵌入式开发推荐使用固定宽度类型，避免平台差异 */
    uint8_t  u8  = 255;       /* 无符号8位，0~255，对应单片机寄存器 */
    uint16_t u16 = 65535;
    uint32_t u32 = 0xDEADBEEF;  /* 十六进制，嵌入式常用 */
    int32_t  i32 = -1000000;

    printf("\n===== 嵌入式常用固定宽度类型 =====\n");
    printf("uint8_t  = %u  (0x%02X)\n", u8, u8);
    printf("uint16_t = %u  (0x%04X)\n", u16, u16);
    printf("uint32_t = %u  (0x%08X)\n", u32, u32);
    printf("int32_t  = %d\n", i32);

    /* ========== 2. 浮点类型 ========== */
    printf("\n===== 浮点类型 =====\n");

    float  f = 3.14f;         /* 单精度，约7位有效数字 */
    double d = 3.14159265358979;  /* 双精度，约15位有效数字 */

    printf("float  大小: %zu 字节, 值: %.7f\n", sizeof(float), f);
    printf("double 大小: %zu 字节, 值: %.15f\n", sizeof(double), d);
    /* 注意：嵌入式中浮点运算昂贵，优先用定点数或整数运算 */

    /* ========== 3. 位运算（嵌入式寄存器操作核心！）========== */
    printf("\n===== 位运算（嵌入式必备）=====\n");

    uint8_t reg = 0x00;       /* 模拟一个8位寄存器，初始全0 */

    /* 置位：将第3位（bit3）置1 */
    reg |= (1 << 3);          /* |= 按位或赋值，(1<<3) = 0b00001000 */
    printf("置位 bit3: 0x%02X = 0b", reg);
    for (int bit = 7; bit >= 0; bit--) {
        printf("%d", (reg >> bit) & 1);
    }
    printf("\n");

    /* 清位：将第3位清0 */
    reg &= ~(1 << 3);         /* &= ~(...) 清除指定位 */
    printf("清位 bit3: 0x%02X\n", reg);

    /* 翻转：将第5位取反 */
    reg ^= (1 << 5);          /* ^= 按位异或 */
    printf("翻转 bit5: 0x%02X\n", reg);

    /* 读取：检查某位是否为1 */
    uint8_t status = 0xA5;    /* 0b10100101 */
    int bit4 = (status >> 4) & 1;
    printf("读取 status bit4: %d\n", bit4);

    /* ========== 4. 宏定义与常量 ========== */
    printf("\n===== 宏定义与常量 =====\n");

#define PI      3.14159f       /* 宏定义：编译器直接替换，不占内存 */
#define MAX_BUF 256            /* 常用：缓冲区大小 */
#define LED_PIN (1 << 5)       /* GPIO引脚掩码 */

    const int MAX_RETRY = 3;   /* const 常量：有类型检查，推荐使用 */

    printf("PI = %.5f\n", PI);
    printf("MAX_BUF = %d\n", MAX_BUF);
    printf("LED_PIN = 0x%02X\n", LED_PIN);
    printf("MAX_RETRY = %d\n", MAX_RETRY);

    /* ========== 5. 类型转换 ========== */
    printf("\n===== 类型转换 =====\n");

    int   a = 7;
    int   b = 2;
    float result;

    result = a / b;            /* 整数除法！结果为3，不是3.5 */
    printf("int/int = %f  (错误！整数除法)\n", result);

    result = (float)a / b;     /* 强制类型转换：先转float再除 */
    printf("(float)int/int = %f  (正确)\n", result);

    return 0;
}

/*
 * TODO 练习：
 *
 * TODO-01: 实验 int 溢出
 *   int max = INT_MAX;
 *   printf("INT_MAX + 1 = %d\n", max + 1);  // 会发生什么？
 *
 * TODO-02: 用宏定义模拟一个嵌入式 GPIO 操作库
 *   #define GPIO_SET(port, pin)   ((port) |= (1 << (pin)))
 *   #define GPIO_CLR(port, pin)   ((port) &= ~(1 << (pin)))
 *   #define GPIO_READ(port, pin)  (((port) >> (pin)) & 1)
 *
 * TODO-03: 用位运算交换两个变量（不使用临时变量）
 *   a ^= b;
 *   b ^= a;
 *   a ^= b;
 *
 * TODO-04: 理解大端/小端存储
 *   union { uint32_t val; uint8_t bytes[4]; } endian;
 *   endian.val = 0x12345678;
 *   打印 bytes[0]，判断是大端还是小端
 *
 * TODO-05: 实现一个 BCD 码转十进制的函数
 *   BCD: 0x45 → 45（每4位表示一个十进制数字）
 */
