# Arduino Core for Seeed i.MX RT Series Boards

This repository contains the Arduino BSP for Seeed i.MX RT Series:
- [Arch Mix](https://www.seeedstudio.com/Arch-Mix-p-2901.html)
## Add the imxrt-based board to the Arduino IDE

### Step 1. Download the latest Arduino IDE

You need a Arduino IDE that after version 1.6.4, please download one if there's no Arduino IDE in your computer.

[![](https://raw.githubusercontent.com/SeeedDocument/Seeeduino_Stalker_V3_1/master/images/Download_IDE.png)](https://www.arduino.cc/en/Main/Software)

### Step 2. Setting your Arduino IDE

Open your Arudino IDE, click on **File > Preferences**, and copy below url to *Additional Boards Manager URLs*

```
https://raw.githubusercontent.com/Seeed-Studio/Seeed_Platform/master/package_seeeduino_boards_index.json
```

## Build ArduinoCore-imxrt with cmake

```
git clone https://github.com/seeed-Studio/ArduinoCore-imxrt 
cd ArduinoCore-imxrt 
mkdir -p examples/Blink
touch  examples/Blink/Blink.ino  #You can write your own arduino code here
mkdir build && cd build
# Make sure you cross-compile the toolchain with the correct path
cmake .. -DPROJ=Blink -DTOOLCHAIN=~/gcc-arm-none-eabi/bin -DBOARD=arch_mix -DCPU=MIMXRT1062DVL6B
```
## Add the imxrt-based board to the platformIO

Arduincore-imxrt supports not only ArduinoIDE development, but also  [PlatformIO](https://platformio.org/) platform. PlatformIO is an open source ecosystem for IoT development. **Currently, you need to manually configure ArduinoCore-imxrt to the platformIO platform.**

### step 1. Install platformIO  IDE 

you can follow this [guide](https://docs.platformio.org/en/latest/ide/pioide.html).

### step 2. Download the following resources

- [platform-nxpimxrt](https://github.com/Seeed-Studio/platform-nxpimxrt)

- [ArduinoCore-imxrt](https://github.com/Seeed-Studio/ArduinoCore-imxrt)

- [flash tool](https://github.com/Seeed-Studio/BOSSA)

  Download the version according to your system.

  - [Windows](https://github.com/Seeed-Studio/BOSSA/releases/download/1.9.1-seeeduino/bossac-1.9.1-seeeduino-windows.tar.bz2)
  - [Linux](https://github.com/Seeed-Studio/BOSSA/releases/download/1.9.1-seeeduino/bossac-1.9.1-seeeduino-linux.tar.gz)

  - [Mac OS](https://github.com/Seeed-Studio/BOSSA/releases/download/1.9.1-seeeduino/bossac-1.9.1-seeeduino-drawin.tar.gz)

### step 3. Extract the corresponding file to the corresponding directory

- Find the installation directory of platformIO IDE. 

  Usually its will be in the user directory, which name  ".platformio"

- Extract the platform-imxrt  to [user]/.platformio/platforms/
- Extract  the ArduinoCore-imxrt to [user]/.platformio/packages/
- Extract  the flash tool to [user]/.platformio/packages/

### step 4. Create the platformIO project

![GIF 2019-10-15 14-53-19](https://github.com/Seeed-Studio/ArduinoCore-imxrt/blob/master/extras/images/GIF%202019-10-15%2014-53-19.gif?raw=true)

## Develop using the **SDK**

You can develop your applications using the SDK Whether you're on platform IDE or Arduino IDE.

```c++
extern "C"
{
  #include "clock_config.h"
  #include "board.h"
  #include "system_MIMXRT1052.h"
  #include "virtual_com.h"
  #include "fsl_iomuxc.h"
  #include "fsl_gpio.h"
}
//USER_LED_R
#define EXAMPLE_LED_GPIO    GPIO1
#define EXAMPLE_LED_GPIO_PIN    9

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_systickCounter;

volatile bool g_pinSet = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
extern "C"
{
// ISR must complier with C
void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}
}

void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while (g_systickCounter != 0U)
    {
    }
}


void LED_Init()
{
  CLOCK_EnableClock(kCLOCK_Iomuxc);       
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_AD_B0_09_GPIO1_IO09,        
      0U);                                   
  IOMUXC_SetPinConfig(
      IOMUXC_GPIO_AD_B0_09_GPIO1_IO09,      
      0x10B0U);     
  // OUTPUT MODE
  gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};
  GPIO_PinInit(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, &led_config);                         
}

int main()
{
    BOARD_BootClockRUN();
    SystemCoreClockUpdate();
     /* Set systick reload value to generate 1ms interrupt */
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        while (1)
        {
        }
    }
    vcom_cdc_init(); // keep it for auto into bootloader next times
    
    while (1)
    {
        /* Delay 1000 ms */
        SysTick_DelayTicks(1000U);
        if (g_pinSet)
        {
            GPIO_PinWrite(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, 0U);
            g_pinSet = false;
        }
        else
        {
            GPIO_PinWrite(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, 1U);
            g_pinSet = true;    
        }
    }

}
```

## FAQ
ArduinoCore-imxrt is a very interesting software project to help all of you develop High performance MCU applications in an easier way. We can't guarantee that there is no any of bug.  If you encounter any compatibility issues or have any new ideas, feel free to check our [GitHub link](https://github.com/Seeed-Studio/ArduinoCore-imxrt/issues) and forum to communicate with us, we will try our best to solve the problems you have encountered. 