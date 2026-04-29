/**
 * @file    binary_search.c
 * @brief   二分搜索及其变体
 * @author  你的名字
 * @date    2026
 *
 * 二分搜索：
 *   - 时间复杂度：O(log n)
 *   - 前提：数组必须有序
 *   - 嵌入式应用：在固化的查找表（ADC校准表、温度补偿表）中快速查找
 *
 * 编译：gcc -Wall -std=c99 -o binary_search binary_search.c
 */

#include <stdio.h>
#include <stdint.h>

/* ===== 版本1：基础二分搜索 ===== */

/**
 * @brief 在有序数组中查找 target
 * @return 找到则返回下标，否则返回 -1
 */
int binary_search(const int *arr, int n, int target)
{
    int low  = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;    /* 防止 (low+high) 溢出！ */

        if (arr[mid] == target) {
            return mid;              /* 找到！ */
        } else if (arr[mid] < target) {
            low = mid + 1;           /* 目标在右半部分 */
        } else {
            high = mid - 1;          /* 目标在左半部分 */
        }
    }
    return -1;                       /* 未找到 */
}

/* ===== 版本2：查找第一个 >= target 的位置（lower_bound）===== */

/**
 * @brief 查找第一个大于等于 target 的位置
 * @return 位置下标（若所有元素 < target，返回 n）
 * @note  适合嵌入式查找表的插值搜索
 */
int lower_bound(const int *arr, int n, int target)
{
    int low = 0, high = n;

    while (low < high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] < target) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}

/* ===== 版本3：在浮点范围内二分（求平方根）===== */

/**
 * @brief 用二分法求平方根（精度1e-6）
 * @param x 非负数
 * @return √x 的近似值
 */
double binary_search_sqrt(double x)
{
    if (x < 0) return -1.0;
    if (x == 0) return 0.0;

    double low  = (x < 1.0) ? x : 1.0;
    double high = (x < 1.0) ? 1.0 : x;

    while (high - low > 1e-9) {
        double mid = low + (high - low) / 2;
        if (mid * mid <= x) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

/* ===== 嵌入式应用：ADC 校准表查找 ===== */

/* ADC 原始值 → 实际电压（mV）的校准表 */
typedef struct {
    uint16_t adc_raw;      /* ADC 原始值（12位，0~4095）*/
    uint16_t voltage_mv;   /* 对应电压（mV）*/
} CalibPoint_t;

/* 校准点（ADC值必须递增排列！）*/
static const CalibPoint_t calib_table[] = {
    {0,    0},
    {400,  330},
    {800,  660},
    {1200, 990},
    {1600, 1320},
    {2000, 1650},
    {2400, 1980},
    {2800, 2310},
    {3200, 2640},
    {3600, 2970},
    {4000, 3300},
};

#define CALIB_SIZE (sizeof(calib_table) / sizeof(calib_table[0]))

/**
 * @brief 根据ADC原始值插值计算实际电压
 * @param adc_raw ADC原始值
 * @return 电压（mV），-1表示超出范围
 */
int adc_to_voltage(uint16_t adc_raw)
{
    /* 超出校准范围 */
    if (adc_raw < calib_table[0].adc_raw) return 0;
    if (adc_raw > calib_table[CALIB_SIZE - 1].adc_raw) return -1;

    /* 二分找到 adc_raw 所在区间 */
    int low = 0, high = (int)CALIB_SIZE - 1;
    while (low + 1 < high) {
        int mid = (low + high) / 2;
        if (calib_table[mid].adc_raw <= adc_raw) {
            low = mid;
        } else {
            high = mid;
        }
    }

    /* 线性插值 */
    int raw_range   = calib_table[high].adc_raw   - calib_table[low].adc_raw;
    int volt_range  = calib_table[high].voltage_mv - calib_table[low].voltage_mv;
    int raw_offset  = adc_raw - calib_table[low].adc_raw;

    return calib_table[low].voltage_mv + (volt_range * raw_offset) / raw_range;
}

int main(void)
{
    printf("===== 基础二分搜索 =====\n");

    int arr[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("数组: ");
    for (int i = 0; i < n; i++) printf("%d ", arr[i]);
    printf("\n");

    int targets[] = {7, 1, 19, 6, 20};
    for (int i = 0; i < 5; i++) {
        int idx = binary_search(arr, n, targets[i]);
        if (idx >= 0) {
            printf("查找 %2d: 下标 %d\n", targets[i], idx);
        } else {
            printf("查找 %2d: 未找到\n", targets[i]);
        }
    }

    printf("\n===== 平方根（二分法）=====\n");
    double vals[] = {2.0, 9.0, 0.25, 100.0};
    for (int i = 0; i < 4; i++) {
        printf("sqrt(%.2f) = %.6f\n", vals[i], binary_search_sqrt(vals[i]));
    }

    printf("\n===== ADC 校准表查找（嵌入式应用）=====\n");
    uint16_t adc_vals[] = {0, 500, 1000, 2048, 3500, 4000};
    for (int i = 0; i < 6; i++) {
        int v = adc_to_voltage(adc_vals[i]);
        printf("ADC=%4u → %4d mV (%.2f V)\n", adc_vals[i], v, v / 1000.0);
    }

    return 0;
}

/*
 * TODO 练习：
 *
 * TODO-01: 实现 upper_bound（找最后一个 <= target 的位置）
 *
 * TODO-02: 在旋转有序数组中搜索（LeetCode #33）
 *   [4,5,6,7,0,1,2] 中搜索 0 → 返回4
 *
 * TODO-03: 实现"峰值查找"（LeetCode #162）
 *   找到一个局部最大值（左右都比它小的元素）
 *
 * TODO-04: 用二分实现嵌入式温度传感器的非线性补偿
 *   给定一组 {adc_value, temperature} 校准点，根据ADC值插值得出温度
 *
 * TODO-05: 求两个有序数组的中位数（LeetCode #4，Hard）
 *   时间复杂度 O(log(m+n))
 */
