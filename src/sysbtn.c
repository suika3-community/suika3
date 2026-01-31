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

/* Flag to show whether the system button is visible or not. */
static bool is_sysbtn_visible;

/* Flag to show whether the system button is pointed or not. */
static bool is_sysbtn_pointed;

/* Flag to show whether the system button is clicked or not. */
static bool is_sysbtn_clicked;

/*
 * Initialize the sysbtn subsystem.
 */
bool
s3i_init_sysbtn(void)
{
	is_sysbtn_visible = false;
	is_sysbtn_pointed = false;
	is_sysbtn_clicked = false;

	return true;
}

/*
 * Cleanup the sysbtn subsystem.
 */
void
s3i_cleanup_sysbtn(void)
{
	is_sysbtn_visible = false;
	is_sysbtn_pointed = false;
	is_sysbtn_clicked = false;
}

/*
 * Show or hide the system button.
 */
bool
s3_show_sysbtn(
	bool is_visible)
{
	is_sysbtn_visible = is_visible;
	return true;
}

/*
 * Check if the system button is visible.
 */
bool
s3_is_sysbtn_visible(void)
{
	return is_sysbtn_visible;
}

/*
 * Update the system button's pointed state by the mouse position.
 */
void
s3_update_sysbtn_state(void)
{
	struct s3_image *img;
	int mouse_pos_x, mouse_pos_y;
	bool mouse_left_clicked;

	mouse_pos_x = s3_get_mouse_pos_x();
	mouse_pos_y = s3_get_mouse_pos_y();
	mouse_left_clicked = s3_is_mouse_left_clicked();

	if (!is_sysbtn_pointed) {
		img = s3_get_sysbtn_idle_image();
		if (mouse_pos_x >= conf_sysbtn_x &&
		    mouse_pos_x < conf_sysbtn_x + img->width &&
		    mouse_pos_y >= conf_sysbtn_y &&
		    mouse_pos_y < conf_sysbtn_y + img->height)
			is_sysbtn_pointed = true;
		else
			is_sysbtn_pointed = false;
	} else {
		img = s3_get_sysbtn_hover_image();
		if (mouse_pos_x >= conf_sysbtn_x &&
		    mouse_pos_x < conf_sysbtn_x + img->width &&
		    mouse_pos_y >= conf_sysbtn_y &&
		    mouse_pos_y < conf_sysbtn_y + img->height)
			is_sysbtn_pointed = true;
		else
			is_sysbtn_pointed = false;
	}

	if (is_sysbtn_pointed) {
		if (mouse_left_clicked)
			is_sysbtn_clicked = true;
		else
			is_sysbtn_clicked = false;
	}
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
