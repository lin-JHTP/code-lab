/**
 * @file    control_flow.c
 * @brief   C 语言控制流：条件、循环、跳转
 * @author  你的名字
 * @date    2026
 *
 * 本文件演示：
 *   1. if / else if / else
 *   2. switch-case（含穿透与 break）
 *   3. for / while / do-while 循环
 *   4. break / continue / goto
 *   5. 嵌入式常见控制流模式
 *
 * 编译命令：gcc -Wall -std=c99 -o control_flow control_flow.c
 */

#include <stdio.h>
#include <stdint.h>

/* ========== 示例1：if-else ========== */
void demo_if_else(void)
{
    printf("\n===== if-else 示例 =====\n");

    int score = 85;

    if (score >= 90) {
        printf("优秀\n");
    } else if (score >= 80) {
        printf("良好\n");       /* 会执行这里 */
    } else if (score >= 60) {
        printf("及格\n");
    } else {
        printf("不及格\n");
    }

    /* 嵌入式中常见的错误码检查 */
    int ret = -1;              /* 模拟函数返回值 */
    if (ret != 0) {
        printf("操作失败，错误码: %d\n", ret);
        /* 真实嵌入式代码会在这里处理错误，如重试、报警等 */
    }
}

/* ========== 示例2：switch-case ========== */
typedef enum {
    CHARGER_STATE_IDLE    = 0,    /* 空闲 */
    CHARGER_STATE_READY   = 1,    /* 就绪 */
    CHARGER_STATE_CHARGING = 2,   /* 充电中 */
    CHARGER_STATE_FAULT   = 3,    /* 故障 */
    CHARGER_STATE_MAX
} ChargerState_t;

void demo_switch(ChargerState_t state)
{
    printf("\n===== switch-case 示例（充电桩状态机）=====\n");

    switch (state) {
        case CHARGER_STATE_IDLE:
            printf("当前状态：空闲，等待车辆连接\n");
            break;              /* break 必不可少！否则会穿透到下一个case */

        case CHARGER_STATE_READY:
            printf("当前状态：就绪，等待用户启动充电\n");
            break;

        case CHARGER_STATE_CHARGING:
            printf("当前状态：充电中\n");
            break;

        case CHARGER_STATE_FAULT:
            printf("当前状态：故障！请检查设备\n");
            break;

        default:
            printf("未知状态: %d\n", state);
            break;
    }
}

/* ========== 示例3：for 循环 ========== */
void demo_for_loop(void)
{
    printf("\n===== for 循环示例 =====\n");

    /* 基础 for 循环：打印0~9 */
    printf("0到9: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", i);
    }
    printf("\n");

    /* 嵌套循环：九九乘法表 */
    printf("九九乘法表:\n");
    for (int i = 1; i <= 9; i++) {
        for (int j = 1; j <= i; j++) {
            printf("%d*%d=%-3d", j, i, i * j);  /* %-3d 左对齐3字符宽 */
        }
        printf("\n");
    }
}

/* ========== 示例4：while 循环 ========== */
void demo_while_loop(void)
{
    printf("\n===== while 循环示例 =====\n");

    /* while 循环：适合条件不确定的场景 */
    int n = 1;
    printf("2的幂次方: ");
    while (n <= 1024) {
        printf("%d ", n);
        n *= 2;
    }
    printf("\n");

    /* do-while 循环：至少执行一次 */
    int input;
    printf("do-while 示例（输入 0 退出）:\n");
    do {
        printf("请输入一个数字（0退出）: ");
        scanf("%d", &input);
        printf("你输入了: %d\n", input);
    } while (input != 0);    /* 先执行，再判断 */
}

/* ========== 示例5：break 和 continue ========== */
void demo_break_continue(void)
{
    printf("\n===== break 和 continue =====\n");

    /* break：跳出整个循环 */
    printf("找到第一个能被7整除的数: ");
    for (int i = 1; i <= 100; i++) {
        if (i % 7 == 0) {
            printf("%d\n", i);
            break;             /* 找到后立即退出循环 */
        }
    }

    /* continue：跳过本次迭代，继续下一次 */
    printf("1到20中的奇数: ");
    for (int i = 1; i <= 20; i++) {
        if (i % 2 == 0) {
            continue;          /* 偶数跳过 */
        }
        printf("%d ", i);
    }
    printf("\n");
}

int main(void)
{
    demo_if_else();
    demo_switch(CHARGER_STATE_CHARGING);
    demo_switch(CHARGER_STATE_FAULT);
    demo_for_loop();
    /* demo_while_loop(); */   /* 需要键盘输入，暂时注释掉 */
    demo_break_continue();

    return 0;
}

/*
 * TODO 练习：
 *
 * TODO-01: 实现一个简单的菜单程序
 *   打印菜单选项，用 switch 处理用户选择，用 while 循环让程序持续运行直到选择"退出"
 *
 * TODO-02: 判断素数
 *   输入一个数 n，用 for 循环判断是否为素数（只能被1和自身整除）
 *
 * TODO-03: 实现 FizzBuzz（经典面试题）
 *   1~100中：3的倍数打印"Fizz"，5的倍数打印"Buzz"，都是的打印"FizzBuzz"，其他打印数字
 *
 * TODO-04: 统计一个字符串中各字符出现的次数
 *   用数组 int count[128] 统计，for 循环遍历字符串
 *
 * TODO-05: 用嵌套循环打印菱形图案
 *
 * TODO-06（嵌入式）: 模拟一个简单的轮询任务调度器
 *   while(1) { 
 *     task_10ms(); 
 *     if (tick % 10 == 0) task_100ms();
 *     if (tick % 100 == 0) task_1s();
 *   }
 */
