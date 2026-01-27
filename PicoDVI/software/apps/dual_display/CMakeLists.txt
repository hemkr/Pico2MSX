# Replace TMDS with 10 bit UART (same baud rate):
# add_definitions(-DDVI_SERIAL_DEBUG=1)
# add_definitions(-DRUN_FROM_CRYSTAL)

add_executable(dual_display
	main.c
)

target_compile_definitions(dual_display PRIVATE
	# DVI_DEFAULT_SERIAL_CONFIG=${DVI_DEFAULT_SERIAL_CONFIG}
    PICO_PLATFORM=rp2350
)

target_compile_options(dual_display PRIVATE -Wall)

target_link_libraries(dual_display
	pico_stdlib
	pico_multicore
	pico_util
	libdvi
)

# create map/bin/hex file etc.
pico_add_extra_outputs(dual_display)
