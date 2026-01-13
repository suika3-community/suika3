/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Playfield Engine
 * Copyright (c) 2025, 2026, Awe Morris. All rights reserved.
 */

/*
 * Playfield Engine C API
 */

#include <playfield/playfield.h>

#include "engine.h"
#include "common.h"
#include "vm.h"
#include "tag.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#define TEXTURE_COUNT	(256)

/* Texture struct. */
struct texture_entry {
	bool is_used;
	struct image *img;
};

/* Texture table. */
static struct texture_entry tex_tbl[TEXTURE_COUNT];

/* Wave table. */
static struct wave *wave_tbl[SOUND_TRACKS];

/* Forward Declaration */
static int search_free_entry(void);
static bool create_texture(int width, int height, int *ret, struct image **img);
static char *make_save_file_name(const char *key);
static char get_hex_char(int val);

/*
 * Initialization
 */

/*
 * Initialize the API.
 */
bool
init_api(void)
{
	return true;
}

/*
 * Cleanup the API.
 */
void
cleanup_api(void)
{
	int i;

	for (i = 0; i < TEXTURE_COUNT; i++) {
		if (tex_tbl[i].is_used) {
			tex_tbl[i].is_used = false;
			destroy_image(tex_tbl[i].img);
		}
	}
	
}

/*
 * Texture
 */

/*
 * Load a texture.
 */
bool
pf_load_texture(
	const char *fname,
	int *ret,
	int *width,
	int *height)
{
	int index;
	const char *ext;
	char *data;
	size_t size;

	/* Allocate a texture entry. */
	index = search_free_entry();
	if (index == -1) {
		log_error("Too many textures.");
		return false;
	}

	/* Get a file extension. */
	ext = strrchr(fname, '.');
	if (ext == NULL) {
		log_error(PPS_TR("Cannot determine the file type for \"%s\"."), fname);
		return false;
	}

	/* Load a file content. */
	if (!load_file(fname, &data, &size))
		return false;

	/* Load an image. */
	if (strcmp(ext, ".jpg") == 0 ||
	    strcmp(ext, ".JPG") == 0 ||
	    strcmp(ext, ".jpeg") == 0 ||
	    strcmp(ext, ".JPEG") == 0) {
		if (!create_image_with_webp((const uint8_t *)data, size, &tex_tbl[index].img)) {
			log_error(PPS_TR("Cannot load an image \"%s\"."), fname);
			return false;
		}
	} else if (strcmp(ext, ".webp") == 0 ||
		   strcmp(ext, ".WebP") == 0 ||
		   strcmp(ext, ".WEBP") == 0) {
		if (!create_image_with_webp((const uint8_t *)data, size, &tex_tbl[index].img)) {
			log_error(PPS_TR("Cannot load an image \"%s\"."), fname);
			return false;
		}
	} else {
		if (!create_image_with_png((const uint8_t *)data, size, &tex_tbl[index].img)) {
			log_error(PPS_TR("Cannot load an image \"%s\"."), fname);
			return false;
		}
	}
	free(data);

	/* Fill alpha channel. */
	notify_image_update(tex_tbl[index].img);

	/* Mark as used. */
	tex_tbl[index].is_used = true;

	/* Succeeded. */
	*ret = index;
	*width = tex_tbl[index].img->width;
	*height = tex_tbl[index].img->height;
	return true;
}

/*
 * Create a color texture.
 */
bool
pf_create_color_texture(
	int width,
	int height,
	int r,
	int g,
	int b,
	int a,
	int *ret)
{
	int index;

	/* Allocate a texture entry. */
	index = search_free_entry();
	if (index == -1) {
		log_error("Too many textures.");
		return false;
	}

	/* Create an image. */
	if (!create_image(width, height, &tex_tbl[index].img))
		return false;

	/* Mark as used. */
	tex_tbl[index].is_used = true;
	tex_tbl[index].img = tex_tbl[index].img;

	/* Clear the image. */
	clear_image(tex_tbl[index].img, make_pixel(a, r, g, b));
	notify_image_update(tex_tbl[index].img);

	/* Succeeded. */
	*ret = index;

	return true;
}

/* Create a texture. (for font drawing) */
static bool
create_texture(
	int width,
	int height,
	int *ret,
	struct image **img)
{
	int index;

	/* Allocate a texture entry. */
	index = search_free_entry();
	if (index == -1) {
		log_error(PPS_TR("Too many textures."));
		return false;
	}

	/* Create an image. */
	if (!create_image(width, height, img))
		return false;
	
	memset((*img)->pixels, 0, width * height * 4);

	/* Mark as used. */
	tex_tbl[index].is_used = true;
	tex_tbl[index].img = *img;

	/* Succeeded. */
	*ret = index;

	return true;
}

/* Search a free texture index. */
static int
search_free_entry(void)
{
	int i;

	for (i = 0; i < TEXTURE_COUNT; i++) {
		if (!tex_tbl[i].is_used)
			return i;
	}
	return -1;
}

/*
 * Destroy a texture.
 */
void
pf_destroy_texture(
	int tex_id)
{
	assert(tex_id >= 0);
	assert(tex_id < TEXTURE_COUNT);
	assert(tex_tbl[tex_id].is_used);
	assert(tex_tbl[tex_id].img != NULL);

	/* Mark as used. */
	tex_tbl[tex_id].is_used = false;
	destroy_image(tex_tbl[tex_id].img);
}

/*
 * Notify a texture update.
 */
bool
pf_notify_texture_update(
	int tex_id)
{
	assert(tex_id >= 0);
	assert(tex_id < TEXTURE_COUNT);
	assert(tex_tbl[tex_id].is_used);
	assert(tex_tbl[tex_id].img != NULL);

	notify_image_update(tex_tbl[tex_id].img);
}

/*
 * Draw a texture image on a texture image. (copy)
 */
void
pf_draw_texture_copy(
	int dst_tex_id,
	int dst_left,
	int dst_top,
	int src_tex_id,
	int width,
	int height,
	int src_left,
	int src_top)
{
	assert(dst_tex_id >= 0);
	assert(dst_tex_id < TEXTURE_COUNT);
	assert(tex_tbl[dst_tex_id].is_used);
	assert(tex_tbl[dst_tex_id].img != NULL);

	assert(src_tex_id >= 0);
	assert(src_tex_id < TEXTURE_COUNT);
	assert(tex_tbl[src_tex_id].is_used);
	assert(tex_tbl[src_tex_id].img != NULL);

	draw_image_copy(
		tex_tbl[dst_tex_id].img,
		dst_left,
		dst_top,
		tex_tbl[src_tex_id].img,
		width,
		height,
		src_left,
		src_top);
}

/*
 * Draw a texture image on a texture image. (alpha-blending, dst_alpha=255)
 */
void
pf_draw_texture_alpha(
	int dst_tex_id,
	int dst_left,
	int dst_top,
	int src_tex_id,
	int width,
	int height,
	int src_left,
	int src_top,
	int alpha)
{
	assert(dst_tex_id >= 0);
	assert(dst_tex_id < TEXTURE_COUNT);
	assert(tex_tbl[dst_tex_id].is_used);
	assert(tex_tbl[dst_tex_id].img != NULL);

	assert(src_tex_id >= 0);
	assert(src_tex_id < TEXTURE_COUNT);
	assert(tex_tbl[src_tex_id].is_used);
	assert(tex_tbl[src_tex_id].img != NULL);

	draw_image_alpha(
		tex_tbl[dst_tex_id].img,
		dst_left,
		dst_top,
		tex_tbl[src_tex_id].img,
		width,
		height,
		src_left,
		src_top,
		alpha);
}

/*
 * Draw a texture image on a texture image. (add-blending)
 */
void
pf_draw_texture_add(
	int dst_tex_id,
	int dst_left,
	int dst_top,
	int src_tex_id,
	int width,
	int height,
	int src_left,
	int src_top,
	int alpha)
{
	assert(dst_tex_id >= 0);
	assert(dst_tex_id < TEXTURE_COUNT);
	assert(tex_tbl[dst_tex_id].is_used);
	assert(tex_tbl[dst_tex_id].img != NULL);

	assert(src_tex_id >= 0);
	assert(src_tex_id < TEXTURE_COUNT);
	assert(tex_tbl[src_tex_id].is_used);
	assert(tex_tbl[src_tex_id].img != NULL);

	draw_image_add(
		tex_tbl[dst_tex_id].img,
		dst_left,
		dst_top,
		tex_tbl[src_tex_id].img,
		width,
		height,
		src_left,
		src_top,
		alpha);
}

/*
 * Draw a texture image on a texture image. (add-blending)
 */
void
pf_draw_texture_sub(
	int dst_tex_id,
	int dst_left,
	int dst_top,
	int src_tex_id,
	int width,
	int height,
	int src_left,
	int src_top,
	int alpha)
{
	assert(dst_tex_id >= 0);
	assert(dst_tex_id < TEXTURE_COUNT);
	assert(tex_tbl[dst_tex_id].is_used);
	assert(tex_tbl[dst_tex_id].img != NULL);

	assert(src_tex_id >= 0);
	assert(src_tex_id < TEXTURE_COUNT);
	assert(tex_tbl[src_tex_id].is_used);
	assert(tex_tbl[src_tex_id].img != NULL);

	draw_image_sub(
		tex_tbl[dst_tex_id].img,
		dst_left,
		dst_top,
		tex_tbl[src_tex_id].img,
		width,
		height,
		src_left,
		src_top,
		alpha);
}

/*
 * Draw a glyph texture image on a texture image.
 * (alphablending, special alpha value)
 */
void
pf_draw_texture_glyph(
	int dst_tex_id,
	int dst_left,
	int dst_top,
	int src_tex_id,
	int width,
	int height,
	int src_left,
	int src_top,
	int alpha)
{
	assert(dst_tex_id >= 0);
	assert(dst_tex_id < TEXTURE_COUNT);
	assert(tex_tbl[dst_tex_id].is_used);
	assert(tex_tbl[dst_tex_id].img != NULL);

	assert(src_tex_id >= 0);
	assert(src_tex_id < TEXTURE_COUNT);
	assert(tex_tbl[src_tex_id].is_used);
	assert(tex_tbl[src_tex_id].img != NULL);

	draw_image_glyph(
		tex_tbl[dst_tex_id].img,
		dst_left,
		dst_top,
		tex_tbl[src_tex_id].img,
		width,
		height,
		src_left,
		src_top,
		alpha);
}

/*
 * Draw an emoji texture image on a texture image.
 * (alphablending, special alpha value)
 */
void
pf_draw_texture_emoji(
	int dst_tex_id,
	int dst_left,
	int dst_top,
	int src_tex_id,
	int width,
	int height,
	int src_left,
	int src_top,
	int alpha)
{
	assert(dst_tex_id >= 0);
	assert(dst_tex_id < TEXTURE_COUNT);
	assert(tex_tbl[dst_tex_id].is_used);
	assert(tex_tbl[dst_tex_id].img != NULL);

	assert(src_tex_id >= 0);
	assert(src_tex_id < TEXTURE_COUNT);
	assert(tex_tbl[src_tex_id].is_used);
	assert(tex_tbl[src_tex_id].img != NULL);

	draw_image_emoji(
		tex_tbl[dst_tex_id].img,
		dst_left,
		dst_top,
		tex_tbl[src_tex_id].img,
		width,
		height,
		src_left,
		src_top,
		alpha);
}

/*
 * Draw a texture image on a texture image. (50% dimming)
 */
void
pf_draw_texture_dim(
	int dst_tex_id,
	int dst_left,
	int dst_top,
	int src_tex_id,
	int width,
	int height,
	int src_left,
	int src_top,
	int alpha)
{
	assert(dst_tex_id >= 0);
	assert(dst_tex_id < TEXTURE_COUNT);
	assert(tex_tbl[dst_tex_id].is_used);
	assert(tex_tbl[dst_tex_id].img != NULL);

	assert(src_tex_id >= 0);
	assert(src_tex_id < TEXTURE_COUNT);
	assert(tex_tbl[src_tex_id].is_used);
	assert(tex_tbl[src_tex_id].img != NULL);

	draw_image_dim(
		tex_tbl[dst_tex_id].img,
		dst_left,
		dst_top,
		tex_tbl[src_tex_id].img,
		width,
		height,
		src_left,
		src_top,
		alpha);
}

/* Draw a texture image on a texture image with scaling. */
void
pf_draw_texture_scale(
	int dst_tex_id,
	int virtual_dst_width,
	int virtual_dst_height,
	int virtual_dst_left,
	int virtual_dst_top,
	int src_tex_id)
{
	assert(dst_tex_id >= 0);
	assert(dst_tex_id < TEXTURE_COUNT);
	assert(tex_tbl[dst_tex_id].is_used);
	assert(tex_tbl[dst_tex_id].img != NULL);

	assert(src_tex_id >= 0);
	assert(src_tex_id < TEXTURE_COUNT);
	assert(tex_tbl[src_tex_id].is_used);
	assert(tex_tbl[src_tex_id].img != NULL);

	draw_image_scale(
		tex_tbl[dst_tex_id].img,
		virtual_dst_width,
		virtual_dst_height,
		virtual_dst_left,
		virtual_dst_top,
		tex_tbl[src_tex_id].img);
}

/*
 * Fill a rectangle on a texture image.
 */
void
pf_fill_texture_rect(
	int tex_id,
	int left,
	int top,
	int width,
	int height,
	pixel_t color)
{
	assert(tex_id >= 0);
	assert(tex_id < TEXTURE_COUNT);
	assert(tex_tbl[tex_id].is_used);
	assert(tex_tbl[tex_id].img != NULL);

	clear_image_rect(
		tex_tbl[tex_id].img,
		left,
		top,
		width,
		height,
		color);
}

/*
 * Get a raw pixel pointer.
 */
pixel_t *
pf_get_texture_pixels(
	int tex_id)
{
	assert(tex_id >= 0);
	assert(tex_id < TEXTURE_COUNT);
	assert(tex_tbl[tex_id].is_used);
	assert(tex_tbl[tex_id].img != NULL);

	return tex_tbl[tex_id].img->pixels;
}

/*
 * Rendering
 */

/*
 * Render a texture. (alpha blending)
 */
void
pf_render_texture(
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	int tex_id,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha)
{
	struct texture_entry *t;

	assert(tex_id >= 0 &&  tex_id < TEXTURE_COUNT);

	t = &tex_tbl[tex_id];
	assert(t->is_used);
	assert(t->img != NULL);

	render_image_normal(dst_left,
			    dst_top,
			    dst_width,
			    dst_height,
			    t->img,
			    src_left,
			    src_top,
			    src_width,
			    src_height,
			    alpha);
}

/*
 * Render a texture. (add blending)
 */
void
pf_render_texture_add(
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	int tex_id,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha)
{
	struct texture_entry *t;

	assert(tex_id >= 0 &&  tex_id < TEXTURE_COUNT);

	t = &tex_tbl[tex_id];
	assert(t->is_used);
	assert(t->img != NULL);

	render_image_add(dst_left,
			 dst_top,
			 dst_width,
			 dst_height,
			 t->img,
			 src_left,
			 src_top,
			 src_width,
			 src_height,
			 alpha);
}

/*
 * Render a texture. (sub blending)
 */
void
pf_render_texture_sub(
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	int tex_id,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha)
{
	struct texture_entry *t;

	assert(tex_id >= 0 &&  tex_id < TEXTURE_COUNT);

	t = &tex_tbl[tex_id];
	assert(t->is_used);
	assert(t->img != NULL);

	render_image_sub(dst_left,
			 dst_top,
			 dst_width,
			 dst_height,
			 t->img,
			 src_left,
			 src_top,
			 src_width,
			 src_height,
			 alpha);
}

/*
 * Render a texture. (dim blending)
 */
void
pf_render_texture_dim(
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	int tex_id,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha)
{
	struct texture_entry *t;

	assert(tex_id >= 0 &&  tex_id < TEXTURE_COUNT);

	t = &tex_tbl[tex_id];
	assert(t->is_used);
	assert(t->img != NULL);

	render_image_dim(dst_left,
			 dst_top,
			 dst_width,
			 dst_height,
			 t->img,
			 src_left,
			 src_top,
			 src_width,
			 src_height,
			 alpha);
}

/*
 * Render a texture. (3D, alpha blending)
 */
void
pf_render_texture_3d(
	float x1,
	float y1,
	float x2,
	float y2,
	float x3,
	float y3,
	float x4,
	float y4,
	int tex_id,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha)
{
	struct texture_entry *t;

	assert(tex_id >= 0 &&  tex_id < TEXTURE_COUNT);

	t = &tex_tbl[tex_id];
	assert(t->is_used);
	assert(t->img != NULL);

	render_image_3d_normal(x1, y1, x2, y2, x3, y3, x4, y4,
			       t->img,
			       src_left, src_top, src_width, src_height,
			       alpha);
}

/*
 * Render a texture. (3D, add blending)
 */
void
pf_render_texture_3d_add(
	float x1,
	float y1,
	float x2,
	float y2,
	float x3,
	float y3,
	float x4,
	float y4,
	int tex_id,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha)
{
	struct texture_entry *t;

	assert(tex_id >= 0 &&  tex_id < TEXTURE_COUNT);

	t = &tex_tbl[tex_id];
	assert(t->is_used);
	assert(t->img != NULL);

	render_image_3d_add(x1, y1, x2, y2, x3, y3, x4, y4,
			    t->img,
			    src_left, src_top, src_width, src_height,
			    alpha);
}

/*
 * Render a texture. (3D, sub blending)
 */
void
pf_render_texture_3d_sub(
	float x1,
	float y1,
	float x2,
	float y2,
	float x3,
	float y3,
	float x4,
	float y4,
	int tex_id,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha)
{
	struct texture_entry *t;

	assert(tex_id >= 0 &&  tex_id < TEXTURE_COUNT);

	t = &tex_tbl[tex_id];
	assert(t->is_used);
	assert(t->img != NULL);

	render_image_3d_sub(x1, y1, x2, y2, x3, y3, x4, y4,
			    t->img,
			    src_left, src_top, src_width, src_height,
			    alpha);
}

/*
 * Render a texture. (3D, dim blending)
 */
void
pf_render_texture_3d_dim(
	float x1,
	float y1,
	float x2,
	float y2,
	float x3,
	float y3,
	float x4,
	float y4,
	int tex_id,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha)
{
	struct texture_entry *t;

	assert(tex_id >= 0 &&  tex_id < TEXTURE_COUNT);

	t = &tex_tbl[tex_id];
	assert(t->is_used);
	assert(t->img != NULL);

	render_image_3d_dim(x1, y1, x2, y2, x3, y3, x4, y4,
			    t->img,
			    src_left, src_top, src_width, src_height,
			    alpha);
}

/*
 * Render a texture.
 */
void
pf_draw(
	int tex_id,
	int x,
	int y)
{
	struct texture_entry *t;

	assert(tex_id >= 0 &&  tex_id < TEXTURE_COUNT);

	t = &tex_tbl[tex_id];
	assert(t->is_used);
	assert(t->img != NULL);

	render_image_normal(x,
			    y,
			    t->img->width,
			    t->img->height,
			    t->img,
			    0,
			    0,
			    t->img->width,
			    t->img->height,
			    255);
}

/*
 * Font
 */

/*
 * Load a font file to a font slot.
 */
bool
pf_load_font(
	int slot,
	const char *file)
{
	uint8_t *data;
	size_t len;

	/* Check the font slot index. */
	if (slot < 0 || slot >= GLYPH_DATA_COUNT) {
		log_error(PPS_TR("Invalid font slot index."));
		return false;
	}

	/* Load the file content. */
	if (!load_file(file, (char **)&data, &len))
		return false;

	/* Load the glyph date. */
	if (!load_glyph_data(slot, data, len)) {
		free(data);
		return false;
	}
	free(data);

	return true;
}

/*
 * Convert a color code (#rrggbb) to a pixel value.
 */
pixel_t color_code_to_pixel_value(const char *code)
{
	uint32_t r, g, b;
	pixel_t cl;
	int rgb;

	if (code[0] != '#')
		return 0;

	/* Read a color string. */
	rgb = 0;
	(void)sscanf(code, "%x", &rgb);
	r = (rgb >> 16) & 0xff;
	g = (rgb >> 8) & 0xff;
	b = rgb & 0xff;
	cl = make_pixel(0xff, r, g, b);

	return cl;
}

/*
 * Create a text texture.
 */
bool
pf_create_text_texture(
	int slot,
	const char *text,
	int size,
	pixel_t color,
	int *tex_id,
	int *width,
	int *height)
{
	if (!pf_create_text_texture_outline(slot,
					    text,
					    size,
					    color,
					    0,
					    0,
					    tex_id,
					    width,
					    height))
		return false;

	return true;
}

/*
 * Create a text texture with outlining.
 */
bool
pf_create_text_texture_outline(
	int slot,
	const char *text,
	int size,
	pixel_t color,
	int outline_width,
	pixel_t outline_color,
	int *tex_id,
	int *width,
	int *height)
{
	struct image *img;
	int w, h;
	int x, y;
	int tid;

	/* Get the rendered width and height of the text. */
	get_string_width_and_height(slot, size, text, &w, &h);
	if (w == 0)
		w = 1;
	if (h == 0)
		h = 1;

	/* Create a texture. */
	if (!create_texture(w, h, &tid, &img))
		return false;

	/* Draw for each character. */
	x = 0;
	y = 0;
	while (*text != '\0') {
		uint32_t codepoint;
		int mblen;
		int cw, ch;

		/* Get a character. */
		mblen = utf8_to_utf32(text, &codepoint);
		if (mblen == -1)
			return -1;

		/* Get a character width. */
		draw_glyph(img,
			   slot,
			   size, 		/* font size */
			   size, 		/* base size */
			   outline_width,
			   x,
			   y,
			   color,
			   outline_color,
			   codepoint,
			   &cw,
			   &ch,
			   false);
		x += cw;

		/* Move to a next character. */
		text += mblen;
	}

	*tex_id = tid;
	*width = w;
	*height = h;

	return true;
}

/*
 * Sound
 */

/*
 * Play a sound file on a stream.
 */
bool
pf_play_sound(
	int stream,
	const char *file)
{
	if (stream < 0 || stream >= SOUND_TRACKS) {
		log_error(PPS_TR("Invalid sound stream index."));
		return false;
	}

	wave_tbl[stream] = create_wave_from_file(file, false);
	if (wave_tbl[stream] == NULL)
		return false;

	if (!play_sound(stream, wave_tbl[stream]))
		return false;

	return true;
}

/*
 * Stop the sound on a stream.
 */
bool
pf_stop_sound(
	int stream)
{
	if (stream < 0 || stream >= SOUND_TRACKS) {
		log_error(PPS_TR("Invalid sound stream index."));
		return false;
	}

	stop_sound(stream);

	destroy_wave(wave_tbl[stream]);
	wave_tbl[stream] = NULL;

	return true;
}

/*
 * Set the sound volume on a stream.
 */
bool
pf_set_sound_volume(
	int stream,
	float vol)
{
	if (stream < 0 || stream >= SOUND_TRACKS) {
		log_error(PPS_TR("Invalid sound stream index."));
		return false;
	}

	set_sound_volume(stream, vol);

	return true;
}

/*
 * Check if a sound stream is completed.
 */
bool
pf_is_sound_finished(
	int stream)
{
	if (!is_sound_finished(stream))
		return false;

	return true;			
}

/*
 * Lap Timer
 */

/*
 * Reset a lap timer and initializes it with a current time.
 */
void
pf_reset_lap_timer(
	uint64_t *origin)
{
	reset_lap_timer(origin);
}

/*
 * Get a lap time in milliseconds.
 */
uint64_t
pf_get_lap_timer_millisec(
	uint64_t *origin)
{
	return get_lap_timer_millisec(origin);
}

/*
 * Save Data
 */

/*
 * Write save data.
 */
bool
pf_write_save_data(
	const char *key,
	const void *data,
	size_t size)
{
	char *fname;
	struct wfile *wf;
	size_t ret;

	/* Make a save file name. */
	fname = make_save_file_name(key);
	if (fname == NULL) {
		log_error(PPS_TR("Save data key too long."));
		return false;
	}

	/* Make the save directory. */
	if (!make_save_directory()) {
		log_error(PPS_TR("Cannot make the save directory."));
		free(fname);
		return false;
	}

	/* Open a save file. */
	if (!open_wfile(fname, &wf)) {
		log_error(PPS_TR("Cannot open a save file."));
		free(fname);
		return false;
	}
	free(fname);

	/* Write data to the save file. */
	if (!write_wfile(wf, data, size, &ret)) {
		log_error(PPS_TR("Cannot write to a save file."));
		return false;
	}

	/* Close the save file. */
	close_wfile(wf);

	return true;
}

/*
 * Read save data.
 */
bool
pf_read_save_data(
	const char *key,
	void *data,
	size_t size,
	size_t *ret)
{
	char *fname;
	struct rfile *rf;

	/* Make a save file name. */
	fname = make_save_file_name(key);
	if (fname == NULL) {
		log_error(PPS_TR("Save data key too long."));
		return false;
	}

	/* Open a save file. */
	if (!open_rfile(fname, &rf)) {
		log_error(PPS_TR("Cannot open a save file."));
		free(fname);
		return false;
	}
	free(fname);

	/* Enable de-obfuscation. */
	decode_rfile(rf);

	/* Get a file size. */
	if (!get_rfile_size(rf, ret)) {
		log_error(PPS_TR("Cannot get the size of a save file."));
		return false;
	}
	if (size < *ret) {
		log_error(PPS_TR("Save file too large."));
		return false;
	}

	/* Read data to the save file. */
	if (!read_rfile(rf, data, *ret, ret)) {
		log_error(PPS_TR("Cannot read a save file."));
		return false;
	}

	/* Close the save file. */
	close_rfile(rf);

	return true;
}

/*
 * Check whether save data exist or not.
 */
bool
pf_check_save_data(
	const char *key)
{
	char *fname;
	bool ret;

	/* Make a save file name. */
	fname = make_save_file_name(key);
	if (fname == NULL) {
		log_error(PPS_TR("Save data key too long."));
		return false;
	}

	ret = check_file_exist(fname);
	free(fname);

	return ret;
}

/* Make a save file name correspond to a key string. */
static char *
make_save_file_name(
	const char *key)
{
	char buf[1024];
	int i, len, pos;

	strcpy(buf, SAVE_DIR "/");

	len = strlen(key);
	if (len * 2 > sizeof(buf) - strlen(buf) - 1) {
		/* File name too long. */
		return NULL;
	}

	pos = strlen(buf);
	for (i = 0; i < len; i++) {
		buf[pos + 0] = get_hex_char(key[i] >> 4);
		buf[pos + 1] = get_hex_char(key[i] & 4);
		pos += 2;
	}
	buf[pos] = '\0';

	return strdup(buf);
}

/* Get a hex character. */
static char get_hex_char(int val)
{
	switch (val) {
	case 0:
		return '0';
	case 1:
		return '1';
	case 2:
		return '2';
	case 3:
		return '3';
	case 4:
		return '4';
	case 5:
		return '5';
	case 6:
		return '6';
	case 7:
		return '7';
	case 8:
		return '8';
	case 9:
		return '9';
	case 10:
		return 'a';
	case 11:
		return 'b';
	case 12:
		return 'c';
	case 13:
		return 'd';
	case 14:
		return 'e';
	case 15:
		return 'f';
	default:
		break;
	}
	assert(0);
	return -1;
}

/*
 * Raw File Read
 */

/*
 * Check if a file exists.
 */
bool
pf_check_file_exists(
	const char *fname)
{
	if (!check_file_exist(fname))
		return false;

	return true;
}

/*
 * Read a file.
 */
bool
pf_read_file_content(
	const char *fname,
	char **buf,
	size_t *len)
{
	if (!load_file(fname, buf, len))
		return false;

	return true;
}

/*
 * VM
 */

/*
 * Get the VM environment.
 */
void *
pf_get_vm_env(void)
{
	return get_vm_env();
}

/*
 * Call a VM function.
 */
bool
pf_call_vm_function(
	const char *func_name)
{
	return call_vm_function(func_name);
}

/*
 * Call a VM function that corresponds to the current tag.
 */
bool
pf_call_vm_tag_function(
	bool *tag_end)
{
	return call_vm_tag_function(tag_end);
}

/*
 * Set a VM integer.
 */
bool
pf_set_vm_int(
	const char *prop_name,
	int val)
{
	return set_vm_int(prop_name, val);
}

/*
 * Get a VM integer.
 */
bool
pf_get_vm_int(
	const char *prop_name,
	int *val)
{
	return get_vm_int(prop_name, val);
}

/*
 * Scripting Interface
 */

/*
 * Install an API function.
 */
bool
pf_install_api(
	const char *name,
	bool (*func)(void *))
{
	NoctEnv *env;
	const char *params[] = {"param"};

	env = get_vm_env();

	if (!noct_register_cfunc(env, name, 1, params, (bool (*)(NoctEnv *))func, NULL))
		return false;

	return true;
}

/*
 * Get a function call parameter as an integer.
 */
bool
pf_get_call_arg_int(
	const char *name,
	int *val)
{
	NoctEnv *env;
	NoctValue param, value;

	env = get_vm_env();

	/* Get the "param" argument. */
	if (!noct_get_arg_check_array(env, 0, &param))
		return false;

	/* Get the element by name. */
	if (!noct_get_dict_elem_check_int(env, &param, name, &value, val))
		return false;

	return true;
}

/*
 * Get a function call parameter as a float.
 */
bool
pf_get_call_arg_float(
	const char *name,
	float *val)
{
	NoctEnv *env;
	NoctValue param, value;

	env = get_vm_env();

	/* Get the "param" argument. */
	if (!noct_get_arg_check_array(env, 0, &param))
		return false;

	/* Get the element by name. */
	if (!noct_get_dict_elem_check_float(env, &param, name, &value, val))
		return false;

	return true;
}

/*
 * Get a function call parameter as a string.
 */
bool
pf_get_call_arg_string(
	const char *name,
	char **val)
{
	NoctEnv *env;
	NoctValue param, value;
	const char *s;

	env = get_vm_env();

	/* Get the "param" argument. */
	if (!noct_get_arg_check_array(env, 0, &param))
		return false;

	/* Get the element by name. */
	if (!noct_get_dict_elem_check_string(env, &param, name, &value, &s))
		return false;

	/* Duplicate the string. */
	*val = strdup(s);
	if (*val == NULL) {
		log_out_of_memory();
		return false;
	}

	return true;
}

/*
 * Get the length of an array of a function call parameter.
 */
bool
pf_get_call_arg_array_length(
	const char *name,
	int *val)
{
	NoctEnv *env;
	NoctValue param, value;

	env = get_vm_env();

	/* Get the "param" argument. */
	if (!noct_get_arg_check_array(env, 0, &param))
		return false;

	/* Get the element by name. */
	if (!noct_get_dict_elem_check_array(env, &param, name, &value))
		return false;

	/* Get the array size. */
	if (!noct_get_array_size(env, &value, val))
		return false;

	return true;
}

/*
 * Get an integer element of an array of a function call parameter.
 */
bool
pf_get_call_arg_array_int(
	const char *name,
	int index,
	int *val)
{
	NoctEnv *env;
	NoctValue param, array, value;

	env = get_vm_env();

	/* Get the "param" argument. */
	if (!noct_get_arg_check_array(env, 0, &param))
		return false;

	/* Get the element by name. */
	if (!noct_get_dict_elem_check_array(env, &param, name, &array))
		return false;

	/* Get the array element. */
	if (!noct_get_array_elem_check_int(env, &array, index, &value, val))
		return false;

	return true;
}

/*
 * Get a float element of an array of a function call parameter.
 */
bool
pf_get_call_arg_array_float(
	const char *name,
	int index,
	float *val)
{
	NoctEnv *env;
	NoctValue param, array, value;

	env = get_vm_env();

	/* Get the "param" argument. */
	if (!noct_get_arg_check_array(env, 0, &param))
		return false;

	/* Get the element by name. */
	if (!noct_get_dict_elem_check_array(env, &param, name, &array))
		return false;

	/* Get the array element. */
	if (!noct_get_array_elem_check_float(env, &array, index, &value, val))
		return false;

	return true;
}

/*
 * Get a string element of an array of a function call parameter.
 */
bool
pf_get_call_arg_array_string(
	const char *name,
	int index,
	char **val)
{
	NoctEnv *env;
	NoctValue param, array, value;
	const char *s;

	env = get_vm_env();

	/* Get the "param" argument. */
	if (!noct_get_arg_check_array(env, 0, &param))
		return false;

	/* Get the element by name. */
	if (!noct_get_dict_elem_check_array(env, &param, name, &array))
		return false;

	/* Get the array element. */
	if (!noct_get_array_elem_check_string(env, &array, index, &value, &s))
		return false;

	/* Duplicate the string. */
	*val = strdup(s);
	if (*val == NULL) {
		log_out_of_memory();
		return false;
	}
	
	return true;
}

/*
 * Get an integer element of a dictionary of a function call parameter.
 */
bool
pf_get_call_arg_dict_int(
	const char *name,
	const char *key,
	int *val)
{
	NoctEnv *env;
	NoctValue param, dict, value;

	env = get_vm_env();

	/* Get the "param" argument. */
	if (!noct_get_arg_check_array(env, 0, &param))
		return false;

	/* Get the element by name. */
	if (!noct_get_dict_elem_check_dict(env, &param, name, &dict))
		return false;

	/* Get the dict element. */
	if (!noct_get_dict_elem_check_int(env, &dict, key, &value, val))
		return false;

	return true;
}

/*
 * Get a float element of a dictionary of a function call parameter.
 */
bool
pf_get_call_arg_dict_float(
	const char *name,
	const char *key,
	float *val)
{
	NoctEnv *env;
	NoctValue param, dict, value;

	env = get_vm_env();

	/* Get the "param" argument. */
	if (!noct_get_arg_check_array(env, 0, &param))
		return false;

	/* Get the element by name. */
	if (!noct_get_dict_elem_check_dict(env, &param, name, &dict))
		return false;

	/* Get the dict element. */
	if (!noct_get_dict_elem_check_float(env, &dict, key, &value, val))
		return false;

	return true;
}

/*
 * Get a string element of an array of a function call parameter.
 */
bool
pf_get_call_arg_dict_string(
	const char *name,
	const char *key,
	char **val)
{
	NoctEnv *env;
	NoctValue param, dict, value;
	const char *s;

	env = get_vm_env();

	/* Get the "param" argument. */
	if (!noct_get_arg_check_array(env, 0, &param))
		return false;

	/* Get the element by name. */
	if (!noct_get_dict_elem_check_dict(env, &param, name, &dict))
		return false;

	/* Get the dict element. */
	if (!noct_get_dict_elem_check_string(env, &dict, key, &value, &s))
		return false;

	/* Duplicate the string. */
	*val = strdup(s);
	if (*val == NULL) {
		log_out_of_memory();
		return false;
	}

	return true;
}

/*
 * Set an integer return value.
 */
bool
pf_set_return_int(
	int val)
{
	NoctEnv *env;
	NoctValue value;

	env = get_vm_env();

	if (!noct_set_return_make_int(env, &value, val))
		return false;

	return true;
}

/*
 * Set a float return value.
 */
bool
pf_set_return_float(
	float val)
{
	NoctEnv *env;
	NoctValue value;

	env = get_vm_env();

	if (!noct_set_return_make_float(env, &value, val))
		return false;

	return true;
}

/*
 * Set a string return value.
 */
bool
pf_set_return_string(
	const char *val)
{
	NoctEnv *env;
	NoctValue value;

	env = get_vm_env();

	if (!noct_set_return_make_string(env, &value, val))
		return false;

	return true;
}

/*
 * Set an integer array return value.
 */
bool
pf_set_return_int_array(
	int *val,
	int len)
{
	NoctEnv *env;
	NoctValue array, value;
	int i;

	env = get_vm_env();

	if (!noct_make_empty_array(env, &array))
		return false;

	for (i = 0; i < len; i++) {
		if (!noct_set_array_elem_make_int(env, &array, i, &value, val[i]))
			return false;
	}

	if (!noct_set_return(env, &array))
		return false;

	return true;
}

/*
 * Set a float array return value.
 */
bool
pf_set_return_float_array(
	float *val,
	int len)
{
	NoctEnv *env;
	NoctValue array, value;
	int i;

	env = get_vm_env();

	if (!noct_make_empty_array(env, &array))
		return false;

	for (i = 0; i < len; i++) {
		if (!noct_set_array_elem_make_float(env, &array, i, &value, val[i]))
			return false;
	}

	if (!noct_set_return(env, &array))
		return false;

	return true;
}

/*
 * Set a string array return value.
 */
bool
pf_set_return_string_array(
	const char **val,
	int len)
{
	NoctEnv *env;
	NoctValue array, value;
	int i;

	env = get_vm_env();

	if (!noct_make_empty_array(env, &array))
		return false;

	for (i = 0; i < len; i++) {
		if (!noct_set_array_elem_make_string(env, &array, i, &value, val[i]))
			return false;
	}

	if (!noct_set_return(env, &array))
		return false;

	return true;
}

/*
 * Set a dictionary return value.
 *  - type_desc ... A string that describes data types.
 *    "III" means 3 integers.
 *    "IIS" means 2 integers and 1 string.
 *    "IFS" means 1 integers, 1 float, and 1 string.
 */
bool
pf_set_return_dictionary(
	const char *type_desc,
	const char **keys,
	void **data)
{
	NoctEnv *env;
	NoctValue dict, value;
	int i;

	env = get_vm_env();

	if (!noct_make_empty_dict(env, &dict))
		return false;

	i = 0;
	while (*type_desc) {
		switch (*type_desc) {
		case 'I':
			if (!noct_set_dict_elem_make_int(env,
							 &dict,
							 keys[i],
							 &value,
							 *(int *)(data[i])))
				return false;
			break;
		case 'F':
			if (!noct_set_dict_elem_make_float(env,
							   &dict,
							   keys[i],
							   &value,
							   *(float *)(data[i])))
				return false;
			break;
		case 'S':
			if (!noct_set_dict_elem_make_string(env,
							    &dict,
							    keys[i],
							    &value,
							    (const char *)(data[i])))
				return false;
			break;
		default:
			assert(INVALID_DESC);
			return false;
		}
		type_desc++;
	}

	if (!noct_set_return(env, &dict))
		return false;

	return true;
}

/*
 * Tag
 */

/*
 * Load a tag file and move to it.
 */
bool
pf_move_to_tag_file(
	const char *file)
{
	return load_tag_file(file);
}

/*
 * Move to a next tag.
 */
bool
pf_move_to_next_tag(void)
{
	return move_to_next_tag();
}

/*
 * Get a tag file name.
 */
const char *
pf_get_tag_file(void)
{
	return get_tag_file_name();
}

/*
 * Get the command index of the current tag.
 */
int
pf_get_tag_index(void)
{
	return get_tag_index();
}

/*
 * Get a tag line.
 */
int
pf_get_tag_line(void)
{
	return get_tag_line();
}

/*
 * Get the name of the current tag.
 */
const char *
pf_get_tag_name(void)
{
	struct tag *t;

	t = get_current_tag();

	return t->tag_name;
}

/*
 * Get the property count of the current tag.
 */
int
pf_get_tag_property_count(void)
{
	struct tag *t;

	t = get_current_tag();

	return t->prop_count;
}

/*
 * Get the property name of the current tag.
 */
const char *
pf_get_tag_property_name(
	int index)
{
	struct tag *t;

	t = get_current_tag();

	assert(index < t->prop_count);
	if (index >= t->prop_count)
		return false;

	return t->prop_name[index];
}

/*
 * Get the property value of the current tag.
 */
const char *
pf_get_tag_property_value(
	int index)
{
	struct tag *t;

	t = get_current_tag();

	assert(index < t->prop_count);
	if (index >= t->prop_count)
		return false;

	return t->prop_value[index];
}

/*
 * Misc
 */

/*
 * Get the system locale.
 */
const char *
pf_get_system_language(void)
{
	return get_system_language();
}

/*
 * Print a debug message.
 */
void
pf_log_info(
	const char *msg,
	...)
{
	char buf[4096];
	va_list ap;

	va_start(ap, msg);
	vsnprintf(buf, sizeof(buf), msg, ap);
	va_end(ap);

	log_info(buf);
}

/*
 * Print a warning message.
 */
void
pf_log_warn(
	const char *msg,
	...)
{
	char buf[4096];
	va_list ap;

	va_start(ap, msg);
	vsnprintf(buf, sizeof(buf), msg, ap);
	va_end(ap);

	log_warn(buf);
}

/*
 * Print an error message.
 */
void
pf_log_error(
	const char *msg,
	...)
{
	char buf[4096];
	va_list ap;

	va_start(ap, msg);
	vsnprintf(buf, sizeof(buf), msg, ap);
	va_end(ap);

	log_error(buf);
}

/*
 * Print an out-of-memory error message.
 */
void
pf_log_out_of_memory(void)
{
	pf_log_error("Out-of-memory.");
}
