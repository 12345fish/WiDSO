#ifndef __MASS_MAL_H__
#define __MASS_MAL_H__

#include "base.h"

// �򿪺�ʱʹ���ⲿSPI Flash
//#define MAL_SPI_FLASH

#define MAL_OK      0
#define MAL_FAIL    1

// ����洢�豸��
#ifdef MAL_SPI_FLASH
#define MAX_LUN     2
#else
#define MAX_LUN     1
#endif // MAL_SPI_FLASH

// �����ڲ�Flash���ô�С������16K��Ϊ����ռ�
#ifdef MCU_STM32L151C8
#define INTER_FLASH_PAGE_SIZE       (256)
#define INTER_FLASH_PAGE_COUNT      (256 - 64)
#define INTER_FLASH_BASE_ADDR       (0x08000000 + 256 * 64)
#endif //MCU_STM32L151C8
#ifdef MCU_STM32F103C8
#define INTER_FLASH_PAGE_SIZE       (1024)
#define INTER_FLASH_PAGE_COUNT      (128 - 16)  // ʹ�ú����64k���ؿռ�
#define INTER_FLASH_BASE_ADDR       (0x08000000 + 1024 * 16)
#endif //MCU_STM32L151C8

// ���洢�豸���С������ΪBULK_MAX_PACKET_SIZE(64)������������512
#ifdef MAL_SPI_FLASH
// ����ȡ4096����ӦSPI Flash��ֻ�������豸ȡ��С��ֵ�ɼ�СSRAMʹ��
#define MAL_BLOCK_SIZE_MAX      4096
#else
// ֻʹ���ڲ�Flashʱ�Ͷ���Ϊҳ��С
#define MAL_BLOCK_SIZE_MAX      INTER_FLASH_PAGE_SIZE
#endif //MAL_SPI_FLASH

// ��֤���С>=512
#if MAL_BLOCK_SIZE_MAX < 512
#undef MAL_BLOCK_SIZE_MAX
#define MAL_BLOCK_SIZE_MAX      512
#endif //MAL_BLOCK_SIZE_MAX

extern uint32_t Mass_Block_Size[MAX_LUN];      // ���С
extern uint32_t Mass_Block_Count[MAX_LUN];     // ����Ŀ
extern uint32_t Mass_Memory_Size[MAX_LUN];     // �ռ��С

uint16_t MAL_Init(uint8_t lun);
uint16_t MAL_GetStatus(uint8_t lun);
uint16_t MAL_Read(uint8_t lun, uint32_t Memory_Offset, uint8_t *Readbuff, uint16_t Transfer_Length);
uint16_t MAL_Write(uint8_t lun, uint32_t Memory_Offset, const uint8_t *Writebuff, uint16_t Transfer_Length);

#endif /* __MASS_MAL_H__ */
