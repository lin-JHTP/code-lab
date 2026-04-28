/**
 * @file    arrays_strings.c
 * @brief   C 语言数组与字符串处理
 * @author  你的名字
 * @date    2026
 *
 * 本文件演示：
 *   1. 一维数组：声明、初始化、遍历
 *   2. 二维数组（矩阵）
 *   3. 字符串：char 数组与字符串字面量
 *   4. 字符串处理函数（strlen, strcpy, strcat, strcmp, sprintf）
 *   5. 嵌入式：数据包解析（实用！）
 *
 * 编译命令：gcc -Wall -std=c99 -o arrays_strings arrays_strings.c
 */

#include <stdio.h>
#include <string.h>   /* strlen, strcpy, strcat, strcmp, strstr */
#include <stdint.h>
#include <ctype.h>    /* toupper, tolower, isdigit, isalpha */

/* ========== 1. 一维数组 ========== */
void demo_arrays(void)
{
    printf("===== 1. 一维数组 =====\n");

    /* 声明并初始化 */
    int scores[5] = {85, 92, 78, 96, 88};

    /* 遍历 */
    int sum = 0;
    printf("成绩: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", scores[i]);
        sum += scores[i];
    }
    printf("\n平均分: %.1f\n", (float)sum / 5);

    /* 未完全初始化：剩余元素自动为0 */
    int arr[10] = {1, 2, 3};    /* arr[3]~arr[9] 全为 0 */
    printf("部分初始化数组的第5个元素: %d\n", arr[4]);  /* 输出 0 */

    /* sizeof 获取数组长度 */
    int len = sizeof(scores) / sizeof(scores[0]);  /* 推荐写法 */
    printf("数组长度: %d\n", len);
}

/* ========== 2. 二维数组（矩阵）========== */
void demo_2d_arrays(void)
{
    printf("\n===== 2. 二维数组 =====\n");

    /* 3x3 矩阵 */
    int matrix[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    printf("矩阵:\n");
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%3d", matrix[i][j]);
        }
        printf("\n");
    }

    /* 计算对角线之和 */
    int diag_sum = 0;
    for (int i = 0; i < 3; i++) {
        diag_sum += matrix[i][i];
    }
    printf("主对角线之和: %d\n", diag_sum);
}

/* ========== 3. 字符串 ========== */
void demo_strings(void)
{
    printf("\n===== 3. 字符串 =====\n");

    /* 字符数组（可修改）vs 字符串字面量（常量，不可修改）*/
    char buf[64] = "Hello";    /* 字符数组，栈上分配，可修改 */

    printf("原字符串: %s\n", buf);
    printf("长度: %zu\n", strlen(buf));

    /* 字符串拼接 */
    strcat(buf, ", World!");    /* 注意：buf 需要足够大！ */
    printf("拼接后: %s\n", buf);

    /* 字符串比较 */
    char s1[] = "apple";
    char s2[] = "banana";
    int cmp = strcmp(s1, s2);  /* <0 表示s1<s2，0表示相等，>0表示s1>s2 */
    printf("strcmp(\"%s\", \"%s\") = %d\n", s1, s2, cmp);

    /* 字符串查找 */
    char sentence[] = "The quick brown fox";
    char *pos = strstr(sentence, "quick");
    if (pos != NULL) {
        printf("找到 \"quick\" 在位置: %ld\n", pos - sentence);
    }

    /* sprintf：格式化到字符串（嵌入式中用于构造显示字符串）*/
    char display[64];
    float voltage = 220.5f;
    int current = 16;
    sprintf(display, "电压: %.1fV, 电流: %dA", voltage, current);
    printf("格式化字符串: %s\n", display);
}

/* ========== 4. 字符处理 ========== */
void demo_char_operations(void)
{
    printf("\n===== 4. 字符操作 =====\n");

    char str[] = "Hello World 123";

    /* 转大写 */
    char upper[64];
    for (int i = 0; str[i]; i++) {
        upper[i] = (char)toupper(str[i]);
    }
    upper[strlen(str)] = '\0';
    printf("转大写: %s\n", upper);

    /* 统计字母、数字、空格 */
    int letters = 0, digits = 0, spaces = 0;
    for (int i = 0; str[i]; i++) {
        if (isalpha(str[i])) letters++;
        else if (isdigit(str[i])) digits++;
        else if (str[i] == ' ') spaces++;
    }
    printf("字母: %d, 数字: %d, 空格: %d\n", letters, digits, spaces);
}

/* ========== 5. 嵌入式实用：数据帧解析 ========== */

/* 模拟一个简单的数据帧格式：
 * [帧头 0xAA][长度 N][数据 N字节][校验 XOR]
 */
#define FRAME_HEADER 0xAA

typedef struct {
    uint8_t header;         /* 帧头 */
    uint8_t length;         /* 数据长度 */
    uint8_t data[16];       /* 数据内容 */
    uint8_t checksum;       /* 校验和（XOR）*/
} Frame_t;

/**
 * @brief 计算数组的 XOR 校验
 */
uint8_t calc_xor(const uint8_t *data, uint8_t len)
{
    uint8_t xor = 0;
    for (uint8_t i = 0; i < len; i++) {
        xor ^= data[i];
    }
    return xor;
}

/**
 * @brief 解析数据帧
 * @return 0 成功，-1 帧头错误，-2 校验错误
 */
int parse_frame(const uint8_t *raw_buf, int buf_len, Frame_t *frame)
{
    if (buf_len < 3) return -1;

    if (raw_buf[0] != FRAME_HEADER) {
        printf("帧头错误: 0x%02X\n", raw_buf[0]);
        return -1;
    }

    frame->header = raw_buf[0];
    frame->length = raw_buf[1];

    if (buf_len < frame->length + 3) return -1;

    memcpy(frame->data, &raw_buf[2], frame->length);

    uint8_t expected_xor = calc_xor(&raw_buf[1], frame->length + 1);
    frame->checksum = raw_buf[2 + frame->length];

    if (frame->checksum != expected_xor) {
        printf("校验错误: 期望 0x%02X，实际 0x%02X\n",
               expected_xor, frame->checksum);
        return -2;
    }

    return 0;
}

void demo_frame_parsing(void)
{
    printf("\n===== 5. 数据帧解析（嵌入式实用）=====\n");

    /* 构造一个测试帧：[AA][03][01 02 03][校验] */
    uint8_t data[] = {0x01, 0x02, 0x03};
    uint8_t len = 3;
    uint8_t xor = calc_xor(&len, 1);
    for (int i = 0; i < len; i++) xor ^= data[i];

    uint8_t raw_buf[] = {FRAME_HEADER, len, 0x01, 0x02, 0x03, xor};

    printf("原始数据: ");
    for (int i = 0; i < (int)sizeof(raw_buf); i++) {
        printf("0x%02X ", raw_buf[i]);
    }
    printf("\n");

    Frame_t frame;
    int ret = parse_frame(raw_buf, sizeof(raw_buf), &frame);

    if (ret == 0) {
        printf("解析成功！长度=%d, 数据: ", frame.length);
        for (int i = 0; i < frame.length; i++) {
            printf("0x%02X ", frame.data[i]);
        }
        printf("\n");
    }
}

int main(void)
{
    demo_arrays();
    demo_2d_arrays();
    demo_strings();
    demo_char_operations();
    demo_frame_parsing();
    return 0;
}

/*
 * TODO 练习：
 *
 * TODO-01: 实现字符串反转（不使用库函数）
 *   void reverse_str(char *s) { ... }
 *
 * TODO-02: 判断字符串是否为回文（如 "racecar", "level"）
 *
 * TODO-03: 去除字符串首尾空格（trim 函数）
 *
 * TODO-04: 实现字符串分割函数（类似 Python 的 split）
 *   void split(const char *str, char delim, char result[][64], int *count)
 *
 * TODO-05: 实现一个16进制字符串转字节数组的函数
 *   "DEADBEEF" → {0xDE, 0xAD, 0xBE, 0xEF}（解析十六进制协议数据常用）
 *
 * TODO-06: 用二维字符数组实现一个名字列表，并按字母序排序
 *
 * TODO-07（嵌入式）: 实现一个完整的 AT 命令解析器
 *   接收形如 "AT+BAUD=9600\r\n" 的字符串
 *   解析出命令名("BAUD")和参数("9600")
 */
