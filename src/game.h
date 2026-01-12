/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (c) 2001-2026, The authors. All rights reserved.
 */

/*
 * Game Main Logic
 */

#ifndef SUIKA_GAMELOOP_H
#define SUIKA_GAMELOOP_H

#include <suika.h>

/*
 * Game Loop
 */

void init_game_loop(void);
bool game_loop_iter(void);
void cleanup_game_loop(void);

/*
 * Input States
 */
extern bool is_left_button_pressed;
extern bool is_right_button_pressed;
extern bool is_left_clicked;
extern bool is_right_clicked;
extern bool is_return_pressed;
extern bool is_space_pressed;
extern bool is_escape_pressed;
extern bool is_up_pressed;
extern bool is_down_pressed;
extern bool is_left_arrow_pressed;
extern bool is_right_arrow_pressed;
extern bool is_page_up_pressed;
extern bool is_page_down_pressed;
extern bool is_control_pressed;
extern bool is_s_pressed;
extern bool is_l_pressed;
extern bool is_h_pressed;
extern int mouse_pos_x;
extern int mouse_pos_y;
extern bool is_mouse_dragging;
extern bool is_touch_canceled;
extern bool is_swiped;

/* Clear input states to avoid further input processing in the current frame. */
void clear_input_state(void);

/*
 * Multiple-Frame Command Control
 */

void start_command_repetition(void);
void stop_command_repetition(void);
bool is_in_command_repetition(void);

/*
 * Message Active Settings
 *  - Set active when a message shown.
 *  - Keep active when move to a GUI.
 *  - Cleared when move to a next command or loaded a save file.
 */

void set_message_active(void);
void clear_message_active(void);
bool is_message_active(void);

/*
 * Auto-Mode Settings
 */

void start_auto_mode(void);
void stop_auto_mode(void);
bool is_auto_mode(void);

/*
 * Skip-Mode Settings
 */

void start_skip_mode(void);
void stop_skip_mode(void);
bool is_skip_mode(void);

/*
 * Save/Load Enable Settings
 */

void set_save_load(bool enable);
bool is_save_load_enabled(void);

/*
 * Non-Interruptible Mode Settings
 */

void set_non_interruptible(bool mode);
bool is_non_interruptible(void);

/*
 * Pen Position Settings
 */

void set_pen_position(int x, int y);
int get_pen_position_x(void);
int get_pen_position_y(void);

/*
 * Argument Settings
 */

bool set_call_argument(int index, const char *val);
const char *get_call_argument(int index);

/*
 * Script Page Mode Settings
 */

bool is_page_mode(void);
bool append_buffered_message(const char *msg);
const char *get_buffered_message(void);
void clear_buffered_message(void);
void reset_page_line(void);
void inc_page_line(void);
bool is_page_top(void);

/*
 * BGVoice Settings
 */

bool register_bgvoice(const char *msg);
const char *get_bgvoice(void);
void set_bgvoice_playing(bool is_playing);
bool is_bgvoice_playing(void);

#endif
