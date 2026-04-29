/**
 * @file    dynamic_memory.c
 * @brief   C 语言动态内存管理：malloc / calloc / realloc / free
 * @author  你的名字
 * @date    2026
 *
 * 本文件演示：
 *   1. malloc / calloc / realloc / free 基本用法
 *   2. 动态数组
 *   3. 动态链表
 *   4. 内存泄漏的危险与防范
 *   5. 嵌入式内存池（避免动态分配的替代方案）
 *
 * 编译命令：gcc -Wall -std=c99 -o dynamic_memory dynamic_memory.c
 *
 * ⚠️ 嵌入式注意：
 *   - 许多嵌入式环境（如无OS的MCU）不推荐使用 malloc，原因：
 *     1. 堆空间有限，可能导致分配失败
 *     2. 内存碎片化
 *     3. 不可预测的时序
 *   - 替代方案：静态内存池（见本文件末）
 */

#include <stdio.h>
#include <stdlib.h>   /* malloc, calloc, realloc, free */
#include <string.h>
#include <stdint.h>

/* ========== 1. 基本动态内存分配 ========== */
void demo_basic_alloc(void)
{
    printf("===== 1. 基本动态内存分配 =====\n");

    /* malloc：分配指定字节数，内容未初始化 */
    int *arr = (int *)malloc(5 * sizeof(int));
    if (arr == NULL) {
        printf("内存分配失败！\n");
        return;
    }

    /* 必须检查 NULL！分配失败时 malloc 返回 NULL */
    for (int i = 0; i < 5; i++) {
        arr[i] = (i + 1) * 10;
    }
    printf("malloc 分配的数组: ");
    for (int i = 0; i < 5; i++) printf("%d ", arr[i]);
    printf("\n");

    free(arr);         /* 释放！必须配对使用 */
    arr = NULL;        /* 防野指针：释放后设为NULL */

    /* calloc：分配并清零（更安全）*/
    double *matrix = (double *)calloc(3 * 3, sizeof(double));
    if (matrix == NULL) return;

    printf("calloc 分配的3x3矩阵（全0）:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%.1f ", matrix[i * 3 + j]);
        }
        printf("\n");
    }
    free(matrix);
    matrix = NULL;

    /* realloc：重新分配（扩大或缩小）*/
    int *dyn = (int *)malloc(3 * sizeof(int));
    if (!dyn) return;
    for (int i = 0; i < 3; i++) dyn[i] = i;

    /* 扩大到5个元素 */
    int *new_dyn = (int *)realloc(dyn, 5 * sizeof(int));
    if (new_dyn == NULL) {
        free(dyn);   /* realloc 失败时，原指针仍有效 */
        return;
    }
    dyn = new_dyn;   /* 注意：realloc 可能返回新地址！ */
    dyn[3] = 3;
    dyn[4] = 4;

    printf("realloc 扩展后: ");
    for (int i = 0; i < 5; i++) printf("%d ", dyn[i]);
    printf("\n");

    free(dyn);
    dyn = NULL;
}

/* ========== 2. 动态单向链表 ========== */

typedef struct Node {
    int          data;
    struct Node *next;
} Node_t;

/** @brief 创建新节点 */
Node_t *node_create(int data)
{
    Node_t *node = (Node_t *)malloc(sizeof(Node_t));
    if (node == NULL) return NULL;
    node->data = data;
    node->next = NULL;
    return node;
}

/** @brief 头插法添加节点 */
void list_push_front(Node_t **head, int data)
{
    Node_t *new_node = node_create(data);
    if (!new_node) return;
    new_node->next = *head;
    *head = new_node;
}

/** @brief 尾插法添加节点 */
void list_push_back(Node_t **head, int data)
{
    Node_t *new_node = node_create(data);
    if (!new_node) return;

    if (*head == NULL) {
        *head = new_node;
        return;
    }

    Node_t *cur = *head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = new_node;
}

/** @brief 打印链表 */
void list_print(Node_t *head)
{
    printf("链表: ");
    Node_t *cur = head;
    while (cur != NULL) {
        printf("%d", cur->data);
        if (cur->next) printf(" → ");
        cur = cur->next;
    }
    printf(" → NULL\n");
}

/** @brief 释放整个链表（必须！否则内存泄漏）*/
void list_free(Node_t **head)
{
    Node_t *cur = *head;
    while (cur != NULL) {
        Node_t *next = cur->next;
        free(cur);
        cur = next;
    }
    *head = NULL;
    printf("链表已释放\n");
}

void demo_linked_list(void)
{
    printf("\n===== 2. 动态链表 =====\n");

    Node_t *head = NULL;

    list_push_back(&head, 10);
    list_push_back(&head, 20);
    list_push_back(&head, 30);
    list_push_front(&head, 5);

    list_print(head);
    list_free(&head);
}

/* ==========================================
 * 3. 嵌入式内存池（Static Memory Pool）
 * 嵌入式替代 malloc 的安全方案！
 * ========================================== */

#define POOL_SIZE    16     /* 池中最多管理16个节点 */
#define NODE_SIZE    32     /* 每个节点32字节 */

typedef struct {
    uint8_t  mem[POOL_SIZE][NODE_SIZE];  /* 静态内存块 */
    uint8_t  used[POOL_SIZE];            /* 使用标记：0=空闲，1=已用 */
    int      total;                       /* 总数量 */
    int      available;                   /* 可用数量 */
} MemPool_t;

static MemPool_t g_mem_pool;

/** @brief 初始化内存池 */
void pool_init(void)
{
    memset(&g_mem_pool, 0, sizeof(g_mem_pool));
    g_mem_pool.total     = POOL_SIZE;
    g_mem_pool.available = POOL_SIZE;
    printf("内存池初始化: 共 %d 块，每块 %d 字节\n",
           POOL_SIZE, NODE_SIZE);
}

/** @brief 从池中分配一块内存 */
void *pool_alloc(void)
{
    for (int i = 0; i < POOL_SIZE; i++) {
        if (!g_mem_pool.used[i]) {
            g_mem_pool.used[i] = 1;
            g_mem_pool.available--;
            return g_mem_pool.mem[i];
        }
    }
    printf("内存池已满！\n");
    return NULL;
}

/** @brief 释放一块内存回池 */
void pool_free(void *ptr)
{
    for (int i = 0; i < POOL_SIZE; i++) {
        if (ptr == g_mem_pool.mem[i]) {
            memset(g_mem_pool.mem[i], 0, NODE_SIZE);  /* 清零 */
            g_mem_pool.used[i] = 0;
            g_mem_pool.available++;
            return;
        }
    }
    printf("警告：释放了不属于本内存池的地址！\n");
}

void demo_memory_pool(void)
{
    printf("\n===== 3. 嵌入式内存池 =====\n");

    pool_init();

    void *p1 = pool_alloc();
    void *p2 = pool_alloc();
    void *p3 = pool_alloc();

    printf("分配3块后，剩余: %d 块\n", g_mem_pool.available);

    /* 写入数据测试 */
    if (p1) {
        snprintf((char *)p1, NODE_SIZE, "数据块1: 充电电流=16A");
        printf("%s\n", (char *)p1);
    }

    pool_free(p1);
    pool_free(p2);
    pool_free(p3);
    printf("释放3块后，剩余: %d 块\n", g_mem_pool.available);
}

int main(void)
{
    demo_basic_alloc();
    demo_linked_list();
    demo_memory_pool();
    return 0;
}

/*
 * TODO 练习：
 *
 * TODO-01: 实现动态字符串库
 *   typedef struct { char *data; int len; int capacity; } DynStr_t;
 *   实现 dynstr_append、dynstr_printf 等函数
 *
 * TODO-02: 实现支持动态扩容的向量（类似 C++ vector）
 *   当容量不足时，自动 realloc 到原来的2倍
 *
 * TODO-03: 实现内存泄漏检测器（简版）
 *   包装 malloc/free，记录每次分配和释放
 *   程序结束时报告未释放的内存
 *
 * TODO-04: 扩展内存池，支持不同大小的内存块
 *   小块池(16B)、中块池(64B)、大块池(256B)
 *   根据申请大小自动选择合适的池
 *
 * TODO-05: 用动态链表实现 LRU 缓存（Least Recently Used）
 *   适合嵌入式中缓存频繁访问的配置参数
 */
