/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (c) 2001-2026, The Suika3 Authors. All rights reserved.
 */

/*
 * Suika3 API for Playfield Script
 */

#include <suika3/suika3.h>
#include <playfield/playfield.h>
#include "game.h"
#include "command.h"

struct api_func {
	const char *name;
	bool (*func)(void *);
};

static bool api_suika_start(void *p);
static bool api_suika_update(void *p);
static bool api_suika_render(void *p);

static struct api_func api_func[] = {
	/* Skeleton */
	{"suikaStart",  api_suika_start},
	{"suikaUpdate", api_suika_update},
	{"suikaRender", api_suika_render},

	/* Commands */
	{"Tag_bg", command_bg},
};

bool init_api(void)
{
	const char *params[] = {"param"};
	const int tbl_size = sizeof(api_func) / sizeof(struct api_func);
	int i;

	/* Register functions. */
	for (i = 0; i < tbl_size; i++) {
		if (!pf_install_api(api_func[i].name, api_func[i].func))
			return false;
	}

	return 0;
}

static bool api_suika_start(void *p)
{
	if (!on_game_start())
		return false;

	return true;
}

static bool api_suika_update(void *p)
{
	if (!on_game_update())
		return false;

	return true;
}

static bool api_suika_render(void *p)
{
	if (!on_game_render())
		return false;

	return true;
}
