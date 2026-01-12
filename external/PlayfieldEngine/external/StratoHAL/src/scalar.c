/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * Scalar (Non-Vectorized) Version of the Image Drawing Functions
 */

#if defined(USE_SSE_DISPATCH)

#define DRAW_IMAGE_COPY		draw_image_copy_scalar
#define DRAW_IMAGE_ALPHA	draw_image_alpha_scalar
#define DRAW_IMAGE_GLYPH	draw_image_glyph_scalar
#define DRAW_IMAGE_ADD		draw_image_add_scalar
#define DRAW_IMAGE_DIM		draw_image_dim_scalar
#define DRAW_IMAGE_RULE		draw_image_rule_scalar
#define DRAW_IMAGE_MELT		draw_image_melt_scalar
#define DRAW_IMAGE_SCALE	draw_image_scale_scalar

#include "drawimage.h"

#endif
