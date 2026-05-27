/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
    #endif

    #include "platform_config.h"

    //--------------------------------------------------------------------
    // Common Configuration
    //--------------------------------------------------------------------

    // [FIX] BOARD_DEVICE_RHPORT_SPEED 제거 또는 FULL_SPEED로 변경
    // PIO USB Host는 Full Speed(12Mbps)만 지원하므로 HIGH_SPEED 설정은 잘못된 것
    // 원래 코드: #define BOARD_DEVICE_RHPORT_SPEED   OPT_MODE_HIGH_SPEED  ← 삭제
    #define BOARD_DEVICE_RHPORT_SPEED   OPT_MODE_FULL_SPEED

    // defined by compiler flags for flexibility
    #ifndef CFG_TUSB_MCU
    #error CFG_TUSB_MCU must be defined
    #endif

    #ifndef CFG_TUSB_OS
    #define CFG_TUSB_OS           OPT_OS_NONE
    #endif

    #ifndef CFG_TUSB_DEBUG
    #define CFG_TUSB_DEBUG        0
    #endif

    /* USB DMA on some MCUs can only access a specific SRAM region with restriction on alignment.
     * Tinyusb use follows macros to declare transferring memory so that they can be put
     * into those specific section.
     * e.g
     * - CFG_TUSB_MEM SECTION : __attribute__ (( section(".usb_ram") ))
     * - CFG_TUSB_MEM_ALIGN   : __attribute__ ((aligned(4)))
     */
    #ifndef CFG_TUH_MEM_SECTION
    #define CFG_TUH_MEM_SECTION
    #endif

    #ifndef CFG_TUH_MEM_ALIGN
    #define CFG_TUH_MEM_ALIGN     __attribute__ ((aligned(4)))
    #endif

    //--------------------------------------------------------------------
    // Host Configuration
    //--------------------------------------------------------------------

    // Select PIO-USB host controller when available. This applies to RP2040 and RP2350 platforms
    // where the TinyUSB RPi PIO USB HCD is used. If HAS_USBPIO is defined by the platform config,
    // default BOARD_TUH_RHPORT to the PIO-USB controller (port 1). Otherwise, default to port 0.
    #ifndef CFG_TUH_RPI_PIO_USB
    #ifdef HAS_USBPIO
    #define CFG_TUH_RPI_PIO_USB   1
    #else
    #define CFG_TUH_RPI_PIO_USB   0
    #endif
    #endif

    // RHPort number used for host can be defined by board.mk, default to port 0
    #ifndef BOARD_TUH_RHPORT
    #define BOARD_TUH_RHPORT      CFG_TUH_RPI_PIO_USB
    #endif

    // [FIX] Host port 모드를 명시적으로 FULL_SPEED로 설정
    // PIO USB HCD는 Full Speed만 지원하므로 HIGH_SPEED 지정 금지
    #define CFG_TUSB_RHPORT0_MODE     (OPT_MODE_HOST | OPT_MODE_FULL_SPEED)

    // [FIX] Host max speed를 FULL_SPEED로 고정
    // 원래: BOARD_TUH_MAX_SPEED → OPT_MODE_DEFAULT_SPEED (플랫폼 기본값으로 모호함)
    #ifndef BOARD_TUH_MAX_SPEED
    #define BOARD_TUH_MAX_SPEED   OPT_MODE_FULL_SPEED
    #endif

    //--------------------------------------------------------------------
    // COMMON CONFIGURATION
    //--------------------------------------------------------------------

    #ifndef CFG_TUSB_MEM_SECTION
    #define CFG_TUSB_MEM_SECTION
    #endif

    #ifndef CFG_TUSB_MEM_ALIGN
    #define CFG_TUSB_MEM_ALIGN    __attribute__ ((aligned(4)))
    #endif

    // Enable Host stack
    #define CFG_TUH_ENABLED       1

    #define CFG_TUH_MAX_SPEED     BOARD_TUH_MAX_SPEED

    //--------------------------------------------------------------------
    // CLASS DRIVER CONFIGURATION
    //--------------------------------------------------------------------

    // Size of buffer to hold descriptors and other data used for enumeration
    #define CFG_TUH_ENUMERATION_BUFSIZE 1024

    #define CFG_TUH_XINPUT        1
    #define CFG_TUH_HUB           1   // Hub 지원 (포트 4개)
    #define CFG_TUH_CDC           0
    // [FIX] HID 인터페이스 수를 4→6으로 확대
    // VID=040b:PID=0a67 같은 복합 HID 장치는 instance를 2개 이상 올림
    // instance 부족 시 두 번째 이후 인터페이스가 드롭되어 키 입력 누락 가능
    #define CFG_TUH_HID           6
    #define CFG_TUH_MSC           0
    #define CFG_TUH_VENDOR        0

    // max device support (excluding hub device): Hub 포트 수 + 여유분
    #define CFG_TUH_DEVICE_MAX    (CFG_TUH_HUB ? 4 : 1)

    //------------- HID -------------//
    // [FIX] Boot Protocol 리포트는 8바이트이므로 64바이트 버퍼로 충분하나
    // Report Protocol 사용 시 더 큰 리포트가 올 수 있어 명시적으로 유지
    #define CFG_TUH_HID_EPIN_BUFSIZE    64
    #define CFG_TUH_HID_EPOUT_BUFSIZE   64

    //------------- CDC -------------//
    #define CFG_TUH_CDC_LINE_CONTROL_ON_ENUM    0x03
    #define CFG_TUH_CDC_LINE_CODING_ON_ENUM   { 115200, CDC_LINE_CONDING_STOP_BITS_1, CDC_LINE_CODING_PARITY_NONE, 8 }

    /*
     * [주의] tusb_config.h 수정 외에 아래 콜백 구현도 반드시 확인할 것
     *
     * 1. tuh_hid_mount_cb() 안에서:
     *    - Boot Protocol 강제 설정 (복합 HID 장치 대응)
     *      if (tuh_hid_interface_protocol(dev_addr, instance) != HID_ITF_PROTOCOL_NONE)
     *          tuh_hid_set_protocol(dev_addr, instance, HID_PROTOCOL_BOOT);
     *    - 반드시 tuh_hid_receive_report(dev_addr, instance) 호출
     *
     * 2. tuh_hid_report_received_cb() 끝에서:
     *    - 반드시 tuh_hid_receive_report(dev_addr, instance) 재호출
     *    - 이게 없으면 첫 키 입력 이후 폴링이 멈춤
     *
     * 3. 메인 루프에서:
     *    - while(1) { tuh_task(); ... } 형태로 tuh_task()가 지속 호출되는지 확인
     *    - SD 초기화 등 블로킹 함수가 루프 진입을 막으면 USB 폴링 중단됨
     */

    #ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_H_ */
