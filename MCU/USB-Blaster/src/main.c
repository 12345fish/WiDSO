#include <stdio.h>
#include "base.h"
#include "timebase.h"
#include "led.h"
#include "bitband.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "blaster.h"

/*-----------------------------------*/

// �����λ
void SoftReset(void)
{
    __set_FAULTMASK(1);     // �ر������ж�
    NVIC_SystemReset();     // ��λ
}

// �жϰ����Ƿ���
BOOL Key_IsPressed(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    BOOL isInto = FALSE;

    // �򿪰����˿�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_DeInit(GPIOC);
    
    // ��ʼ��KEY�˿�(PC13)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    delay_ms(10);

    // KEY����ʱΪ�͵�ƽ
    isInto = !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);

    // ��λ�����˿�״̬
    GPIO_DeInit(GPIOC);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, DISABLE);

    return isInto;
}

/*-----------------------------------*/

int main(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // ��JTAG����ʹ��SWD
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    TimeBase_Init();
    LED_Init();
    blaster_init();
    
    if (!Key_IsPressed()) {
        USB_HW_Config();
        USB_Init();
    }

    LED_Flash(1000, 200, 0);

    while (1) {
        LED_Exec();
        if (bDeviceState == CONFIGURED)
        {
            blaster_exec();
        }
    }
}

/*-----------------------------------*/

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* Infinite loop */
    while (1) {
    }
}
#endif //USE_FULL_ASSERT
