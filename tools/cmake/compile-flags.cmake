add_compile_flags(LD 
        -L${ARDUINO_CMSIS_PATH}/CMSIS/Lib/GCC
        -Os 
        -Wl,--gc-sections 
        -save-temps 
        -T ${ARDUPY_BOARD_PATH}/flash_with_bootloader.ld
        -Wl,-Map,${CMAKE_BINARY_DIR}/firmware.map
        )

# C Flags Settings
add_compile_flags(BOTH
            -g 
            -Os -Wall 
            -w 
            -ffunction-sections 
            -fdata-sections
            -fno-common 
            -ffreestanding 
            -fno-builtin 
            -nostdlib  
            --param max-inline-insns-single=500
            -MMD  -MP
            -DNDEBUG
            -DXIP_EXTERNAL_FLASH=1 
            -DXIP_BOOT_HEADER_ENABLE=1 
            -DCPU_MIMXRT1052DVL6B 
            -DUSB_STACK_BM
            -mthumb -mapcs -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 
        )

add_compile_flags(C -std=gnu99 

                    )

add_compile_flags(CXX 
                -std=gnu++11 
                -fno-rtti 
                -fno-exceptions
                -fno-threadsafe-statics
                )

