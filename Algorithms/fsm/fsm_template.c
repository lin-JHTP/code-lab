/**
 * @file    fsm_template.c
 * @brief   有限状态机（FSM）通用框架模板
 * @author  你的名字
 * @date    2026
 *
 * 本文件提供一个可扩展的 FSM 框架，适用于：
 *   - 充电控制流程
 *   - 通信协议解析（Modbus/OCPP/IEC 61851）
 *   - 用户交互界面状态
 *   - 设备故障处理流程
 *
 * FSM 要素：
 *   - 状态（States）：系统所有可能的情况
 *   - 事件（Events）：触发状态转移的输入
 *   - 转移（Transitions）：从一个状态到另一个状态的规则
 *   - 动作（Actions）：状态进入/退出/转移时执行的操作
 *
 * 编译：gcc -Wall -std=c99 -o fsm_template fsm_template.c
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* ========== 状态与事件定义 ========== */

typedef enum {
    /* 充电枪状态（符合 IEC 61851 标准）*/
    STATE_A = 0,   /* 未连接（CP: 12V）*/
    STATE_B,       /* 已连接，未充电（CP: 9V）*/
    STATE_C,       /* 充电中（CP: 6V）*/
    STATE_D,       /* 充电中（带通风，CP: 3V）*/
    STATE_E,       /* 故障（CP: 0V）*/
    STATE_F,       /* 桩侧故障 */
    STATE_NUM      /* 状态总数 */
} State_t;

typedef enum {
    EVENT_GUN_INSERT = 0,   /* 插枪 */
    EVENT_GUN_REMOVE,       /* 拔枪 */
    EVENT_AUTH_SUCCESS,     /* 认证成功 */
    EVENT_AUTH_FAIL,        /* 认证失败 */
    EVENT_CHARGE_START,     /* 开始充电 */
    EVENT_CHARGE_STOP,      /* 停止充电 */
    EVENT_SOC_FULL,         /* 电量充满 */
    EVENT_FAULT_OCCUR,      /* 故障发生 */
    EVENT_FAULT_CLEAR,      /* 故障解除 */
    EVENT_NUM               /* 事件总数 */
} Event_t;

/* ========== 框架数据结构 ========== */

typedef void (*ActionFunc_t)(void *ctx);  /* 动作函数类型 */

typedef struct {
    State_t  from_state;     /* 当前状态 */
    Event_t  event;          /* 触发事件 */
    State_t  to_state;       /* 目标状态 */
    ActionFunc_t action;     /* 转移时执行的动作（可以为NULL）*/
} Transition_t;

typedef struct {
    ActionFunc_t on_enter;   /* 进入状态时执行 */
    ActionFunc_t on_exit;    /* 退出状态时执行 */
    const char  *name;       /* 状态名（调试用）*/
} StateHandler_t;

typedef struct {
    State_t          current;       /* 当前状态 */
    uint32_t         event_count;   /* 处理的事件总数 */
    StateHandler_t  *state_table;   /* 状态表 */
    Transition_t    *trans_table;   /* 转移表 */
    int              trans_count;   /* 转移规则数量 */
    void            *user_ctx;      /* 用户数据（透传到回调）*/
} Fsm_t;

/* ========== 状态处理函数 ========== */

/* 充电枪业务上下文 */
typedef struct {
    uint8_t  gun_id;
    uint16_t voltage_v;
    uint16_t current_a;
    uint32_t energy_wh;
} GunCtx_t;

void on_enter_state_b(void *ctx)
{
    GunCtx_t *gun = (GunCtx_t *)ctx;
    printf("  → 枪%d: 进入STATE_B（已插枪，等待认证）\n", gun->gun_id);
}

void on_enter_state_c(void *ctx)
{
    GunCtx_t *gun = (GunCtx_t *)ctx;
    gun->voltage_v = 220;
    gun->current_a = 16;
    printf("  → 枪%d: 进入STATE_C（充电中 %dV/%dA）\n",
           gun->gun_id, gun->voltage_v, gun->current_a);
}

void on_exit_state_c(void *ctx)
{
    GunCtx_t *gun = (GunCtx_t *)ctx;
    gun->voltage_v = 0;
    gun->current_a = 0;
    printf("  → 枪%d: 退出STATE_C（停止供电）\n", gun->gun_id);
}

void on_enter_state_e(void *ctx)
{
    GunCtx_t *gun = (GunCtx_t *)ctx;
    printf("  → 枪%d: ⚠️  进入STATE_E（故障！）\n", gun->gun_id);
}

/* ========== 转移动作 ========== */

void action_start_billing(void *ctx)
{
    printf("  [动作] 开始计费\n");
}

void action_send_stop_cmd(void *ctx)
{
    printf("  [动作] 发送停充命令到充电模块\n");
}

void action_raise_fault_alarm(void *ctx)
{
    printf("  [动作] 上报故障告警到云端\n");
}

/* ========== FSM 框架实现 ========== */

void fsm_init(Fsm_t *fsm, State_t init_state,
              StateHandler_t *state_table,
              Transition_t *trans_table, int trans_count,
              void *user_ctx)
{
    fsm->current     = init_state;
    fsm->event_count = 0;
    fsm->state_table = state_table;
    fsm->trans_table = trans_table;
    fsm->trans_count = trans_count;
    fsm->user_ctx    = user_ctx;

    printf("[FSM] 初始化，起始状态: %s\n", state_table[init_state].name);

    /* 执行初始状态的 on_enter */
    if (state_table[init_state].on_enter) {
        state_table[init_state].on_enter(user_ctx);
    }
}

/**
 * @brief 处理一个事件（核心函数）
 * @return 0: 找到匹配的转移并执行；-1: 没有匹配的转移（忽略事件）
 */
int fsm_dispatch(Fsm_t *fsm, Event_t event, const char *event_name)
{
    printf("\n[FSM] 事件: %-20s  当前状态: %s\n",
           event_name, fsm->state_table[fsm->current].name);

    /* 在转移表中查找匹配规则 */
    for (int i = 0; i < fsm->trans_count; i++) {
        Transition_t *t = &fsm->trans_table[i];

        if (t->from_state == fsm->current && t->event == event) {
            State_t next = t->to_state;

            /* 1. 执行当前状态的退出动作 */
            if (fsm->state_table[fsm->current].on_exit) {
                fsm->state_table[fsm->current].on_exit(fsm->user_ctx);
            }

            /* 2. 执行转移动作 */
            if (t->action) {
                t->action(fsm->user_ctx);
            }

            /* 3. 切换到新状态 */
            fsm->current = next;
            fsm->event_count++;

            /* 4. 执行新状态的进入动作 */
            if (fsm->state_table[next].on_enter) {
                fsm->state_table[next].on_enter(fsm->user_ctx);
            }

            printf("  → 新状态: %s\n", fsm->state_table[next].name);
            return 0;
        }
    }

    printf("  → 忽略事件（无匹配转移）\n");
    return -1;
}

/* ========== 充电枪 FSM 实例 ========== */

/* 状态表 */
static StateHandler_t s_state_table[STATE_NUM] = {
    [STATE_A] = {"STATE_A(未连接)", NULL,          NULL},
    [STATE_B] = {"STATE_B(已连接)", on_enter_state_b, NULL},
    [STATE_C] = {"STATE_C(充电中)", on_enter_state_c, on_exit_state_c},
    [STATE_D] = {"STATE_D(充电/通风)", NULL,        NULL},
    [STATE_E] = {"STATE_E(故障)",   on_enter_state_e, NULL},
    [STATE_F] = {"STATE_F(桩故障)", NULL,            NULL},
};

/* 转移表（有限状态机的"地图"）*/
static Transition_t s_trans_table[] = {
    /* 从STATE_A  插枪 → STATE_B */
    {STATE_A, EVENT_GUN_INSERT,    STATE_B, NULL},
    /* 从STATE_B  拔枪 → STATE_A */
    {STATE_B, EVENT_GUN_REMOVE,    STATE_A, NULL},
    /* 从STATE_B  认证成功 + 开始充电 → STATE_C */
    {STATE_B, EVENT_CHARGE_START,  STATE_C, action_start_billing},
    /* 从STATE_C  停止充电 → STATE_B */
    {STATE_C, EVENT_CHARGE_STOP,   STATE_B, action_send_stop_cmd},
    /* 从STATE_C  充满 → STATE_B */
    {STATE_C, EVENT_SOC_FULL,      STATE_B, action_send_stop_cmd},
    /* 从STATE_C  故障 → STATE_E */
    {STATE_C, EVENT_FAULT_OCCUR,   STATE_E, action_raise_fault_alarm},
    /* 从STATE_E  故障解除 → STATE_A */
    {STATE_E, EVENT_FAULT_CLEAR,   STATE_A, NULL},
};

#define EVENT_NAME(e) #e   /* 将枚举名转为字符串（调试用）*/

int main(void)
{
    GunCtx_t gun_ctx = {.gun_id = 1};
    Fsm_t charger_fsm;

    fsm_init(&charger_fsm, STATE_A, s_state_table,
             s_trans_table,
             sizeof(s_trans_table) / sizeof(s_trans_table[0]),
             &gun_ctx);

    printf("\n===== 模拟正常充电流程 =====");
    fsm_dispatch(&charger_fsm, EVENT_GUN_INSERT,    "GUN_INSERT");
    fsm_dispatch(&charger_fsm, EVENT_CHARGE_START,  "CHARGE_START");
    fsm_dispatch(&charger_fsm, EVENT_SOC_FULL,      "SOC_FULL");
    fsm_dispatch(&charger_fsm, EVENT_GUN_REMOVE,    "GUN_REMOVE");

    printf("\n===== 模拟故障场景 =====");
    fsm_dispatch(&charger_fsm, EVENT_GUN_INSERT,    "GUN_INSERT");
    fsm_dispatch(&charger_fsm, EVENT_CHARGE_START,  "CHARGE_START");
    fsm_dispatch(&charger_fsm, EVENT_FAULT_OCCUR,   "FAULT_OCCUR");
    fsm_dispatch(&charger_fsm, EVENT_FAULT_CLEAR,   "FAULT_CLEAR");

    printf("\n[FSM] 总共处理事件: %u\n", charger_fsm.event_count);

    return 0;
}

/*
 * TODO 练习：
 *
 * TODO-01: 为 FSM 添加"历史状态"功能
 *   返回到上一个状态的事件（EVENT_BACK）
 *
 * TODO-02: 添加看门狗超时机制
 *   在 STATE_B 超过 60 秒没有充电开始，自动回到 STATE_A
 *
 * TODO-03: 用 FSM 实现 Modbus RTU 帧接收解析器
 *   状态：IDLE → RECV_ADDR → RECV_FUNC → RECV_DATA → RECV_CRC → DONE
 *
 * TODO-04: 将 FSM 状态持久化（掉电保存当前状态到EEPROM）
 *   重启后从保存的状态恢复
 *
 * TODO-05: 实现支持子状态机（Hierarchical FSM）的框架
 */
