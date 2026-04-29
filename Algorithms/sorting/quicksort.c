/**
 * @file    quicksort.c
 * @brief   快速排序（Quick Sort）实现 - 分治法经典案例
 * @author  你的名字
 * @date    2026
 *
 * 快速排序：
 *   - 平均时间复杂度：O(n log n)
 *   - 最坏时间复杂度：O(n²)（已排序数组）
 *   - 空间复杂度：O(log n)（递归栈）
 *   - 不稳定排序
 *
 * 思想：选一个"基准"（pivot），把数组分成"比pivot小"和"比pivot大"两部分，
 *       然后递归地对两部分继续排序。
 *
 * 编译：gcc -Wall -std=c99 -o quicksort quicksort.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* ===== 工具函数 ===== */
void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void print_array(const int *arr, int n, const char *label)
{
    printf("%s: [", label);
    for (int i = 0; i < n; i++) {
        printf("%d%s", arr[i], (i < n - 1) ? ", " : "");
    }
    printf("]\n");
}

/* ===== 版本1：基础快速排序（Lomuto 分区）===== */

/**
 * @brief Lomuto 分区：选最后一个元素作为 pivot
 * @return 分区后 pivot 的最终位置
 */
int partition_lomuto(int *arr, int low, int high)
{
    int pivot = arr[high];    /* 选最后一个元素作为基准 */
    int i = low - 1;          /* i 指向"小于pivot区域"的边界 */

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);   /* 小于等于pivot的移到左边 */
        }
    }
    swap(&arr[i + 1], &arr[high]);    /* pivot 放到最终位置 */
    return i + 1;
}

void quick_sort(int *arr, int low, int high)
{
    if (low < high) {
        int pi = partition_lomuto(arr, low, high);  /* 分区 */
        quick_sort(arr, low, pi - 1);               /* 递归左半部分 */
        quick_sort(arr, pi + 1, high);              /* 递归右半部分 */
    }
}

/* ===== 版本2：三路快排（处理大量重复元素效率更高）===== */

/**
 * @brief 三路分区：将数组分为"小于"、"等于"、"大于" pivot 三部分
 * @note  当有大量重复元素时，性能远好于普通快排
 */
void quick_sort_3way(int *arr, int low, int high)
{
    if (low >= high) return;

    int pivot = arr[low];
    int lt = low;         /* arr[low..lt-1]  < pivot */
    int gt = high;        /* arr[gt+1..high] > pivot */
    int i  = low + 1;     /* arr[lt..i-1]   == pivot */

    while (i <= gt) {
        if (arr[i] < pivot) {
            swap(&arr[lt], &arr[i]);
            lt++;
            i++;
        } else if (arr[i] > pivot) {
            swap(&arr[i], &arr[gt]);
            gt--;
        } else {
            i++;
        }
    }

    quick_sort_3way(arr, low, lt - 1);
    quick_sort_3way(arr, gt + 1, high);
}

/* ===== 版本3：随机化快排（避免最坏情况）===== */

void quick_sort_random(int *arr, int low, int high)
{
    if (low < high) {
        /* 随机选 pivot，与最后一个元素交换 */
        int rand_idx = low + rand() % (high - low + 1);
        swap(&arr[rand_idx], &arr[high]);

        int pi = partition_lomuto(arr, low, high);
        quick_sort_random(arr, low, pi - 1);
        quick_sort_random(arr, pi + 1, high);
    }
}

/* 用于 stdlib qsort 的比较函数（必须在文件作用域定义）*/
static int cmp_int(const void *a, const void *b)
{
    return *(const int *)a - *(const int *)b;
}

/* ===== 性能测试 ===== */
void benchmark_sort(int n)
{
    printf("\n===== 性能测试（n=%d）=====\n", n);

    int *arr1 = (int *)malloc(n * sizeof(int));
    int *arr2 = (int *)malloc(n * sizeof(int));

    /* 生成随机数据 */
    srand(42);
    for (int i = 0; i < n; i++) {
        arr1[i] = rand() % 10000;
    }
    memcpy(arr2, arr1, n * sizeof(int));

    /* 测试 quick_sort */
    clock_t t0 = clock();
    quick_sort(arr1, 0, n - 1);
    double t_qsort = (double)(clock() - t0) / CLOCKS_PER_SEC * 1000;

    /* 测试 stdlib qsort */
    t0 = clock();
    qsort(arr2, n, sizeof(int), cmp_int);
    double t_stdlib = (double)(clock() - t0) / CLOCKS_PER_SEC * 1000;

    printf("自实现快排: %.2fms\n", t_qsort);
    printf("stdlib qsort: %.2fms\n", t_stdlib);

    /* 验证正确性 */
    int correct = 1;
    for (int i = 0; i < n; i++) {
        if (arr1[i] != arr2[i]) { correct = 0; break; }
    }
    printf("排序结果: %s\n", correct ? "✓ 正确" : "✗ 错误！");

    free(arr1);
    free(arr2);
}

int main(void)
{
    srand((unsigned)time(NULL));

    /* 基础演示 */
    int arr[] = {64, 34, 25, 12, 22, 11, 90, 7};
    int n = sizeof(arr) / sizeof(arr[0]);

    print_array(arr, n, "排序前");
    quick_sort(arr, 0, n - 1);
    print_array(arr, n, "快排后");

    /* 三路快排（含重复元素）*/
    int arr2[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    print_array(arr2, n2, "\n三路快排前");
    quick_sort_3way(arr2, 0, n2 - 1);
    print_array(arr2, n2, "三路快排后");

    /* 性能测试 */
    benchmark_sort(100000);

    return 0;
}

/*
 * TODO 练习：
 *
 * TODO-01: 实现归并排序（Merge Sort）
 *   思想：分成两半，递归排序，然后合并两个有序数组
 *   时间：O(n log n)，稳定排序，但需要 O(n) 额外空间
 *
 * TODO-02: 实现堆排序（Heap Sort）
 *   先建最大堆，再反复把堆顶（最大值）与末尾交换
 *
 * TODO-03: 实现计数排序（Counting Sort）
 *   适用于整数范围不大的场景，时间 O(n+k)
 *
 * TODO-04: 对比各种排序的速度（绘制折线图，用Python+matplotlib）
 *
 * TODO-05: 快排优化：当子数组长度 < 10 时改用插入排序（工程实践）
 */
