/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (c) 2001-2026 The Suika3 Authors
 */

/*
 * Animation Subsystem
 */

#ifndef SUIKA3_ANIME_H
#define SUIKA3_ANIME_H

#include <suika3/suika3.h>

/*
 * Maximum amount of registered anime files.
 */
enum reg_anime_index {
	REG_ANIME_COUNT = 16,
};

/*
 * Acceleration
 */
enum anime_accel {
	ANIME_ACCEL_UNIFORM,
	ANIME_ACCEL_ACCEL,
	ANIME_ACCEL_DEACCEL,
};

/* Initialiez the anime subsystem. */
bool init_anime(void);

/* Cleanup the anime subsystem. */
void cleanup_anime(void);

#endif
