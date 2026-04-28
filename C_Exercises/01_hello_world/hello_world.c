/**
 * @file    hello_world.c
 * @brief   C 语言入门：Hello World 与基础输入输出
 * @author  你的名字
 * @date    2026
 *
 * 本文件演示：
 *   1. C 程序基本结构（头文件、main函数、return）
 *   2. printf 格式化输出
 *   3. scanf 输入
 *   4. 编译流程：.c → 预处理 → 编译 → 汇编 → 链接 → 可执行文件
 *
 * 编译命令：
 *   gcc -Wall -Wextra -std=c99 -o hello hello_world.c
 *
 * TODO 练习：
 *   TODO-01: 修改程序，让用户输入名字，然后打印 "你好, [名字]!"
 *   TODO-02: 打印九九乘法表（提示：双层 for 循环）
 *   TODO-03: 计算 1+2+3+...+100 的和并打印结果
 */

#include <stdio.h>   /* 标准输入输出库：printf, scanf */
#include <stdlib.h>  /* 标准库：exit, malloc, free 等 */

/**
 * @brief 程序入口函数
 * @return int 0 表示正常退出，非0表示异常
 *
 * 注意：main 函数是每个 C 程序的起点，操作系统从这里开始执行。
 */
int main(void)
{
    /* ========== 1. 基础输出 ========== */
    printf("Hello, World!\n");          /* \n 是换行符 */
    printf("这是我的第一个C程序！\n");

    /* ========== 2. 格式化输出 ========== */
    int age = 25;
    float height = 1.75f;              /* f 后缀表示 float 类型 */
    char name[] = "嵌入式工程师";       /* 字符数组（字符串） */

    printf("姓名: %s\n", name);         /* %s 格式化字符串 */
    printf("年龄: %d 岁\n", age);       /* %d 格式化整数 */
    printf("身高: %.2f 米\n", height);  /* %.2f 保留2位小数 */

    /* ========== 3. 输入演示 ========== */
    int num;
    printf("\n请输入一个整数: ");
    scanf("%d", &num);                  /* & 是取地址运算符，scanf 需要变量的地址 */
    printf("你输入的是: %d\n", num);
    printf("它的两倍是: %d\n", num * 2);

    /* ========== 4. 简单运算 ========== */
    printf("\n--- 基础运算演示 ---\n");
    printf("10 + 3 = %d\n", 10 + 3);
    printf("10 - 3 = %d\n", 10 - 3);
    printf("10 * 3 = %d\n", 10 * 3);
    printf("10 / 3 = %d  (整数除法，结果截断)\n", 10 / 3);
    printf("10 %% 3 = %d  (%% 是取余运算符)\n", 10 % 3);  /* %% 打印 % 字符 */
    printf("10.0 / 3 = %.4f  (浮点除法)\n", 10.0 / 3);

    return 0;  /* 返回 0 表示程序正常结束 */
}

/*
 * ============================================================
 * TODO 练习区（完成后删除注释，填入代码）
 * ============================================================
 *
 * TODO-01: 打印九九乘法表
 *   提示：
 *     for (int i = 1; i <= 9; i++) {
 *         for (int j = 1; j <= i; j++) {
 *             printf("%d*%d=%-3d", j, i, i*j);
 *         }
 *         printf("\n");
 *     }
 *
 * TODO-02: 华氏温度转摄氏温度
 *   公式：C = (F - 32) * 5 / 9
 *   让用户输入华氏温度，输出摄氏温度
 *
 * TODO-03: 打印 ASCII 码表（可见字符部分，32~126）
 *   提示：char c = 32; printf("%c = %d\n", c, c);
 */
