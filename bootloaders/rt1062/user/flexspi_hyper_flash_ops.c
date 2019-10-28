/**
  ******************************************************************
  * @file    bsp_norflash.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   nor flash driver
  ******************************************************************
  */
#include "fsl_flexspi.h"
#include "fsl_debug_console.h"

#include "fsl_iomuxc.h"
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_common.h"

#include "app.h"

/*******************************************************************************
* Definitions
******************************************************************************/
/* Defines the number of instructions in the LUT */
#define NOR_CMD_LUT_SEQ_IDX_READ_NORMAL                 0
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST                   1
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD              2
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS                  3
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE                 4
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR                 5
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE          6
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD            7
#define NOR_CMD_LUT_SEQ_IDX_READID                      8
#define NOR_CMD_LUT_SEQ_IDX_READJEDECID                 9
#define NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG              10
#define NOR_CMD_LUT_SEQ_IDX_READSTATUSREG               11
#define NOR_CMD_LUT_SEQ_IDX_ERASECHIP                   12
#define NOR_CMD_LUT_SEQ_IDX_AHB_PAGEPROGRAM_QUAD_1      13
#define NOR_CMD_LUT_SEQ_IDX_AHB_PAGEPROGRAM_QUAD_2      14
#define NOR_CMD_LUT_SEQ_IDX_READ_UUID_ISSI              15
#define NOR_CMD_LUT_SEQ_IDX_READ_UUID_WB                16

/* length of LUT */
#define CUSTOM_LUT_LENGTH           90


/* Pins for FLEXSPI use the same PAD configuration */
#define FLEXSPI_PAD_CONFIG_DATA         (SRE_1_FAST_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_3_MAX_200MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED)   

/*******************************************************************************
* Prototypes
******************************************************************************/
static void FlexSPI_NorFlash_IOMUXC_MUX_Config(void);
static void FlexSPI_NorFlash_IOMUXC_PAD_Config(void);
static void NorFlash_FlexSPI_ModeInit(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/

extern flexspi_device_config_t deviceconfig;
extern const uint32_t customLUT[CUSTOM_LUT_LENGTH];

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Parameters related to equipment characteristics */
flexspi_device_config_t deviceconfig = {
    .flexspiRootClk = 120000000,
    .flashSize = FLASH_SIZE,
    .CSIntervalUnit = kFLEXSPI_CsIntervalUnit1SckCycle,
    .CSInterval = 2,
    .CSHoldTime = 1,
    .CSSetupTime = 1,
    .dataValidTime = 2,
    .columnspace = 0,
    .enableWordAddress = false,
    .AWRSeqIndex = NOR_CMD_LUT_SEQ_IDX_AHB_PAGEPROGRAM_QUAD_1,
    .AWRSeqNumber = 2,
    .ARDSeqIndex = NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD,
    .ARDSeqNumber = 1,
    /* W25Q256 typical time=0.7ms,max time=3ms 
     *  fAHB = 528MHz,T AHB = 1/528us
     *  unit = 32768/528 = 62.06us
     *  delay 1ms,
     *  AHBWriteWaitInterval = 1*1000/62.06 = 17
     */
    .AHBWriteWaitUnit = kFLEXSPI_AhbWriteWaitUnit32768AhbCycle,
    .AHBWriteWaitInterval = 17,
};

/*Define lookup table LUT
* the following table represents 1 sequence with [4 * NOR_CMD_LUT_SEQ_IDX_xxxx],
* a sequence can contain up to 8 instructions. Using the FLEXSPI_LUT_SEQ macro, you can define 2 instructions at a time.
* one FLEXSPI_LUT_SEQ occupies one LUT register, port A and port B each have 64 LUT registers,
* so CUSTOM_LUT_LENGTH is at most 64.
*
* FLEXSPI_LUT_SEQ format is as follows (LUT instruction 0, number of data lines used, parameters of the instruction,
LUT instruction 1, number of data lines used, parameters of the instruction)
*
* the sequence of less than 8 instructions should end with STOP instruction, namely kFLEXSPI_Command_STOP.
* but since all the arguments in the STOP instruction are 0 and the array starts with 0,
* so the STOP command is ignored at the end of some sequences and works fine.
*/
const uint32_t customLUT[CUSTOM_LUT_LENGTH] = {
        /* Normal read mode -SDR */
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_NORMAL] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_ReadData_4Addr, 
                            kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, FLASH_ADDR_LENGTH),
            
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_NORMAL + 1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, 
                            kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Fast read mode - SDR */
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_FastReadData_4Addr, 
                            kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, FLASH_ADDR_LENGTH),
            
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST + 1] = 
          FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x08,
                          kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

        /* Fast read quad mode - SDR */
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_FastReadQuad_4Addr, 
                            kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, FLASH_ADDR_LENGTH),
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD + 1] = 
          FLEXSPI_LUT_SEQ(kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_4PAD, 0x08, 
                          kFLEXSPI_Command_READ_SDR, kFLEXSPI_4PAD, 0x04),

        /* Read extend parameters */
        [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_ReadStatusReg, 
                            kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

        /* Write Enable */
        [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_WriteEnable, 
                            kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Erase Sector  */
        [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_SectorErase_4Addr, 
                            kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, FLASH_ADDR_LENGTH),

        /* Page Program - single mode */
        [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_PageProgram_4Addr, 
                            kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, FLASH_ADDR_LENGTH),
        [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE + 1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04, 
                            kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),

        /* Page Program - quad mode */
        [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_PageProgramQuad_4Addr, 
                            kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, FLASH_ADDR_LENGTH),
        [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD + 1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_4PAD, 0x04, 
                            kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),  

        /* Read ID ,ID7-ID0*/
        [4 * NOR_CMD_LUT_SEQ_IDX_READID] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_DeviceID, 
                            kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, FLASH_ADDR_LENGTH),
        [4 * NOR_CMD_LUT_SEQ_IDX_READID + 1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, 
                            kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
        
        /* MF7-MF0+ID15-ID0 */
        [4 * NOR_CMD_LUT_SEQ_IDX_READJEDECID] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_JedecDeviceID, 
                            kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

        /* Enable Quad mode */
        [4 * NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_WriteStatusReg, 
                            kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04),

        /* Read status register */
        [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUSREG] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_ReadStatusReg, 
                            kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),

        /* Erase Chip */
        [4 * NOR_CMD_LUT_SEQ_IDX_ERASECHIP] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_ChipErase, 
                            kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
            
        

        /* Write Enable */
        [4 * NOR_CMD_LUT_SEQ_IDX_AHB_PAGEPROGRAM_QUAD_1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_WriteEnable, 
                            kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
            
        /* Page Program - quad mode */
        [4 * NOR_CMD_LUT_SEQ_IDX_AHB_PAGEPROGRAM_QUAD_2] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, W25Q_PageProgramQuad_4Addr, 
                            kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, FLASH_ADDR_LENGTH),
        [4 * NOR_CMD_LUT_SEQ_IDX_AHB_PAGEPROGRAM_QUAD_2 + 1] =
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_4PAD, 0x04, 
                            kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),  	

        //Read  UUID ISSI chip, command 0X4B/
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_UUID_ISSI] 	=
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR,kFLEXSPI_1PAD,0x4B, 
                              kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, FLASH_ADDR_LENGTH),
                              
         [4 * NOR_CMD_LUT_SEQ_IDX_READ_UUID_ISSI + 1] 	=
             FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR,kFLEXSPI_1PAD,0x04,
                              kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
                              
        // Read the unique ID huabang chip, command 0X4B/
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_UUID_WB] 	=
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x4B, 
                              kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x18),
                              
        [4 * NOR_CMD_LUT_SEQ_IDX_READ_UUID_WB + 1] 	=
            FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, 
                              kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),


 

};



/**
* @brief  Initializes the MUX reuse configuration for NORFLASH related IOMUXC
* @param  NULL
* @retval NULL
*/
static void FlexSPI_NorFlash_IOMUXC_MUX_Config(void)
{
  /* FlexSPI communication pin */
  IOMUXC_SetPinMux(NORFLASH_SS_IOMUXC, 1U);                                   
  IOMUXC_SetPinMux(NORFLASH_SCLK_IOMUXC, 1U);  
  IOMUXC_SetPinMux(NORFLASH_DATA00_IOMUXC, 1U);  
  IOMUXC_SetPinMux(NORFLASH_DATA01_IOMUXC, 1U);  
  IOMUXC_SetPinMux(NORFLASH_DATA02_IOMUXC, 1U);  
  IOMUXC_SetPinMux(NORFLASH_DATA03_IOMUXC, 1U);  
}

/**
* @brief  Initializes the PAD property configuration for NORFLASH related IOMUXC
* @param  NULL
* @retval NULL
*/
static void FlexSPI_NorFlash_IOMUXC_PAD_Config(void)
{
  /* The FlexSPI communication pins are configured using the same properties */
  IOMUXC_SetPinConfig(NORFLASH_SS_IOMUXC, FLEXSPI_PAD_CONFIG_DATA);   
  IOMUXC_SetPinConfig(NORFLASH_SCLK_IOMUXC, FLEXSPI_PAD_CONFIG_DATA);                               
  IOMUXC_SetPinConfig(NORFLASH_DATA00_IOMUXC, FLEXSPI_PAD_CONFIG_DATA);   
  IOMUXC_SetPinConfig(NORFLASH_DATA01_IOMUXC, FLEXSPI_PAD_CONFIG_DATA);                               
  IOMUXC_SetPinConfig(NORFLASH_DATA02_IOMUXC, FLEXSPI_PAD_CONFIG_DATA);   
  IOMUXC_SetPinConfig(NORFLASH_DATA03_IOMUXC, FLEXSPI_PAD_CONFIG_DATA);                               
}

/**
* @brief  Initialize NorFlash using FlexSPI peripheral mode and clock
* @param  NULL
* @retval NULL
*/  
static void NorFlash_FlexSPI_ModeInit(void)
{
  flexspi_config_t config;

  const clock_usb_pll_config_t g_ccmConfigUsbPll = {.loopDivider = 0U};

  /* USB1PLL,PLL3,loopDivider=0,
      USB1PLL=PLL3 = 24*20 = 480MHz */
  CLOCK_InitUsb1Pll(&g_ccmConfigUsbPll);
  /* PLL3 PFD0:PLL3*18/24 = 360MHZ. */
  CLOCK_InitUsb1Pfd(kCLOCK_Pfd0, 24);  
  /* Select PLL3 PFD0 as flexspi clock source
      00b derive clock from semc_clk_root_pre
      01b derive clock from pll3_sw_clk
      10b derive clock from PLL2 PFD2
      11b derive clock from PLL3 PFD0 */  
  CLOCK_SetMux(kCLOCK_FlexspiMux, 0x3); 
  /* Setting flexspiDiv frequency division factor, FLEXSPI_CLK_ROOT = PLL3 PFD0/(flexspiDiv+1) = 120M. */
  CLOCK_SetDiv(kCLOCK_FlexspiDiv, 2);  
  
  /* Turn off the DCache function */
  SCB_DisableDCache();

  /*Get FlexSPI default Settings commonly used */
  FLEXSPI_GetDefaultConfig(&config);

  /*The ability to allow AHB to read ahead */
  config.ahbConfig.enableAHBPrefetch = true;
  
  /* Write to the configuration */
  FLEXSPI_Init(FLEXSPI, &config);

  /* Configure the flash Settings based on the serial flash function */
  FLEXSPI_SetFlashConfig(FLEXSPI, &deviceconfig, kFLEXSPI_PortA1);

  /* Update lookup table */
  FLEXSPI_UpdateLUT(FLEXSPI, 0, customLUT, CUSTOM_LUT_LENGTH);
}

/**
* @brief  FlexSPI Init
* @param  NULL
* @retval NULL
*/
void FlexSPI_NorFlash_Init(void)
{
	uint32_t JedecDeviceID;
	
  FlexSPI_NorFlash_IOMUXC_MUX_Config();
  FlexSPI_NorFlash_IOMUXC_PAD_Config();
  NorFlash_FlexSPI_ModeInit();
	
	/* get JedecDevice ID. */
  FlexSPI_NorFlash_Get_JedecDevice_ID(FLEXSPI, &JedecDeviceID);
	
	/* FLASH from an ISSI vendor needs to enable Quad mode with a vendor ID of up to 8 bits */
	if((JedecDeviceID &0xFF0000) == (FLASH_ISSI_JEDECDEVICE_ID &0xFF0000))
	{
		FlexSPI_NorFlash_Enable_Quad_Mode(FLEXSPI);
	}
}


/**
* @brief  Read the JedecDevice ID of the FLASH chip
* @param  Base: FlexSPI port used
* @param VendorID [out]: a buffer for storing the received ID value of 1 byte, normally 0xEF4019
* @retval The state value returned by FlexSPI transfer is normally 0
*/
status_t FlexSPI_NorFlash_Get_JedecDevice_ID(FLEXSPI_Type *base, uint32_t *vendorID)
{
    uint32_t temp;
    flexspi_transfer_t flashXfer;
    
   
    flashXfer.deviceAddress = 0;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READJEDECID;
    flashXfer.data = &temp;
    flashXfer.dataSize = 3;
  
    status_t status = FLEXSPI_TransferBlocking(base, &flashXfer);

    *vendorID = ((temp&0xFF)<<16) | (temp&0xFF00) | ((temp&0xFF0000)>>16);

    return status;
}

/**
* @brief  Read the Device ID of the FLASH chip
* @param Base: FlexSPI port used
* @param  VendorID [out]: a buffer for storing the received ID value of 1 byte, normally0x18
* @retval the state value returned by FlexSPI transfer is normally 0
*/
status_t FlexSPI_NorFlash_Get_Device_ID(FLEXSPI_Type *base, uint8_t *vendorID)
{
    uint32_t temp;
    flexspi_transfer_t flashXfer;
    
    flashXfer.deviceAddress = 0;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READID;
    flashXfer.data = &temp;
    flashXfer.dataSize = 1;
	
    status_t status = FLEXSPI_TransferBlocking(base, &flashXfer);
	
    *vendorID = temp;

    return status;
}

/**
* @brief Read FLASH chip UUID (ISSI chip)
* @param Buf: read UUID, 16 bytes of IS chip and 8 bytes of wb chip
* @retval 1
*/
uint8_t FlexSPI_FlashUUID_Get_ISSI(uint8_t *buf)
{
    volatile uint32_t pid,uid,data;
    flexspi_transfer_t FlashTransfer;

    FlashTransfer.deviceAddress=0;                     
    FlashTransfer.port=kFLEXSPI_PortA1;                
    FlashTransfer.cmdType=kFLEXSPI_Read;               
    FlashTransfer.SeqNumber=1;                          
    FlashTransfer.seqIndex=NOR_CMD_LUT_SEQ_IDX_READ_UUID_ISSI;  
    FlashTransfer.data=(uint32_t*)buf;                 
    FlashTransfer.dataSize=16;                          
    FLEXSPI_TransferBlocking(FLEXSPI,&FlashTransfer);
    return 1;

}

/**
* @brief  Read FLASH chip UUID (WB chip)
* @param  Buf: read UUID, 16 bytes of IS chip and 8 bytes of wb chip
* @retval 1
*/
uint8_t FlexSPI_FlashUUID_Get_WB(uint8_t *buf)
{
    volatile uint32_t pid,uid,data;
    flexspi_transfer_t FlashTransfer;

    FlashTransfer.deviceAddress=0;                      
    FlashTransfer.port=kFLEXSPI_PortA1;                
    FlashTransfer.cmdType=kFLEXSPI_Read;               
    FlashTransfer.SeqNumber=1;                         
    FlashTransfer.seqIndex=NOR_CMD_LUT_SEQ_IDX_READ_UUID_WB;  
    FlashTransfer.data=(uint32_t*)buf;               
    FlashTransfer.dataSize=8;                           
    FLEXSPI_TransferBlocking(FLEXSPI,&FlashTransfer);
    return 1;

}
/**
* @brief  Write_Enable
* @param Base: FlexSPI port used
* @retval the state value returned by FlexSPI transfer is normally 0
*/
status_t FlexSPI_NorFlash_Write_Enable(FLEXSPI_Type *base)
{
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write_Enable */
    flashXfer.deviceAddress = 0;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Command;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

/**
* @brief  Wait_Bus_Busy
* @param Base: FlexSPI port used
* @retval the state value returned by FlexSPI transfer is normally 0
*/
status_t FlexSPI_NorFlash_Wait_Bus_Busy(FLEXSPI_Type *base)
{
    /* Wait_Bus_Busy */
    bool isBusy;
    uint32_t readValue;
    status_t status;
    flexspi_transfer_t flashXfer;

    flashXfer.deviceAddress = 0;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READSTATUSREG;
    flashXfer.data = &readValue;
    flashXfer.dataSize = 1;

    do
    {
        status = FLEXSPI_TransferBlocking(base, &flashXfer);

        if (status != kStatus_Success)
        {
            return status;
        }
       
        if (FLASH_BUSY_STATUS_POL)
        {
     
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET))
            {
                isBusy = true;
            }
            else
            {
                isBusy = false;
            }
        }
        else 
        {
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET))
            {
                isBusy = false;
            }
            else
            {
                isBusy = true;
            }
        }

    } while (isBusy);

    return status;
}


/**
* @brief  Read Status Registe
* @param Base: FlexSPI port used
* @retval the state value returned by FlexSPI transfer is normally 0
*/
status_t FlexSPI_NorFlash_Read_Status_Register(FLEXSPI_Type *base, uint8_t *SR_value)
{

    uint32_t readValue;
    status_t status;
    flexspi_transfer_t flashXfer;

   
    flashXfer.deviceAddress = 0;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READSTATUSREG;
    flashXfer.data = &readValue;
    flashXfer.dataSize = 1;

		status = FLEXSPI_TransferBlocking(base, &flashXfer);
		
		*SR_value = readValue;
   
		return status;
}

/**
* @brief  Write_Status_Register
* @param Base: FlexSPI port used
* @param  config
* @retval the state value returned by FlexSPI transfer is normally 0
*/
status_t FlexSPI_NorFlash_Write_Status_Register(FLEXSPI_Type *base, uint8_t *config)
{
		flexspi_transfer_t flashXfer;
    status_t status;
		
		
    status = FlexSPI_NorFlash_Write_Enable(base);

    if (status != kStatus_Success)
    {
        return status;
    }

   
    flashXfer.deviceAddress = 0;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Write;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG;
		flashXfer.data = (uint32_t *)config;
    flashXfer.dataSize = 1;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);
		
		if (status != kStatus_Success)
    {
        return status;
    }
    
  
    status = FlexSPI_NorFlash_Wait_Bus_Busy(base);
    
    return status;
}

/**
* @brief  Enable Quad mode (FLASH on ISSI must be called, Winbond should not be called)
* @note ISSI vendor FLASH, bit6-qe of the status register is used to configure this feature, and when the bit is 1 it means enable Quad mode
*       Winbond FLASH, status register bit6-TB is configured with write protection
*       so when using Winbond FLASH should not call this function,
*        this function must be called when using FLASH from ISSI vendors		
* @param Base: FlexSPI port used
* @param  config:
* @retval the state value returned by FlexSPI transfer is normally 0
*/
status_t FlexSPI_NorFlash_Enable_Quad_Mode(FLEXSPI_Type *base)
{
	uint8_t SR_value;
	status_t status;
	
	status = FlexSPI_NorFlash_Read_Status_Register(base, &SR_value);
	
	if (status != kStatus_Success)
	{
		return status;
	}
	
	SR_value |= 0x40;

	status = FlexSPI_NorFlash_Write_Status_Register(base, &SR_value);
	
	return status;
}


/**
* @brief  Erase_Secto
* @param Base: FlexSPI port used
* @param  dstAddr:
* @retval the state value returned by FlexSPI transfer is normally 0
*/
status_t FlexSPI_NorFlash_Erase_Sector(FLEXSPI_Type *base, uint32_t dstAddr)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    status = FlexSPI_NorFlash_Write_Enable(base);

    if (status != kStatus_Success)
    {
        return status;
    }

    flashXfer.deviceAddress = dstAddr;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Command;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR;
    
    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }
    
   
    status = FlexSPI_NorFlash_Wait_Bus_Busy(base);
    
    return status;
}

/**
* @brief  Page_Program
* @param Base: FlexSPI port used
* @param  dstAddr
* @param  src:
* @param  dataSize:
* @retval the state value returned by FlexSPI transfer is normally 0
*/
status_t FlexSPI_NorFlash_Page_Program(FLEXSPI_Type *base, 
                                       uint32_t dstAddr, 
                                       uint8_t *src,
                                       uint16_t dataSize)
{
    status_t status;
    flexspi_transfer_t flashXfer;

   
    status = FlexSPI_NorFlash_Write_Enable(base);

    if (status != kStatus_Success)
    {
        return status;
    }

  
    flashXfer.deviceAddress = dstAddr;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Write;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD;
    flashXfer.data = (uint32_t *)src;
    flashXfer.dataSize = dataSize;
    
    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success)
    {
        return status;
    }

    
    status = FlexSPI_NorFlash_Wait_Bus_Busy(base);

    return status;
}


/**
  * @brief Write an unlimited amount of data to FLASH
  * @note  Make sure the space is erased before writing
  * @param Base: FlexSPI port used
  * @param  dstAddr:
  * @param  src:
  * @param  dataSize:
  * @retval Writes a return value of 0 to indicate normal
  */
status_t FlexSPI_NorFlash_Buffer_Program(FLEXSPI_Type *base, 
                                         uint32_t dstAddr, 
                                         uint8_t *src,
                                         uint16_t dataSize)                              
{
  status_t status = kStatus_Success;
  uint16_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
 
  uint16_t NumByteToWriteRest = dataSize;
  /* treat as follows:
   1. Whether the first address written is aligned
   2. Whether the last write is exactly one full page */
  Addr = dstAddr % FLASH_PAGE_SIZE;
  count = FLASH_PAGE_SIZE - Addr;
  
  /*If NumByteToWrite > count:
    The first page writes count bytes, the rest of the bytes are processed,
    So use (NumByteToWriteRest = datasize-count) to figure out the subsequent NumOfPage and NumOfSingle.
    If NumByteToWrite < count:
    If there is less than one page of data, NumOfPage and NumOfSingle can be calculated by NumByteToWriteRest = NumByteToWrite */
  NumByteToWriteRest = (dataSize > count) ? (dataSize - count) : dataSize;
  
  /* number of pages to be written completely (excluding previous count bytes) */
  NumOfPage =  NumByteToWriteRest / FLASH_PAGE_SIZE;
  /* the number of bytes to write on the last page (excluding the previous count bytes) */
  NumOfSingle = NumByteToWriteRest % FLASH_PAGE_SIZE;
 
  /* When dataSize > count, write count to the first page
  NULL does this when dataSize < count */
  if(count != 0 && dataSize > count)
  {  
    status = FlexSPI_NorFlash_Page_Program(base, dstAddr, src, count);    
    if(status != kStatus_Success) return status;
    
    dstAddr += count;
    src += count;
  }   
  
  /*Handle subsequent data */
  if(NumOfPage== 0 ) 
  {
    status = FlexSPI_NorFlash_Page_Program(base, dstAddr, src, NumOfSingle);
    if(status != kStatus_Success) return status;
  }
  else
  {   
    /* Subsequent data is greater than one page */
    while(NumOfPage--)
    {
      status = FlexSPI_NorFlash_Page_Program(base, dstAddr, src, FLASH_PAGE_SIZE);
      if(status != kStatus_Success) return status;
      
      dstAddr +=  FLASH_PAGE_SIZE;
      src += FLASH_PAGE_SIZE;  
    }
    /* last page */
    if(NumOfSingle != 0)
    {
      status = FlexSPI_NorFlash_Page_Program(base, dstAddr, src, NumOfSingle); 
      if(status != kStatus_Success) return status;

    }
  }

  return status;
}

/**
* @brief  Buffer_Read
* @param Base: FlexSPI port used
* @param  address:
* @param  dst[out]:
* @param  dataSize:The amount of data to be read should not be greater than 65535
* @retval the state value returned by FlexSPI transfer is normally 0
*/
status_t FlexSPI_NorFlash_Buffer_Read(FLEXSPI_Type *base, 
                                      uint32_t address, 
                                      uint8_t *dst,
                                      uint16_t dataSize)
{
    status_t status;
    flexspi_transfer_t flashXfer;

    flashXfer.deviceAddress = address;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD;
    flashXfer.data = (uint32_t *)dst;
    flashXfer.dataSize = dataSize;
    
    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

/****************************END OF FILE**********************/
