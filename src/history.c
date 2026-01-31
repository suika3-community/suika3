/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * History Subsystem
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
#include "history.h"

/*
 * Initialize the history subsystem.
 */
bool
s3i_init_history(void)
{
	return true;
}

/*
 * Cleanup the sysbtn subsystem.
 */
void
s3i_cleanup_history(void)
{
}

/*
 * Clear the history.
 */
void
s3_clear_history(void)
{
}

/*
 * Add a history.
 */
bool
s3_add_history(
	const char *name,
	const char *msg,
	const char *voice)
{
	return true;
}

/*
 * Get the number of the history.
 */
int
s3_get_history_count(void)
{
	return 0;
}

/*
 * Get the name at the history index.
 */
const char *
s3_get_history_name(int index)
{
	return "";
}

/*
 * Get the message at the history index.
 */
const char *
s3_get_history_message(int index)
{
	return "";
}

/*
 * Get the message at the history index.
 */
const char *
s3_get_history_voice(int index)
{
	return "";
}
