/**
 * @file    file_io.c
 * @brief   C 语言文件 I/O 操作（标准 C 文件读写）
 * @author  你的名字
 * @date    2026
 *
 * 本文件演示：
 *   1. fopen/fclose/fread/fwrite
 *   2. fprintf/fscanf 格式化文件 I/O
 *   3. 文件复制
 *   4. 嵌入式：EEPROM 数据持久化模拟
 *
 * 编译：gcc -Wall -std=c99 -o file_io file_io.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* ===== 1. 写入文本文件 ===== */
void demo_write_text(void)
{
    printf("===== 文本文件写入 =====\n");

    FILE *fp = fopen("/tmp/charger_log.txt", "w");
    if (fp == NULL) {
        perror("无法打开文件");   /* 打印系统错误信息 */
        return;
    }

    fprintf(fp, "充电桩日志\n");
    fprintf(fp, "时间,枪号,事件,电压(V),电流(A)\n");

    /* 写入几条记录 */
    struct { int gun; const char *event; int v; int a; } logs[] = {
        {1, "插枪",   0,   0},
        {1, "开始充电", 220, 16},
        {1, "充电中", 220, 16},
        {1, "充电完成", 220, 0},
        {1, "拔枪",   0,   0},
    };

    for (int i = 0; i < 5; i++) {
        fprintf(fp, "2026-04-28 10:%02d:00,枪%d,%s,%d,%d\n",
                i * 5, logs[i].gun, logs[i].event, logs[i].v, logs[i].a);
    }

    fclose(fp);   /* 必须关闭！否则数据可能没有真正写入 */
    printf("日志已写入 /tmp/charger_log.txt\n");
}

/* ===== 2. 读取文本文件 ===== */
void demo_read_text(void)
{
    printf("\n===== 文本文件读取 =====\n");

    FILE *fp = fopen("/tmp/charger_log.txt", "r");
    if (!fp) { perror("打开失败"); return; }

    char line[256];
    int line_num = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        line_num++;
        /* 去掉末尾换行符 */
        line[strcspn(line, "\n")] = '\0';
        printf("  第%d行: %s\n", line_num, line);
    }

    fclose(fp);
    printf("共读取 %d 行\n", line_num);
}

/* ===== 3. 二进制文件（嵌入式参数存储模拟）===== */

#define MAGIC_NUMBER  0xDEADBEEF   /* 魔数：用于验证数据有效性 */
#define PARAM_VERSION 0x0102       /* 版本号：参数格式变化时需要升级 */

typedef struct {
    uint32_t magic;          /* 魔数（必须为 MAGIC_NUMBER）*/
    uint16_t version;        /* 参数版本号 */
    uint16_t checksum;       /* 简单校验和 */
    /* 实际参数 */
    uint16_t rated_voltage;  /* 额定电压 (V) */
    uint16_t max_current;    /* 最大电流 (A) */
    float    meter_factor;   /* 计量倍率 */
    char     station_id[16]; /* 站点 ID */
} SavedParams_t;

/**
 * @brief 计算参数的简单校验和
 */
static uint16_t calc_checksum(const SavedParams_t *p)
{
    uint16_t sum = 0;
    const uint8_t *ptr = (const uint8_t *)&p->version;   /* 跳过 magic 和 checksum */
    int len = sizeof(SavedParams_t) - sizeof(p->magic) - sizeof(p->checksum);
    for (int i = 0; i < len; i++) {
        sum += ptr[i];
    }
    return sum;
}

void demo_binary_file(void)
{
    printf("\n===== 二进制文件（参数存储）=====\n");

    const char *fname = "/tmp/charger_params.bin";

    /* 写入参数 */
    SavedParams_t params = {
        .magic        = MAGIC_NUMBER,
        .version      = PARAM_VERSION,
        .rated_voltage = 220,
        .max_current   = 16,
        .meter_factor  = 1.0005f,
        .station_id    = "CS001",
    };
    params.checksum = calc_checksum(&params);

    FILE *fp = fopen(fname, "wb");   /* "wb" = write binary */
    if (!fp) { perror("写入失败"); return; }

    size_t written = fwrite(&params, sizeof(params), 1, fp);
    fclose(fp);
    printf("写入 %zu 个结构体，大小 %zu 字节\n", written, sizeof(params));

    /* 读取并验证 */
    SavedParams_t loaded;
    fp = fopen(fname, "rb");         /* "rb" = read binary */
    if (!fp) { perror("读取失败"); return; }

    size_t read_cnt = fread(&loaded, sizeof(loaded), 1, fp);
    fclose(fp);

    if (read_cnt != 1) {
        printf("读取失败！\n");
        return;
    }

    /* 验证魔数 */
    if (loaded.magic != MAGIC_NUMBER) {
        printf("魔数错误！数据无效\n");
        return;
    }

    /* 验证校验和 */
    uint16_t expected_crc = calc_checksum(&loaded);
    if (loaded.checksum != expected_crc) {
        printf("校验和错误！数据损坏\n");
        return;
    }

    printf("参数读取成功！\n");
    printf("  额定电压: %u V\n", loaded.rated_voltage);
    printf("  最大电流: %u A\n", loaded.max_current);
    printf("  计量倍率: %.4f\n", loaded.meter_factor);
    printf("  站点 ID:  %s\n", loaded.station_id);
}

int main(void)
{
    demo_write_text();
    demo_read_text();
    demo_binary_file();
    return 0;
}

/*
 * TODO 练习：
 *
 * TODO-01: 实现文件复制函数 copy_file(src, dst)
 *   用 fread/fwrite 按块读写，处理大文件
 *
 * TODO-02: 实现 CSV 解析器（不使用第三方库）
 *   读取 "a,b,c\n1,2,3" 格式，存入二维字符数组
 *
 * TODO-03: 实现循环日志文件（限制最大行数，超出后覆盖最旧记录）
 *   用 fseek/ftell 实现文件随机访问
 *
 * TODO-04: 模拟 EEPROM 读写接口
 *   int eeprom_write(uint16_t addr, const void *data, uint16_t len)
 *   int eeprom_read(uint16_t addr, void *data, uint16_t len)
 *   底层使用文件存储，上层接口和真实 EEPROM 驱动相同
 *
 * TODO-05: 实现带版本迁移的参数存储
 *   当读取到旧版本参数时，自动转换为新版本格式
 */
