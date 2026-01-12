/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * Image Manipulation
 */

#include "stratohal/platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#if defined(TARGET_WINDOWS)
#include <malloc.h>	/* _aligned_mallo() */
#endif

/* 512-bit alignment */
#define ALIGN_BYTES	(64)

/* Texture ID */
static int id_top;

/*
 * SSE Flags
 */
#if defined(USE_SSE_DISPATCH)
extern bool is_avx2_available;
extern bool is_avx_available;
extern bool is_sse42_available;
extern bool is_sse4_available;
extern bool is_sse3_available;
extern bool is_sse2_available;
extern bool is_sse_available;
#endif

/*
 * Forward Declaraion
 */

bool check_draw_image(struct image *dst_image, int *dst_left, int *dst_top,
		      struct image *src_image, int *width, int *height,
		      int *src_left, int *src_top, int alpha);

#if defined(TARGET_WINDOWS)
static void *wrap_aligned_malloc(size_t size, size_t align);
static void wrap_aligned_free(void *p);
#endif

/*
 * Initialization
 */

#if defined(USE_DLL)
void cleanup_image(void)
{
	id_top = 0;
}
#endif

/*
 * Create an image.
 */
bool create_image(int w, int h, struct image **img)
{
	pixel_t *pixels;

	assert(w > 0 && h > 0);
	assert(img != NULL);

	/* Allocate a struct buffer. */
	*img = malloc(sizeof(struct image));
	if (*img == NULL) {
		log_out_of_memory();
		return false;
	}
	memset(*img, 0, sizeof(struct image));

	/* Allocate a pixel buffer. */
#if defined(TARGET_WINDOWS)
	pixels = wrap_aligned_malloc((size_t)w * (size_t)h * sizeof(pixel_t), 64);
	if (pixels == NULL) {
		log_out_of_memory();
		free(*img);
		return false;
	}
#elif !defined(USE_UNITY)
	if (posix_memalign((void **)&pixels, 64, (size_t)w * (size_t)h * sizeof(pixel_t)) != 0) {
		log_out_of_memory();
		free(*img);
		return false;
	}
#else
	pixels = malloc((size_t)w * (size_t)h * sizeof(pixel_t));
	if (pixels == NULL) {
		log_out_of_memory();
		free(*img);
		return false;
	}
#endif

	/* Initialize struct members. */
	(*img)->width = w;
	(*img)->height = h;
	(*img)->pixels = pixels;
	(*img)->id = id_top++;

	return true;
}

/*
 * Creata an image with a color string. (#rrggbb)
 */
struct image *create_image_from_color_string(int w, int h, const char *color)
{
	struct image *img;
	uint32_t r, g, b;
	pixel_t cl;
	int rgb;

	/* Create an image. */
	if (!create_image(w, h, &img))
		return NULL;

	/* Read a color string. */
	rgb = 0;
	(void)sscanf(color, "%x", &rgb);
	r = (rgb >> 16) & 0xff;
	g = (rgb >> 8) & 0xff;
	b = rgb & 0xff;
	cl = make_pixel(0xff, r, g, b);

	/* Fill the image with the color. */
	clear_image(img, cl);

	return img;
}

/*
 * Create an image with a pixel buffer.
 */
bool create_image_with_pixels(int w, int h, pixel_t *pixels, struct image **img)
{
	assert(w > 0 && h > 0);
	assert(pixels != NULL);
	assert(img != NULL);

	/* Alloc a struct buffer. */
	*img = malloc(sizeof(struct image));
	if (*img == NULL) {
		log_out_of_memory();
		return false;
	}
	memset(*img, 0, sizeof(struct image));

	/* Initialize the members. */
	(*img)->width = w;
	(*img)->height = h;
	(*img)->pixels = pixels;
	(*img)->no_free = true;
	(*img)->id = id_top++;

	return true;
}

/*
 * Destroy an image.
 */
void destroy_image(struct image *img)
{
	assert(img != NULL);
	assert(img->width > 0 && img->height > 0);
	assert(img->pixels != NULL);

	/* Free a texture. */
	notify_image_free(img);

	/* Free a pixel buffer. */
	if (!img->no_free) {
#if defined(TARGET_WINDOWS)
		wrap_aligned_free(img->pixels);
#else
		free(img->pixels);
#endif
	}
	img->pixels = NULL;

	/* Free a struct buffer. */
	free(img);
}

/*
 * Clear
 */

/*
 * Clear an image with black color.
 */
void clear_image(struct image *img, pixel_t cl)
{
	clear_image_rect(img, 0, 0, img->width, img->height, cl);
}

/*
 * Clear a rectangle with a specified color.
 */
void clear_image_rect(struct image *img, int x, int y, int w, int h, pixel_t color)
{
	pixel_t *pixels;
	int i, j, sx, sy;

	assert(img != NULL);
	assert(img->width > 0 && img->height > 0);
	assert(img->pixels != NULL);

	/* Return if no need for drawing. */
	if(w == 0 || h == 0)
		return;
	sx = sy = 0;
	if(!clip_by_dest(img->width, img->height, &w, &h, &x, &y, &sx, &sy))
		return;

	assert(x >= 0 && x < img->width);
	assert(w >= 0 && x + w <= img->width);
	assert(y >= 0 && y < img->height);
	assert(h >= 0 && y + h <= img->height);

	/* Clear pixels of a rectangle. */
	pixels = img->pixels;
	for (i = y; i < y + h; i++)
		for (j = x; j < x + w; j++)
			pixels[img->width * i + j] = color;

	/* Request a texture update. */
	notify_image_update(img);
}

/*
 * Clear alpha channel with 255.
 */
void fill_image_alpha(struct image *img)
{
	pixel_t *p;
	int y, x;

	assert(img != NULL);

	p = img->pixels;
	for (y = 0; y < img->height; y++) {
		for (x = 0; x < img->width; x++) {
			*p = (*p) | 0xff000000;
			p++;
		}
	}
}

/*
 * Drawing
 */

#if !defined(USE_SSE_DISPATCH)

/*
 * Define the draw_image_*() functions directly.
 */
#define DRAW_IMAGE_COPY		draw_image_copy
#define DRAW_IMAGE_ALPHA	draw_image_alpha
#define DRAW_IMAGE_GLYPH	draw_image_glyph
#define DRAW_IMAGE_EMOJI	draw_image_emoji
#define DRAW_IMAGE_ADD		draw_image_add
#define DRAW_IMAGE_SUB		draw_image_sub
#define DRAW_IMAGE_DIM		draw_image_dim
#define DRAW_IMAGE_RULE		draw_image_rule
#define DRAW_IMAGE_MELT		draw_image_melt
#define DRAW_IMAGE_SCALE	draw_image_scale
#include "drawimage.h"

#else

/*
 * Define the dispatch functions.
 */

void draw_image_copy(struct image *dst_image,
		     int dst_left,
		     int dst_top,
		     struct image *src_image,
		     int width,
		     int height,
		     int src_left,
		     int src_top)
{
	void draw_image_copy_avx2(struct image *, int dst_left, int, struct image *, int, int, int, int);
	void draw_image_copy_avx(struct image *, int dst_left, int, struct image *, int, int, int, int);
	void draw_image_copy_sse42(struct image *, int dst_left, int, struct image *, int, int, int, int);
	void draw_image_copy_sse4(struct image *, int dst_left, int, struct image *, int, int, int, int);
	void draw_image_copy_sse3(struct image *, int dst_left, int, struct image *, int, int, int, int);
	void draw_image_copy_sse2(struct image *, int dst_left, int, struct image *, int, int, int, int);
	void draw_image_copy_sse(struct image *, int dst_left, int, struct image *, int, int, int, int);
	void draw_image_copy_scalar(struct image *, int dst_left, int, struct image *, int, int, int, int);

	if (is_avx2_available)
		draw_image_copy_avx2(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top);
	else if (is_avx_available)
		draw_image_copy_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top);
#if !defined(_MSC_VER)
	else if (is_sse42_available)
		draw_image_copy_sse42(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top);
	else if (is_sse4_available)
		draw_image_copy_sse4(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top);
	else if (is_sse3_available)
		draw_image_copy_sse3(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top);
#endif
	else if (is_sse2_available)
		draw_image_copy_sse2(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top);
#if !defined(_MSC_VER) && defined(ARCH_X86)
	else if (is_sse_available)
		draw_image_copy_sse(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top);
#endif
	else
		draw_image_copy_scalar(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top);
}

void draw_image_alpha(struct image *dst_image,
		      int dst_left,
		      int dst_top,
		      struct image *src_image,
		      int width,
		      int height,
		      int src_left,
		      int src_top,
		      int alpha)
{
	void draw_image_alpha_avx2(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_alpha_avx(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_alpha_sse42(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_alpha_sse4(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_alpha_sse3(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_alpha_sse2(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_alpha_sse(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_alpha_scalar(struct image *, int dst_left, int, struct image *, int, int, int, int, int);

	if (is_avx2_available)
		draw_image_alpha_avx2(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_avx_available)
		draw_image_alpha_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#if !defined(_MSC_VER)
	else if (is_sse42_available)
		draw_image_alpha_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_sse4_available)
		draw_image_alpha_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_sse3_available)
		draw_image_alpha_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#endif
	else if (is_sse2_available)
		draw_image_alpha_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#if !defined(_MSC_VER) && defined(ARCH_X86)
	else if (is_sse_available)
		draw_image_alpha_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#endif
	else
		draw_image_alpha_scalar(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
}

void draw_image_glyph(struct image *dst_image,
		      int dst_left,
		      int dst_top,
		      struct image *src_image,
		      int width,
		      int height,
		      int src_left,
		      int src_top,
		      int alpha)
{
	void draw_image_glyph_avx2(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_glyph_avx(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_glyph_sse42(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_glyph_sse4(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_glyph_sse3(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_glyph_sse2(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_glyph_sse(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_glyph_scalar(struct image *, int dst_left, int, struct image *, int, int, int, int, int);

	if (is_avx2_available)
		draw_image_glyph_avx2(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_avx_available)
		draw_image_glyph_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#if !defined(_MSC_VER)
	else if (is_sse42_available)
		draw_image_glyph_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_sse4_available)
		draw_image_glyph_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_sse3_available)
		draw_image_glyph_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#endif
	else if (is_sse2_available)
		draw_image_glyph_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#if !defined(_MSC_VER) && defined(ARCH_X86)
	else if (is_sse_available)
		draw_image_glyph_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#endif
	else
		draw_image_glyph_scalar(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
}

void draw_image_emoji(struct image *dst_image,
		      int dst_left,
		      int dst_top,
		      struct image *src_image,
		      int width,
		      int height,
		      int src_left,
		      int src_top,
		      int alpha)
{
	void draw_image_emoji_avx2(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_emoji_avx(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_emoji_sse42(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_emoji_sse4(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_emoji_sse3(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_emoji_sse2(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_emoji_sse(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_emoji_scalar(struct image *, int dst_left, int, struct image *, int, int, int, int, int);

	if (is_avx2_available)
		draw_image_emoji_avx2(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_avx_available)
		draw_image_emoji_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#if !defined(_MSC_VER)
	else if (is_sse42_available)
		draw_image_emoji_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_sse4_available)
		draw_image_emoji_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_sse3_available)
		draw_image_emoji_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#endif
	else if (is_sse2_available)
		draw_image_emoji_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#if !defined(_MSC_VER) && defined(ARCH_X86)
	else if (is_sse_available)
		draw_image_emoji_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#endif
	else
		draw_image_emoji_scalar(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
}

void draw_image_add(struct image *dst_image,
		    int dst_left,
		    int dst_top,
		    struct image *src_image,
		    int width,
		    int height,
		    int src_left,
		    int src_top,
		    int alpha)
{
	void draw_image_add_avx2(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_add_avx(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_add_sse42(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_add_sse4(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_add_sse3(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_add_sse2(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_add_sse(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_add_scalar(struct image *, int dst_left, int, struct image *, int, int, int, int, int);

	if (is_avx2_available)
		draw_image_add_avx2(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_avx_available)
		draw_image_add_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#if !defined(_MSC_VER)
	else if (is_sse42_available)
		draw_image_add_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_sse4_available)
		draw_image_add_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_sse3_available)
		draw_image_add_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#endif
	else if (is_sse2_available)
		draw_image_add_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#if !defined(_MSC_VER) && defined(ARCH_X86)
	else if (is_sse_available)
		draw_image_add_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#endif
	else
		draw_image_add_scalar(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
}

void draw_image_sub(struct image *dst_image,
		    int dst_left,
		    int dst_top,
		    struct image *src_image,
		    int width,
		    int height,
		    int src_left,
		    int src_top,
		    int alpha)
{
	void draw_image_sub_avx2(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_sub_avx(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_sub_sse42(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_sub_sse4(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_sub_sse3(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_sub_sse2(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_sub_sse(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_sub_scalar(struct image *, int dst_left, int, struct image *, int, int, int, int, int);

	if (is_avx2_available)
		draw_image_sub_avx2(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_avx_available)
		draw_image_sub_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#if !defined(_MSC_VER)
	else if (is_sse42_available)
		draw_image_sub_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_sse4_available)
		draw_image_sub_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_sse3_available)
		draw_image_sub_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#endif
	else if (is_sse2_available)
		draw_image_sub_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#if !defined(_MSC_VER) && defined(ARCH_X86)
	else if (is_sse_available)
		draw_image_sub_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#endif
	else
		draw_image_sub_scalar(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
}

void draw_image_dim(struct image *dst_image,
		    int dst_left,
		    int dst_top,
		    struct image *src_image,
		    int width,
		    int height,
		    int src_left,
		    int src_top,
		    int alpha)
{
	void draw_image_dim_avx2(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_dim_avx(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_dim_sse42(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_dim_sse4(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_dim_sse3(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_dim_sse2(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_dim_sse(struct image *, int dst_left, int, struct image *, int, int, int, int, int);
	void draw_image_dim_scalar(struct image *, int dst_left, int, struct image *, int, int, int, int, int);

	if (is_avx2_available)
		draw_image_dim_avx2(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_avx_available)
		draw_image_dim_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#if !defined(_MSC_VER)
	else if (is_sse42_available)
		draw_image_dim_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_sse4_available)
		draw_image_dim_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
	else if (is_sse3_available)
		draw_image_dim_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#endif
	else if (is_sse2_available)
		draw_image_dim_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#if !defined(_MSC_VER) && defined(ARCH_X86)
	else if (is_sse_available)
		draw_image_dim_avx(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
#endif
	else
		draw_image_dim_scalar(dst_image, dst_left, dst_top, src_image, width, height, src_left, src_top, alpha);
}

void draw_image_rule(struct image *dst_image,
		     struct image *src_image,
		     struct image *rule_image,
		     int threshold)
{
	void draw_image_rule_avx2(struct image *, struct image *, struct image *, int);
	void draw_image_rule_avx(struct image *, struct image *, struct image *, int);
	void draw_image_rule_sse42(struct image *, struct image *, struct image *, int);
	void draw_image_rule_sse4(struct image *, struct image *, struct image *, int);
	void draw_image_rule_sse3(struct image *, struct image *, struct image *, int);
	void draw_image_rule_sse2(struct image *, struct image *, struct image *, int);
	void draw_image_rule_sse(struct image *, struct image *, struct image *, int);
	void draw_image_rule_scalar(struct image *, struct image *, struct image *, int);

	if (is_avx2_available)
		draw_image_rule_avx2(dst_image, src_image, rule_image, threshold);
	else if (is_avx_available)
		draw_image_rule_avx(dst_image, src_image, rule_image, threshold);
#if !defined(_MSC_VER)
	else if (is_sse42_available)
		draw_image_rule_sse42(dst_image, src_image, rule_image, threshold);
	else if (is_sse4_available)
		draw_image_rule_sse4(dst_image, src_image, rule_image, threshold);
	else if (is_sse3_available)
		draw_image_rule_sse3(dst_image, src_image, rule_image, threshold);
#endif
	else if (is_sse2_available)
		draw_image_rule_sse2(dst_image, src_image, rule_image, threshold);
#if !defined(_MSC_VER) && defined(ARCH_X86)
	else if (is_sse_available)
		draw_image_rule_sse(dst_image, src_image, rule_image, threshold);
#endif
	else
		draw_image_rule_scalar(dst_image, src_image, rule_image, threshold);
}

void draw_image_melt(struct image *dst_image,
		     struct image *src_image,
		     struct image *rule_image,
		     int threshold)
{
	void draw_image_melt_avx2(struct image *, struct image *, struct image *, int);
	void draw_image_melt_avx(struct image *, struct image *, struct image *, int);
	void draw_image_melt_sse42(struct image *, struct image *, struct image *, int);
	void draw_image_melt_sse4(struct image *, struct image *, struct image *, int);
	void draw_image_melt_sse3(struct image *, struct image *, struct image *, int);
	void draw_image_melt_sse2(struct image *, struct image *, struct image *, int);
	void draw_image_melt_sse(struct image *, struct image *, struct image *, int);
	void draw_image_melt_scalar(struct image *, struct image *, struct image *, int);

	if (is_avx2_available)
		draw_image_melt_avx2(dst_image, src_image, rule_image, threshold);
	else if (is_avx_available)
		draw_image_melt_avx(dst_image, src_image, rule_image, threshold);
#if !defined(_MSC_VER)
	else if (is_sse42_available)
		draw_image_melt_sse42(dst_image, src_image, rule_image, threshold);
	else if (is_sse4_available)
		draw_image_melt_sse4(dst_image, src_image, rule_image, threshold);
	else if (is_sse3_available)
		draw_image_melt_sse3(dst_image, src_image, rule_image, threshold);
#endif
	else if (is_sse2_available)
		draw_image_melt_sse2(dst_image, src_image, rule_image, threshold);
#if !defined(_MSC_VER) && defined(ARCH_X86)
	else if (is_sse_available)
		draw_image_melt_sse(dst_image, src_image, rule_image, threshold);
#endif
	else
		draw_image_melt_scalar(dst_image, src_image, rule_image, threshold);
}

void draw_image_scale(struct image *dst_image,
		      int virtual_dst_width,
		      int virtual_dst_height,
		      int virtual_dst_left,
		      int virtual_dst_top,
		      struct image *src_image)
{
	void draw_image_scale_avx2(struct image *, int, int, int, int, struct image *);
	void draw_image_scale_avx(struct image *, int, int, int, int, struct image *);
	void draw_image_scale_sse42(struct image *, int, int, int, int, struct image *);
	void draw_image_scale_sse4(struct image *, int, int, int, int, struct image *);
	void draw_image_scale_sse3(struct image *, int, int, int, int, struct image *);
	void draw_image_scale_sse2(struct image *, int, int, int, int, struct image *);
	void draw_image_scale_sse(struct image *, int, int, int, int, struct image *);
	void draw_image_scale_scalar(struct image *, int, int, int, int, struct image *);

	if (is_avx2_available)
		draw_image_scale_avx2(dst_image, virtual_dst_width, virtual_dst_height, virtual_dst_left, virtual_dst_top, src_image);
	else if (is_avx_available)
		draw_image_scale_avx(dst_image, virtual_dst_width, virtual_dst_height, virtual_dst_left, virtual_dst_top, src_image);
#if !defined(_MSC_VER)
	else if (is_sse42_available)
		draw_image_scale_sse42(dst_image, virtual_dst_width, virtual_dst_height, virtual_dst_left, virtual_dst_top, src_image);
	else if (is_sse4_available)
		draw_image_scale_sse4(dst_image, virtual_dst_width, virtual_dst_height, virtual_dst_left, virtual_dst_top, src_image);
	else if (is_sse3_available)
		draw_image_scale_sse3(dst_image, virtual_dst_width, virtual_dst_height, virtual_dst_left, virtual_dst_top, src_image);
#endif
	else if (is_sse2_available)
		draw_image_scale_sse2(dst_image, virtual_dst_width, virtual_dst_height, virtual_dst_left, virtual_dst_top, src_image);
#if !defined(_MSC_VER) && defined(ARCH_X86)
	else if (is_sse_available)
		draw_image_scale_sse(dst_image, virtual_dst_width, virtual_dst_height, virtual_dst_left, virtual_dst_top, src_image);
#endif
	else
		draw_image_scale_scalar(dst_image, virtual_dst_width, virtual_dst_height, virtual_dst_left, virtual_dst_top, src_image);
		
}

#endif

/*
 * Clipping
 */

/* Check for draw_image_*() parameters. */
bool check_draw_image(struct image *dst_image,
		      int *dst_left,
		      int *dst_top,
		      struct image *src_image,
		      int *width,
		      int *height,
		      int *src_left,
		      int *src_top,
		      int alpha)
{
	/* Check the parameters. */
	assert(dst_image != NULL);
	assert(dst_image != src_image);
	assert(dst_image->width > 0 && dst_image->height > 0);
	assert(dst_image->pixels != NULL);
	assert(src_image != NULL);
	assert(src_image->width > 0 && src_image->height > 0);
	assert(src_image->pixels != NULL);

	/* Return false if no need for drawing. */
	if(alpha == 0 || *width == 0 || *height == 0)
		return false;
	if(!clip_by_source(src_image->width, src_image->height, width, height, dst_left, dst_top, src_left, src_top))
		return false;
	if(!clip_by_dest(dst_image->width, dst_image->height, width, height, dst_left, dst_top, src_left, src_top))
		return false;

	/* Need for draw. */
	return true;
}

/*
 * Clip by a source size.
 */
bool clip_by_source(
	int src_cx,	/* Source image width. */
	int src_cy,	/* Source image height. */
	int *cx,	/* [IN/OUT] Rect width. */
	int *cy,	/* [IN/OUT] Rect height. */
	int *dst_x,	/* [IN/OUT] Destination left. */
	int *dst_y,	/* [IN/OUT] Destination top. */
	int *src_x,	/* [IN/OUT] Source left. */
	int *src_y)	/* [IN/OUT] Source top. */
{
	/* Return false is the rect is completely out-of-range. */
	if(*src_x < 0 && -*src_x >= *cx)
		return false;	/* Left, out-of-range. */
	if(*src_y < 0 && -*src_y >= *cy)
		return false;	/* Top, out-of-range. */
	if(*src_x > 0 && *src_x >= src_cx)
		return false;	/* Right, out-of-range. */
	if(*src_y > 0 && *src_y >= src_cy)
		return false;	/* Bottom, out-of-range. */

	/* Cut a left out-of-range. */
	if(*src_x < 0) {
		*cx += *src_x;
		*dst_x -= *src_x;
		*src_x = 0;
	}

	/* Cut a top out-of-range. */
	if(*src_y < 0) {
		*cy += *src_y;
		*dst_y -= *src_y;
		*src_y = 0;
	}

	/* Cut a right out-of-range. */
	if(*src_x + *cx > src_cx)
		*cx = src_cx - *src_x;

	/* Cut a bottom out-of-range. */
	if(*src_y + *cy > src_cy)
		*cy = src_cy - *src_y;

	if (*cx <= 0 || *cy <= 0)
		return false;

	/* Succeeded. */
	return true;
}

/*
 * Clip by a destination size.
 */
bool clip_by_dest(
	int dst_cx,	/* Destination image width. */
	int dst_cy,	/* Destination image height. */
	int *cx,	/* [IN/OUT] Rect width. */
	int *cy,	/* [IN/OUT] Rect height. */
	int *dst_x,	/* [IN/OUT] Destination left. */
	int *dst_y,	/* [IN/OUT] Destination top. */
	int *src_x,	/* [IN/OUT] Source left. */
	int *src_y)	/* [IN/OUT] Source top. */
{
	/* Return false is the rect is completely out-of-range. */
	if(*dst_x < 0 && -*dst_x >= *cx)
		return false;	/* Left, out-of-range. */
	if(*dst_y < 0 && -*dst_y >= *cy)
		return false;	/* Top, out-of-range. */
	if(*dst_x > 0 && *dst_x >= dst_cx)
		return false;	/* Right, out-of-range. */
	if(*dst_y > 0 && *dst_y >= dst_cy)
		return false;	/* Bottom, out-of-range. */

	/* Cut a left out-of-range. */
	if(*dst_x < 0) {
		*cx += *dst_x;
		*src_x -= *dst_x;
		*dst_x = 0;
	}

	/* Cut a top out-of-range. */
	if(*dst_y < 0) {
		*cy += *dst_y;
		*src_y -= *dst_y;
		*dst_y = 0;
	}

	/* Cut a right out-of-range. */
	if(*dst_x + *cx > dst_cx)
		*cx = dst_cx - *dst_x;

	/* Cut a bottom out-of-range. */
	if(*dst_y + *cy > dst_cy)
		*cy = dst_cy - *dst_y;

	if (*cx <= 0 || *cy <= 0)
		return false;

	/* Succeeded. */
	return true;
}

/*
 * _aligned_malloc() is not supported on Windows 9x and 2000.
 * Just use malloc() instead on 9x and 2000.
 */
#if defined(TARGET_WINDOWS)

#include <windows.h>

static void *wrap_aligned_malloc(size_t size, size_t align)
{
	static void *(__cdecl *p_aligned_malloc)(size_t, size_t) = NULL;
	static bool first_time = true;

	if (first_time) {
		HMODULE hModule = LoadLibraryW(L"msvcrt.dll");
		if (hModule != NULL)
			p_aligned_malloc = (void *)GetProcAddress(hModule, "_aligned_malloc");
		first_time = false;
	}

	/* Windows XP or later. */
	if (p_aligned_malloc != NULL)
		return p_aligned_malloc(size, align);

	/* Windows 9x or 2000. */
	return malloc(size);
}

static void wrap_aligned_free(void *p)
{
	static void (*p_aligned_free)(void *) = NULL;
	static bool first_time = true;

	if (first_time) {
		HMODULE hModule = LoadLibraryW(L"msvcrt.dll");
		if (hModule != NULL)
			p_aligned_free = (void *)GetProcAddress(hModule, "_aligned_free");
		first_time = false;
	}

	/* Windows XP or later. */
	if (p_aligned_free != NULL) {
		p_aligned_free(p);
		return;
	}

	/* Windows 9x or 2000. */
	free(p);
}
#endif

/*
 * PNG
 */

#define PNG_DEBUG 3
#if defined(TARGET_WASM) || \
    defined(TARGET_ANDROID) || \
    (defined(TARGET_POSIX) && defined(USE_SHARED)) || \
    defined(USE_QT)
#include <png.h>
#else
#include <png/png.h>
#endif

struct png_reader {
	const uint8_t *data;
	size_t size;
	size_t pos;
};

static void png_read_callback(png_structp png_ptr, png_bytep buf, png_size_t len);

/*
 * Create an image with a PNG file.
 */
bool create_image_with_png(const uint8_t *data, size_t size, struct image **img)
{
	struct png_reader reader;
	png_structp png_ptr;
	png_byte color_type, bit_depth;
	png_infop info_ptr;
	png_bytep *rows;
	int width;
	int height;
	int y;
	pixel_t *pixels;

	/* Check a signature. */
	if (size < 8)
		return false;
	if (png_sig_cmp(data, 0, 8) != 0)
		return false;

	/* Create a png read struct. */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL)
		return false;

	/* Create a png info struct. */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return false;
	}

	/* Return here if failed. */
	rows = NULL;
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		if (*img != NULL) {
			destroy_image(*img);
			*img = NULL;
		}
		if (rows != NULL)
			free(rows);
		return false;
	}

	/* Read a header. */
	reader.data = data + 8;
	reader.size = size;
	reader.pos = 0;
	png_set_read_fn(png_ptr, &reader, png_read_callback);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);

	/* Get metrics. */
	width = (int)png_get_image_width(png_ptr, info_ptr);
	height = (int)png_get_image_height(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	/* Convert palette to RGBA. */
	switch(color_type) {
	case PNG_COLOR_TYPE_GRAY:
		png_set_gray_to_rgb(png_ptr);
		png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_AFTER);
		png_read_update_info(png_ptr, info_ptr);
		break;
	case PNG_COLOR_TYPE_PALETTE:
#if !defined(ORDER_OPENGL)
		png_set_bgr(png_ptr);
#endif
		png_set_palette_to_rgb(png_ptr);
		png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_AFTER);
		png_read_update_info(png_ptr, info_ptr);
		break;
	case PNG_COLOR_TYPE_RGB:
#if !defined(ORDER_OPENGL)
		png_set_bgr(png_ptr);
#endif
		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
			png_set_tRNS_to_alpha(png_ptr);
		} else {
			png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_AFTER);
			png_read_update_info(png_ptr, info_ptr);
		}
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
#if !defined(ORDER_OPENGL)
		png_set_bgr(png_ptr);
#endif
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		png_set_gray_to_rgb(png_ptr);
		png_set_add_alpha(png_ptr, 0xff, PNG_FILLER_AFTER);
		png_read_update_info(png_ptr, info_ptr);
		break;
	default:
		return false;
	}

	/* 16-bit to 8-bit. */
	if (bit_depth == 16)
		png_set_strip_16(png_ptr);

	/* Allocate an image. */
	if (!create_image(width, height, img))
		return false;

	/* Allocate a rows buffer. */
	rows = malloc(sizeof(png_bytep) * (size_t)height);
	if (rows == NULL) {
		destroy_image(*img);
		log_out_of_memory();
		return false;
	}
	assert(png_get_rowbytes(png_ptr, info_ptr) == (size_t)(width*4));

	/* Read an image. */
#ifdef _MSC_VER
#pragma warning(disable:6386)
#endif
	pixels = (*img)->pixels;
	for (y = 0; y < height; y++)
		rows[y] = (png_bytep)&pixels[width * y];
	png_read_image(png_ptr, rows);

	/* Cleanup. */
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	free(rows);

	return true;
}

static void png_read_callback(png_structp png_ptr, png_bytep buf, png_size_t len)
{
	struct png_reader *reader;

	reader = png_get_io_ptr(png_ptr);

	if (reader->pos + len > reader->size)
		len = reader->size - reader->pos;

	memcpy(buf, reader->data + reader->pos, len);

	reader->pos += len;
}

/*
 * JPEG
 */

#if defined(TARGET_WASM) || \
    (defined(TARGET_POSIX) && defined(USE_SHARED)) || \
    defined(USE_QT)
#include <jpeglib.h>
#else
#include <jpeg/jpeglib.h>
#endif

/*
 * Create an image with a JPEG file.
 */
bool create_image_with_jpeg(const uint8_t *data, size_t size, struct image **img)
{
	struct jpeg_decompress_struct jpeg;
	struct jpeg_error_mgr jerr;
	pixel_t *p;
	unsigned char *line;
	unsigned int width, height, x, y;
	int components;

	/* Start decoding. */
	jpeg_create_decompress(&jpeg);
	jpeg_mem_src(&jpeg, data, size);
	jpeg.err = jpeg_std_error(&jerr);
	jpeg_read_header(&jpeg, TRUE);
	jpeg_start_decompress(&jpeg);

	/* Get metrics. */
	width = jpeg.output_width;
	height = jpeg.output_height;
	components = jpeg.out_color_components;
	if (components != 3) {
		jpeg_destroy_decompress(&jpeg);
		return false;
	}

	/* Create an image. */
	if (!create_image((int)width, (int)height, img)) {
		jpeg_destroy_decompress(&jpeg);
		return false;
	}

	/* Allocate a line buffer. */
	line = malloc(width * height * 3);
	if (line == NULL) {
		log_out_of_memory();
		jpeg_destroy_decompress(&jpeg);
		destroy_image(*img);
		*img = NULL;
		return false;
	}

	/* Decode each line. */
	p = (*img)->pixels;
#if defined(TARGET_WINDOWS)
	for (y = 0; y < height; y++) {
		jpeg_read_scanlines(&jpeg, &line, 1);
		for (x = 0; x < width; x++) {
			*p++ = make_pixel(255, line[x * 3 + 2], line[x * 3 + 1], line[x * 3 + 0]);
		}
	}
#else
	for (y = 0; y < height; y++) {
		jpeg_read_scanlines(&jpeg, &line, 1);
		for (x = 0; x < width; x++) {
			*p++ = make_pixel(255, line[x * 3], line[x * 3 + 1], line[x * 3 + 2]);
		}
	}
#endif

	/* Cleanup. */
	free(line);
	jpeg_destroy_decompress(&jpeg);

	return true;
}

/*
 * WebP
 */

#include <webp/decode.h>

/*
 * Create an image with a WebP file.
 */
bool create_image_with_webp(const uint8_t *data, size_t size, struct image **img)
{
	pixel_t *p;
	uint8_t *pixels;
	int width, height;
	int x, y;

	/* Get metrics. */
	if (!WebPGetInfo(data, size, &width, &height))
		return false;

	/* Create an image. */
	if (!create_image(width, height, img))
		return false;

	/* Do decoding. */
	pixels = WebPDecodeRGBA(data, size, &width, &height);
	if (pixels == NULL) {
		destroy_image(*img);
		*img = NULL;
		return false;
	}

	/* Copy pixels. */
	p = (*img)->pixels;
#if defined(ORDER_RGBA)
	/* Use RGBA as is. */
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			*p++ = make_pixel(pixels[y * width * 4 + x * 4 + 3],
					  pixels[y * width * 4 + x * 4 + 0],
					  pixels[y * width * 4 + x * 4 + 1],
					  pixels[y * width * 4 + x * 4 + 2]);
		}
	}
#else
	/* Reverse the pixel order for OpenGL. (RGB -> BGR) */
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			*p++ = make_pixel(pixels[y * width * 4 + x * 4 + 3],
					  pixels[y * width * 4 + x * 4 + 2],
					  pixels[y * width * 4 + x * 4 + 1],
					  pixels[y * width * 4 + x * 4 + 0]);
		}
	}
#endif

	/* Cleanup. */
	WebPFree(pixels);

	return true;
}
