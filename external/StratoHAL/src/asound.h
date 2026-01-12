/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * ALSA Sound
 */

#ifndef PLATFORM_ASOUND_H
#define PLATFORM_ASOUND_H

#include "stratohal/c89compat.h"

/* Initialize ALSA. */
bool init_sound(void);

/* Cleanup ALSA. */
void cleanup_sound(void);

#endif
