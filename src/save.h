/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (C) 2001-2026 The Suika3 Authors
 */

/*
 * Save Subsystem
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
 * Numbers of the save slots.
 */
#define ALL_SAVE_SLOTS		(101)
#define NORMAL_SAVE_SLOTS	(100)
#define QUICK_SAVE_INDEX	(100)

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
 * Check if right after load.
 */
bool
s3_check_if_right_after_load(void);

/*
 * Execute a save.
 */
bool
s3_execute_save(
	int index);

/*
 * Execute a load.
 */
bool
s3_execute_load(
	int index);

/*
 * Execute a global save.
 */
void
s3_execute_save_global(void);

/*
 * Check if the quick save data exist.
 */
bool
s3_check_save_exists(
	int index);

/*
 * Delete a save data.
 */
void
s3_delete_save(int index);

/*
 * Delete the global save data.
 */
void
s3_delete_global_save(void);

/*
 * Get the timestamp of a save data.
 */
time_t
s3_get_save_date(
	int index);

/*
 * Get the latest save index.
 */
int
s3_get_latest_save_index(void);

/*
 * Get the chapter title of a save data.
 */
const char *
s3_get_save_chapter_name(
	int index);

/*
 * Get the last message of a save data.
 */
const char *
s3_get_save_last_message(
	int index);

/*
 * Get the thumbnail of a save data.
 */
struct s3_image *
s3_get_save_thumbnail(
	int index);

/*
 * Get the pending message (message box content right after load)
 */
char *
get_pending_message(void);

#endif
