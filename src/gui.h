/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * GUI Subsystem
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

#ifndef SUIKA3_GUI_H
#define SUIKA3_GUI_H

#include <suika3/suika3.h>

/*
 * Initialize the GUI subsystem.
 */
bool
s3i_init_gui(void);

/*
 * Cleanup the GUI subsytem.
 */
void
s3i_cleanup_gui(void);

/*
 * Check if right after returned from a GUI.
 */
bool
s3_check_if_right_after_gui(void);

/*
 * Load a GUI file and prepare for a start.
 */
bool
s3_load_gui_file(const char *file, bool sys);

/*
 * Start the loaded GUI.
 */
void
s3_start_gui(void);

/*
 * Stop the running GUI.
 */
void
s3_stop_gui(void);

/*
 * Check if a GUI is running.
 */
bool
s3_is_gui_running(void);

/*
 * Run a GUI frame update.
 */
bool
s3i_run_gui_update(void);

/*
 * Run a GUI frame rendering.
 */
bool
s3i_run_gui_render(void);

/*
 * Get the label of the selected button.
 */
const char *
s3_get_gui_result_label(void);

/*
 * Check if the selected GUI button is "back to title".
 */
bool
s3_is_gui_result_title(void);

/*
 * Check if the selected GUI button is "exit".
 */
bool
s3_is_gui_result_exit(void);

/*
 * Check if any save is issued in the current GUI.
 */
bool
s3_check_if_saved_in_gui(void);

/*
 * Check if a load is issued in the current GUI.
 */
bool
s3_check_if_loaded_in_gui(void);

#endif
