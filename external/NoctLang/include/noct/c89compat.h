/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * C89 Compatibility
 */

#ifndef NOCT_C89COMPAT_H
#define NOCT_C89COMPAT_H

/*
 * Here, we define two macros that indicates the target architecture
 * and the byte order.
 *
 * |Macro           |Architecture           |
 * |----------------|-----------------------|
 * |ARCH_X86        |Inte 80386             |
 * |ARCH_X86_64     |AMD64 / Intel 64       |
 * |ARCH_ARM32      |ARMv7                  |
 * |ARCH_ARM64      |Armv8 / Armv9          |
 * |ARCH_PPC32      |PowerPC 32-bit         |
 * |ARCH_PPC64      |PowerPC 64-bit / POWER |
 * |ARCH_MIPS32     |MIPS 32-bit            |
 * |ARCH_MIPS64     |MIPS 64-bit            |
 * |ARCH_RISCV32    |RISC-V 32-bit          |
 * |ARCH_RISCV64    |RISC-V 32-bit          |
 *
 * |Macro           |Byte-Order             |
 * |----------------|-----------------------|
 * |ARCH_LE         |Little Endian          |
 * |ARCH_BE         |Big Endian             |
 */

#if (defined(__i386__) && !defined(__x86_64__)) || defined(_M_IX86)

/* x86 */
#define ARCH_X86
#define ARCH_LE

#elif defined(__x86_64__) || defined(_M_X64)

/* x86_64 */
#define ARCH_X86_64
#define ARCH_LE

#elif defined(__aarch64__) || defined(_M_ARM6)

/* Arm64 */
#define ARCH_ARM64
#define ARCH_LE

#elif defined(__arm__)

/* Arm32 */
#define ARCH_ARM32

#elif defined(_ARCH_PPC64)

/* PowerPC 64 / POWER */
#define ARCH_PPC64

#elif defined(_ARCH_PPC)

/* PowerPC */
#define ARCH_PPC32
#define ARCH_BE

#elif defined(__mips64) || defined(__mips64__)

/* MIPS64 */
#define ARCH_MIPS64

#elif defined(__mips__)

/* MIPS32 */
#define ARCH_MIPS32

#elif defined(__riscv) && (__riscv_xlen == 64)

/* RISC-V 64bit */
#define ARCH_RISCV64
#define ARCH_LE

#elif defined(__riscv) && (__riscv_xlen == 32)

/* RISC-V 32bit */
#define ARCH_RISCV32
#define ARCH_LE

#endif

#if !defined(ARCH_LE) && !defined(ARCH_BE)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define ARCH_LE
#else
#define ARCH_BE
#endif
#endif

/*
 * Here, we define two macros that indicates the target platform and
 * its characteristics.
 *
 * |Macro           |Platform              |Description                 |
 * |----------------|----------------------|----------------------------|
 * |TARGET_WINDOWS  |Windows               |                            |
 * |TARGET_MACOS    |macOS                 |                            |
 * |TARGET_LINUX    |Linux                 |Excluding Adnroid           |
 * |TARGET_IOS      |iOS                   |                            |
 * |TARGET_ANDROID  |Android NDK           |                            |
 * |TARGET_WASM     |Wasm                  |Emscripten                  |
 * |TARGET_UNITY    |Unity                 |Gaming Consoles             |
 * |TARGET_FREEBSD  |FreeBSD               |Excluding Gaming Consoles   |
 * |TARGET_NETBSD   |NetBSD                |                            |
 * |TARGET_OPENBSD  |OpenBSD               |                            |
 * |TARGET_SOLARIS  |Solaris               |                            |
 * |TARGET_BEOS     |BeOS and Haiku        |                            |
 *
 * |Macro           |Description                     |
 * |----------------|--------------------------------|
 * |TARGET_POSIX    |Generic POSIX compliant systems |
 */

/* Windows */
#if defined(_WIN32) && !defined(USE_UNITY)
#define TARGET_WINDOWS
#endif

/* macOS / iOS */
#if !defined(USE_UNITY)
#if defined(__GNUC__)
#if defined(__APPLE__) && __has_include(<TargetConditionals.h>)
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE
#define TARGET_IOS
#elif TARGET_OS_OSX
#define TARGET_MACOS
#endif
#endif
#endif
#endif

/* Linux (non-Android) */
#if defined(__linux) && !defined(__ANDROID__) && !defined(USE_UNITY)
#define TARGET_LINUX
#ifndef TARGET_POSIX
#define TARGET_POSIX
#endif
#endif

/* FreeBSD */
#if defined(__FreeBSD__) && !defined(USE_UNITY)
#define TARGET_FREEBSD
#ifndef TARGET_POSIX
#define TARGET_POSIX
#endif
#endif

/* Unity */
#if defined(USE_UNITY)
#define TARGET_UNITY
#endif

/* NetBSD */
#if defined(__NetBSD__)
#define TARGET_NETBSD
#ifndef TARGET_POSIX
#define TARGET_POSIX
#endif
#endif

/* OpenBSD */
#if defined(__OpenBSD__)
#define TARGET_OPENBSD
#ifndef TARGET_POSIX
#define TARGET_POSIX
#endif
#endif

/* Solaris */
#if defined(__sun)
#define TARGET_SOLARIS
#ifndef TARGET_POSIX
#define TARGET_POSIX
#endif
#endif

/* Android */
#if defined(__ANDROID__)
#define TARGET_ANDROID
#endif

/* Wasm (Emscripten) */
#if defined(__EMSCRIPTEN__)
#define TARGET_WASM
#endif

/* BeOS / Haiku */
#if defined(__BEOS__) || defined(__HAIKU__)
#define TARGET_BEOS
#endif

/* Error: No target detected. */
#if !defined(TARGET_WINDOWS) && \
    !defined(TARGET_MACOS) &&   \
    !defined(TARGET_LINUX) &&   \
    !defined(TARGET_FREEBSD) &&   \
    !defined(TARGET_NETBSD) &&   \
    !defined(TARGET_OPENBSD) &&   \
    !defined(TARGET_SOLARIS) &&   \
    !defined(TARGET_IOS) &&     \
    !defined(TARGET_ANDROID) && \
    !defined(TARGET_WASM) && \
    !defined(TARGET_BEOS) && \
    !defined(USE_UNITY)
#error "No target detected."
#endif

/*
 * Definition of the bool type.
 */
#ifndef __cplusplus
#if defined(HAVE_STDBOOL_H)
#include <stdbool.h>
#elif defined(_MSC_VER) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#include <stdbool.h>
#else
#if !defined(bool) && !defined(BOOL_DEF)
#define BOOL_DEF
typedef int bool;
enum { false = 0, true = 1 };
#endif
#endif
#endif

/*
 * Definitions of the intN_t and uintN_t types.
 */
#if defined(HAVE_STDINT_H)
#include <stdint.h>
#elif defined(HAVE_INTTYPES_H)
#include <inttypes.h>
#elif defined(HAVE_SYS_TYPES_H)
#include <sys/types.h>
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#include <stdint.h>
#elif !defined(INTN_DEF)
#define INTN_DEF
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
#endif

/*
 * Definition of the inline keyword.
 */
#if !defined(INLINE)
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define INLINE			inline		/* C99 */
#elif defined(__GNUC__)
#define INLINE			__inline__	/* GCC */
#elif defined(__xlc__)
#define INLINE			__inline	/* IBM (PPC64) */
#elif defined(_MSC_VER)
#define INLINE			__inline	/* MSVC (x86, x64, arm64) */
#elif defined(INLINE)
#define INLINE			INLINE
#else
#define INLINE
#endif
#endif

/*
 * Definition of the restrict keyword.
 */
#if !defined(RESTRICT)
#if defined(__GNUC__)
#define RESTRICT		__restrict
#elif defined(_MSC_VER)
#define RESTRICT		__restrict
#else
#define RESTRICT
#endif
#endif

/*
 * size_t
 */
#include <stddef.h>

/*
 * Suppress unused warnings.
 */
#ifndef UNUSED_PARAMETER
#define UNUSED_PARAMETER(x)	(void)(x)
#endif

/*
 * UTF-8 string literal.
 */
#ifndef U8
#if defined(__GNUC__) || defined(_MSC_VER)
#define U8(s)			u8##s
#else
#define U8(s)			s
#endif
#endif

/*
 * UTF-32 character literal.
 */
#ifndef U32
#if defined(__GNUC__) || defined(_MSC_VER)
#define U32C(literal, unicode)	U##literal
#else
#define U32C(literal, unicode)	(unicode)
#endif
#endif

/*
 * strdup()
 */
#if defined(_MSC_VER)
#if !defined(strdup)
#define strdup _strdup
#endif
#endif

/*
 * strcasecmp()
 */
#if defined(_MSC_VER)
#if !defined(strcasecmp)
#define strcasecmp _stricmp
#endif
#endif

/*
 * Do not get warnings for usages of string.h functions.
 */
#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

/*
 * Byteorder
 */
#if !defined(BYTEORDER_CONV)
#define BYTEORDER_CONV
#if defined(ARCH_LE)
#define HOSTTOLE64(d)	(d)
#define HOSTTOLE32(d)	(d)
#define HOSTTOLE16(d)	(d)
#define HOSTTOBE64(d)	((((d) & 0xff) << 48) | ((((d) >> 8) & 0xff) << 40) | ((((d) >> 16) & 0xff) << 32) | ((((d) >> 24) & 0xff) << 24) | ((((d) >> 32) & 0xff) << 16) | ((((d) >> 40) & 0xff) << 8) | ((((d) >> 48) & 0xff)))
#define HOSTTOBE32(d)	((((d) & 0xff) << 24) | ((((d) >> 8) & 0xff) << 16) | ((((d) >> 16) & 0xff) << 8) | (((d) >> 24) & 0xff))
#define HOSTTOBE16(d)	((((d) & 0xff) << 8) | (((d) >> 8) & 0xff))
#define LETOHOST64(d)	(d)
#define LETOHOST32(d)	(d)
#define LETOHOST16(d)	(d)
#define BETOHOST64(d)	((((d) & 0xff) << 48) | ((((d) >> 8) & 0xff) << 40) | ((((d) >> 16) & 0xff) << 32) | ((((d) >> 24) & 0xff) << 24) | ((((d) >> 32) & 0xff) << 16) | ((((d) >> 40) & 0xff) << 8) | ((((d) >> 48) & 0xff)))
#define BETOHOST32(d)	((((d) & 0xff) << 24) | ((((d) >> 8) & 0xff) << 16) | ((((d) >> 16) & 0xff) << 8) | (((d) >> 24) & 0xff))
#define BETOHOST16(d)	((((d) & 0xff) << 8) | (((d) >> 8) & 0xff))
#else
#define HOSTTOLE64(d)	((((d) & 0xff) << 48) | ((((d) >> 8) & 0xff) << 40) | ((((d) >> 16) & 0xff) << 32) | ((((d) >> 24) & 0xff) << 24) | ((((d) >> 32) & 0xff) << 16) | ((((d) >> 40) & 0xff) << 8) | ((((d) >> 48) & 0xff)))
#define HOSTTOLE32(d)	((((d) & 0xff) << 24) | ((((d) >> 8) & 0xff) << 16) | ((((d) >> 16) & 0xff) << 8) | (((d) >> 24) & 0xff))
#define HOSTTOLE16(d)	((((d) & 0xff) << 8) | (((d) >> 8) & 0xff))
#define HOSTTOBE64(d)	(d)
#define HOSTTOBE32(d)	(d)
#define HOSTTOBE16(d)	(d)
#define LETOHOST64(d)	((((d) & 0xff) << 48) | ((((d) >> 8) & 0xff) << 40) | ((((d) >> 16) & 0xff) << 32) | ((((d) >> 24) & 0xff) << 24) | ((((d) >> 32) & 0xff) << 16) | ((((d) >> 40) & 0xff) << 8) | ((((d) >> 48) & 0xff)))
#define LETOHOST32(d)	((((d) & 0xff) << 24) | ((((d) >> 8) & 0xff) << 16) | ((((d) >> 16) & 0xff) << 8) | (((d) >> 24) & 0xff))
#define LETOHOST16(d)	((((d) & 0xff) << 8) | (((d) >> 8) & 0xff))
#define BETOHOST64(d)	(d)
#define BETOHOST32(d)	(d)
#define BETOHOST16(d)	(d)
#endif
#endif

/*
 * Message Translation
 */
#if defined(USE_TRANSLATION)
#define N_TR(s)	noct_gettext(s)
const char *noct_gettext(const char *s);
#else
#define N_TR(s)	(s)
#endif

#endif
