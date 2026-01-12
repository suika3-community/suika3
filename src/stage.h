/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (c) 2001-2026, The Suika3 Authors. All rights reserved.
 */

/*
 * Stage Management and Rendering
 */

#ifndef SUIKA3_STAGE_H
#define SUIKA3_STAGE_H

/* Initialize the stage subsystem. */
bool init_stage(void);

/* Cleanup the stage subsystem. */
void cleanup_stage(void);

/* Setup the name box. */
bool setup_namebox(void);

/* Setup the message box. */
bool setup_msgbox(void);

/* Setup the click animation. */
bool setup_click(void);

/* Setup the choose boxes. */
bool setup_choose(bool is_hover, int index);

/* Setup the system button. */
bool setup_sysbtn(void);

/* Setup the banners. */
bool setup_banners(void);

#endif
