/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (c) 2001-2026 The Suika3 Authors
 */

/*
 * Game Main Logic
 */

#ifndef SUIKA_GAME_H
#define SUIKA_GAME_H

#include <suika3/suika3.h>

/* Called before the game starts. */
bool playfield_init_hook(void);

/* Called when the game starts. */
bool on_game_start(void);

/* Called when a frame is updated. */
bool on_game_update(void);

/* Called when a frame is rendering. */
bool on_game_render(void);

#endif
