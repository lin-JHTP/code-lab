/**
 * @file    embedded_patterns.c
 * @brief   嵌入式C常用设计模式与最佳实践
 * @author  你的名字
 * @date    2026
 *
 * 本文件演示嵌入式开发中最常用的编程模式：
 *   1. 模块化编程（伪OOP风格）
 *   2. 状态机（FSM）模式
 *   3. 循环队列（Ring Buffer / FIFO）
 *   4. 软件定时器（基于Tick计数）
 *   5. 观察者模式（事件/回调系统）
 *   6. 单例模式（全局唯一实例）
 *
 * 编译命令：gcc -Wall -std=c99 -o embedded_patterns embedded_patterns.c
 *
 * 注意：这些模式在真实的GD32/STM32项目中可直接参考使用！
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* ==========================================
 * 模式1：循环队列（Ring Buffer）
 * 用途：串口接收缓冲、数据流处理
 * ========================================== */

#define RING_BUF_SIZE 16   /* 必须是2的幂次，便于取模优化 */

typedef struct {
    uint8_t  buf[RING_BUF_SIZE];  /* 数据缓冲区 */
    uint16_t head;                 /* 读指针 */
    uint16_t tail;                 /* 写指针 */
    uint16_t count;                /* 有效数据数量 */
} RingBuffer_t;

/** @brief 初始化环形缓冲区 */
void ring_buf_init(RingBuffer_t *rb)
{
    memset(rb->buf, 0, sizeof(rb->buf));
    rb->head = rb->tail = rb->count = 0;
}

/** @brief 写入一个字节，成功返回1，满返回0 */
int ring_buf_write(RingBuffer_t *rb, uint8_t byte)
{
    if (rb->count >= RING_BUF_SIZE) return 0;   /* 满了 */
    rb->buf[rb->tail] = byte;
    rb->tail = (rb->tail + 1) % RING_BUF_SIZE;  /* 取模实现循环 */
    rb->count++;
    return 1;
}

/** @brief 读取一个字节，成功返回1，空返回0 */
int ring_buf_read(RingBuffer_t *rb, uint8_t *byte)
{
    if (rb->count == 0) return 0;               /* 空了 */
    *byte = rb->buf[rb->head];
    rb->head = (rb->head + 1) % RING_BUF_SIZE;
    rb->count--;
    return 1;
}

void demo_ring_buffer(void)
{
    printf("===== 循环队列（Ring Buffer）=====\n");

    RingBuffer_t rb;
    ring_buf_init(&rb);

    /* 写入数据（模拟串口中断接收）*/
    const char *msg = "HELLO";
    for (int i = 0; msg[i]; i++) {
        ring_buf_write(&rb, (uint8_t)msg[i]);
    }
    printf("写入 %d 字节，当前count=%d\n", (int)strlen(msg), rb.count);

    /* 读取数据（模拟主循环处理）*/
    printf("读取: ");
    uint8_t byte;
    while (ring_buf_read(&rb, &byte)) {
        printf("%c", byte);
    }
    printf("\n");
}

/* ==========================================
 * 模式2：有限状态机（FSM）
 * 用途：充电流程控制、通信协议处理
 * ========================================== */

typedef enum {
    FSM_STATE_IDLE = 0,       /* 空闲 */
    FSM_STATE_WAIT_GUN,       /* 等待插枪 */
    FSM_STATE_WAIT_AUTH,      /* 等待认证 */
    FSM_STATE_CHARGING,       /* 充电中 */
    FSM_STATE_FINISH,         /* 充电完成 */
    FSM_STATE_FAULT,          /* 故障 */
    FSM_STATE_MAX
} FsmState_t;

typedef enum {
    FSM_EVENT_GUN_IN = 0,     /* 插枪事件 */
    FSM_EVENT_AUTH_OK,        /* 认证成功 */
    FSM_EVENT_CHARGE_START,   /* 开始充电 */
    FSM_EVENT_CHARGE_DONE,    /* 充电完成 */
    FSM_EVENT_FAULT,          /* 故障发生 */
    FSM_EVENT_RESET,          /* 复位 */
    FSM_EVENT_MAX
} FsmEvent_t;

typedef struct {
    FsmState_t current_state;    /* 当前状态 */
    uint32_t   tick_enter;       /* 进入当前状态的时刻 */
} Fsm_t;

/* 状态名字符串（调试用）*/
static const char *state_names[] = {
    "IDLE", "WAIT_GUN", "WAIT_AUTH", "CHARGING", "FINISH", "FAULT"
};
static const char *event_names[] = {
    "GUN_IN", "AUTH_OK", "CHARGE_START", "CHARGE_DONE", "FAULT", "RESET"
};

/** @brief 状态机初始化 */
void fsm_init(Fsm_t *fsm)
{
    fsm->current_state = FSM_STATE_IDLE;
    fsm->tick_enter = 0;
    printf("FSM 初始化，当前状态: %s\n", state_names[fsm->current_state]);
}

/** @brief 状态机事件处理（核心！）*/
void fsm_process_event(Fsm_t *fsm, FsmEvent_t event)
{
    FsmState_t next_state = fsm->current_state;  /* 默认不跳转 */

    printf("[FSM] 当前: %-12s + 事件: %-14s → ",
           state_names[fsm->current_state], event_names[event]);

    /* 状态转移表 */
    switch (fsm->current_state) {
        case FSM_STATE_IDLE:
            if (event == FSM_EVENT_GUN_IN) {
                next_state = FSM_STATE_WAIT_AUTH;
            }
            break;

        case FSM_STATE_WAIT_AUTH:
            if (event == FSM_EVENT_AUTH_OK) {
                next_state = FSM_STATE_CHARGING;
            } else if (event == FSM_EVENT_FAULT) {
                next_state = FSM_STATE_FAULT;
            }
            break;

        case FSM_STATE_CHARGING:
            if (event == FSM_EVENT_CHARGE_DONE) {
                next_state = FSM_STATE_FINISH;
            } else if (event == FSM_EVENT_FAULT) {
                next_state = FSM_STATE_FAULT;
            }
            break;

        case FSM_STATE_FINISH:
        case FSM_STATE_FAULT:
            if (event == FSM_EVENT_RESET) {
                next_state = FSM_STATE_IDLE;
            }
            break;

        default:
            break;
    }

    printf("%s\n", state_names[next_state]);
    fsm->current_state = next_state;
}

void demo_fsm(void)
{
    printf("\n===== 有限状态机（FSM）- 充电流程 =====\n");

    Fsm_t fsm;
    fsm_init(&fsm);

    /* 模拟一次完整充电流程 */
    fsm_process_event(&fsm, FSM_EVENT_GUN_IN);       /* 插枪 */
    fsm_process_event(&fsm, FSM_EVENT_AUTH_OK);      /* 认证成功 */
    fsm_process_event(&fsm, FSM_EVENT_CHARGE_DONE);  /* 充电完成 */
    fsm_process_event(&fsm, FSM_EVENT_RESET);        /* 复位 */

    /* 模拟故障流程 */
    printf("\n--- 故障场景 ---\n");
    fsm_process_event(&fsm, FSM_EVENT_GUN_IN);
    fsm_process_event(&fsm, FSM_EVENT_FAULT);        /* 故障！ */
    fsm_process_event(&fsm, FSM_EVENT_RESET);        /* 恢复 */
}

/* ==========================================
 * 模式3：软件定时器
 * 用途：超时检测、周期任务调度
 * ========================================== */

typedef struct {
    uint32_t period_ms;    /* 定时周期（毫秒）*/
    uint32_t last_tick;    /* 上次触发时的tick值 */
    uint8_t  enabled;      /* 是否启用 */
    void (*callback)(void);/* 到时回调函数 */
} SoftTimer_t;

static uint32_t g_sys_tick = 0;   /* 系统心跳（模拟）*/

/** @brief 初始化定时器 */
void timer_init(SoftTimer_t *timer, uint32_t period_ms, void (*cb)(void))
{
    timer->period_ms = period_ms;
    timer->last_tick = g_sys_tick;
    timer->enabled   = 1;
    timer->callback  = cb;
}

/** @brief 在主循环中调用，检查是否触发 */
void timer_tick(SoftTimer_t *timer)
{
    if (!timer->enabled) return;
    if ((g_sys_tick - timer->last_tick) >= timer->period_ms) {
        timer->last_tick = g_sys_tick;
        if (timer->callback) {
            timer->callback();
        }
    }
}

/* 定时器回调函数 */
static int s_heartbeat_count = 0;
void heartbeat_callback(void)
{
    s_heartbeat_count++;
    printf("  心跳 #%d\n", s_heartbeat_count);
}

void demo_soft_timer(void)
{
    printf("\n===== 软件定时器 =====\n");

    SoftTimer_t heartbeat_timer;
    timer_init(&heartbeat_timer, 100, heartbeat_callback);

    /* 模拟500ms的主循环（每次tick+1ms）*/
    printf("模拟 500ms 运行，心跳周期 100ms:\n");
    for (int ms = 0; ms < 500; ms++) {
        g_sys_tick++;
        timer_tick(&heartbeat_timer);
    }
    printf("共触发 %d 次\n", s_heartbeat_count);
}

/* ==========================================
 * main 函数
 * ========================================== */
int main(void)
{
    demo_ring_buffer();
    demo_fsm();
    demo_soft_timer();
    return 0;
}

/*
 * TODO 练习（嵌入式进阶，与实际项目高度相关！）
 *
 * TODO-01: 扩展 RingBuffer，支持批量写入和批量读取
 *   int ring_buf_write_n(RingBuffer_t *rb, const uint8_t *data, int n)
 *   int ring_buf_read_n(RingBuffer_t *rb, uint8_t *buf, int n)
 *
 * TODO-02: 用函数指针表实现一个更完整的 FSM
 *   typedef void (*StateHandler)(FsmEvent_t event);
 *   StateHandler state_table[FSM_STATE_MAX];
 *   每个状态对应一个处理函数，减少 switch-case 嵌套
 *
 * TODO-03: 实现一个多定时器管理器
 *   支持同时管理8个软件定时器，轮询检查
 *
 * TODO-04: 实现一个简单的命令解析器（CLI）
 *   通过串口输入命令（如 "help", "status", "reset"），执行对应操作
 *   用函数指针表实现命令分发
 *
 * TODO-05: 实现生产者-消费者模型（基于循环队列）
 *   Producer：定时器中断将数据写入队列
 *   Consumer：主循环从队列读取并处理
 *
 * TODO-06: 实现 Modbus RTU 帧解析（完整版）
 *   包含：帧头检测、CRC16校验、功能码解析、寄存器读写
 *
 * TODO-07: 实现一个基于链表的事件队列
 *   事件可以携带参数，支持优先级
 *
 * TODO-08: 实现一个掉电保护的参数存储模块（模拟EEPROM）
 *   使用结构体+魔数+CRC16校验判断数据有效性
 */
