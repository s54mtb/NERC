
#include "main.h"


/* Base address of the Flash sectors */

#define ADDR_EDATA1_STRT_0     (0x0900A800U) /* Base @ of last sector of Bank1 reserved to EDATA (EDATA1_STRT = 0), 6 Kbytes    */
#define ADDR_EDATA1_STRT_1     (0x09009000U) /* Base @ of last 2 sectors of Bank1 reserved to EDATA (EDATA1_STRT = 1), 6 Kbytes */
#define ADDR_EDATA1_STRT_2     (0x09007800U) /* Base @ of last 3 sectors of Bank1 reserved to EDATA (EDATA1_STRT = 2), 6 Kbytes */
#define ADDR_EDATA1_STRT_3     (0x09006000U) /* Base @ of last 4 sectors of Bank1 reserved to EDATA (EDATA1_STRT = 3), 6 Kbytes */
#define ADDR_EDATA1_STRT_4     (0x09004800U) /* Base @ of last 5 sectors of Bank1 reserved to EDATA (EDATA1_STRT = 4), 6 Kbytes */
#define ADDR_EDATA1_STRT_5     (0x09003000U) /* Base @ of last 6 sectors of Bank1 reserved to EDATA (EDATA1_STRT = 5), 6 Kbytes */
#define ADDR_EDATA1_STRT_6     (0x09001800U) /* Base @ of last 7 sectors of Bank1 reserved to EDATA (EDATA1_STRT = 6), 6 Kbytes */
#define ADDR_EDATA1_STRT_7     (0x09000000U) /* Base @ of last 8 sectors of Bank1 reserved to EDATA (EDATA1_STRT = 7), 6 Kbytes */


uint32_t FirstSector = 0, NbOfSectors = 0, EndSector = 0;
uint32_t Address = 0, SectorError = 0;
uint32_t Index= 0;

uint32_t offset = 2;
uint16_t FlashHalfWord[1] = { 0xAA55 };
uint16_t FlashHalfWord_FF[1] = { 0xFFFF };

static uint32_t GetSector_EDATA(uint32_t Address);
static uint32_t GetBank_EDATA(uint32_t Address);
static void MPU_Config(void);
static uint32_t Check_Flash_Content(uint32_t StartAddress, uint32_t EndAddress, uint16_t *Data);

/*Variable used for Erase procedure*/
static FLASH_EraseInitTypeDef EraseInitStruct;
/* Variable used for OB Program procedure */
FLASH_OBProgramInitTypeDef FLASH_OBInitStruct;

/* Start @ of user Flash eData area */
#define EDATA_USER_START_ADDR   ADDR_EDATA1_STRT_7
/* End @ of user Flash eData area */
/* (FLASH_EDATA_SIZE/16) is the sector size of high-cycle area (6KB) */
#define EDATA_USER_END_ADDR     (ADDR_EDATA1_STRT_7 + (8*(FLASH_EDATA_SIZE/16)) - 1)



void test_flash(void) {
  /* MPU Configuration--------------------------------------------------------*/
  /* By default, all the AHB memory range is cacheable. For regions where caching is not
     practical (High-cycle data area), MPU has to be used to disable local cacheability.
  */
  MPU_Config();
  /* Initialize LED1, LED2 and LED3 */

  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  /* Unlock the Flash option bytes to enable the flash option control register access */
  HAL_FLASH_OB_Unlock();

  /* Erase the EDATA Flash area
    (area defined by EDATA_USER_START_ADDR and EDATA_USER_END_ADDR) ***********/

  /* Get the 1st sector of FLASH high-cycle data area */
  FirstSector = GetSector_EDATA(EDATA_USER_START_ADDR);

  /* Get the last sector of FLASH high-cycle data area */
  EndSector = GetSector_EDATA(EDATA_USER_END_ADDR);

  /* Get the number of sectors */
  NbOfSectors = EndSector - FirstSector + 1;

  /* Configure 8 sectors for FLASH high-cycle data */
  FLASH_OBInitStruct.OptionType = OPTIONBYTE_EDATA;
  FLASH_OBInitStruct.Banks = GetBank_EDATA(EDATA_USER_START_ADDR);
  FLASH_OBInitStruct.EDATASize = NbOfSectors;
  if(HAL_FLASHEx_OBProgram(&FLASH_OBInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Start option byte load operation after successful programming operation */
  HAL_FLASH_OB_Launch();

  /* Get the first sector of FLASH high-cycle data area */
  FirstSector = GetSector_EDATA(EDATA_USER_START_ADDR);

  /* Get the last sector of FLASH high-cycle data area */
  EndSector = GetSector_EDATA(EDATA_USER_END_ADDR);

  /* Get the number of sectors to erase */
  NbOfSectors = EndSector - FirstSector + 1;

  EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.Banks = GetBank_EDATA(EDATA_USER_START_ADDR);
  EraseInitStruct.Sector = FirstSector;
  EraseInitStruct.NbSectors = NbOfSectors;

  if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
  {
    /*
      Error occurred while sector erase.
      User can add here some code to deal with this error.
      SectorError will contain the faulty sector and then to know the code error on this sector,
      user can call function 'HAL_FLASH_GetError()'
    */
    /* Infinite loop */
    Error_Handler();
  }

  /* Program the FLASH high-cycle data area of BANK1 */
  Address = EDATA_USER_START_ADDR;

  while(Address < EDATA_USER_END_ADDR)
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD_EDATA, Address, (uint32_t)FlashHalfWord) == HAL_OK)
    {
      Address = Address + offset; /* increment for the next Flash word*/
    }
    else
    {
      /* Error occurred while half word Programming */
      Error_Handler();
    }
  }

  /* Check if the programmed data is OK */
  if (Check_Flash_Content(EDATA_USER_START_ADDR, EDATA_USER_END_ADDR, FlashHalfWord) != 0)
  {
    Error_Handler();
  }

  /* Disable the FLASH High-cycle data */
  FLASH_OBInitStruct.OptionType = OPTIONBYTE_EDATA;
  FLASH_OBInitStruct.Banks = GetBank_EDATA(EDATA_USER_START_ADDR);
  FLASH_OBInitStruct.EDATASize = 0;
  if (HAL_FLASHEx_OBProgram(&FLASH_OBInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Start option byte load operation after successful programming operation */
  HAL_FLASH_OB_Launch();

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation)
  */
  HAL_FLASH_Lock();

  /* Lock the Flash control option to restrict register access */
  HAL_FLASH_OB_Lock();


}




/**
  * @brief  Gets the sector of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The sector of a given address
  */
static uint32_t GetSector_EDATA(uint32_t Address)
{
  uint32_t sector = 0;
  uint32_t edataSectorSize = 0;

  /*
    (FLASH_EDATA_SIZE/2) is the size of high-cycle area of flash BANK1.
    Flash high-cycle area have 8 sectors in each Bank.
  */
  edataSectorSize = (FLASH_EDATA_SIZE / 2) / 8;
  /* Check if the address is located in the FLASH high-cycle data area of BANK1 */
  if((Address >= FLASH_EDATA_BASE) && (Address < FLASH_EDATA_BASE + (FLASH_EDATA_SIZE / 2)))
  {
    sector = (Address & ~FLASH_EDATA_BASE) / edataSectorSize;
    sector += 120;
  }
  /* Check if the address is located in the FLASH high-cycle data area of BANK2 */
  else if ((Address >= FLASH_EDATA_BASE + (FLASH_EDATA_SIZE / 2)) && (Address < FLASH_EDATA_BASE + FLASH_EDATA_SIZE))
  {
    sector = ((Address & ~FLASH_EDATA_BASE) - (FLASH_EDATA_SIZE / 2)) / edataSectorSize;
    sector += 120;
  }
  else
  {
    sector = 0xFFFFFFFF; /* Address out of range */
  }

  return sector;
}

/**
  * @brief  Gets the bank of a given address in EDATA area
  * @param  Addr: Address of A given address in EDATA area
  * @retval The bank of a given address in EDATA area
  */
static uint32_t GetBank_EDATA(uint32_t Addr)
{
  uint32_t bank = 0;

  /* (FLASH_EDATA_SIZE/2) is the size of high-cycle area of flash BANK1 */
  if((Addr >= FLASH_EDATA_BASE) && (Addr < FLASH_EDATA_BASE + (FLASH_EDATA_SIZE/2)))
  {
    bank = FLASH_BANK_1;
  }
  else if ((Addr >= FLASH_EDATA_BASE + (FLASH_EDATA_SIZE/2)) && (Addr < FLASH_EDATA_BASE + FLASH_EDATA_SIZE))
  {
    bank = FLASH_BANK_2;
  }
  else
  {
    bank = 0xFFFFFFFF; /* Address out of range */
  }
  return bank;
}

/**
  * @brief  Configure the MPU attributes as non-cacheable for Flash high-cycle data area
  * @note   The Base Address is Flash high-cycle data area
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Attributes_InitTypeDef   attr;
  MPU_Region_InitTypeDef       region;

  /* Disable MPU before perloading and config update */
  HAL_MPU_Disable();

  /* Define cacheable memory via MPU */
  attr.Number             = MPU_ATTRIBUTES_NUMBER0;
  attr.Attributes         = 0 ;
  HAL_MPU_ConfigMemoryAttributes(&attr);

  /* BaseAddress-LimitAddress configuration */
  region.Enable           = MPU_REGION_ENABLE;
  region.Number           = MPU_REGION_NUMBER0;
  region.AttributesIndex  = MPU_ATTRIBUTES_NUMBER0;
  region.BaseAddress      = EDATA_USER_START_ADDR;
  region.LimitAddress     = EDATA_USER_END_ADDR;
  region.AccessPermission = MPU_REGION_ALL_RW;
  region.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;
  region.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
  HAL_MPU_ConfigRegion(&region);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  Check program operation.
  * param StartAddress Area start address
  * param EndAddress Area end address
  * param Data Expected data
  * @retval FailCounter
  */
static uint32_t Check_Flash_Content(uint32_t StartAddress, uint32_t EndAddress, uint16_t *Data)
{
  uint32_t Address;
  uint32_t FailCounter = 0;
  uint16_t data16;

  Address = StartAddress;

  while(Address < EndAddress)
  {
    for(Index = 0; Index<4; Index++)
    {
      data16 = *(uint16_t*)Address;
      if(data16 != Data[0])
      {
        FailCounter++;
      }
      Address += offset;
    }
  }
  return FailCounter;
}
