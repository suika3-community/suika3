/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Stage Subsystem
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

#ifndef SUIKA3_STAGE_H
#define SUIKA3_STAGE_H

/*
 * Initialize the stage subsystem.
 */
bool
s3i_init_stage(void);

/*
 * Cleanup the stage subsystem.
 */
void
s3i_cleanup_stage(void);

/*
 * Setup the name box.
 */
bool
s3i_setup_namebox(void);

/*
 * Setup the message box.
 */
bool
s3i_setup_msgbox(void);

/*
 * Setup the click animation.
 */
bool
s3i_setup_click(void);

/*
 * Setup the choose boxes.
 */
bool
s3i_setup_choose(
	bool is_hover,
	int index);

/*
 * Setup the system button.
 */
bool
s3i_setup_sysbtn(void);

/*
 * Setup the banners.
 */
bool
s3i_setup_banners(void);

#endif
