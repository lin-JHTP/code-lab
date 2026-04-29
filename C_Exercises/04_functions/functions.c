/**
 * @file    functions.c
 * @brief   C 语言函数：定义、参数传递、递归、函数指针
 * @author  你的名字
 * @date    2026
 *
 * 本文件演示：
 *   1. 函数定义与声明（前向声明）
 *   2. 值传递 vs 指针传递
 *   3. 递归函数
 *   4. 函数指针（嵌入式回调函数核心！）
 *   5. 内联函数与 static 函数
 *
 * 编译命令：gcc -Wall -std=c99 -o functions functions.c
 */

#include <stdio.h>
#include <stdint.h>

/* ========== 前向声明（函数原型）========== */
/* 当函数定义在调用之后时，必须在前面声明原型 */
int add(int a, int b);
void swap_by_value(int a, int b);
void swap_by_pointer(int *a, int *b);
int factorial(int n);
int fibonacci(int n);

/* ========== 1. 基本函数 ========== */

/**
 * @brief 两数相加
 * @param a 第一个整数
 * @param b 第二个整数
 * @return 两数之和
 */
int add(int a, int b)
{
    return a + b;   /* 函数返回值 */
}

/**
 * @brief 求两个浮点数中的较大值
 * @note  使用 double 避免浮点精度问题
 */
double max_double(double a, double b)
{
    return (a > b) ? a : b;    /* 三元运算符：条件 ? 真值 : 假值 */
}

/* ========== 2. 值传递 vs 指针传递 ========== */

/**
 * @brief 值传递交换（无效！）
 * @note  形参是实参的副本，修改形参不影响实参
 */
void swap_by_value(int a, int b)
{
    int temp = a;
    a = b;
    b = temp;
    /* 函数返回后，实参的值不变 */
}

/**
 * @brief 指针传递交换（有效！）
 * @param a 指向第一个整数的指针
 * @param b 指向第二个整数的指针
 * @note  通过指针直接修改原变量，嵌入式中广泛使用
 */
void swap_by_pointer(int *a, int *b)
{
    int temp = *a;   /* *a 解引用：获取指针指向的值 */
    *a = *b;
    *b = temp;
}

/* ========== 3. 递归函数 ========== */

/**
 * @brief 递归求阶乘 n!
 * @param n 非负整数
 * @return n 的阶乘
 * @warning 递归深度过大会栈溢出！嵌入式中慎用
 */
int factorial(int n)
{
    if (n <= 1) {
        return 1;           /* 基准情况（递归终止条件） */
    }
    return n * factorial(n - 1);  /* 递归调用 */
}

/**
 * @brief 递归求斐波那契数列第n项
 * @note  这是低效实现（有大量重复计算），用于演示递归原理
 *        实际应使用迭代或记忆化递归
 */
int fibonacci(int n)
{
    if (n <= 0) return 0;
    if (n == 1) return 1;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

/* ========== 4. 函数指针（嵌入式回调函数！）========== */

/**
 * @brief 对数组每个元素执行操作的通用函数
 * @param arr    整数数组
 * @param len    数组长度
 * @param action 函数指针：对每个元素执行的操作
 *
 * 函数指针语法：返回类型 (*指针名)(参数类型列表)
 * 这里 action 是一个"接受int、返回int"的函数指针
 */
void apply_to_array(int arr[], int len, int (*action)(int))
{
    for (int i = 0; i < len; i++) {
        arr[i] = action(arr[i]);
    }
}

/* 可以作为 action 传入的函数 */
int double_it(int x)   { return x * 2; }
int square_it(int x)   { return x * x; }
int negate_it(int x)   { return -x; }

/* 用 typedef 简化函数指针类型（推荐！）*/
typedef void (*EventCallback_t)(uint8_t event_id, void *param);

/* 模拟事件处理器表（嵌入式中非常常见的模式）*/
void on_charge_start(uint8_t event_id, void *param)
{
    printf("事件处理: 充电开始 (id=%d)\n", event_id);
}

void on_charge_stop(uint8_t event_id, void *param)
{
    printf("事件处理: 充电停止 (id=%d)\n", event_id);
}

/* 事件处理器表：用数组存放函数指针 */
EventCallback_t event_handlers[] = {
    on_charge_start,   /* index 0 */
    on_charge_stop,    /* index 1 */
};

/* ========== 5. static 函数（仅本文件可见）========== */

/**
 * @brief 内部辅助函数，不对外暴露
 * @note  static 函数相当于"私有函数"，不会污染全局命名空间
 *        嵌入式模块化编程中强烈推荐！
 */
static int internal_helper(int x)
{
    return x + 100;
}

/* ========== main 函数 ========== */
int main(void)
{
    /* 1. 基本函数调用 */
    printf("===== 基本函数 =====\n");
    printf("add(3, 5) = %d\n", add(3, 5));
    printf("max(3.14, 2.71) = %.2f\n", max_double(3.14, 2.71));

    /* 2. 值传递 vs 指针传递 */
    printf("\n===== 参数传递 =====\n");
    int x = 10, y = 20;

    swap_by_value(x, y);
    printf("值传递后: x=%d, y=%d  (没变！)\n", x, y);

    swap_by_pointer(&x, &y);   /* 传递地址 */
    printf("指针传递后: x=%d, y=%d  (成功交换！)\n", x, y);

    /* 3. 递归 */
    printf("\n===== 递归 =====\n");
    printf("5! = %d\n", factorial(5));
    printf("fibonacci(10) = %d\n", fibonacci(10));

    /* 4. 函数指针 */
    printf("\n===== 函数指针 =====\n");
    int arr[] = {1, 2, 3, 4, 5};
    int len = 5;

    printf("原数组: ");
    for (int i = 0; i < len; i++) printf("%d ", arr[i]);
    printf("\n");

    apply_to_array(arr, len, double_it);
    printf("翻倍后: ");
    for (int i = 0; i < len; i++) printf("%d ", arr[i]);
    printf("\n");

    apply_to_array(arr, len, square_it);
    printf("平方后: ");
    for (int i = 0; i < len; i++) printf("%d ", arr[i]);
    printf("\n");

    /* 5. 事件处理器 */
    printf("\n===== 函数指针表（事件系统）=====\n");
    event_handlers[0](0, NULL);   /* 触发充电开始事件 */
    event_handlers[1](1, NULL);   /* 触发充电停止事件 */

    return 0;
}

/*
 * TODO 练习：
 *
 * TODO-01: 实现 power(base, exp) 函数（不使用 pow 库函数）
 *   用递归：power(2, 8) = 2 * power(2, 7)
 *   用迭代：for 循环累乘
 *
 * TODO-02: 实现冒泡排序函数 void bubble_sort(int *arr, int len)
 *   通过指针操作数组，排序结果反映到原数组
 *
 * TODO-03: 用函数指针实现一个简单的命令分发器
 *   输入字符串命令（如 "start", "stop", "status"），调用对应函数
 *
 * TODO-04: 实现汉诺塔（Tower of Hanoi）递归解法
 *   hanoi(n, 'A', 'C', 'B') 表示把n个盘从A借助B移到C
 *
 * TODO-05: 实现一个通用的比较+排序框架（类似 qsort）
 *   void my_sort(void *arr, int len, int elem_size, int (*cmp)(void*, void*))
 */
