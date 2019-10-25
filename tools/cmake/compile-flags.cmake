add_compile_flags(LD 
        -DXIP_EXTERNAL_FLASH=1 
        -DXIP_BOOT_HEADER_ENABLE=1 
        -DNDEBUG 
        -DCPU_${CPU} 
        -Os 
        -Wall 
        -fno-common 
        -ffunction-sections 
        -fdata-sections 
        -ffreestanding 
        -fno-builtin -mthumb -mapcs 
        -std=gnu99 -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 -MMD -MP   
        --specs=nano.specs --specs=nosys.specs 
        -Wall -fno-common -ffunction-sections 
        -fdata-sections -ffreestanding 
        -fno-builtin -mthumb -mapcs -Xlinker 
        --gc-sections -Xlinker -static 
        -Xlinker -z -Xlinker muldefs -Xlinker -Map=output.map 
        -mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16
        -T ${CMAKE_CURRENT_SOURCE_DIR}/variants/${BOARD}/flexspi_nor_arduino.ld -static
        # -Wl,--start-group 
        # -lm -lc -lgcc -lnosys   libfsl_bsp.a libarduinocore.a libfsl_xip_drivers.a libfsl_usb_drivers.a
        # -Wl,--end-group 
        # -lm -lc -lgcc -lnosys
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
            -DCPU_${CPU} 
            -DXIP_EXTERNAL_FLASH=1 
            -DXIP_BOOT_HEADER_ENABLE=1 
            -DUSB_STACK_BM
            -DARDUINO_MAIN=1
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

