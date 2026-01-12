/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (c) 2001-2026, The Suika3 Authors. All rights reserved.
 */

/*
 * Suika3 API for Playfield Script
 */

#include <suika.h>
#include "command.h"

static NoctEnv *env;

struct api_func {
	const char *name;
	bool (*func)(NoctEnv *);
};

static bool api_suika_start(NoctEnv *env);
static bool api_suika_update(NoctEnv *env);
static bool api_suika_render(NoctEnv *env);

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
	const int tbl_size = sizeof(funcs) / sizeof(struct func);
	int i;

	/* Register functions. */
	for (i = 0; i < tbl_size; i++) {
		if (!pf_register_func(api_func[i].name, api_func[i].func))
			return false;
	}

	return 0;
}

static bool api_suika_start(NoctEnv *env)
{
	if (!on_game_start())
		return false;

	return true;
}

static bool api_suika_update(NoctEnv *env)
{
	if (!on_game_update())
		return false;

	return true;
}

static bool api_suika_render(NoctEnv *env)
{
	if (!on_game_render())
		return false;

	return true;
}
