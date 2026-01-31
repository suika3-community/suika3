/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * SeedScript API
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
#include <playfield/playfield.h>
#include "game.h"
#include "command.h"

struct api_func {
	const char *name;
	bool (*func)(void *);
	bool no_args;
};

struct tag_func {
	const char *name;
	bool (*func)(void *);
};

static bool Suika_start(void *p);
static bool Suika_update(void *p);
static bool Suika_render(void *p);

static struct api_func api_func[] = {
	/* Skeleton */
	{"start", Suika_start, true},
	{"update", Suika_update, true},
	{"render", Suika_render, true},
};

static struct tag_func tag_func[] = {
	/* Commands */
	{"Tag_bg", s3i_command_bg},
};

/*
 * Install the API.
 */
bool
s3i_install_default_api(void)
{
	const char *params[] = {"param"};
	int i;

	/* Register functions. */
	for (i = 0; i < sizeof(api_func) / sizeof(struct api_func); i++) {
		if (!s3_install_api(api_func[i].name,
				    api_func[i].func,
				    api_func[i].no_args))
			return false;
	}

	/* Register functions. */
	for (i = 0; i < sizeof(tag_func) / sizeof(struct tag_func); i++) {
		if (!s3_install_tag(tag_func[i].name,
				    tag_func[i].func))
			return false;
	}

	return true;
}

/* suikaStart() */
static bool
Suika_start(
	void *p)
{
	if (!s3i_on_game_start())
		return false;

	return true;
}

/* suikaUpdate() */
static bool
Suika_update(
	void *p)
{
	if (!s3i_on_game_update())
		return false;

	return true;
}

/* suikaRender() */
static bool
Suika_render(
	void *p)
{
	if (!s3i_on_game_render())
		return false;

	return true;
}
