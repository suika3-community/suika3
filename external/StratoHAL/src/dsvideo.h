/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * DirectShow Video Playback
 */

#ifndef PLATFORM_DSVIDEO_H
#define PLATFORM_DSVIDEO_H

#include "stratohal/platform.h"

#include <windows.h>

#define WM_GRAPHNOTIFY	(WM_APP + 13)

BOOL DShowPlayVideo(HWND hWnd, const char *pszFileName, int nOfsX, int nOfsY, int nWidth, int nHeight);
VOID DShowStopVideo(void);
BOOL DShowProcessEvent(void);

#endif
