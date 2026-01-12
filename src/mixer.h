/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (c) 2001-2026 The Suika3 Authors
 */

/*
 * Audio Mixer
 */

#ifndef SUIKA3_MIXER_H
#define SUIKA3_MIXER_H

#include <suika3/suika3.h>

/* Initialize the mixer subsystem. */
bool init_mixer(void);

/* Cleanup the mixer subsystem. */
void cleanup_mixer(void);

/* Process sound fading. */
void process_sound_fading(void);

#endif
