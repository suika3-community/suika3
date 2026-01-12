/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * Emscripten OpenAL 1.1 Sound
 */

#ifndef PLATFORM_ALSOUND_H
#define PLATFORM_ALSOUND_H

#include "stratohal/platform.h"

bool init_openal(void);
void cleanup_openal(void);
void fill_sound_buffer(void);
void pause_sound(void);
void resume_sound(void);

#endif
