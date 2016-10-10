/*
 * MiCore���İ�USB BootLoader for stm32f103c8
 * eleqian 2014-9-12
 */

#include "base.h"
#include "timebase.h"
#include "led.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "mass_mal.h"

uint8_t gDiskWriteProtect[MAX_LUN] = {0};

/*-----------------------------------*/

// Ӧ�ó��������ʼ��ַ
#define FLASH_APP_ADDR      INTER_FLASH_BASE_ADDR
// FLASHҳ��С��STM32F103C8T6��Ϊ1K��
#define FLASH_PAGE_SIZE     INTER_FLASH_PAGE_SIZE

typedef void (*pAppMain_t)(void);

// �ж��Ƿ����IAPģʽ
BOOL IAP_Check(void)
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

// ����Ӧ�ô���
// ����ɹ������򱾺��������˳���ʧ��ʱ�᷵��
void IAP_LoadApp(void)
{
    pAppMain_t pAppMain;
    uint32_t JumpAddress;

    // ���ջ����ַ�Ƿ�Ϸ�
    if (((*(__IO uint32_t *)FLASH_APP_ADDR) & 0x2FFE0000) == 0x20000000) {
        // ��ת���û�Ӧ�ô�����
        JumpAddress = *(__IO uint32_t *)(FLASH_APP_ADDR + 4);
        pAppMain = (pAppMain_t)JumpAddress;
        // ��ʼ���û�Ӧ�ó���Ķ�ջָ��
        __set_MSP(*(__IO uint32_t *)FLASH_APP_ADDR);
        pAppMain();
    }
}

// ִ��IAP
void IAP_Update(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // ��JTAG����ʹ��SWD
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    LED_Init();

    // ��˸LED��ʾ���ȴ�����USB����IAPģʽ
    USB_SetSystem();
    USB_SetClock();
    //USB_Led_Config();
    USB_Interrupts_Config();
    USB_Init();
    while (bDeviceState != CONFIGURED) {
        LED_Light(TRUE);
        delay_ms(50);
        LED_Light(FALSE);
        delay_ms(50);
    }
    //USB_Configured_LED();
    LED_Light(TRUE);
}

/*-----------------------------------*/

// ������
int main(void)
{
    TimeBase_Init();
    
    if (!IAP_Check()) {
        // ����Ӧ�ó���
        IAP_LoadApp();
    }
    
    // ����IAPģʽ
    IAP_Update();

    while (1) {
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
