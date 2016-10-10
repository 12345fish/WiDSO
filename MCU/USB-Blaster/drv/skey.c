/* ͨ�õ���������
 * eleqian 2014-10-2
 */

#include "base.h"
#include "skey.h"

/*
˵������ʼ��һ����������
������skey��ָ�򰴼������ָ��
      getstate��״̬��⺯��ָ��
      pdata��״̬��⺯������
*/
void SKey_Init(skey_t *skey, skey_getk_t getstate, void *pdata)
{
    skey->getstate = getstate;
    skey->pdata = pdata;
    skey->state = SKEY_STATE_RELEASE;
    skey->flag = SKEY_FLAG_NONE;
#if SKEY_USE_SEM
    skey->sem = SKEY_SEM_FREE;
#endif //SKEY_USE_SEM
    skey->timer = 0;        //���ڹ̶����ڵ���״̬���º����ļ�ʱ
}

// ʹ��id��ʼ����������
void SKey_InitArrayID(skey_t *skeys, uint8_t keycnt, skey_getk_t getstate, uint8_t idstart)
{
    uint8_t i;

    for (i = 0; i < keycnt; i++) {
        SKey_Init(skeys + i, getstate, (void *)(idstart + i));
    }
}

/*
˵�������°���״̬
Ҫ�󣺵���Ƶ�����㰴���������Ƶ�ʣ�һ��Ҫ���������С��50ms
*/
void SKey_Update(skey_t *skey)
{
#if SKEY_USE_SEM
    if (SKEY_SEM_FREE == skey->sem) {
        skey->sem = SKEY_SEM_USING;
#endif //SKEY_USE_SEM

        switch (skey->state) {
        case SKEY_STATE_RELEASE:
            if (skey->getstate(skey->pdata)) {
                skey->timer = 0;
                skey->flag = SKEY_FLAG_NONE;
                skey->state = SKEY_STATE_PRESS_PRE;
            }
            break;
        case SKEY_STATE_PRESS_PRE:
            skey->timer += SKEY_TIME_UPDATE;
            if (skey->timer >= SKEY_TIME_ENSURE) {
                if (skey->getstate(skey->pdata)) {
                    // ������ȷ�ϰ�������
                    skey->flag |= SKEY_FLAG_PRESS_IN | SKEY_FLAG_PRESS;
                    skey->state = SKEY_STATE_PRESS;
                } else {
                    skey->state = SKEY_STATE_RELEASE;
                }
            }
            break;
        case SKEY_STATE_PRESS:
            if (!skey->getstate(skey->pdata)) {
                skey->timer = 0;
                skey->state = SKEY_STATE_RELEASE_PRE;
            } else {
                skey->timer += SKEY_TIME_UPDATE;
                if (skey->timer >= SKEY_TIME_HOLD) {
                    if (!(skey->flag & SKEY_FLAG_HOLD_IN)) {
                        // ���δ�ڳ�����״̬������λ��������ʶ
                        skey->flag |= SKEY_FLAG_HOLD_IN | SKEY_FLAG_HOLD;
                    } else if (skey->timer >= SKEY_TIME_HOLD + SKEY_TIME_REPEAT) {
                        // ���ڳ�����״̬ʱ��ʱ��λ�ظ���ʶ
                        skey->timer -= SKEY_TIME_REPEAT;
                        skey->flag |= SKEY_FLAG_REPEAT_IN | SKEY_FLAG_REPEAT;
                    }
                }
            }
            break;
        case SKEY_STATE_RELEASE_PRE:
            skey->timer += SKEY_TIME_UPDATE;
            if (skey->timer >= SKEY_TIME_ENSURE) {
                if (!skey->getstate(skey->pdata)) {
                    skey->state = SKEY_STATE_RELEASE;
                    skey->flag &= ~SKEY_FLAG_IN_MASK;
                } else {
                    skey->state = SKEY_STATE_PRESS;
                }
            }
            break;
        default:
            break;
        }

#if SKEY_USE_SEM
        skey->sem = SKEY_SEM_FREE;
    }
#endif //SKEY_USE_SEM
}

// ���°�������
void SKey_UpdateArray(skey_t *skeys, uint8_t cnt)
{
    uint8_t i;

    for (i = 0; i < cnt; i++) {
        SKey_Update(skeys + i);
    }
}

/*
˵������ȡ������ʶ״̬
������flag: Ҫ��ȡ�ı�־���ɶ��λ��, ���key_flag_t����
����ֵ���Ƿ���ָ���ı�־����λ
*/
BOOL SKey_CheckFlag(skey_t *skey, skey_flag_t flag)
{
    BOOL res;

    if (skey == NULL) {
        return FALSE;
    }

#if SKEY_USE_SEM
    if (SKEY_SEM_FREE == skey->sem) {
        skey->sem = SKEY_SEM_USING;
#endif //SKEY_USE_SEM

        res = ((skey->flag & flag) == flag);

        // ���ڲ���ʶ��ȡ���Զ����
        skey->flag &= ~(flag & ~SKEY_FLAG_IN_MASK);

#if SKEY_USE_SEM
        skey->sem = SKEY_SEM_FREE;
    } else {
        res = FLASE;
    }
#endif //SKEY_USE_SEM

    return res;
}
