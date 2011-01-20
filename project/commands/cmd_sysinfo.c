/**************************************************************************/
/*! 
    @file     cmd_sysinfo.c
    @author   K. Townsend (microBuilder.eu)

    @brief    Code to execute for cmd_sysinfo in the 'core/cmd'
              command-line interpretter.

    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2010, microBuilder SARL
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#include <stdio.h>

#include "projectconfig.h"
#include "core/cmd/cmd.h"
#include "core/systick/systick.h"
#include "project/commands.h"       // Generic helper functions

#ifdef CFG_CHIBI
  #include "drivers/chibi/chb.h"
  #include "drivers/chibi/chb_drvr.h"
#endif

#ifdef CFG_LM75B
  #include "drivers/sensors/lm75b/lm75b.h"
#endif

#ifdef CFG_TFTLCD
  #include "drivers/lcd/tft/lcd.h"
#endif

/**************************************************************************/
/*! 
    'sysinfo' command handler
*/
/**************************************************************************/
void cmd_sysinfo(uint8_t argc, char **argv)
{
  printf("%-30s : %d.%d MHz %s", "System Clock", CFG_CPU_CCLK / 1000000, CFG_CPU_CCLK % 1000000, CFG_PRINTF_NEWLINE);
  printf("%-30s : %d mS %s", "Systick Timer", CFG_SYSTICK_DELAY_IN_MS, CFG_PRINTF_NEWLINE);
  printf("%-30s : %u %s", "Systick Counter", (unsigned int)systickGetTicks(), CFG_PRINTF_NEWLINE);
  printf("%-30s : v%d.%d.%d %s", "Firmware", CFG_FIRMWARE_VERSION_MAJOR, CFG_FIRMWARE_VERSION_MINOR, CFG_FIRMWARE_VERSION_REVISION, CFG_PRINTF_NEWLINE);

  // CLI and buffer Settings
  #ifdef CFG_INTERFACE
    printf("%-30s : %d bytes %s", "Max. Command Length", CFG_INTERFACE_MAXMSGSIZE, CFG_PRINTF_NEWLINE);
  #endif
  #ifdef CFG_PRINTF_UART
    printf("%-30s : %d bytes %s", "UART Buffer", CFG_UART_BUFSIZE, CFG_PRINTF_NEWLINE);
  #endif

  // TFT LCD Settings (if CFG_TFTLCD enabled)
  #ifdef CFG_TFTLCD
    printf("%-30s : %d %s", "LCD Width", (int)lcdGetWidth(), CFG_PRINTF_NEWLINE);
    printf("%-30s : %d %s", "LCD Height", (int)lcdGetHeight(), CFG_PRINTF_NEWLINE);
  #endif

  // Wireless Settings (if CFG_CHIBI enabled)
  #ifdef CFG_CHIBI
    chb_pcb_t *pcb = chb_get_pcb();
    printf("%-30s : %s %s", "Wireless", "AT86RF212", CFG_PRINTF_NEWLINE);
    printf("%-30s : 0x%04X %s", "802.15.4 PAN ID", CFG_CHIBI_PANID, CFG_PRINTF_NEWLINE);
    printf("%-30s : 0x%04X %s", "802.15.4 Node Address", pcb->src_addr, CFG_PRINTF_NEWLINE);
    printf("%-30s : %d %s", "802.15.4 Channel", CFG_CHIBI_CHANNEL, CFG_PRINTF_NEWLINE);
  #endif

  // System Temperature (if LM75B Present)
  #ifdef CFG_LM75B
    int32_t temp = 0;
    lm75bGetTemperature(&temp);
    temp *= 125;
    printf("%-30s : %d.%d C %s", "Temperature", temp / 1000, temp % 1000, CFG_PRINTF_NEWLINE);
  #endif
}