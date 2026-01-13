/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (C) 2001-2026 The Suika3 Authors
 */

/*
 * GUI Subsystem
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
