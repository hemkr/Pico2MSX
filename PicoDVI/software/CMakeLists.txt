# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD waveshare_rp2350_pizero CACHE STRING "Board type")
set(PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_LIST_DIR}/boards)

cmake_minimum_required(VERSION 3.12)
include(pico_sdk_import.cmake)

project(apps/sprite_bounce_audio/sprite_bounce_audio C CXX ASM)
# project(apps/mandelbrot/mandelbrot C CXX ASM)
# project(apps/sprite_bounce/sprite_bounce C CXX ASM)
# project(apps/hello_dvi/hello_dvi C CXX ASM)
# project(apps/terminal/terminal C CXX ASM)
# project(apps/dht_logging/dht_logging C CXX ASM)
# project(apps/vista-palette/vista-palette C CXX ASM)
# project(apps/vista/vista C CXX ASM)
# project(apps/moon/moon C CXX ASM)
# project(apps/tiles_parallax/tiles_parallax C CXX ASM)
# project(apps/tiles_and_sprites/tiles_and_sprites C CXX ASM)
# project(apps/tiles/tiles C CXX ASM)
# project(apps/mandel-full/mandel-full C CXX ASM)
# project(apps/bad_apple/bad_apple C CXX ASM)
# project(apps/christmas_snowflakes/christmas_snowflakes C CXX ASM)
# project(apps/colour_terminal/colour_terminal C CXX ASM)
# project(apps/dual_display/dual_display C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_compile_options(-Wall -Wunused-variable)

# To change the default serial config for all apps, pass e.g.
#   cmake -DDVI_DEFAULT_SERIAL_CONFIG=pimoroni_demo_hdmi_cfg ..
# then rebuild. This is applied via target_compile_definitions on each app
# individually.
# set(DVI_DEFAULT_SERIAL_CONFIG "waveshare_rp2350_pizero_cfg" CACHE STRING
# 	"Select a default pin configuration from common_dvi_pin_configs.h")


include_directories(
	assets
	include
	)

add_subdirectory(libdvi)
add_subdirectory(libsprite)

add_subdirectory(apps)
