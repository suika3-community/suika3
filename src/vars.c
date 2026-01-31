/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Variable Subsystem
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
#include "vars.h"

/*
 * Initialize the variable subsystem.
 */
void
s3i_init_vars(void)
{
}

/*
 * Cleanup the variable subsystem.
 */
void
s3i_cleanup_vars(void)
{
}

/*
 * Set an integer value to a variable.
 */
bool
s3_set_variable_int(
	const char *name,
	int val)
{
	return true;
}

/*
 * Set a float value to a variable.
 */
bool
s3_set_variable_float(
	const char *name,
	float val)
{
	return true;
}

/*
 * Set a string value to a variable.
 */
bool
s3_set_variable_string(
	const char *name,
	const char* val)
{
	return true;
}

/*
 * Unset a variable.
 */
bool
s3_unset_variable(
	const char *name)
{
	return true;
}


/*
 * Set a variable global.
 */
bool
s3_make_variable_global(
	const char *name,
	bool is_global)
{
	return false;
}

/*
 * Get an integer value from a variable.
 */
int
s3_get_variable_int(
	const char *name)
{
	return 0;
}

/*
 * Get a float value from a variable.
 */
float
s3_get_variable_float(
	const char *name)
{
	return 0.0f;
}

/*
 * Get a string value from a variable.
 */
const char *
s3_get_variable_string(
	const char *name)
{
	return "";
}

/*
 * Get the numbers of the variables.
 */
int
s3_get_variable_count(void)
{
	return 0;
}

/*
 * Get the name of the variable at an index.
 */
const char *
s3_get_variable_name(
	int index)
{
	return "";
}

/*
 * Check if a variable exists for a name.
 */
bool
s3_check_variable_exists(
	const char *name)
{
	return false;
}

/*
 * Check if a variable is global.
 */
bool
s3_is_global_variable(
	const char *namel)
{
	return false;
}

/*
 * Expand a string that may contain variable references.
 */
char *
s3_expand_string_with_variable(
	const char *msg)
{
	return "";
}

/*
 * Clear all local variables.
 */
bool
s3_clear_local_variables(void)
{
	return true;
}
