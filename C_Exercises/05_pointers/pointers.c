/**
 * @file    pointers.c
 * @brief   C 语言指针精讲：嵌入式开发核心技能
 * @author  你的名字
 * @date    2026
 *
 * 本文件演示：
 *   1. 指针基础：地址、解引用
 *   2. 指针与数组
 *   3. 指针与字符串
 *   4. 多级指针
 *   5. const 指针
 *   6. 函数指针（复习）
 *   7. 嵌入式寄存器地址映射
 *   8. volatile 关键字（嵌入式必备！）
 *
 * 编译命令：gcc -Wall -std=c99 -o pointers pointers.c
 *
 * ⚠️ 指针是C语言中最强大也最危险的特性，使用时必须：
 *   - 确保指针已初始化（非野指针）
 *   - 不要访问已释放的内存
 *   - 注意数组越界
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main(void)
{
    /* ========== 1. 指针基础 ========== */
    printf("===== 1. 指针基础 =====\n");

    int num = 42;
    int *p = &num;    /* p 是指向 int 的指针，& 取 num 的地址 */

    printf("num 的值:  %d\n", num);
    printf("num 的地址: %p\n", (void *)&num);  /* %p 打印地址 */
    printf("p 的值:    %p\n", (void *)p);       /* p 存储的就是 num 的地址 */
    printf("*p 的值:   %d\n", *p);              /* * 解引用：获取 p 指向的值 */

    *p = 100;          /* 通过指针修改 num 的值 */
    printf("通过指针修改后，num = %d\n", num);

    /* 指针本身也有地址 */
    printf("p 自身的地址: %p\n", (void *)&p);
    printf("指针大小: %zu 字节（64位系统为8字节）\n", sizeof(p));

    /* ========== 2. 指针与数组 ========== */
    printf("\n===== 2. 指针与数组 =====\n");

    int arr[] = {10, 20, 30, 40, 50};
    int *pa = arr;     /* 数组名本身就是首元素的地址，不需要 & */

    printf("arr[0] = %d,  *pa = %d\n", arr[0], *pa);
    printf("arr[1] = %d,  *(pa+1) = %d\n", arr[1], *(pa + 1));  /* 指针算术 */

    /* 指针遍历数组（比下标访问稍快，嵌入式常见）*/
    printf("用指针遍历数组: ");
    for (int *ptr = arr; ptr < arr + 5; ptr++) {
        printf("%d ", *ptr);
    }
    printf("\n");

    /* 两种等价的写法 */
    printf("arr[2] = %d  等价于  *(arr+2) = %d\n", arr[2], *(arr + 2));

    /* ========== 3. 指针与字符串 ========== */
    printf("\n===== 3. 指针与字符串 =====\n");

    char str[] = "Hello, 嵌入式!";    /* 字符数组（可修改）*/
    char *ps = str;                    /* 指向字符串首字符 */

    printf("字符串: %s\n", ps);
    printf("第一个字符: %c\n", *ps);
    printf("字符串长度: %zu\n", strlen(str));

    /* 指针遍历字符串 */
    printf("逐字符打印: ");
    for (char *c = str; *c != '\0'; c++) {   /* '\0' 是字符串结束符 */
        if (*c > 0) printf("%c", *c);         /* 只打印ASCII可见字符 */
    }
    printf("\n");

    /* ========== 4. const 指针 ========== */
    printf("\n===== 4. const 指针 =====\n");

    int val = 10;
    int val2 = 20;

    /* const int *p：指向const int，不能通过指针修改值，但可以改变指向 */
    const int *pc1 = &val;
    /* *pc1 = 20;  // 错误！不能修改值 */
    pc1 = &val2;   /* 正确，可以改变指向 */
    printf("pc1 现在指向: %d\n", *pc1);

    /* int * const p：const指针，可以修改值，但不能改变指向 */
    int * const pc2 = &val;
    *pc2 = 99;     /* 正确，可以修改值 */
    /* pc2 = &val2;  // 错误！不能改变指向 */
    printf("通过const指针修改后 val = %d\n", val);

    /* const int * const p：既不能改值也不能改指向（最严格，嵌入式只读寄存器）*/
    const int * const pc3 = &val2;
    printf("双const指针: %d\n", *pc3);

    /* ========== 5. 二级指针（指针的指针）========== */
    printf("\n===== 5. 二级指针 =====\n");

    int x = 5;
    int *p1 = &x;
    int **pp = &p1;   /* pp 是指向指针的指针 */

    printf("x = %d\n", x);
    printf("*p1 = %d\n", *p1);
    printf("**pp = %d\n", **pp);    /* 两次解引用 */

    **pp = 999;        /* 通过二级指针修改 x */
    printf("通过 **pp 修改后 x = %d\n", x);

    /* ========== 6. 嵌入式：寄存器地址映射（重要！）========== */
    printf("\n===== 6. 嵌入式寄存器操作模拟 =====\n");

    /*
     * 在真实嵌入式代码中，寄存器地址是固定的，如：
     * #define GPIOA_ODR  (*(volatile uint32_t *)0x4001080C)
     *
     * 这里用局部变量模拟寄存器
     */
    uint32_t fake_gpio_odr = 0x00000000;  /* 模拟 GPIO 输出寄存器 */

    /* 通过指针操作，就像操作真实寄存器 */
    volatile uint32_t *gpio_odr = &fake_gpio_odr;

    *gpio_odr |=  (1 << 5);   /* 置位 bit5：点亮 LED */
    printf("GPIO ODR 置位后: 0x%08X\n", fake_gpio_odr);

    *gpio_odr &= ~(1 << 5);   /* 清位 bit5：熄灭 LED */
    printf("GPIO ODR 清位后: 0x%08X\n", fake_gpio_odr);

    /* ========== 7. 空指针与野指针警告 ========== */
    printf("\n===== 7. 指针安全 =====\n");

    int *null_ptr = NULL;   /* 空指针：明确表示"不指向任何东西" */

    /* 使用前必须检查！*/
    if (null_ptr != NULL) {
        printf("指针有效: %d\n", *null_ptr);
    } else {
        printf("指针为 NULL，安全跳过\n");
    }

    /* 野指针示例（危险！仅演示概念，不要取消注释运行）*/
    /*
     * int *wild;       // 未初始化的指针 = 野指针！
     * *wild = 42;      // 未定义行为，可能崩溃！
     *
     * int *dangling;
     * { int temp = 5; dangling = &temp; }
     * // 离开作用域后 temp 不存在，dangling 变成悬空指针
     * printf("%d", *dangling);  // 危险！
     */

    return 0;
}

/*
 * TODO 练习（指针是嵌入式核心，必须熟练！）
 *
 * TODO-01: 实现 my_strlen(const char *s)，不使用 strlen 库函数
 *   从 s 开始遍历，直到 '\0'，统计字符数
 *
 * TODO-02: 实现 my_strcpy(char *dest, const char *src)
 *   用指针逐字符复制，最后要加 '\0'
 *
 * TODO-03: 实现 my_memset(void *ptr, int val, int size)
 *   将 ptr 指向的内存块全部设为 val（嵌入式初始化内存常用）
 *
 * TODO-04: 实现 my_memcpy(void *dest, const void *src, int size)
 *   注意：当 dest 和 src 有重叠时要处理（memmove 的功能）
 *
 * TODO-05: 用指针实现矩阵乘法（二维数组通过指针传递）
 *
 * TODO-06（嵌入式）: 模拟一个 STM32 风格的外设寄存器结构体
 *   typedef struct {
 *     volatile uint32_t MODER;   // 模式寄存器
 *     volatile uint32_t OTYPER;  // 输出类型寄存器
 *     volatile uint32_t ODR;     // 输出数据寄存器
 *   } GPIO_TypeDef;
 *   GPIO_TypeDef *GPIOA = (GPIO_TypeDef *)0x40020000;
 *
 * TODO-07: 实现一个通用链表节点，用二级指针实现头插法
 */
