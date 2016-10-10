#ifndef __SKEY_H__
#define __SKEY_H__

#include "base.h"

#define SKEY_USE_SEM            (0)     // !=0ʹ�ð��������ź���

// ������ʱ����λms
#define SKEY_TIME_UPDATE        (17)    // ����״̬���¼������Ҫ�͵���SKey_Update������һ��
#define SKEY_TIME_ENSURE        (20)    // ��������ʱ��
#define SKEY_TIME_HOLD          (800)   // ���볤����״̬ʱ��
#define SKEY_TIME_REPEAT        (150)   // ������״̬���Զ��ظ�ʱ��

// ����״̬���ʲ���ö��
typedef enum {
    SKEY_ACCESS_READ = 0x00,
    SKEY_ACCESS_CLEAR = 0X80,

    SKEY_ACCESS_MAX
} skey_access_t;

// ����״̬��ö��
typedef enum {
    SKEY_STATE_RELEASE_PRE,     // �����ͷ�����״̬
    SKEY_STATE_RELEASE,         // �������ͷ�״̬
    SKEY_STATE_PRESS_PRE,       // ������������״̬
    SKEY_STATE_PRESS,           // �����Ѱ���״̬
    SKEY_STATE_MAX
} skey_state_t;

// ������ʶλö��
typedef enum {
    // ���ⲿ���ʵı�־����ȡ���Զ����
    SKEY_FLAG_PRESS = 0x1,      // �������±�ʶ
    SKEY_FLAG_HOLD = 0x2,       // ��������־
    SKEY_FLAG_REPEAT = 0x4,     // �Զ��ظ�������־

    // ������ʶ���ڲ�״̬��ֻ�ܻ�ȡ�����ⲿ���
    SKEY_FLAG_PRESS_IN = 0x10,
    SKEY_FLAG_HOLD_IN = 0x20,
    SKEY_FLAG_REPEAT_IN = 0x40,

    SKEY_FLAG_NONE = 0x0
} skey_flag_t;

// �����ڲ���ʶ����
#define SKEY_FLAG_IN_MASK   0xf0

#if KEY_USE_SEM
// �����ź������������ʱʹ��
typedef enum {
    SKEY_SEM_USING = 0,
    SKEY_SEM_FREE  = 1
} skey_sem_t;
#endif //SKEY_USE_SEM

// ��ȡ��������״̬�Ļص�����
typedef BOOL (*skey_getk_t)(void *pdata);

// ������ʱ��������
typedef uint16_t skey_timer_t;

// �������ݽṹ��
typedef struct {
    skey_getk_t getstate;   // ��ȡ����״̬�Ļص�����
    void *pdata;                // ���溯���Ĳ���
    skey_timer_t timer;         // ����״̬��ʱ
    skey_state_t state;         // ����״̬
    skey_flag_t flag;           // ������ʶ
} skey_t;

// ��ʼ��һ������
// �����������Ľṹ�壬״̬��⺯�����û�����
void SKey_Init(skey_t *skey, skey_getk_t getstate, void *pdata);

// ʹ��id��ʼ����������
void SKey_InitArrayID(skey_t *skeys, uint8_t keycnt, skey_getk_t getstate, uint8_t idstart);

// ����һ������״̬
void SKey_Update(skey_t *skey);

// ���°�������
void SKey_UpdateArray(skey_t *skeys, uint8_t cnt);

// ��ȡ������ʶ״̬
BOOL SKey_CheckFlag(skey_t *skey, skey_flag_t flag);

#endif //__SKEY_H__
