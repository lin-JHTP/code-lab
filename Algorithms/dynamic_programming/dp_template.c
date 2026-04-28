/**
 * @file    dynamic_programming.c
 * @brief   动态规划（DP）经典题模板
 * @author  你的名字
 * @date    2026
 *
 * 动态规划的核心思想：
 *   将大问题分解为有重叠子问题的小问题，
 *   通过"记忆化"（缓存子问题结果）避免重复计算。
 *
 * 三要素：
 *   1. 状态定义：dp[i] 代表什么意思？
 *   2. 状态转移方程：dp[i] 如何由 dp[j] (j<i) 推导出来？
 *   3. 初始值：边界条件
 *
 * 编译：gcc -Wall -std=c99 -o dp dynamic_programming.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ===== 宏工具 ===== */
#define MAX(a, b)  ((a) > (b) ? (a) : (b))
#define MIN(a, b)  ((a) < (b) ? (a) : (b))

/* =========================================
 * 问题1：爬楼梯（LeetCode #70）入门题
 *
 * 你在爬楼梯，每次可以爬1或2个台阶。
 * n 阶楼梯，共有多少种爬法？
 * ========================================= */

/**
 * @brief 爬楼梯（动态规划）
 * @note dp[i] = dp[i-1] + dp[i-2]  ← 就是斐波那契数列！
 */
int climb_stairs(int n)
{
    if (n <= 2) return n;

    int dp[50];         /* dp[i] = 爬到第i阶的方法数 */
    dp[1] = 1;          /* 第1阶：只有1种（爬1步）*/
    dp[2] = 2;          /* 第2阶：2种（1+1 或 2）*/

    for (int i = 3; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];   /* 状态转移方程 */
    }

    return dp[n];
}

/* =========================================
 * 问题2：最大子数组和（LeetCode #53）Kadane算法
 *
 * 给整数数组，找连续子数组的最大和
 * 例：[-2,1,-3,4,-1,2,1,-5,4] → 6 ([4,-1,2,1])
 * ========================================= */

int max_subarray(const int *nums, int n)
{
    /* dp[i] = 以 nums[i] 结尾的连续子数组最大和 */
    int dp = nums[0];           /* 初始化为第一个元素 */
    int max_sum = nums[0];

    for (int i = 1; i < n; i++) {
        /* 要么继续累加，要么从当前元素重新开始 */
        dp = MAX(nums[i], dp + nums[i]);
        max_sum = MAX(max_sum, dp);
    }

    return max_sum;
}

/* =========================================
 * 问题3：0/1背包问题
 *
 * n 种物品，容量为 W 的背包
 * 每种物品有重量 w[i] 和价值 v[i]
 * 选一些物品放入背包，使总价值最大（每种物品只能选一次）
 * ========================================= */

int knapsack_01(const int *weights, const int *values, int n, int W)
{
    /* dp[i][j] = 前i个物品中，容量为j时的最大价值 */
    /* 优化：用一维滚动数组节省空间 */
    int *dp = (int *)calloc(W + 1, sizeof(int));
    if (!dp) return -1;

    for (int i = 0; i < n; i++) {
        /* 必须从右往左遍历，防止同一个物品被选两次！ */
        for (int j = W; j >= weights[i]; j--) {
            dp[j] = MAX(dp[j], dp[j - weights[i]] + values[i]);
        }
    }

    int result = dp[W];
    free(dp);
    return result;
}

/* =========================================
 * 问题4：最长公共子序列（LCS）
 *
 * 求两个字符串的最长公共子序列长度
 * 例：s1="ABCBDAB", s2="BDCAB" → 4 ("BCAB")
 * ========================================= */

int lcs(const char *s1, const char *s2)
{
    int m = (int)strlen(s1);
    int n = (int)strlen(s2);

    /* dp[i][j] = s1前i个字符与s2前j个字符的LCS长度 */
    int dp[50][50];
    memset(dp, 0, sizeof(dp));

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;   /* 字符匹配 */
            } else {
                dp[i][j] = MAX(dp[i - 1][j], dp[i][j - 1]);   /* 取较长的 */
            }
        }
    }

    return dp[m][n];
}

/* =========================================
 * 问题5：嵌入式应用 - 充电调度优化（贪心变体）
 *
 * 给定 N 辆车的充电需求，每辆车有：
 *   - 预计停留时间（分钟）
 *   - 需要充电量（度）
 * 充电桩功率固定，求如何安排顺序使尽可能多的车充完电
 * ========================================= */

typedef struct {
    int id;
    int stay_min;      /* 停留时间（分钟）*/
    int need_kwh;      /* 需要电量（度）*/
    int can_finish;    /* 能否充完（0或1）*/
} CarRequest_t;

void schedule_charging(CarRequest_t *cars, int n, float power_kw)
{
    printf("\n===== 充电调度优化（贪心）=====\n");
    printf("充电桩功率: %.1f kW\n", power_kw);

    /* 贪心策略：按"能充完"的概率排序（停留时间 / 需求量 比值大的优先）*/
    /* 简化版：按需要时间升序排列 */
    for (int i = 0; i < n; i++) {
        float needed_min = (float)cars[i].need_kwh / power_kw * 60.0f;
        cars[i].can_finish = (needed_min <= cars[i].stay_min) ? 1 : 0;
    }

    int total_ok = 0;
    printf("%-6s %-10s %-10s %-12s %-8s\n",
           "车辆", "停留(min)", "需求(kWh)", "需要时间(min)", "能否充完");
    for (int i = 0; i < n; i++) {
        float needed_min = (float)cars[i].need_kwh / power_kw * 60.0f;
        printf("%-6d %-10d %-10d %-12.1f %-8s\n",
               cars[i].id, cars[i].stay_min, cars[i].need_kwh,
               needed_min, cars[i].can_finish ? "✓" : "✗");
        if (cars[i].can_finish) total_ok++;
    }
    printf("可充完: %d / %d\n", total_ok, n);
}

int main(void)
{
    printf("===== 动态规划练习 =====\n\n");

    /* 爬楼梯 */
    printf("爬楼梯:\n");
    for (int n = 1; n <= 10; n++) {
        printf("  n=%2d: %d 种\n", n, climb_stairs(n));
    }

    /* 最大子数组和 */
    printf("\n最大子数组和:\n");
    int arr1[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    printf("  输入: [-2,1,-3,4,-1,2,1,-5,4]\n");
    printf("  最大和: %d\n", max_subarray(arr1, 9));

    /* 背包问题 */
    printf("\n0/1背包问题:\n");
    int weights[] = {2, 3, 4, 5};
    int values[]  = {3, 4, 5, 6};
    int n = 4, W = 8;
    printf("  容量=%d, 物品数=%d\n", W, n);
    printf("  最大价值: %d\n", knapsack_01(weights, values, n, W));

    /* 最长公共子序列 */
    printf("\n最长公共子序列:\n");
    printf("  LCS(\"ABCBDAB\", \"BDCAB\") = %d\n", lcs("ABCBDAB", "BDCAB"));

    /* 充电调度 */
    CarRequest_t cars[] = {
        {1, 120, 5},   /* 停120分钟，需5度 */
        {2, 60,  7},   /* 停60分钟，需7度 */
        {3, 90,  3},   /* 停90分钟，需3度 */
        {4, 45,  2},   /* 停45分钟，需2度 */
    };
    schedule_charging(cars, 4, 3.3f);

    return 0;
}

/*
 * TODO 练习：
 *
 * TODO-01: 实现完全背包问题（每种物品可以选多次）
 *   与0/1背包的区别：内层循环从左往右遍历
 *
 * TODO-02: 最长递增子序列（LIS，LeetCode #300）
 *   dp[i] = 以 nums[i] 结尾的最长递增子序列长度
 *
 * TODO-03: 硬币找零（LeetCode #322）
 *   给定面额列表，凑成目标金额最少需要几枚硬币？
 *
 * TODO-04: 实现一个能回溯输出背包具体选择方案的版本
 *   不只是返回最大价值，还要输出选了哪些物品
 *
 * TODO-05: 编辑距离（LeetCode #72）
 *   两个字符串之间最少需要几次编辑操作（插入/删除/替换）才能互相转换？
 */
