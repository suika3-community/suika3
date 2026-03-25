/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * System Button Subsystem
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
#include "sysbtn.h"
#include "conf.h"
#include "image.h"
#include "sysbtn.h"

#include <stdio.h>
#include <assert.h>

/* Fade in time in milliseconds. */
#define FADE_IN_TIME	200

/* Fade out time in milliseconds. */
#define FADE_OUT_TIME	500

/* Amount to detect mouse move to show the sysbtn. */
#define MOUSE_DELTA	5

/* Time to hide the sysbtn in milliseconds. */
#define DISAPPEAR_TIME	1000

/* Time to wait after system menu GUI in milliseconds. */
#define GUI_GRACE_TIME	500

/* Flag to show whether the system button is enabled or not. */
static bool is_sysbtn_enabled;

/* Flag to show whether the system button is pointed or not. */
static bool is_sysbtn_pointed;

/* Flag to show whether the system button is clicked or not. */
static bool is_sysbtn_clicked;

static int last_mouse_pos_x;
static int last_mouse_pos_y;
static int state;
static uint64_t sw;
static int alpha;

enum State {
	ST_OUT,
	ST_FADE_IN,
	ST_APPEAR,
	ST_HOVER,
	ST_FADE_OUT,
	ST_GUI,
	ST_GUI_WAIT,
};

/*
 * Initialize the sysbtn subsystem.
 */
bool
s3i_init_sysbtn(void)
{
	state = ST_OUT;
	s3_reset_lap_timer(&sw);

	is_sysbtn_enabled = false;
	is_sysbtn_pointed = false;
	is_sysbtn_clicked = false;

	last_mouse_pos_x = 0;
	last_mouse_pos_y = 0;

	return true;
}

/*
 * Cleanup the sysbtn subsystem.
 */
void
s3i_cleanup_sysbtn(void)
{
	is_sysbtn_enabled = false;
	is_sysbtn_pointed = false;
	is_sysbtn_clicked = false;
}

/*
 * Control the system button.
 */
void
s3_enable_sysbtn(
	bool is_enabled)
{
	if (!is_enabled) {
		switch (state) {
		case ST_OUT:
			/* Leave as is. (hidden) */
			break;
		case ST_FADE_OUT:
			/* Leave as is. (fading out) */
			break;
		case ST_FADE_IN:
		case ST_APPEAR:
		case ST_HOVER:
			/* Start fading out. */
			state = ST_FADE_OUT;
			s3_reset_lap_timer(&sw);
			alpha = 255;
			break;
		case ST_GUI_WAIT:
			/* Leave as is. (GUI timeout) */
			break;
		}

		is_sysbtn_enabled = false;
	} else {
		if (!is_sysbtn_enabled) {
			/* Set the initial state. */
			state = ST_OUT;
			s3_reset_lap_timer(&sw);
			alpha = 0;
			is_sysbtn_enabled = true;
		} else {
			/* Leave as is. */
		}
	}
}

/*
 * Check if the system button is enabled.
 */
bool
s3_is_sysbtn_enabled(void)
{
	return is_sysbtn_enabled;
}

/*
 * Update the system button's pointed state by the mouse position.
 */
void
s3_update_sysbtn_state(void)
{
	struct s3_image *idle_img, *hover_img;
	int mouse_pos_x, mouse_pos_y;
	bool mouse_left_clicked;

	mouse_pos_x = s3_get_mouse_pos_x();
	mouse_pos_y = s3_get_mouse_pos_y();
	mouse_left_clicked = s3_is_mouse_left_clicked();

	idle_img = s3_get_sysbtn_idle_image();
	hover_img = s3_get_sysbtn_hover_image();

	if (s3_is_gui_running()) {
		/* Special state. */
		state = ST_GUI;
	}

	if (state == ST_OUT) {
		if (is_sysbtn_enabled) {
			if (mouse_pos_x - last_mouse_pos_x > MOUSE_DELTA ||
			    mouse_pos_y - last_mouse_pos_y > MOUSE_DELTA) {
				/* Fade in. */
				state = ST_FADE_IN;
				s3_reset_lap_timer(&sw);
				alpha = 0;

				if (mouse_pos_x >= conf_sysbtn_x &&
				    mouse_pos_x < conf_sysbtn_x + idle_img->width &&
				    mouse_pos_y >= conf_sysbtn_y &&
				    mouse_pos_y < conf_sysbtn_y + idle_img->height)
					is_sysbtn_pointed = true;
				else
					is_sysbtn_pointed = false;
			} else {
				is_sysbtn_pointed = false;
				alpha = 0;
			}
		}
	} else if (state == ST_FADE_IN) {
		uint64_t lap;

		assert(is_sysbtn_enabled);

		lap = s3_get_lap_timer_millisec(&sw);
		if (lap >= FADE_IN_TIME) {
			/* Show. */
			state = ST_APPEAR;
			s3_reset_lap_timer(&sw);
			alpha = 255;
		} else {
			alpha = (int)((float)lap / (float)FADE_IN_TIME * 255.0f);
		}

		if (mouse_pos_x >= conf_sysbtn_x &&
		    mouse_pos_x < conf_sysbtn_x + idle_img->width &&
		    mouse_pos_y >= conf_sysbtn_y &&
		    mouse_pos_y < conf_sysbtn_y + idle_img->height)
			is_sysbtn_pointed = true;
		else
			is_sysbtn_pointed = false;
	} else if (state == ST_APPEAR) {
		assert(is_sysbtn_enabled);

		if (mouse_pos_x >= conf_sysbtn_x &&
		    mouse_pos_x < conf_sysbtn_x + idle_img->width &&
		    mouse_pos_y >= conf_sysbtn_y &&
		    mouse_pos_y < conf_sysbtn_y + idle_img->height) {
			/* Pointed. */
			state = ST_HOVER;
			s3_reset_lap_timer(&sw);
			is_sysbtn_pointed = true;
			alpha = 255;
		} else {
			/* Not pointed. */
			if (mouse_pos_x - last_mouse_pos_x > MOUSE_DELTA ||
			    mouse_pos_y - last_mouse_pos_y > MOUSE_DELTA) {
				/* Mouse moved, extend the timer. */
				s3_reset_lap_timer(&sw);
				is_sysbtn_pointed = false;
				alpha = 255;
			} else if (s3_get_lap_timer_millisec(&sw) >= DISAPPEAR_TIME) {
				/* Hide. */
				state = ST_FADE_OUT;
				s3_reset_lap_timer(&sw);
				is_sysbtn_pointed = false;
				alpha = 255;
			}
		}
	} else if (state == ST_HOVER) {
		assert(is_sysbtn_enabled);

		if (mouse_pos_x >= conf_sysbtn_x &&
		    mouse_pos_x < conf_sysbtn_x + hover_img->width &&
		    mouse_pos_y >= conf_sysbtn_y &&
		    mouse_pos_y < conf_sysbtn_y + hover_img->height) {
			/* Pointed. */
			is_sysbtn_pointed = true;
			alpha = 255;
		} else {
			/* Not pointed. */
			state = ST_APPEAR;
			s3_reset_lap_timer(&sw);
			is_sysbtn_pointed = false;
			alpha = 255;
		}
	} else if (state == ST_FADE_OUT) {
		uint64_t lap = s3_get_lap_timer_millisec(&sw);
		if (is_sysbtn_enabled &&
		    ((mouse_pos_x - last_mouse_pos_x > MOUSE_DELTA) ||
		     (mouse_pos_y - last_mouse_pos_y > MOUSE_DELTA))) {
			/* Fade in. */
			state = ST_APPEAR;
			s3_reset_lap_timer(&sw);
			alpha = 255;
			if (mouse_pos_x >= conf_sysbtn_x &&
			    mouse_pos_x < conf_sysbtn_x + idle_img->width &&
			    mouse_pos_y >= conf_sysbtn_y &&
			    mouse_pos_y < conf_sysbtn_y + idle_img->height)
				is_sysbtn_pointed = true;
			else
				is_sysbtn_pointed = false;
		} else if (lap >= FADE_OUT_TIME) {
			/* Hide. */
			state = ST_OUT;
			s3_reset_lap_timer(&sw);
			is_sysbtn_pointed = false;
			alpha = 0;
		} else {
			is_sysbtn_pointed = false;
			alpha = 255 - (int)((float)lap / (float)FADE_OUT_TIME * 255.0f);
		}
	} else if (state == ST_GUI) {
		if (!s3_is_gui_running()) {
			state = ST_GUI_WAIT;
			s3_reset_lap_timer(&sw);
			is_sysbtn_pointed = false;
			alpha = 0;
		}
	} else if (state == ST_GUI_WAIT) {
		uint64_t lap = s3_get_lap_timer_millisec(&sw);
		if (lap >= GUI_GRACE_TIME) {
			state = ST_OUT;
			is_sysbtn_pointed = false;
			alpha = 0;
		} else {
			is_sysbtn_pointed = false;
			alpha = 0;
		}
	}

	if (is_sysbtn_enabled && is_sysbtn_pointed && state == ST_HOVER) {
		if (mouse_left_clicked)
			is_sysbtn_clicked = true;
		else
			is_sysbtn_clicked = false;
	}

	last_mouse_pos_x = mouse_pos_x;
	last_mouse_pos_y = mouse_pos_y;
}

/*
 * Check if the system bytton is pointed.
 */
bool
s3_is_sysbtn_pointed(void)
{
	return is_sysbtn_pointed;
}

/*
 * Check if the system bytton is clicked.
 */
bool
s3_is_sysbtn_clicked(void)
{
	return is_sysbtn_clicked;
}

/*
 * Get the alpha value of the system button.
 */
int
s3i_get_sysbtn_alpha(void)
{
	return alpha;
}
