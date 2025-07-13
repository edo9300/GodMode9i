/*
 io_sc_common.h 

 Routines common to all version of the Super Card

 Copyright (c) 2006 Michael "Chishm" Chisholm
	
 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.
  3. The name of the author may not be used to endorse or promote products derived
     from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	2006-07-11 - Chishm
		* Original release
*/

#ifndef IO_SC_COMMON_H
#define IO_SC_COMMON_H

#include <nds/ndstypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SCSFW_PARAMETERS {
    unsigned int scsfw_magic;
    unsigned int miniboot_arm7;
    unsigned int miniboot_arm7_size;
    unsigned int miniboot_arm9;
    unsigned int miniboot_arm9_size;
    unsigned int nds_rom;
    unsigned int nds_rom_size;
    unsigned int sc_lite_dldi;
    unsigned int sc_lite_dldi_size;
    unsigned int scsd_dldi;
    unsigned int scsd_dldi_size;
    unsigned int sccf_dldi;
    unsigned int sccf_dldi_size;
} SCSFW_PARAMETERS;

typedef enum SUPERCARD_TYPE {
    SC_SD = 0x00,
    SC_LITE = 0x01,
    SC_CF = 0x02,
    SC_RUMBLE = (0x10 | SC_LITE),
    UNK = ~SC_RUMBLE,
} SUPERCARD_TYPE;

// Values for changing mode
#define SC_MODE_RAM 0x5
#define SC_MODE_MEDIA 0x3 
#define SC_MODE_RAM_RO 0x1

extern void _SC_changeMode (u8 mode);
extern SUPERCARD_TYPE _SC_detectType(void);
extern void _SC_enableFlashRW(SUPERCARD_TYPE supercardType);
extern bool findSCSFWParameters(SCSFW_PARAMETERS* params);

#ifdef __cplusplus
}
#endif

#endif // IO_SC_COMMON_H
