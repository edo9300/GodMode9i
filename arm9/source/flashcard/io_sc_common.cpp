/*
	io_m3_common.h 

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
*/

#include <nds/memory.h>
#include <cstring>

#include "io_sc_common.h"

/*-----------------------------------------------------------------
_SC_changeMode (was SC_Unlock)
Added by MightyMax
Modified by Chishm
Modified again by loopy
1=ram(readonly), 5=ram, 3=SD interface?
-----------------------------------------------------------------*/
void _SC_changeMode(u8 mode) {
	vu16 *unlockAddress = (vu16*)0x09FFFFFE;
	*unlockAddress = 0xA55A ;
	*unlockAddress = 0xA55A ;
	*unlockAddress = mode ;
	*unlockAddress = mode ;
} 

void _SC_enableFlashRW(SUPERCARD_TYPE supercardType) {
	constexpr uint16_t SC_MODE_FLASH_RW		= 0x0004;
	constexpr uint16_t SC_MODE_FLASH_RW_LITE	= 0x1510;
	_SC_changeMode((supercardType & SC_LITE) ? SC_MODE_FLASH_RW_LITE : SC_MODE_FLASH_RW);
}

SUPERCARD_TYPE _SC_detectType() {
	auto type = []{
		constexpr uint16_t SC_MODE_SDCARD = 0x0002;
		_SC_changeMode(SC_MODE_SDCARD);
		auto val = *(volatile uint16_t*)0x09800000;
		switch(val & 0xe300) {
			case 0xa000:
				return SUPERCARD_TYPE::SC_LITE;
			case 0xc000:
				return SUPERCARD_TYPE::SC_RUMBLE;
			case 0xe000:
				return SUPERCARD_TYPE::SC_SD;
			default:
				return SUPERCARD_TYPE::SC_CF;
		}
	}();
	_SC_enableFlashRW(type);
	return type;
}

#define GBA_BUS_U8 ((char*)GBA_BUS)
bool findSCSFWParameters(SCSFW_PARAMETERS* params) {
	memcpy(params, (void*)&GBA_BUS_U8[0xc0 + 4], sizeof(SCSFW_PARAMETERS));
	if(params->scsfw_magic == 0x57464353) {
		return true;
	}
	// supercard rumble
	memcpy(params, (void*)&GBA_BUS_U8[0xc0 + 4 + 0x40000], sizeof(SCSFW_PARAMETERS));
	// account for the values being offsetted
	params->miniboot_arm7 += 0x40000;
	params->miniboot_arm9 += 0x40000;
	params->nds_rom += 0x40000;
	params->sc_lite_dldi += 0x40000;
	params->scsd_dldi += 0x40000;
	params->sccf_dldi += 0x40000;
	return params->scsfw_magic == 0x57464353;
}
