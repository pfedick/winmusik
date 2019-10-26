/*
 * compat.h
 *
 *  Created on: 27.12.2016
 *      Author: patrick
 */

#ifndef INCLUDE_COMPAT_H_
#define INCLUDE_COMPAT_H_


#ifdef MINGW32
#define ppl_printf __mingw_printf
#define ppl_sprintf __mingw_sprintf

#else
#define ppl_printf printf
#define ppl_sprintf sprintf

#endif


#endif /* INCLUDE_COMPAT_H_ */
