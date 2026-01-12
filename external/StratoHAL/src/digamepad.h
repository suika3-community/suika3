/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 4; c-basic-offset: 4; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * DirectInput Gamepad Input
 */

#ifndef STRATOHAL_DIGAMEPAD_H
#define STRATOHAL_DIGAMEPAD_H

#include <windows.h>

BOOL DInputInitialize(HINSTANCE hInst, HWND hWnd);
VOID DInputCleanup(VOID);
VOID DInputUpdate(VOID);

#endif
