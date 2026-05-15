/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Playfield Engine
 * Copyright (c) 2025, 2026, Awe Morris. All rights reserved.
 */

/*
 * Dummy Initialization Hook
 */

#include <playfield/playfield.h>

/* Entrypoint. */
PF_DEFINE_MAIN()

/*
 * Initialization function for derived engines.
 */
bool
pf_init_hook(
	int width,
	int height)
{
	UNUSED_PARAMETER(width);
	UNUSED_PARAMETER(height);

	/* Stub. */
	return true;
}
