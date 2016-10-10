/*
 * LED����ģ��
 * LED���� PA8
 * eleqian 2016-10-7
 */

#include "base.h"
#include "timebase.h"
#include "led.h"

// LEDģʽö��
enum {
    LED_MODE_NORMAL,    // ��ͨģʽ����/��
    LED_MODE_FLASH,     // ��˸ģʽ

    LED_MODE_MAX
} LED_Mode;

// LEDģʽ
static uint8_t gLEDMode;

// �ϴθ���ʱ��(ms)
static uint32_t gLEDLastTime;

// LED��ʱ(ms)
static uint16_t gLEDTimer;

// LED��˸����(ms)
static uint16_t gLEDPeriod;

// LEDÿ��˸���ڵ���ʱ��(ms)
static uint16_t gLEDPulse;

// LED��˸����
static uint16_t gLEDCycleCnt;

/*-----------------------------------*/

// ��/��LED
#define LED_ON()        GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define LED_OFF()       GPIO_ResetBits(GPIOA, GPIO_Pin_8)

// LED�˿ڳ�ʼ��
static void LED_GPIOConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // GPIOA Configuration: Pin 8
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*-----------------------------------*/

// LED��ʼ��
void LED_Init(void)
{
    LED_GPIOConfig();
    LED_OFF();

    gLEDLastTime = millis();
    gLEDMode = LED_MODE_NORMAL;
}

// LED����
void LED_Light(BOOL isLight)
{
    gLEDMode = LED_MODE_NORMAL;
    if (isLight) {
        LED_ON();
    } else {
        LED_OFF();
    }
}

// LED��˸
// ����������(ms)������ʱ��(ms)����˸����(=0ʱһֱ��˸)
void LED_Flash(uint16_t period, uint16_t pulse, uint16_t cycles)
{
    gLEDMode = LED_MODE_FLASH;
    gLEDPeriod = period;
    gLEDPulse = pulse;
    gLEDCycleCnt = cycles;
    gLEDTimer = 0;
    gLEDLastTime = millis();
    LED_OFF();
}

/*-----------------------------------*/

// LED��ʱ����
void LED_Exec(void)
{
    uint32_t t;

    switch (gLEDMode) {
    case LED_MODE_FLASH:
        t = millis();
        gLEDTimer += (t - gLEDLastTime);
        gLEDLastTime = t;
        if (gLEDTimer >= gLEDPulse) {
            LED_OFF();
        }
        if (gLEDTimer >= gLEDPeriod) {
            if (gLEDCycleCnt > 0) {
                gLEDCycleCnt--;
                if (gLEDCycleCnt == 0) {
                    gLEDMode = LED_MODE_NORMAL;
                    LED_OFF();
                    return;
                }
            }
            gLEDTimer = 0;
            LED_ON();
        }
        break;
    default:
        break;
    }
}
