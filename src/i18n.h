/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Playfield Engine
 * Copyright (c) 2025, 2026, Awe Morris. All rights reserved.
 */

/*
 * Pseudo gettext support.
 */

#ifndef PLAYFIELD_I18N_H
#define PLAYFIELD_I18N_H

#include <playfield/playfield.h>

#ifdef USE_TRANSLATION

/* Initialized the locale. */
void playfield_init_locale(void);

#endif

#endif
