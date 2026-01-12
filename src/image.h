/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (C) 2001-2026 The Suika3 Authors
 */

/*
 * Image Management
 */

#ifndef SUIKA3_IMAGE_H
#define SUIKA3_IMAGE_H

#include <suika3/suika3.h>

struct s3_image {
	int tex_id;
	int width;
	int height;
};

/* Initialize the image subsystem. */
bool
init_image(void);

/* Cleanup the image subsystem. */
void
cleanup_image(void);

#endif
