/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (c) 2001-2026, The authors. All rights reserved.
 */

/*
 * Game Loop and Game States
 */

#include "suika.h"

/* false assertion */
#define COMMAND_DISPATCH_NOT_IMPLEMENTED	(0)

/*
 * Input States
 */

bool is_left_button_pressed;
bool is_right_button_pressed;
bool is_left_clicked;
bool is_right_clicked;
bool is_return_pressed;
bool is_space_pressed;
bool is_escape_pressed;
bool is_up_pressed;
bool is_down_pressed;
bool is_left_arrow_pressed;
bool is_right_arrow_pressed;
bool is_page_up_pressed;
bool is_page_down_pressed;
bool is_control_pressed;
bool is_s_pressed;
bool is_l_pressed;
bool is_h_pressed;
bool is_touch_canceled;
bool is_swiped;
int mouse_pos_x;
int mouse_pos_y;
bool is_mouse_dragging;

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
static char *call_arg[CALL_ARGS];

/* Stored message for the page mode. */
static char *buffered_message;

/* Lines in a page. */
static int page_line;

/* BGVoice */
static char *bgvoice;
static bool flag_bgvoice_playing;

/* Forward declarations. */
static void post_process(void);
static bool dispatch_command(bool *cont);

/*
 * Initialize the game loop.
 */
void init_game_loop(void)
{
	is_left_button_pressed = false;
	is_right_button_pressed = false;
	is_left_clicked = false;
	is_right_clicked = false;
	is_return_pressed = false;
	is_space_pressed = false;
	is_escape_pressed = false;
	is_up_pressed = false;
	is_down_pressed = false;
	is_page_up_pressed = false;
	is_page_down_pressed = false;
	is_control_pressed = false;
	is_s_pressed = false;
	is_l_pressed = false;
	is_h_pressed = false;
	mouse_pos_x = 0;
	mouse_pos_y = 0;
	is_mouse_dragging = false;
	is_in_repetition = false;
	flag_message_active = false;
	flag_auto_mode = false;
	flag_save_load_enabled = true;
	flag_non_interruptible = false;
	page_line = 0;

	srand((unsigned int)time(NULL));
}

/*
 * Cleanup the game looop.
 */
void cleanup_game_loop(void)
{
}

/*
 * Run a game loop content.
 */
bool game_loop_iter(void)
{
	bool is_gui, cont;

	/* Check whether GUI is running or not. */
	is_gui = is_gui_mode();

	/* If a GUI is running. */
	if (is_gui) {
		bool is_cmd_gui = get_command_type() == COMMAND_MENU;

		/* Run a GUI frame. */
		if (!run_gui_mode())
			return false; /* Error. */

		/* If the GUI was finished. */
		if (!is_gui_mode()) {
			if (is_gui_loaded()) {
				/* Cleanup the GUI as a save file was loaded. */
				cleanup_gui();
			} else if (is_cmd_gui) {
				/* Reset is_gui as we are in the gui command. */
				is_gui = false;
			} else {
				/* We were in a system GUI and goto was selected. */
				const char *label = get_gui_result_label();
				if (label != NULL) {
					if (!move_to_label(label))
						return false;
				}
			}
		}
	}

	/* If a GUI is not running. (Or a GUI was just finished.) */
	if (!is_gui) {
		/* Run continuous commands. */
		do {
			/* Do a dispatch. */
			if (!dispatch_command(&cont))
				return false;
		} while (cont);
	}

	post_process();

	return true;
}

/* Do a postprocessing of a frame. */
static void post_process(void)
{
	/* Do a sound fading. */
	process_sound_fading();

	/* Reset input states. */
	is_left_clicked = false;
	is_right_clicked = false;
	is_space_pressed = false;
	is_return_pressed = false;
	is_escape_pressed = false;
	is_up_pressed = false;
	is_down_pressed = false;
	is_left_arrow_pressed = false;
	is_right_arrow_pressed = false;
	is_s_pressed = false;
	is_l_pressed = false;
	is_h_pressed = false;
	is_touch_canceled = false;
	is_swiped = false;
}

/* Dispatch a command. */
static bool dispatch_command(bool *cont)
{
	const char *locale;
	int command_type;

	/* *cont shows whether we should run the next command in this frame or not. */
	*cont = false;

	/* Check the language specifier. */
	if (!is_in_command_repetition()) {
		/* Get the language specifier. */
		locale = get_command_locale();

		/* Remember the last English command. */
		if (strcmp(locale, "en") == 0)
			set_last_en_command();

		/* If the language locale doesn't match. */
		if (strcmp(locale, "") != 0) {
			if (strcmp(locale, conf_game_locale) != 0) {
				/* Do not run the command. */
				*cont = true;
				if (!move_to_next_command())
					return false;
				return true;
			}
		}
	}

	/* Dispatch a command. */
	command_type = get_command_type();
	switch (command_type) {
	case COMMAND_MESSAGE:
	case COMMAND_SERIF:
		if (!message_command(cont))
			return false;
		break;
	case COMMAND_ANIME:
		if (!anime_command())
			return false;
		break;
	case COMMAND_BG:
		if (!bg_command())
			return false;
		break;
	case COMMAND_CH:
		if (!ch_command())
			return false;
		break;
	case COMMAND_CHCH:
		if (!chch_command())
			return false;
		break;
	case COMMAND_CHAPTER:
		if (!chapter_command())
			return false;
		*cont = true;
		break;
	case COMMAND_CHOOSE:
	case COMMAND_ICHOOSE:
	case COMMAND_MCHOOSE:
	case COMMAND_MICHOOSE:
		if (!choose_command())
			return false;
		break;
	case COMMAND_CLICK:
		if (!click_command())
			return false;
		break;
	case COMMAND_CONFIG:
		if (!config_command())
			return false;
		*cont = true;
		break;
	case COMMAND_GOSUB:
		if (!gosub_command())
			return false;
		*cont = true;
		break;
	case COMMAND_GOTO:
	case COMMAND_GOTOLABEL:
		if (!goto_command(cont))
			return false;
		break;
	case COMMAND_IF:
	case COMMAND_UNLESS:
		if (!if_command())
			return false;
		*cont = true;
		break;
	case COMMAND_RETURN:
		if (!return_command(cont))
			return false;
		break;
	case COMMAND_STORY:
		if (!story_command())
			return false;
		*cont = true;
		break;
	case COMMAND_SET:
		if (!set_command())
			return false;
		*cont = true;
		break;
	case COMMAND_SHAKE:
		if (!shake_command())
			return false;
		break;
	case COMMAND_SKIP:
		if (!skip_command())
			return false;
		*cont = true;
		break;
	case COMMAND_SOUND:
		if (!sound_command())
			return false;
		*cont = true;
		break;
	case COMMAND_TEXT:
		if (!text_command())
			return false;
		break;
	case COMMAND_TIME:
		if (!time_command())
			return false;
		break;
	case COMMAND_LABEL:
		*cont = true;
		if (!move_to_next_command())
			return false;
		break;
	case COMMAND_LAYER:
		if (!layer_command())
			return false;
		*cont = true;
		break;
	case COMMAND_MENU:
		if (!menu_command())
			return false;
		break;
	case COMMAND_MOVE:
		if (!move_command())
			return false;
		break;
	case COMMAND_MUSIC:
		if (!music_command())
			return false;
		*cont = true;
		break;
	case COMMAND_NULL:
		if (!move_to_next_command())
			return false;
		*cont = true;
		break;
	case COMMAND_PLUGIN:
		if (!plugin_command())
			return false;
		*cont = true;
		break;
	case COMMAND_VIDEO:
		if (!video_command())
			return false;
		break;
	case COMMAND_VOLUME:
		if (!volume_command())
			return false;
		*cont = true;
		break;
	case COMMAND_CIEL:
		if (!ciel_command(cont))
			return false;
		break;
	default:
		/* No case for the command. */
		assert(COMMAND_DISPATCH_NOT_IMPLEMENTED);
		break;
	}

	return true;
}

/*
 * Clear input states to avoid further input processing in the current frame.
 */
void clear_input_state(void)
{
	is_left_button_pressed = false;
	is_right_button_pressed = false;
	is_left_clicked = false;
	is_right_clicked = false;
	is_return_pressed = false;
	is_up_pressed = false;
	is_down_pressed = false;
	is_left_arrow_pressed = false;
	is_right_arrow_pressed = false;
	is_escape_pressed = false;
}

/*
 * Start a multiple-frame command execution.
 */
void start_command_repetition(void)
{
	assert(!is_in_repetition);
	is_in_repetition = true;
}

/*
 * Stop a multiple-frame command execution.
 */
void stop_command_repetition(void)
{
	assert(is_in_repetition);
	is_in_repetition = false;
}

/*
 * Check whether we are in a multiple-frame command execution or not.
 */
bool is_in_command_repetition(void)
{
	return is_in_repetition;
}

/*
 * Set the message showing state.
 *  - Set active when a message shown.
 *  - Keep active when move to a GUI.
 *  - Cleared when move to a next command or loaded a save file.
 */
void set_message_active(void)
{
	assert(!flag_message_active);
	flag_message_active = true;
}

/*
 * Reset the message showing state.
 */
void clear_message_active(void)
{
	assert(flag_message_active);
	flag_message_active = false;
}

/*
 * Check whether the message showing state is set or not.
 */
bool is_message_active(void)
{
	return flag_message_active;
}

/*
 * Start the auto-mode.
 */
void start_auto_mode(void)
{
	assert(!flag_auto_mode);
	assert(!flag_skip_mode);
	flag_auto_mode = true;
}

/*
 * Stop the auto-mode.
 */
void stop_auto_mode(void)
{
	assert(flag_auto_mode);
	assert(!flag_skip_mode);
	flag_auto_mode = false;
}

/*
 * Check whether we are in the auto-mode or not.
 */
bool is_auto_mode(void)
{
	return flag_auto_mode;
}

/*
 * Start the skip-mode.
 */
void start_skip_mode(void)
{
	assert(!flag_skip_mode);
	assert(!flag_auto_mode);
	flag_skip_mode = true;
}

/*
 * Stop the skip-mode.
 */
void stop_skip_mode(void)
{
	assert(flag_skip_mode);
	assert(!flag_auto_mode);
	flag_skip_mode = false;
}

/*
 * Check whether we are in the skip-mode or not.
 */
bool is_skip_mode(void)
{
	return flag_skip_mode;
}

/*
 * Set the save/load enable setting.
 */
void set_save_load(bool enable)
{
	flag_save_load_enabled = enable;
}

/*
 * Get the save/load enable setting.
 */
bool is_save_load_enabled(void)
{
	return flag_save_load_enabled;
}

/*
 * Set the non-interruptible mode setting.
 */
void set_non_interruptible(bool mode)
{
	flag_non_interruptible = mode;
}

/*
 * Get the non-interruptible mode setting.
 */
bool is_non_interruptible(void)
{
	return flag_non_interruptible;
}

/*
 * Set the pen position.
 */
void set_pen_position(int x, int y)
{
	saved_pen_x = x;
	saved_pen_y = y;
}

/*
 * Get the pen X position.
 */
int get_pen_position_x(void)
{
	return saved_pen_x;
}

/*
 * Get the pen Y position.
 */
int get_pen_position_y(void)
{
	return saved_pen_y;
}

/*
 * Set a calling argument.
 */
bool set_call_argument(int index, const char *val)
{
	assert(index < CALL_ARGS);

	if (call_arg[index] != NULL)
		free(call_arg[index]);
	call_arg[index] = strdup(val);
	if (call_arg[index] == NULL) {
		log_memory();
		return false;
	}
	return true;
}

/*
 * Get a calling argument.
 */
const char *get_call_argument(int index)
{
	assert(index < CALL_ARGS);

	if (call_arg[index] == NULL)
		return "";

	return call_arg[index];
}

/*
 * Set the script page mode.
 */
bool is_page_mode(void)
{
	return conf_game_novel;
}

/*
 * Append a string to the page mode buffer string.
 */
bool append_buffered_message(const char *msg)
{
	char *s;
	size_t len;

	len = 0;
	if (buffered_message != NULL)
		len += strlen(buffered_message);
	len += strlen(msg);

	s = malloc(len + 1);
	if (s == NULL) {
		log_memory();
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
const char *get_buffered_message(void)
{
	if (buffered_message == NULL)
		return "";

	return buffered_message;
}

/*
 * Clear the page mode buffer string.
 */
void clear_buffered_message(void)
{
	if (buffered_message != NULL) {
	    free(buffered_message);
	    buffered_message = NULL;
	}
}

/*
 * Reset the message line count in a page.
 */
void reset_page_line(void)
{
	page_line = 0;
}

/*
 * Increment the line count in a page.
 */
void inc_page_line(void)
{
	page_line++;
}

/*
 * Check we are at the first line in a page.
 */
bool is_page_top(void)
{
	if (page_line == 0)
		return true;

	return false;
}

/*
 * Register a BGVoice.
 */
bool register_bgvoice(const char *file)
{
	if (bgvoice != NULL) {
		free(bgvoice);
		bgvoice = NULL;
	}

	if (file != NULL) {
		bgvoice = strdup(file);
		if (bgvoice == NULL) {
			log_memory();
			return false;
		}
	}

	return true;
}

/*
 * Get the BGVoice.
 */
const char *get_bgvoice(void)
{
	return bgvoice;
}

/*
 * Set the BGVoice state playing.
 */
void set_bgvoice_playing(bool is_playing)
{
	flag_bgvoice_playing = is_playing;
}

/*
 * Check whether the BGVoice is playing or not.
 */
bool is_bgvoice_playing(void)
{
	return flag_bgvoice_playing;
}

/*
 * Embedded License String
 */

const char license_info[]
#ifdef __GNUC__
 /* Don't remove this string even if it's not referenced. */
 __attribute__((used))
#endif
 = "Suika3: Copyright (c) 2001-2026, The authors. All rights reserved.\n"
   "zlib: Copyright (C) 1995-2013 Jean-loup Gailly and Mark Adler. All rights reserved.\n"
   "libpng: Copyright (C) 2000-2002, 2004, 2006-2016, Glenn Randers-Pehrson and the original authors. All rights reserved.\n"
   "jpeg: copyright (C) 1991-2022, Thomas G. Lane, Guido Vollbeding. All rights reserved.\n"
   "bzip2: Copyright (C) 1996-2010 Julian R Seward. All rights reserved.\n"
   "libwebp: Copyright (C) 2010, Google Inc. All rights reserved.\n"
   "libogg: Copyright (C) 2002, Xiph.org Foundation. All rights reserved.\n"
   "libvorbis: Copyright (C) 2002-2015, Xiph.org Foundation. All rights reserved.\n"
   "brotli: Copyright (c) 2009, 2010, 2013-2016 by the Brotli Authors. All rights reserved.\n"
   "FreeType2: Copyright (C) 1996-2002, 2006 by David Turner, Robert Wilhelm, and Werner Lemberg. All rights reserved.\n";
