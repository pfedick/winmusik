/*******************************************************************************
 * This file is part of "Patrick's Programming Library", Version 6 (PPL6).
 * Web: http://www.pfp.de/ppl/
 *
 * $Author$
 * $Revision$
 * $Date$
 * $Id$
 *
 *******************************************************************************
 * Copyright (c) 2010, Patrick Fedick <patrick@pfp.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *******************************************************************************/

#include "prolog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef _WIN32
#include <strings.h>
#include <ctype.h>
#endif

#include "ppl6.h"

#ifdef HAVE_X86_ASSEMBLER
extern "C" {
	ppluint32 GetASMBits();
	ppluint32 HaveCPUID();
	ppluint32 HaveMMX();
	ppluint32 HaveMMX2();
	ppluint32 Have3DNow();
	ppluint32 Have3DNow2();
	ppluint32 HaveSSE();
}
#endif

namespace ppl6 {


#ifndef HAVE_X86_ASSEMBLER

ppluint32 HaveCPUID() {
	ppluint32 retv=0;
	#ifdef _WIN32
	__asm {
		// check whether CPUID is supported
		// (bit 21 of Eflags can be toggled)
		// cli							// Interupts abschalten
		pushfd						// save Eflags
		pop eax						// transfers Eflags into EAX
		mov edx, eax				// save original Flags
		xor eax, 00200000h			// toggle bit 21
		push eax					// put new value of stack
		popfd						// transfer new value to Eflags
		pushfd						// save updated Eflags
		pop eax						// transfers Eflags into EAX
		xor eax,edx					// updated Eflags and original differ?
		jz NO_CPUID					// no fiffer, bit 21 can't be toggled
			mov retv,1
		NO_CPUID:
		// sti							// Interupts wieder zulassen
	}
	#endif
	return retv;
}

ppluint32 HaveMMX() {
	ppluint32 retv=0;
	#ifdef _WIN32
	if (!HaveCPUID()) return false;
	__asm {
		// test whether extended function 80000001h is supported
		mov eax, 80000000h		// call extended function 80000000h
		cpuid
		cmp eax, 80000000h		// supports functions > 80000000h?
		jbe NO_EXTENDED
			// test if function 80000001h indicates MMX support
			mov eax, 80000001h	// call extended function 80000001h
			cpuid				// reports back extended feature flags
			test edx, 800000h	// bit 23 in extended features
			jz NO_3DNow			// if set, MMX is supported
				mov retv,1
		NO_3DNow:
		NO_EXTENDED:
		// sti							// Interupts wieder zulassen
	}
	#endif
	return retv;
}

ppluint32 HaveMMX2() {
	ppluint32 retv=0;
	#ifdef _WIN32
	if (!HaveCPUID()) return false;
	__asm {
		// test whether extended function 80000001h is supported
		mov eax, 80000000h		// call extended function 80000000h
		cpuid
		cmp eax, 80000000h		// supports functions > 80000000h?
		jbe NO_EXTENDED
			// test if function 80000001h indicates MMX Extension support
			mov eax, 80000001h	// call extended function 80000001h
			cpuid				// reports back extended feature flags
			test edx, 400000h	// bit 22 in extended features
			jz NO_3DNow			// if set, MMX Extension is supported
				mov retv,1
		NO_3DNow:
		NO_EXTENDED:
		// sti							// Interupts wieder zulassen
	}
	#endif
	return retv;
}

ppluint32 Have3DNow() {
	ppluint32 retv=0;
	#ifdef _WIN32
	if (!HaveCPUID()) return false;
	__asm {
		// test whether extended function 80000001h is supported
		mov eax, 80000000h		// call extended function 80000000h
		cpuid
		cmp eax, 80000000h		// supports functions > 80000000h?
		jbe NO_EXTENDED			// no 3DNow! support either
			// test if function 80000001h indicates 3DNow! support
			mov eax, 80000001h	// call extended function 80000001h
			cpuid				// reports back extended feature flags
			test edx, 80000000h	// bit 31 in extended features
			jz NO_3DNow			// if set, 3DNow! is supported
				mov retv,1
		NO_3DNow:
		NO_EXTENDED:
		// sti							// Interupts wieder zulassen
	}
	#endif
	return retv;
}

ppluint32 Have3DNow2() {
	ppluint32 retv=0;
	#ifdef _WIN32
	if (!HaveCPUID()) return false;
	__asm {
		// test whether extended function 80000001h is supported
		mov eax, 80000000h		// call extended function 80000000h
		cpuid
		cmp eax, 80000000h		// supports functions > 80000000h?
		jbe NO_EXTENDED
			// test if function 80000001h indicates 3DNow! Extension support
			mov eax, 80000001h	// call extended function 80000001h
			cpuid				// reports back extended feature flags
			test edx, 40000000h	// bit 30 in extended features
			jz NO_3DNow			// if set, 3DNow! Extension is supported
				mov retv,1
		NO_3DNow:
		NO_EXTENDED:
		// sti							// Interupts wieder zulassen
	}
	#endif
	return retv;
}

ppluint32 HaveSSE() {
	ppluint32 retv=0;
	#ifdef _WIN32
	if (!HaveCPUID()) return false;
	__asm {
		xor edx,edx
		mov eax, 2				// standard feature flags auslesen
		cpuid
		test edx, 2000000h		// bit 25 in extended features
		jz NO_SSE				// if set, SSE is supported
			mov retv,1
		NO_SSE:
		// sti							// Interupts wieder zulassen
	}
	#endif
	return retv;
}

ppluint32 GetASMBits() {
	if (sizeof(void*)==16) return 16;
	if (sizeof(void*)==32) return 32;
	if (sizeof(void*)==64) return 64;
	if (sizeof(void*)==128) return 128;
	return 0;
}

#endif


ppluint32 GetCPUCaps (CPUCaps *cpu)
{
	ppluint32 caps=0;
	if (HaveCPUID()) {
		caps|=CPUCAPS::HAVE_CPUID;
		if (HaveMMX()) caps|=CPUCAPS::HAVE_MMX;
		if (HaveMMX2()) caps|=CPUCAPS::HAVE_MMX2;
		if (Have3DNow()) caps|=CPUCAPS::HAVE_3DNow;
		if (Have3DNow2()) caps|=CPUCAPS::HAVE_3DNow2;
		if (HaveSSE()) caps|=CPUCAPS::HAVE_SSE;
	}
	if (cpu) {
		cpu->caps=caps;
		cpu->bits=GetASMBits();
	}
	return caps;
}

} // end of namespace ppl

