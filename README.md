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
cmake .. -DPROJ=Blink -DTOOLCHAIN=/opt/gcc-arm-none-eabi-8.2/bin -DBOARD=arch_mix
```


## FAQ
ArduinoCore-imxrt is a very interesting software project to help all of you develop High performance MCU applications in an easier way. We can't guarantee that there is no any of bug.  If you encounter any compatibility issues or have any new ideas, feel free to check our [GitHub link](https://github.com/Seeed-Studio/ArduinoCore-imxrt/issues) and forum to communicate with us, we will try our best to solve the problems you have encountered. 