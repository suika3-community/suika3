/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Image Management (wrappers of Playfield Engine)
 */

/*-
 * SPDX-License-Identifier: Zlib
 *
 * Copyright (c) 2026 The Suika3 Community
 * Copyright (c) 2025-2026 The Playfield Engine Project
 * Copyright (c) 2025-2026 The NoctVM Project
 * Copyright (c) 2025-2026 Awe Morris
 * Copyright (c) 2016-2024 The Suika2 Development Team
 * Copyright (c) 1996-2024 Keiichi Tabata
 *
 * This software is derived from the codebase of Playfield Engine, NoctLang,
 * Suika2, Suika Studio, Wind Game Lib, and 98/AT Game Lib.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <suika3/suika3.h>
#include <playfield/playfield.h>
#include "image.h"

#include <stdlib.h>

/*
 * Initialize the image subsystem.
 */
bool
init_image(void)
{
	return true;
}

/*
 * Cleanup the image subsystem.
 */
void
cleanup_image(void)
{
}

/*
 * Load an image from a file.
 */
struct s3_image *
s3_create_image_from_file(
	const char *file)
{
	struct s3_image *img;

	img = malloc(sizeof(struct s3_image));
	if (img == NULL) {
		s3_log_out_of_memory();
		return NULL;
	}

	if (!pf_load_texture(file, &img->tex_id, &img->width, &img->height)) {
		free(img);
		return false;
	}

	return img;
}

/*
 * Create an image.
 */
struct s3_image *
s3_create_image(int width,
		int height)
{
	struct s3_image *img;

	img = malloc(sizeof(struct s3_image));
	if (img == NULL) {
		s3_log_out_of_memory();
		return NULL;
	}

	if (!pf_create_color_texture(width, height, 0, 0, 0, 0, &img->tex_id)) {
		free(img);
		return false;
	}

	img->width = width;
	img->height = height;

	return img;
}

/*
 * Get the width of an image.
 */
int
s3_get_image_width(
	struct s3_image *img)
{
	return img->width;
}

/*
 * Get the height of an image.
 */
int
s3_get_image_height(
	struct s3_image *img)
{
	return img->height;
}

/*
 * Load a glyph image.
 */
struct s3_image *
s3_load_glyph_image(
	int slot,
	int codepoint,
	int size,
	hal_pixel_t color,
	int outline_width,
	hal_pixel_t outline_color)
{
	struct s3_image *img;
	char mbs[6];

	/* Create an struct s3_image. */
	img = malloc(sizeof(struct s3_image));
	if (img == NULL) {
		s3_log_out_of_memory();
		return NULL;
	}

	/* Convert utf-32 to utf-8.*/
	if (codepoint > 0x10FFFF ||
	    (codepoint >= 0xD800 && codepoint <= 0xDFFF)) {
		mbs[0] = '\0';	/* Error. */
	} else if (codepoint <= 0x7F) {
		mbs[0] = (char)codepoint;
		mbs[1] = '\0';
	} else if (codepoint <= 0x7FF) {
		mbs[0] = (char)(0xC0 | (codepoint >> 6));
		mbs[1] = (char)(0x80 | (codepoint & 0x3F));
		mbs[2] = '\0';
	} else if (codepoint <= 0xFFFF) {
		mbs[0] = (char)(0xE0 | (codepoint >> 12));
		mbs[1] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
		mbs[2] = (char)(0x80 | (codepoint & 0x3F));
		mbs[3] = '\0';
	} else { /* codepoint <= 0x10FFFF */
		mbs[0] = (char)(0xF0 | (codepoint >> 18));
		mbs[1] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
		mbs[2] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
		mbs[3] = (char)(0x80 | (codepoint & 0x3F));
		mbs[4] = '\0';
	}

	/* Get a texture. */
	if (!pf_create_text_texture_outline(slot,
					    mbs,
					    size,
					    color,
					    outline_width,
					    outline_color,
					    &img->tex_id,
					    &img->width,
					    &img->height)) {
		free(img);
		return NULL;
	}

	return img;
}

/*
 * Destroy an image.
 */
void
s3_destroy_image(
	struct s3_image *image)
{
	pf_destroy_texture(image->tex_id);
	free(image);
}

/*
 * Notify an update of an image.
 */
void s3_notify_image_update(
	struct s3_image *image)
{
	pf_notify_texture_update(image->tex_id);
}

/*
 * Copy an image to an image.
 */
void
s3_draw_image_copy(
	struct s3_image *dst,
	int dst_left,
	int dst_top,
	struct s3_image *src,
	int dst_width,
	int dst_height,
	int src_left,
	int src_top)
{
	pf_draw_texture_copy(
		dst->tex_id,
		dst_left,
		dst_top,
		src->tex_id,
		dst_width,
		dst_height,
		src_left,
		src_top);
	pf_notify_texture_update(dst->tex_id);
}

/*
 * Draw an image to an image with alpha blending.
 */
void
s3_draw_image_alpha(
	struct s3_image *dst,
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	struct s3_image *src,
	int src_left,
	int src_top,
	int alpha)
{
	pf_draw_texture_alpha(
		dst->tex_id,
		dst_left,
		dst_top,
		src->tex_id,
		dst_width,
		dst_height,
		src_left,
		src_top,
		alpha);
	pf_notify_texture_update(dst->tex_id);
}

/*
 * Draw an image to an image with add blending.
 */
void
s3_draw_image_add(
	struct s3_image *dst,
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	struct s3_image *src,
	int src_left,
	int src_top,
	int alpha)
{
	pf_draw_texture_add(
		dst->tex_id,
		dst_left,
		dst_top,
		src->tex_id,
		dst_width,
		dst_height,
		src_left,
		src_top,
		alpha);
	pf_notify_texture_update(dst->tex_id);
}

/*
 * Draw an image to an image with sub blending.
 */
void
s3_draw_image_sub(
	struct s3_image *dst,
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	struct s3_image *src,
	int src_left,
	int src_top,
	int alpha)
{
	pf_draw_texture_sub(
		dst->tex_id,
		dst_left,
		dst_top,
		src->tex_id,
		dst_width,
		dst_height,
		src_left,
		src_top,
		alpha);
	pf_notify_texture_update(dst->tex_id);
}

/*
 * Draw an image to an image with sub blending.
 */
void
s3_draw_image_dim(
	struct s3_image *dst,
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	struct s3_image *src,
	int src_left,
	int src_top,
	int alpha)
{
	pf_draw_texture_dim(
		dst->tex_id,
		dst_left,
		dst_top,
		src->tex_id,
		dst_width,
		dst_height,
		src_left,
		src_top,
		alpha);
	pf_notify_texture_update(dst->tex_id);
}

/*
 * Draw a glyph image to an image.
 */
void
s3_draw_image_glyph(
	struct s3_image *dst,
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	struct s3_image *src,
	int src_left,
	int src_top,
	int alpha)
{
	pf_draw_texture_glyph(
		dst->tex_id,
		dst_left,
		dst_top,
		src->tex_id,
		dst_width,
		dst_height,
		src_left,
		src_top,
		alpha);
	pf_notify_texture_update(dst->tex_id);
}

/* Draw an emoji image to an image. */
void
s3_draw_image_emoji(
	struct s3_image *dst,
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	struct s3_image *src,
	int src_left,
	int src_top,
	int alpha)
{
	pf_draw_texture_emoji(
		dst->tex_id,
		dst_left,
		dst_top,
		src->tex_id,
		dst_width,
		dst_height,
		src_left,
		src_top,
		alpha);
	pf_notify_texture_update(dst->tex_id);
}

/* Draw an image with scaling. */
void
s3_draw_image_scale(
	struct s3_image *dst,
	int virtual_dst_width,
	int virtual_dst_height,
	int virtual_dst_left,
	int virtual_dst_top,
	struct s3_image *src)
{
	pf_draw_texture_scale(
		dst->tex_id,
		virtual_dst_width,
		virtual_dst_height,
		virtual_dst_left,
		virtual_dst_top,
		src->tex_id);
	pf_notify_texture_update(dst->tex_id);
}

/*
 * Make a pixel value.
 */
s3_pixel_t
s3_make_pixel(
	int a,
	int r,
	int g,
	int b)
{
	return hal_make_pixel(a, r, g, b);
}

/*
 * Fill a rectangle on an image.
 */
void
s3_fill_image_rect(
	struct s3_image *image,
	int left,
	int top,
	int width,
	int height,
	s3_pixel_t color)
{
	pf_fill_texture_rect(image->tex_id,
			     left,
			     top,
			     width,
			     height,
			     color);
}

/*
 * Get the raw pixel pointer of an image.
 */
s3_pixel_t *
s3_get_image_pixels(
	struct s3_image *image)
{
	return pf_get_texture_pixels(image->tex_id);
}
