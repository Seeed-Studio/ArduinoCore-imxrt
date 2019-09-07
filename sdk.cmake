

add_lib_files(${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/MIMXRT1052/drivers/*.c)
set(fsl_bsp_drivers_files ${LIB_FILES})
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)



add_lib_files(${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/boards/evkbimxrt1050/xip/*.c)
set(fsl_xip_drivers_files ${LIB_FILES})
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
add_library(fsl_xip_drivers STATIC  ${fsl_xip_drivers_files})
target_include_directories(fsl_xip_drivers PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/MIMXRT1052
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/MIMXRT1052/drivers
                    )


add_lib_files(${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/*.c)
set(fsl_usb_drivers_files ${LIB_FILES})
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
add_library(fsl_usb_drivers STATIC  ${fsl_usb_drivers_files})
target_include_directories(fsl_usb_drivers PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/MIMXRT1052
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/MIMXRT1052/drivers
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/osa
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/phy
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/host
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/MIMXRT1052/utilities/debug_console
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/serial_manager
                    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/USB
                    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino
                    )

add_library(fsl_bsp STATIC  ${fsl_bsp_drivers_files}
                            ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/MIMXRT1052/system_MIMXRT1052.c
                            )
target_include_directories(fsl_bsp PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/MIMXRT1052
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/MIMXRT1052/drivers
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/MIMXRT1052/xip
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/MIMXRT1052/utilities/str
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/MIMXRT1052/utilities/debug_console
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/uart
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/serial_manager
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/lists
                                        )
                    

                                            
add_lib_files(${CMAKE_CURRENT_SOURCE_DIR}/cores/*.c)
add_lib_files(${CMAKE_CURRENT_SOURCE_DIR}/cores/*.cpp)
set(arduinocore_files ${LIB_FILES})
set(LIB_FILES "" CACHE STRING "Lib Files" FORCE)
add_library(arduinocore STATIC  ${arduinocore_files})
target_include_directories(arduinocore PUBLIC
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/CMSIS/Include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/MIMXRT1052
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/MIMXRT1052/drivers
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/osa
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/device
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/phy
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/middleware/usb/host
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/devices/MIMXRT1052/utilities/debug_console
                    ${CMAKE_CURRENT_SOURCE_DIR}/tools/sdk/components/serial_manager
                    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/USB
                    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino
                    )
