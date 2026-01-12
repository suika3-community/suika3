/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * AVX Version of the Image Drawing Functions
 */

#if defined(USE_SSE_DISPATCH)

#define DRAW_IMAGE_COPY		draw_image_copy_avx
#define DRAW_IMAGE_ALPHA	draw_image_alpha_avx
#define DRAW_IMAGE_GLYPH	draw_image_glyph_avx
#define DRAW_IMAGE_ADD		draw_image_add_avx
#define DRAW_IMAGE_DIM		draw_image_dim_avx
#define DRAW_IMAGE_RULE		draw_image_rule_avx
#define DRAW_IMAGE_MELT		draw_image_melt_avx
#define DRAW_IMAGE_SCALE	draw_image_scale_avx

#include "drawimage.h"

#endif
