/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * Image Drawing Functions
 *  - Included by image.h
 *  - Included by SSE source files
 */

#include "stratohal/platform.h"
#include <assert.h>

extern bool is_avx2_available;
extern bool is_avx_available;
extern bool is_sse42_available;
extern bool is_sse4_available;
extern bool is_sse3_available;
extern bool is_sse2_available;
extern bool is_sse_available;

bool check_draw_image(struct image *dst_image, int *dst_left, int *dst_top,
		      struct image *src_image, int *width, int *height,
		      int *src_left, int *src_top, int alpha);

void
DRAW_IMAGE_COPY(
	struct image *dst_image,
	int dst_left,
	int dst_top,
	struct image *src_image,
	int width,
	int height,
	int src_left,
	int src_top)
{
	pixel_t * RESTRICT src_ptr;
	pixel_t * RESTRICT dst_ptr;
	int x, y, sw, dw;

	if (!check_draw_image(dst_image, &dst_left, &dst_top, src_image, &width, &height, &src_left, &src_top, 255))
		return;

	sw = src_image->width;
	dw = dst_image->width;
	src_ptr = src_image->pixels + sw * src_top + src_left;
	dst_ptr = dst_image->pixels + dw * dst_top + dst_left;

	for(y = 0; y < height; y++) {
		for(x = 0; x < width; x++)
			*(dst_ptr + x) = *(src_ptr + x);
		src_ptr += sw;
		dst_ptr += dw;
	}

	notify_image_update(dst_image);
}

void
DRAW_IMAGE_ALPHA(
	struct image *dst_image,
	int dst_left,
	int dst_top,
	struct image *src_image,
	int width,
	int height,
	int src_left,
	int src_top,
	int alpha)
{
	pixel_t * RESTRICT src_ptr;
	pixel_t * RESTRICT dst_ptr;
	float a, src_r, src_g, src_b, src_a, dst_r, dst_g, dst_b, dst_a;
	uint32_t src_pix, dst_pix;
	int src_line_inc, dst_line_inc, x, y, sw, dw;

	if (!check_draw_image(dst_image, &dst_left, &dst_top, src_image, &width, &height, &src_left, &src_top, alpha))
		return;

	sw = src_image->width;
	dw = dst_image->width;
	src_ptr = src_image->pixels + sw * src_top + src_left;
	dst_ptr = dst_image->pixels + dw * dst_top + dst_left;
	src_line_inc = sw - width;
	dst_line_inc = dw - width;
	a = (float)alpha / 255.0f;

	for(y = 0; y < height; y++) {
		for(x = 0; x < width; x++) {
			/* Get the source and destination pixel values. */
			src_pix	= *src_ptr++;
			dst_pix	= *dst_ptr;

			/* Calc alpha values. */
			src_a = a * ((float)get_pixel_a(src_pix) / 255.0f);
			dst_a = 1.0f - src_a;

			/* Multiply the alpha value and the source pixel value. */
			src_r = src_a * (float)get_pixel_r(src_pix);
			src_g = src_a * (float)get_pixel_g(src_pix);
			src_b = src_a * (float)get_pixel_b(src_pix);

			/* Multiply the alpha value and the destination pixel value. */
			dst_r = dst_a * (float)get_pixel_r(dst_pix);
			dst_g = dst_a * (float)get_pixel_g(dst_pix);
			dst_b = dst_a * (float)get_pixel_b(dst_pix);

			/* Store to the destination. */
			*dst_ptr++ = make_pixel(0xff,
						(uint32_t)(src_r + dst_r),
						(uint32_t)(src_g + dst_g),
						(uint32_t)(src_b + dst_b));
		}
		src_ptr += src_line_inc;
		dst_ptr += dst_line_inc;
	}

	notify_image_update(dst_image);
}

void
DRAW_IMAGE_GLYPH(
	struct image *dst_image,
	int dst_left,
	int dst_top,
	struct image *src_image,
	int width,
	int height,
	int src_left,
	int src_top,
	int alpha)
{
	pixel_t * RESTRICT src_ptr;
	pixel_t * RESTRICT dst_ptr;
	float a, src_r, src_g, src_b, src_a, dst_r, dst_g, dst_b, dst_a;
	uint32_t src_pix, dst_pix, dst_a_i, alpha_i;
	int src_line_inc, dst_line_inc, x, y, sw, dw;

	if (!check_draw_image(dst_image, &dst_left, &dst_top, src_image, &width, &height, &src_left, &src_top, alpha))
		return;

	sw = src_image->width;
	dw = dst_image->width;
	src_ptr = src_image->pixels + sw * src_top + src_left;
	dst_ptr = dst_image->pixels + dw * dst_top + dst_left;
	src_line_inc = sw - width;
	dst_line_inc = dw - width;
	a = (float)alpha / 255.0f;

	for(y = 0; y < height; y++) {
		for(x = 0; x < width; x++) {
			/* Get the source and destination pixel values. */
			src_pix	= *src_ptr++;
			dst_pix	= *dst_ptr;

			/* Calc alpha values. */
			src_a = a * ((float)get_pixel_a(src_pix) / 255.0f);
			dst_a = 1.0f - src_a;

			/* Multiply the alpha value and the source pixel value. */
			src_r = src_a * (float)get_pixel_r(src_pix);
			src_g = src_a * (float)get_pixel_g(src_pix);
			src_b = src_a * (float)get_pixel_b(src_pix);

			/* Multiply the alpha value and the destination pixel value. */
			dst_r = dst_a * (float)get_pixel_r(dst_pix);
			dst_g = dst_a * (float)get_pixel_g(dst_pix);
			dst_b = dst_a * (float)get_pixel_b(dst_pix);
			dst_a_i = get_pixel_a(dst_pix);

			alpha_i = src_a > dst_a ? (uint32_t)(src_a * 255.0f) : dst_a_i;

			/* Store to the destination. */
			*dst_ptr++ = make_pixel(alpha_i,
						(uint32_t)(src_r + dst_r),
						(uint32_t)(src_g + dst_g),
						(uint32_t)(src_b + dst_b));
		}
		src_ptr += src_line_inc;
		dst_ptr += dst_line_inc;
	}

	notify_image_update(dst_image);
}

void
DRAW_IMAGE_EMOJI(
	struct image *dst_image,
	int dst_left,
	int dst_top,
	struct image *src_image,
	int width,
	int height,
	int src_left,
	int src_top,
	int alpha)
{
	pixel_t * RESTRICT src_ptr;
	pixel_t * RESTRICT dst_ptr;
	float a, src_r, src_g, src_b, src_a, dst_r, dst_g, dst_b, dst_a;
	uint32_t src_pix, dst_pix, dst_a_i, alpha_i;
	int src_line_inc, dst_line_inc, x, y, sw, dw;

	if (!check_draw_image(dst_image, &dst_left, &dst_top, src_image, &width, &height, &src_left, &src_top, alpha))
		return;

	sw = src_image->width;
	dw = dst_image->width;
	src_ptr = src_image->pixels + sw * src_top + src_left;
	dst_ptr = dst_image->pixels + dw * dst_top + dst_left;
	src_line_inc = sw - width;
	dst_line_inc = dw - width;
	a = (float)alpha / 255.0f;

	for(y = 0; y < height; y++) {
		for(x = 0; x < width; x++) {
			src_pix	= *src_ptr++;
			dst_pix	= *dst_ptr;

			src_a = a * ((float)get_pixel_a(src_pix) / 255.0f);
			dst_a = 1.0f - src_a;

			src_r = src_a * (float)get_pixel_r(src_pix);
			src_g = src_a * (float)get_pixel_g(src_pix);
			src_b = src_a * (float)get_pixel_b(src_pix);

			dst_r = dst_a * (float)get_pixel_r(dst_pix);
			dst_g = dst_a * (float)get_pixel_g(dst_pix);
			dst_b = dst_a * (float)get_pixel_b(dst_pix);
			dst_a_i = get_pixel_a(dst_pix);

			alpha_i = src_a > dst_a ? (uint32_t)(src_a * 255.0f) : dst_a_i;

			*dst_ptr++ = make_pixel(alpha_i,
						(uint32_t)(src_r + dst_r),
						(uint32_t)(src_g + dst_g),
						(uint32_t)(src_b + dst_b));
		}
		src_ptr += src_line_inc;
		dst_ptr += dst_line_inc;
	}

	notify_image_update(dst_image);
}

void
DRAW_IMAGE_ADD(
	struct image *dst_image,
	int dst_left,
	int dst_top,
	struct image *src_image,
	int width,
	int height,
	int src_left,
	int src_top,
	int alpha)
{
	pixel_t * RESTRICT src_ptr;
	pixel_t * RESTRICT dst_ptr;
	float a, src_a;
	uint32_t src_pix, src_r, src_g, src_b;
	uint32_t dst_pix, dst_r, dst_g, dst_b;
	uint32_t add_r, add_g, add_b;
	int src_line_inc, dst_line_inc, x, y, sw, dw;

	if (!check_draw_image(dst_image, &dst_left, &dst_top, src_image, &width, &height, &src_left, &src_top, alpha))
		return;

	sw = src_image->width;
	dw = dst_image->width;
	src_ptr = src_image->pixels + sw * src_top + src_left;
	dst_ptr = dst_image->pixels + dw * dst_top + dst_left;
	src_line_inc = sw - width;
	dst_line_inc = dw - width;
	a = (float)alpha / 255.0f;

	for(y = 0; y < height; y++) {
		for(x = 0; x < width; x++) {
			/* Get the source and destination pixel values. */
			src_pix	= *src_ptr++;
			dst_pix	= *dst_ptr;

			/* Calc alpha values. */
			src_a = a * ((float)get_pixel_a(src_pix) / 255.0f);

			/* Multiply the alpha value and the source pixel value. */
			src_r = (uint32_t)(src_a * ((float)get_pixel_r(src_pix) / 255.0f) * 255.0f);
			src_g = (uint32_t)(src_a * ((float)get_pixel_g(src_pix) / 255.0f) * 255.0f);
			src_b = (uint32_t)(src_a * ((float)get_pixel_b(src_pix) / 255.0f) * 255.0f);

			/* Multiply the alpha value and the destination pixel value. */
			dst_r = get_pixel_r(dst_pix);
			dst_g = get_pixel_g(dst_pix);
			dst_b = get_pixel_b(dst_pix);

			/* Add with saturation. */
			add_r = src_r + dst_r;
			if (add_r > 255)
				add_r = 255;
			add_g = src_g + dst_g;
			if (add_g > 255)
				add_g = 255;
			add_b = src_b + dst_b;
			if (add_b > 255)
				add_b = 255;

			/* Store to the destination. */
			*dst_ptr++ = make_pixel(0xff,
						add_r,
						add_g,
						add_b);
		}
		src_ptr += src_line_inc;
		dst_ptr += dst_line_inc;
	}

	notify_image_update(dst_image);
}

void
DRAW_IMAGE_SUB(
	struct image *dst_image,
	int dst_left,
	int dst_top,
	struct image *src_image,
	int width,
	int height,
	int src_left,
	int src_top,
	int alpha)
{
	pixel_t * RESTRICT src_ptr;
	pixel_t * RESTRICT dst_ptr;
	float a, src_a;
	uint32_t src_pix, src_r, src_g, src_b;
	uint32_t dst_pix, dst_r, dst_g, dst_b;
	uint32_t add_r, add_g, add_b;
	int src_line_inc, dst_line_inc, x, y, sw, dw;

	if (!check_draw_image(dst_image, &dst_left, &dst_top, src_image, &width, &height, &src_left, &src_top, alpha))
		return;

	sw = src_image->width;
	dw = dst_image->width;
	src_ptr = src_image->pixels + sw * src_top + src_left;
	dst_ptr = dst_image->pixels + dw * dst_top + dst_left;
	src_line_inc = sw - width;
	dst_line_inc = dw - width;
	a = (float)alpha / 255.0f;

	for(y = 0; y < height; y++) {
		for(x = 0; x < width; x++) {
			/* Get the source and destination pixel values. */
			src_pix	= *src_ptr++;
			dst_pix	= *dst_ptr;

			/* Calc alpha values. */
			src_a = a * ((float)get_pixel_a(src_pix) / 255.0f);

			/* Multiply the alpha value and the source pixel value. */
			src_r = (uint32_t)(src_a * ((float)get_pixel_r(src_pix) / 255.0f) * 255.0f);
			src_g = (uint32_t)(src_a * ((float)get_pixel_g(src_pix) / 255.0f) * 255.0f);
			src_b = (uint32_t)(src_a * ((float)get_pixel_b(src_pix) / 255.0f) * 255.0f);

			/* Multiply the alpha value and the destination pixel value. */
			dst_r = get_pixel_r(dst_pix);
			dst_g = get_pixel_g(dst_pix);
			dst_b = get_pixel_b(dst_pix);

			/* Add with saturation. */
			add_r = dst_r - src_r;
			if (add_r > 255)
				add_r = 0;
			add_g = dst_g - src_g;
			if (add_g > 255)
				add_g = 0;
			add_b = dst_b - src_b;
			if (add_b > 255)
				add_b = 0;

			/* Store to the destination. */
			*dst_ptr++ = make_pixel(0xff,
						add_r,
						add_g,
						add_b);
		}
		src_ptr += src_line_inc;
		dst_ptr += dst_line_inc;
	}

	notify_image_update(dst_image);
}

void
DRAW_IMAGE_DIM(
	struct image *dst_image,
	int dst_left,
	int dst_top,
	struct image *src_image,
	int width,
	int height,
	int src_left,
	int src_top,
	int alpha)
{
	pixel_t * RESTRICT src_ptr, * RESTRICT dst_ptr;
	float a, src_r, src_g, src_b, src_a, dst_r, dst_g, dst_b, dst_a;
	uint32_t src_pix, dst_pix;
	int src_line_inc, dst_line_inc, x, y, sw, dw;

	if (!check_draw_image(dst_image, &dst_left, &dst_top, src_image, &width, &height, &src_left, &src_top, 255))
		return;

	sw = src_image->width;
	dw = dst_image->width;
	src_ptr = src_image->pixels + sw * src_top + src_left;
	dst_ptr = dst_image->pixels + dw * dst_top + dst_left;
	src_line_inc = sw - width;
	dst_line_inc = dw - width;
	a = (float)alpha / 255.0f;

	for(y = 0; y < height; y++) {
		for(x = 0; x < width; x++) {
			/* Get the source and destination pixel values. */
			src_pix	= *src_ptr++;
			dst_pix	= *dst_ptr;

			/* Calc alpha values. */
			src_a = a * ((float)get_pixel_a(src_pix) / 255.0f);
			dst_a = 1.0f - src_a;

			/* Multiply 0.5 x alpha value and the source pixel value. */
			src_r = src_a * 0.5f * (float)get_pixel_r(src_pix);
			src_g = src_a * 0.5f * (float)get_pixel_g(src_pix);
			src_b = src_a * 0.5f * (float)get_pixel_b(src_pix);

			/* Multiply the alpha value and the destination pixel value. */
			dst_r = dst_a * (float)get_pixel_r(dst_pix);
			dst_g = dst_a * (float)get_pixel_g(dst_pix);
			dst_b = dst_a * (float)get_pixel_b(dst_pix);

			/* Store to the destination. */
			*dst_ptr++ = make_pixel(0xff,
						(uint32_t)(src_r + dst_r),
						(uint32_t)(src_g + dst_g),
						(uint32_t)(src_b + dst_b));
		}
		src_ptr += src_line_inc;
		dst_ptr += dst_line_inc;
	}

	notify_image_update(dst_image);
}

void
DRAW_IMAGE_RULE(
	struct image *dst_image,
	struct image *src_image,
	struct image *rule_image,
	int threshold)
{
	pixel_t * RESTRICT src_ptr;
	pixel_t * RESTRICT dst_ptr;
	pixel_t * RESTRICT rule_ptr;
	int x, y, dw, sw, rw, w, dh, sh, rh, h;

	assert(dst_image != NULL);
	assert(src_image != NULL);
	assert(rule_image != NULL);

	/* Calc the width. */
	dw = dst_image->width;
	sw = src_image->width;
	rw = rule_image->width;
	w = dw;
	if (sw < w)
		w = sw;
	if (rw < w)
		w = rw;
	
	/* Calc the height. */
	dh = dst_image->height;
	sh = src_image->height;
	rh = rule_image->height;
	h = dh;
	if (sh < h)
		h = sh;
	if (rh < h)
		h = rh;

	/* Draw. */
	dst_ptr = dst_image->pixels;
	src_ptr = src_image->pixels;
	rule_ptr = rule_image->pixels;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++)
			if (get_pixel_b(*(rule_ptr + x)) <= (unsigned char)threshold)
				*(dst_ptr + x) = *(src_ptr + x);
		dst_ptr += dw;
		src_ptr += sw;
		rule_ptr += rw;
	}

	notify_image_update(dst_image);
}

void
DRAW_IMAGE_MELT(
	struct image *dst_image,
	struct image *src_image,
	struct image *rule_image,
	int threshold)
{
	pixel_t * RESTRICT src_ptr;
	pixel_t * RESTRICT dst_ptr;
	pixel_t * RESTRICT rule_ptr;
	pixel_t src_pix, dst_pix, rule_pix;
	float src_a, src_r, src_g, src_b, dst_a, dst_r, dst_g, dst_b, rule_a;
	int x, y, dw, sw, rw, w, dh, sh, rh, h;

	assert(dst_image != NULL);
	assert(src_image != NULL);
	assert(rule_image != NULL);

	/* Calc the width. */
	dw = dst_image->width;
	sw = src_image->width;
	rw = rule_image->width;
	w = dw;
	if (sw < w)
		w = sw;
	if (rw < w)
		w = rw;
	
	/* Calc the height. */
	dh = dst_image->height;
	sh = src_image->height;
	rh = rule_image->height;
	h = dh;
	if (sh < h)
		h = sh;
	if (rh < h)
		h = rh;

	/* Draw. */
	dst_ptr = dst_image->pixels;
	src_ptr = src_image->pixels;
	rule_ptr = rule_image->pixels;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			/* Get a source pixel. */
			src_pix = src_ptr[x];

			/* Get a destination pixel. */
			dst_pix = dst_ptr[x];

			/* Get a rule pixel. */
			rule_pix = rule_ptr[x];

			/* Calc alpha. */
			rule_a = (float)get_pixel_b(rule_pix) / 255.0f;
			src_a = 2.0f * ((float)threshold / 255.0f) - rule_a;
			src_a = src_a < 0 ? 0 : src_a;
			src_a = src_a > 1.0f ? 1.0f : src_a;
			dst_a = 1.0f - src_a;

			/* Multiply alpha. */
			src_r = src_a * (float)get_pixel_r(src_pix);
			src_g = src_a * (float)get_pixel_g(src_pix);
			src_b = src_a * (float)get_pixel_b(src_pix);

			/* Multiply the alpha value and the destination pixel value. */
			dst_r = dst_a * (float)get_pixel_r(dst_pix);
			dst_g = dst_a * (float)get_pixel_g(dst_pix);
			dst_b = dst_a * (float)get_pixel_b(dst_pix);

			/* Store to the destination. */
			dst_ptr[x] = make_pixel(0xff,
						(uint32_t)(src_r + dst_r),
						(uint32_t)(src_g + dst_g),
						(uint32_t)(src_b + dst_b));
		}
		dst_ptr += dw;
		src_ptr += sw;
		rule_ptr += rw;
	}

	notify_image_update(dst_image);
}

void
DRAW_IMAGE_SCALE(
	struct image *dst_image,
	int virtual_dst_width,
	int virtual_dst_height,
	int virtual_dst_left,
	int virtual_dst_top,
	struct image *src_image)
{
	pixel_t * RESTRICT dst_ptr;
	pixel_t * RESTRICT src_ptr;
	float scale_x, scale_y;
	pixel_t src_pix, dst_pix;
	float src_a, src_r, src_g, src_b, dst_a, dst_r, dst_g, dst_b;
	int real_dst_width, real_dst_height;
	int real_src_width, real_src_height;
	int real_draw_left, real_draw_top, real_draw_width, real_draw_height;
	int virtual_x, virtual_y;
	int i, j;

	assert(dst_image != NULL);
	assert(src_image != NULL);

	/* Get the destination size. */
	real_dst_width = dst_image->width;
	real_dst_height = dst_image->height;

	/* Calc the scale. */
	scale_x = (float)real_dst_width / (float)virtual_dst_width;
	scale_y = (float)real_dst_height / (float)virtual_dst_height;

	/* Get the source size. */
	real_src_width = src_image->width;
	real_src_height = src_image->height;

	/* Calc the destination position and size. */
	real_draw_left = (int)((float)virtual_dst_left * scale_x);
	real_draw_top = (int)((float)virtual_dst_top * scale_y);
	real_draw_width = (int)((float)real_src_width * scale_x);
	real_draw_height = (int)((float)real_src_height * scale_y);

	/* Get the pixel pointes. */
	dst_ptr = dst_image->pixels;
	src_ptr = src_image->pixels;

	/* Draw. */
	for (i = real_draw_top; i < real_draw_top + real_draw_height; i++) {
		/* Clip by a destination Y. */
		if (i < 0)
			continue;
		if (i >= real_dst_height)
			continue;

		/* Calc a source Y, and clip. */
		virtual_y = (int)((float)i / scale_y) - virtual_dst_top;
		if (virtual_y < 0)
			continue;
		if (virtual_y >= real_src_height)
			continue;

		for (j = real_draw_left; j < real_draw_left + real_draw_width;
		     j++) {
			/* Clip by a destination X. */
			if (j < 0)
				continue;
			if (j >= real_dst_width)
				continue;

			/* Calc a source X, and clip. */
			virtual_x = (int)((float)j / scale_x) - virtual_dst_left;
			if (virtual_x < 0)
				continue;
			if (virtual_x >= real_src_width)
				continue;

			/* Get a source pixel. */
			src_pix = src_ptr[real_src_width * virtual_y + virtual_x];

			/* Get a destination pixel. */
			dst_pix = dst_ptr[real_dst_width * i + j];

			/* Calc alpha. */
			src_a = (float)get_pixel_a(src_pix) / 255.0f;
			dst_a = 1.0f - src_a;

			/* Multiply alpha to a source pixel. */
			src_r = src_a * (float)get_pixel_r(src_pix);
			src_g = src_a * (float)get_pixel_g(src_pix);
			src_b = src_a * (float)get_pixel_b(src_pix);

			/* Multiply alpha to a destination pixel. */
			dst_r = dst_a * (float)get_pixel_r(dst_pix);
			dst_g = dst_a * (float)get_pixel_g(dst_pix);
			dst_b = dst_a * (float)get_pixel_b(dst_pix);

			/* Store to the destination. */
			dst_ptr[real_dst_width * i + j] = make_pixel(0xff,
								     (uint32_t)(src_r + dst_r),
								     (uint32_t)(src_g + dst_g),
								     (uint32_t)(src_b + dst_b));
		}
	}

	notify_image_update(dst_image);
}
