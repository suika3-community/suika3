/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * DirectSound Audio
 */

#ifndef PLATFORM_DSOUND_H
#define PLATFORM_DSOUND_H

#include <windows.h>

BOOL DSInitialize(HWND hWnd);
VOID DSCleanup();

#endif
