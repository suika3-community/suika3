/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Save Subsystem
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

#ifndef SUIKA3_SAVE_H
#define SUIKA3_SAVE_H

#include <suika3/suika3.h>

/*
 * File name for the global save data.
 */
#define GLOBAL_SAVE_FILE	"g000.sav"

/*
 * File name for the quick save data.
 */
#define QUICK_SAVE_FILE		"q000.sav"

/*
 * File name for the quick save data. (extra)
 */
#define QUICK_SAVE_EXTRA_FILE	"q001.sav"

/*
 * Initialize the save subsystem.
 */
bool
s3i_init_save(void);

/*
 * Cleanup the save subsystem.
 */
void
s3i_cleanup_save(void);

/*
 * Get the pending message (message box content right after load)
 */
char *
s3i_get_pending_message(void);

#endif
