/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Wrappers for APIs of Playfield Engine and NoctLang
 */

/*-
 * SPDX-License-Identifier: Zlib
 *
 * Copyright (c) 2026 The Suika3 Community
 * Copyright (c) 2025-2026 The Playfield Engine Project
 * Copyright (c) 2025-2026 The NoctVM Project
 * Copyright (c) 2025-2026 Awe Morris
 * Copyright (c) 2016-2024 The Suika2 Development Team
 * Copyright (c) 1996-2024 Keiichi Tabata
 *
 * This software is derived from the codebase of Playfield Engine, NoctLang,
 * Suika2, Suika Studio, Wind Game Lib, and 98/AT Game Lib.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <suika3/suika3.h>
#include "tag.h"

#include <playfield/playfield.h>
#include <noct/noct.h>

#include <stdarg.h>

/*
 * Lap Timer
 */

/*
 * Reset a lap timer and initializes it with a current time.
 */
void
s3_reset_lap_timer(
	uint64_t *origin)
{
	pf_reset_lap_timer(origin);
}

/*
 * Get a lap time in milliseconds.
 */
uint64_t
s3_get_lap_timer_millisec(
	uint64_t *origin)
{
	pf_get_lap_timer_millisec(origin);
}

/*
 * Raw File I/O
 */

/* Check if a file exists. */
bool
s3_check_file_exists(
	const char *fname)
{
	return pf_check_file_exists(fname);
}

/* Read a file. */
bool
s3_read_file_content(
	const char *fname,
	char **buf,
	size_t *len)
{
	return pf_read_file_content(fname, buf, len);
}

/*
 * Save Data
 */

/*
 * Write save data.
 */
bool
s3_write_save_data(
	const char *key,
	const void *data,
	size_t size)
{
	return s3_write_save_data(key, data, size);
}

/*
 * Read save data.
 */
bool
s3_read_save_data(
	const char *key,
	void *data,
	size_t size,
	size_t *ret)
{
	return s3_read_save_data(key, data, size, ret);
}

/*
 * Check whether save data exist or not.
 */
bool
s3_check_save_data(
	const char *key)
{
	return s3_check_save_data(key);
}

/*
 * VM Access
 */

/*
 * Set a VM integer variable.
 */
bool
s3_set_vm_int_variable(
	const char *name,
	int val)
{
	NoctEnv *env;
	NoctValue value;

	env = pf_get_vm_env();
	if (!noct_make_int(env, &value, val))
		return false;
	if (!noct_set_global(env, name, &value))
		return false;

	return true;
}

/*
 * Set a VM float variable.
 */
bool
s3_set_vm_float_variable(
	const char *name,
	float val)
{
	NoctEnv *env;
	NoctValue value;

	env = pf_get_vm_env();
	if (!noct_make_float(env, &value, val))
		return false;
	if (!noct_set_global(env, name, &value))
		return false;

	return true;
}

/*
 * Set a VM string variable.
 */
bool
s3_set_vm_string_variable(
	const char *name,
	const char *val)
{
	NoctEnv *env;
	NoctValue value;

	env = pf_get_vm_env();
	if (!noct_make_string(env, &value, val))
		return false;
	if (!noct_set_global(env, name, &value))
		return false;

	return true;
}

/*
 * Call a VM function.
 */
bool
s3_call_vm_function(
	const char *name)
{
	return pf_call_vm_function(name);
}

/*
 * Call a VM function that corresponds to the current tag.
 */
bool
s3_call_vm_tag_function(
	bool *tag_end)
{
	return pf_call_vm_tag_function(tag_end);
}

/*
 * Logging
 */

/*
 * Print a debug message.
 */
void
s3_log_info(
	const char *msg,
	...)
{
	char buf[4096];
	va_list ap;

	va_start(ap, msg);
	vsnprintf(buf, sizeof(buf), msg, ap);
	va_end(ap);

	log_info(buf);
}

/*
 * Print a warning message.
 */
void
s3_log_warn(
	const char *msg,
	...)
{
	char buf[4096];
	va_list ap;

	va_start(ap, msg);
	vsnprintf(buf, sizeof(buf), msg, ap);
	va_end(ap);

	log_warn(buf);
}

/*
 * Print an error message.
 */
void
s3_log_error(
	const char *msg,
	...)
{
	char buf[4096];
	va_list ap;

	va_start(ap, msg);
	vsnprintf(buf, sizeof(buf), msg, ap);
	va_end(ap);

	log_error(buf);
}

/*
 * Print an out-of-memory error message.
 */
void
s3_log_out_of_memory(void)
{
	pf_log_error("Out-of-memory.");
}

/*
 * Print a log footer for execution error.
 */
void
s3_log_script_exec_footer(void)
{
	/* TODO */
}
