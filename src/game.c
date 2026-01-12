/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (c) 2001-2026 The Suika3 Authors
 */

/*
 * Game Main Logic
 */

#include <suika3/suika3.h>
#include "api.h"
#include "conf.h"
#include "stage.h"
#include "sysbtn.h"
#include "mixer.h"
#include "tag.h"

#include <playfield/playfield.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <assert.h>

/*
 * Input States
 */

static int mouse_pos_x;
static int mouse_pos_y;
static bool is_mouse_left_pressed;
static bool is_mouse_right_pressed;
static bool is_mouse_left_clicked;
static bool is_mouse_right_clicked;
static bool is_mouse_dragging;
static bool is_return_key_pressed;
static bool is_space_key_pressed;
static bool is_escape_key_pressed;
static bool is_up_key_pressed;
static bool is_down_key_pressed;
static bool is_left_key_pressed;
static bool is_right_key_pressed;
static bool is_pageup_key_pressed;
static bool is_pagedown_key_pressed;
static bool is_control_key_pressed;
static bool is_s_key_pressed;
static bool is_l_key_pressed;
static bool is_h_key_pressed;
static bool is_touch_canceled;
static bool is_swiped;

/*
 * Internal Game States
 */

/* Is in a multiple-frame command execution? */
static bool is_in_repetition;

/* Is message showing? */
static bool flag_message_active;

/* Is executing auto-mode? */
static bool flag_auto_mode;

/* Is executing skip-mode? */
static bool flag_skip_mode;

/* Is save/load allowed? */
static bool flag_save_load_enabled = true;

/* Is non-interruptible mode? */
static bool flag_non_interruptible;

/* Pen position. */
static int saved_pen_x;
static int saved_pen_y;

/* Calling arguments. */
static char *call_arg[S3_CALL_ARGS];

/* Stored message for the page mode. */
static char *buffered_message;

/* Lines in a page. */
static int page_line;

/* BGVoice */
static char *bgvoice;
static bool flag_bgvoice_playing;

/* Forward declarations. */
static bool dispatch_update(bool *should_continue);
static bool dispatch_render(void);

/*
 * Called before the game starts.
 */
bool playfield_init_hook(void)
{
	/* Install the s3* API. */
	if (!install_s3_api())
		return false;
		
	return 0;
}

/*
 * Called when the game starts.
 */
bool on_game_start(void)
{
	/* Initialize the pseudo random number. */
	srand((unsigned int)time(NULL));

	/* Initialize the config subsystem. */
	if (!init_conf())
		return false;

	/* Initialize the stage subsystem. */
	if (!init_stage())
		return false;

	/* Initialize the sysbtn subsystem. */
	if (!init_sysbtn())
		return false;

	/* Initialize the mixer subsystem. */
	if (!init_mixer())
		return false;

	/* Initialize the tag subsystem. */
	if (!init_tag())
		return false;

	/* Initialize the game states. */
	is_in_repetition = false;
	flag_message_active = false;
	flag_auto_mode = false;
	flag_save_load_enabled = true;
	flag_non_interruptible = false;
	page_line = 0;

	/* Clear the input states. */
	mouse_pos_x = 0;
	mouse_pos_y = 0;
	is_mouse_left_pressed = false;
	is_mouse_right_pressed = false;
	is_mouse_left_clicked = false;
	is_mouse_right_clicked = false;
	is_mouse_dragging = false;
	is_return_key_pressed = false;
	is_space_key_pressed = false;
	is_escape_key_pressed = false;
	is_up_key_pressed = false;
	is_down_key_pressed = false;
	is_pageup_key_pressed = false;
	is_pagedown_key_pressed = false;
	is_control_key_pressed = false;
	is_s_key_pressed = false;
	is_l_key_pressed = false;
	is_h_key_pressed = false;
}

/*
 * Called when a frame is updated.
 */
bool on_game_update(void)
{
	int s3_continue;
	bool tag_end;

	/* Call update functions. */
	while (1) {
		/* Clear the continue flag. */
		pf_set_vm_int("s3Continue", 0);

		/* Call the tag function. */
		pf_call_vm_tag_function(&tag_end);
		if (tag_end) {
			/* Stop the game after the next rendering. */
			pf_set_vm_int("exitFlag", 1);
			break;
		}

		/* Check the continue flag. */
		pf_get_vm_int("s3Continue", &s3_continue);
		if (s3_continue) {
			/* Process the next tag. */
			continue;
		}

		break;
	} 

	/* Do a sound fading. */
	process_sound_fading();

	/* Reset input states. */
	is_mouse_left_clicked = false;
	is_mouse_right_clicked = false;
	is_space_key_pressed = false;
	is_return_key_pressed = false;
	is_escape_key_pressed = false;
	is_up_key_pressed = false;
	is_down_key_pressed = false;
	is_left_key_pressed = false;
	is_right_key_pressed = false;
	is_s_key_pressed = false;
	is_l_key_pressed = false;
	is_h_key_pressed = false;
	is_touch_canceled = false;
	is_swiped = false;

	return true;
}

/*
 * Called when a frame is rendering.
 */
bool on_game_render(void)
{
	s3_render_stage();

	return true;
}

/*
 * Argument Access
 */

/*
 * Check if an argument exists.
 */
bool s3_check_arg(const char *name)
{
	int i, count;
	const char *prop_name;

	count = pf_get_tag_property_count();

	for (i = 0; i < count; i++) {
		prop_name = pf_get_tag_property_name(i);
		if (strcmp(name, prop_name) == 0)
			return true;
	}

	return false;
}

/*
 * Get an integer argument.
 */
int s3_get_arg_int(const char *name)
{
	int i, count;
	const char *prop_name;
	const char *prop_value;
	int val;

	count = pf_get_tag_property_count();

	for (i = 0; i < count; i++) {
		prop_name = pf_get_tag_property_name(i);
		if (strcmp(name, prop_name) == 0)
			break;
	}
	if (i == count) {
		/* Not found, return the default value. */
		return 0;
	}

	prop_value = pf_get_tag_property_value(i);
	val = atoi(prop_value);

	return val;
}

/*
 * Get a float argument.
 */
float s3_get_arg_float(const char *name)
{
	int i, count;
	const char *prop_name;
	const char *prop_value;
	float val;

	count = pf_get_tag_property_count();

	for (i = 0; i < count; i++) {
		prop_name = pf_get_tag_property_name(i);
		if (strcmp(name, prop_name) == 0)
			break;
	}
	if (i == count) {
		/* Not found, return the default value. */
		return 0;
	}

	prop_value = pf_get_tag_property_value(i);
	val = (float)atof(prop_value);

	return val;
}

/*
 * Get a string argument.
 */
const char *s3_get_arg_string(
	const char *name)
{
	int i, count;
	const char *prop_name;
	const char *prop_value;

	count = pf_get_tag_property_count();

	for (i = 0; i < count; i++) {
		prop_name = pf_get_tag_property_name(i);
		if (strcmp(name, prop_name) == 0)
			break;
	}
	if (i == count) {
		/* Not found, return the default value. */
		return 0;
	}

	prop_value = pf_get_tag_property_value(i);

	return prop_value;
}

/*
 * Input State
 */

/*
 * Get the mouse X position.
 */
int
s3_get_mouse_pos_x(void)
{
	return mouse_pos_x;
}

/*
 * Get the mouse Y position.
 */
int s3_get_mouse_pos_y(void)
{
	return mouse_pos_y;
}

/*
 * Check if mouse left button is pressed.
 */
bool s3_is_mouse_left_pressed(void)
{
	return is_mouse_left_pressed;
}

/*
 * Check if mouse right button is pressed.
 */
bool s3_is_mouse_right_pressed(void)
{
	return is_mouse_right_pressed;
}

/*
 * Check if mouse left button is pressed then released.
 */
bool s3_is_mouse_left_clicked(void)
{
	return is_mouse_left_clicked;
}

/*
 * Check if mouse right button is pressed then released.
 */
bool s3_is_mouse_right_clicked(void)
{
	return is_mouse_right_clicked;
}

/*
 * Check if mouse is dragging.
 */
bool s3_is_mouse_dragging(void)
{
	return is_mouse_dragging;
}

/*
 * Check if return key is pressed.
 */
bool s3_is_return_key_pressed(void)
{
	return is_return_key_pressed;
}

/*
 * Check if space key is pressed.
 */
bool s3_is_space_key_pressed(void)
{
	return is_space_key_pressed;
}

/*
 * Check if escape key is pressed.
 */
bool s3_is_escape_key_pressed(void)
{
	return is_escape_key_pressed;
}

/*
 * Check if up key is pressed.
 */
bool s3_is_up_key_pressed(void)
{
	return is_up_key_pressed;
}

/*
 * Check if down key is pressed.
 */
bool s3_is_down_key_pressed(void)
{
	return is_down_key_pressed;
}

/*
 * Check if left key is pressed.
 */
bool s3_is_left_key_pressed(void)
{
	return is_left_key_pressed;
}

/*
 * Check if right key is pressed.
 */
bool s3_is_right_key_pressed(void)
{
	return is_right_key_pressed;
}

/*
 * Check if pageup key is pressed.
 */
bool s3_is_pageup_key_pressed(void)
{
	return is_pageup_key_pressed;
}

/*
 * Check if pagedown key is pressed.
 */
bool s3_is_pagedown_key_pressed(void)
{
	return is_pagedown_key_pressed;
}

/*
 * Check if control key is pressed.
 */
bool s3_is_control_key_pressed(void)
{
	return is_control_key_pressed;
}

/*
 * Check if S key is pressed.
 */
bool s3_is_s_key_pressed(void)
{
	return is_s_key_pressed;
}

/*
 * Check if L key is pressed.
 */
bool s3_is_l_key_pressed(void)
{
	return is_l_key_pressed;
}

/*
 * Check if H key is pressed.
 */
bool s3_is_h_key_pressed(void)
{
	return is_h_key_pressed;
}

/*
 * Check if touch is canceled.
 */
bool s3_is_touch_canceled(void)
{
	return is_touch_canceled;
}

/*
 * Check if swiped.
 */
bool s3_is_swiped(void)
{
	return is_swiped;
}

/*
 * Clear input states to avoid further input processing in the current frame.
 */
void s3_clear_input_state(void)
{
	is_mouse_left_pressed = false;
	is_mouse_right_pressed = false;
	is_mouse_left_clicked = false;
	is_mouse_right_clicked = false;
	is_return_key_pressed = false;
	is_up_key_pressed = false;
	is_down_key_pressed = false;
	is_left_key_pressed = false;
	is_right_key_pressed = false;
	is_escape_key_pressed = false;
}

/*
 * Multiple-Frame Command State
 */

/*
 * Start a multiple-frame command execution.
 */
void s3_start_command_repetition(void)
{
	assert(!is_in_repetition);
	is_in_repetition = true;
}

/*
 * Stop a multiple-frame command execution.
 */
void s3_stop_command_repetition(void)
{
	assert(is_in_repetition);
	is_in_repetition = false;
}

/*
 * Check whether we are in a multiple-frame command execution or not.
 */
bool s3_is_in_command_repetition(void)
{
	return is_in_repetition;
}

/*
 * Message Showing State
 *  - Set active when a message shown.
 *  - Keep active when move to a GUI.
 *  - Cleared when move to a next command or loaded a save file.
 */

/*
 * Set the message showing state.
 */
void s3_set_message_active(void)
{
	assert(!flag_message_active);
	flag_message_active = true;
}

/*
 * Reset the message showing state.
 */
void s3_clear_message_active(void)
{
	assert(flag_message_active);
	flag_message_active = false;
}

/*
 * Check whether the message showing state is set or not.
 */
bool s3_is_message_active(void)
{
	return flag_message_active;
}

/*
 * Auto-Mode State
 */

/*
 * Start the auto-mode.
 */
void s3_start_auto_mode(void)
{
	assert(!flag_auto_mode);
	assert(!flag_skip_mode);
	flag_auto_mode = true;
}

/*
 * Stop the auto-mode.
 */
void s3_stop_auto_mode(void)
{
	assert(flag_auto_mode);
	assert(!flag_skip_mode);
	flag_auto_mode = false;
}

/*
 * Check whether we are in the auto-mode or not.
 */
bool s3_is_auto_mode(void)
{
	return flag_auto_mode;
}

/*
 * Skip-Mode State
 */

/*
 * Start the skip-mode.
 */
void s3_start_skip_mode(void)
{
	assert(!flag_skip_mode);
	assert(!flag_auto_mode);
	flag_skip_mode = true;
}

/*
 * Stop the skip-mode.
 */
void s3_stop_skip_mode(void)
{
	assert(flag_skip_mode);
	assert(!flag_auto_mode);
	flag_skip_mode = false;
}

/*
 * Check whether we are in the skip-mode or not.
 */
bool s3_is_skip_mode(void)
{
	return flag_skip_mode;
}

/*
 * Save/Load Enable State
 */

/*
 * Set the save/load enable setting.
 */
void s3_set_save_load(bool enable)
{
	flag_save_load_enabled = enable;
}

/*
 * Get the save/load enable setting.
 */
bool s3_is_save_load_enabled(void)
{
	return flag_save_load_enabled;
}

/*
 * Non-Interruptible State
 */

/*
 * Set the non-interruptible mode setting.
 */
void s3_set_non_interruptible(bool mode)
{
	flag_non_interruptible = mode;
}

/*
 * Get the non-interruptible mode setting.
 */
bool s3_is_non_interruptible(void)
{
	return flag_non_interruptible;
}

/*
 * Pen State
 */

/*
 * Set the pen position.
 */
void s3_set_pen_position(int x, int y)
{
	saved_pen_x = x;
	saved_pen_y = y;
}

/*
 * Get the pen X position.
 */
int s3_get_pen_position_x(void)
{
	return saved_pen_x;
}

/*
 * Get the pen Y position.
 */
int s3_get_pen_position_y(void)
{
	return saved_pen_y;
}

/*
 * Call Arguments
 */

/*
 * Set a calling argument.
 */
bool s3_set_call_argument(int index, const char *val)
{
	assert(index < CALL_ARGS);

	if (call_arg[index] != NULL)
		free(call_arg[index]);
	call_arg[index] = strdup(val);
	if (call_arg[index] == NULL) {
		s3_log_out_of_memory();
		return false;
	}
	return true;
}

/*
 * Get a calling argument.
 */
const char *s3_get_call_argument(int index)
{
	assert(index < CALL_ARGS);

	if (call_arg[index] == NULL)
		return "";

	return call_arg[index];
}

/*
 * Page Mode
 */

/*
 * Set the script page mode.
 */
bool s3_is_page_mode(void)
{
	return conf_game_novel;
}

/*
 * Append a string to the page mode buffer string.
 */
bool s3_append_buffered_message(const char *msg)
{
	char *s;
	size_t len;

	len = 0;
	if (buffered_message != NULL)
		len += strlen(buffered_message);
	len += strlen(msg);

	s = malloc(len + 1);
	if (s == NULL) {
		s3_log_out_of_memory();
		return false;
	}

	strcpy(s, "");
	if (buffered_message != NULL)
		strcat(s, buffered_message);
	strcat(s, msg);

	if (buffered_message != NULL)
	    free(buffered_message);
	buffered_message = s;

	return true;
}

/*
 * Get the page mode buffer string.
 */
const char *s3_get_buffered_message(void)
{
	if (buffered_message == NULL)
		return "";

	return buffered_message;
}

/*
 * Clear the page mode buffer string.
 */
void s3_clear_buffered_message(void)
{
	if (buffered_message != NULL) {
	    free(buffered_message);
	    buffered_message = NULL;
	}
}

/*
 * Reset the message line count in a page.
 */
void s3_reset_page_line(void)
{
	page_line = 0;
}

/*
 * Increment the line count in a page.
 */
void s3_inc_page_line(void)
{
	page_line++;
}

/*
 * Check we are at the first line in a page.
 */
bool s3_is_page_top(void)
{
	if (page_line == 0)
		return true;

	return false;
}

/*
 * BGVoice
 */

/*
 * Register a BGVoice.
 */
bool s3_register_bgvoice(const char *file)
{
	if (bgvoice != NULL) {
		free(bgvoice);
		bgvoice = NULL;
	}

	if (file != NULL) {
		bgvoice = strdup(file);
		if (bgvoice == NULL) {
			s3_log_out_of_memory();
			return false;
		}
	}

	return true;
}

/*
 * Get the BGVoice.
 */
const char *s3_get_bgvoice(void)
{
	return bgvoice;
}

/*
 * Set the BGVoice state playing.
 */
void s3_set_bgvoice_playing(bool is_playing)
{
	flag_bgvoice_playing = is_playing;
}

/*
 * Check whether the BGVoice is playing or not.
 */
bool s3_is_bgvoice_playing(void)
{
	return flag_bgvoice_playing;
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
 * Embedded License String
 */

const char license_info[]
#ifdef __GNUC__
 /* Don't remove this string even if it's not referenced. */
 __attribute__((used))
#endif
 = "Suika3: Copyright (C) 2001-2026 The Suika3 Authors\n"
   "zlib: Copyright (C) 1995-2013 Jean-loup Gailly and Mark Adler. All rights reserved.\n"
   "libpng: Copyright (C) 2000-2002, 2004, 2006-2016, Glenn Randers-Pehrson and the original authors. All rights reserved.\n"
   "jpeg: copyright (C) 1991-2022, Thomas G. Lane, Guido Vollbeding. All rights reserved.\n"
   "bzip2: Copyright (C) 1996-2010 Julian R Seward. All rights reserved.\n"
   "libwebp: Copyright (C) 2010, Google Inc. All rights reserved.\n"
   "libogg: Copyright (C) 2002, Xiph.org Foundation. All rights reserved.\n"
   "libvorbis: Copyright (C) 2002-2015, Xiph.org Foundation. All rights reserved.\n"
   "brotli: Copyright (c) 2009, 2010, 2013-2016 by the Brotli Authors. All rights reserved.\n"
   "FreeType2: Copyright (C) 1996-2002, 2006 by David Turner, Robert Wilhelm, and Werner Lemberg. All rights reserved.\n";
