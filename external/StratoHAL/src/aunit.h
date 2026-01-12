/* -*- coding: utf-8; indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * Apple Audio Unit Sound
 */

#ifndef PLATFORM_AUNIT_H
#define PLATFORM_AUNIT_H

#include "stratohal/c89compat.h"

bool init_aunit(void);
void cleanup_aunit(void);
void pause_sound(void);
void resume_sound(void);

#endif
