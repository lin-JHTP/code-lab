/**
 * @file    linked_list.c
 * @brief   链表实现：单链表、双向链表操作
 * @author  你的名字
 * @date    2026
 *
 * 链表是嵌入式开发中的重要数据结构：
 *   - 动态大小，不需要预分配固定内存
 *   - 插入/删除效率高 O(1)（知道位置时）
 *   - 随机访问慢 O(n)
 *
 * 嵌入式应用：
 *   - 事件队列（动态长度）
 *   - 任务调度链表
 *   - 配置参数链表
 *
 * 编译：gcc -Wall -std=c99 -o linked_list linked_list.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===== 单链表 ===== */

typedef struct SNode {
    int          val;
    struct SNode *next;
} SNode_t;

typedef struct {
    SNode_t *head;
    int      size;
} SList_t;

/* 创建新节点 */
static SNode_t *snode_new(int val)
{
    SNode_t *n = (SNode_t *)malloc(sizeof(SNode_t));
    if (!n) return NULL;
    n->val  = val;
    n->next = NULL;
    return n;
}

void slist_init(SList_t *list)
{
    list->head = NULL;
    list->size = 0;
}

/** @brief 头部插入 O(1) */
void slist_push_front(SList_t *list, int val)
{
    SNode_t *n = snode_new(val);
    if (!n) return;
    n->next    = list->head;
    list->head = n;
    list->size++;
}

/** @brief 尾部插入 O(n) */
void slist_push_back(SList_t *list, int val)
{
    SNode_t *n = snode_new(val);
    if (!n) return;

    if (!list->head) {
        list->head = n;
    } else {
        SNode_t *cur = list->head;
        while (cur->next) cur = cur->next;
        cur->next = n;
    }
    list->size++;
}

/** @brief 删除第一个值等于 val 的节点 */
int slist_remove(SList_t *list, int val)
{
    if (!list->head) return -1;

    /* 头节点就是目标 */
    if (list->head->val == val) {
        SNode_t *tmp = list->head;
        list->head = list->head->next;
        free(tmp);
        list->size--;
        return 0;
    }

    /* 找前驱节点 */
    SNode_t *prev = list->head;
    while (prev->next && prev->next->val != val) {
        prev = prev->next;
    }

    if (!prev->next) return -1;   /* 未找到 */

    SNode_t *tmp = prev->next;
    prev->next = tmp->next;
    free(tmp);
    list->size--;
    return 0;
}

/** @brief 反转链表 O(n) */
void slist_reverse(SList_t *list)
{
    SNode_t *prev = NULL;
    SNode_t *curr = list->head;

    while (curr) {
        SNode_t *next = curr->next;   /* 保存下一个节点 */
        curr->next = prev;             /* 反转指向 */
        prev = curr;
        curr = next;
    }
    list->head = prev;
}

void slist_print(const SList_t *list, const char *label)
{
    printf("%s (size=%d): ", label, list->size);
    SNode_t *cur = list->head;
    while (cur) {
        printf("%d", cur->val);
        if (cur->next) printf("→");
        cur = cur->next;
    }
    printf("→NULL\n");
}

void slist_free(SList_t *list)
{
    SNode_t *cur = list->head;
    while (cur) {
        SNode_t *next = cur->next;
        free(cur);
        cur = next;
    }
    list->head = NULL;
    list->size = 0;
}

/* ===== 检测链表中是否有环（Floyd算法）===== */

/**
 * @brief 快慢指针检测环
 * @return 1 有环；0 无环
 */
int slist_has_cycle(SNode_t *head)
{
    SNode_t *slow = head;   /* 慢指针：每次走1步 */
    SNode_t *fast = head;   /* 快指针：每次走2步 */

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) return 1;   /* 相遇 → 有环 */
    }
    return 0;
}

/* ===== 演示 ===== */
int main(void)
{
    printf("===== 单链表操作 =====\n");

    SList_t list;
    slist_init(&list);

    slist_push_back(&list, 1);
    slist_push_back(&list, 2);
    slist_push_back(&list, 3);
    slist_push_front(&list, 0);
    slist_print(&list, "初始");

    slist_remove(&list, 2);
    slist_print(&list, "删除2后");

    slist_reverse(&list);
    slist_print(&list, "反转后");

    slist_free(&list);
    printf("链表已释放\n");

    printf("\n===== 环检测 =====\n");
    SNode_t *n1 = snode_new(1);
    SNode_t *n2 = snode_new(2);
    SNode_t *n3 = snode_new(3);
    n1->next = n2; n2->next = n3; n3->next = NULL;
    printf("无环链表: %s\n", slist_has_cycle(n1) ? "有环" : "无环");

    n3->next = n1;   /* 制造环 */
    printf("有环链表: %s\n", slist_has_cycle(n1) ? "有环" : "无环");

    n3->next = NULL;   /* 解除环，避免 free 死循环 */
    free(n1); free(n2); free(n3);

    return 0;
}

/*
 * TODO 练习：
 *
 * TODO-01: 实现双向链表（每个节点有 prev 和 next 指针）
 *
 * TODO-02: 合并两个有序链表（LeetCode #21）
 *
 * TODO-03: 找链表中间节点（快慢指针）
 *   slow 走1步，fast 走2步，fast到末尾时 slow 在中间
 *
 * TODO-04: K 个一组反转链表（LeetCode #25）
 *
 * TODO-05: 用链表实现一个嵌入式 OS 就绪队列
 *   每个节点是一个任务 TCB（任务控制块）
 *   支持按优先级插入、取出最高优先级任务
 */
