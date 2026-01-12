/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * BSD / UNIX Sound
 */

#ifndef PLATFORM_BSDSOUND_H
#define PLATFORM_BSDSOUND_H

#include <stratohal/c89compat.h>

/* Initialize sound. */
bool init_sound(void);

/* Cleanup sound. */
void cleanup_sound(void);

#endif
