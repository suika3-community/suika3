/* -*- tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Strato HAL
 * Main code for IBM PC/AT compatibles (DOS/4G, VBE 2.0 LFB)
 */

/*-
 * SPDX-License-Identifier: Zlib
 *
 * Copyright (c) 2025-2026 Awe Morris
 * Copyright (c) 1996-2024 Keiichi Tabata
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

/* HAL */
#include <strato/strato.h>	/* Public Interface */
#include "stdfile.h"		/* Standard C File Implementation */

/* Standard C */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <locale.h>
#include <time.h>
#include <assert.h>

/* DOS */
#include <dos.h>
#include <conio.h>
#include <i86.h>

/* Screen Size */
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define BYTES_PER_PIXEL	3

/* VRAM */
#define VBE_SUCCESS		0x004f
#define VBE_MODE_LFB		0x4000
#define VBE_ATTR_SUPPORTED	0x0001
#define VBE_ATTR_COLOR		0x0008
#define VBE_ATTR_GRAPHICS	0x0010
#define VBE_ATTR_LFB		0x0080
#define VBE_MEMORY_PACKEDPIXEL	4
#define VBE_MEMORY_DIRECTCOLOR	6

/* Log */
#define LOG_FILE	"log.txt"

/*
 * VBE
 */
#pragma pack(push, 1)
struct vbe_info_block {
	char		signature[4];
	uint16_t	version;
	uint32_t	oem_string_ptr;
	uint32_t	capabilities;
	uint32_t	video_mode_ptr;
	uint16_t	total_memory;
	uint8_t		reserved[236];
	uint8_t		oem_data[256];
};

struct vbe_mode_info_block {
	uint16_t	mode_attributes;
	uint8_t		win_a_attributes;
	uint8_t		win_b_attributes;
	uint16_t	win_granularity;
	uint16_t	win_size;
	uint16_t	win_a_segment;
	uint16_t	win_b_segment;
	uint32_t	win_func_ptr;
	uint16_t	bytes_per_scan_line;
	uint16_t	x_resolution;
	uint16_t	y_resolution;
	uint8_t		x_char_size;
	uint8_t		y_char_size;
	uint8_t		number_of_planes;
	uint8_t		bits_per_pixel;
	uint8_t		number_of_banks;
	uint8_t		memory_model;
	uint8_t		bank_size;
	uint8_t		number_of_image_pages;
	uint8_t		reserved0;
	uint8_t		red_mask_size;
	uint8_t		red_field_position;
	uint8_t		green_mask_size;
	uint8_t		green_field_position;
	uint8_t		blue_mask_size;
	uint8_t		blue_field_position;
	uint8_t		reserved_mask_size;
	uint8_t		reserved_field_position;
	uint8_t		direct_color_mode_info;
	uint32_t	phys_base_ptr;
	uint32_t	off_screen_mem_offset;
	uint16_t	off_screen_mem_size;
	uint16_t	lin_bytes_per_scan_line;
	uint8_t		bank_number_of_image_pages;
	uint8_t		lin_number_of_image_pages;
	uint8_t		lin_red_mask_size;
	uint8_t		lin_red_field_position;
	uint8_t		lin_green_mask_size;
	uint8_t		lin_green_field_position;
	uint8_t		lin_blue_mask_size;
	uint8_t		lin_blue_field_position;
	uint8_t		lin_reserved_mask_size;
	uint8_t		lin_reserved_field_position;
	uint32_t	max_pixel_clock;
	uint8_t		reserved1[189];
};
#pragma pack(pop)

/* Game Info */
static char *game_title;
static int game_width;
static int game_height;

/* Screen */
static struct hal_image *back_image;
static uint8_t *lfb;
static uint32_t lfb_linear;
static uint32_t lfb_size;
static int pitch;
static int vbe_mode;
static struct vbe_mode_info_block active_mode_info;

/* Log */
static FILE *log_fp;

/* Callback. */
static struct hal_callback hal_callback;
HAL_DLL bool (*hal_bootstrap_ptr)(char **title, int *width, int *height, struct hal_callback *callback);

/* Forward Declaration */
static void init_vram(void);
static void cleanup_vram(void);
static void process_input(void);
static void flip(void);
static bool open_log_file(void);
static bool get_vbe_info(struct vbe_info_block *info);
static bool get_vbe_mode_info(int mode, struct vbe_mode_info_block *info);
static bool find_vbe_mode(int *mode, struct vbe_mode_info_block *info);
static bool set_vbe_mode(int mode);
static uint32_t map_physical_address(uint32_t phys_addr, uint32_t size);
static void unmap_physical_address(uint32_t linear_addr);
static void put_pixel_24(uint8_t *dst, uint32_t pix);

int hal_main(int argc, char *argv[])
{
	printf("\n"
	       "Suika3 Game Engine for PC/AT\n"
	       "Copyright (c) 2026 Awe Morris\n");

	if (argc >= 2) {
		if (strcmp(argv[1], "--version") == 0) {
			printf("Version 2026.05\n");
			return 0;
		}
	}

	if (!init_file()) {
		printf("Failed to initialize the file system.\n");
		return 1;
	}

	if (!hal_bootstrap_ptr(&game_title,
			       &game_width,
			       &game_height,
			       &hal_callback)) {
		printf("Error on boot.\n");
		return 1;
	}

	if (!hal_create_image(game_width, game_height, &back_image)) {
		printf("Error on creating image.\n");
		return 1;
	}

	if (!hal_callback.on_start()) {
		printf("Error on start.\n");
		return 1;
	}

	init_vram();

	while (1) {
		process_input();
		hal_clear_image(back_image, 0);
		if (!hal_callback.on_update())
			break;
		hal_callback.on_render();
		flip();
	}

	cleanup_vram();
	return 0;
}

static bool
get_vbe_info(struct vbe_info_block *info)
{
	union REGS r;
	struct SREGS s;

	memset(info, 0, sizeof(*info));
	memcpy(info->signature, "VBE2", 4);
	memset(&r, 0, sizeof(r));
	memset(&s, 0, sizeof(s));

	r.w.ax = 0x4f00;
	s.es = FP_SEG(info);
	r.x.edi = FP_OFF(info);
	int386x(0x10, &r, &r, &s);

	return r.w.ax == VBE_SUCCESS && memcmp(info->signature, "VESA", 4) == 0;
}

static bool
get_vbe_mode_info(int mode, struct vbe_mode_info_block *info)
{
	union REGS r;
	struct SREGS s;

	memset(info, 0, sizeof(*info));
	memset(&r, 0, sizeof(r));
	memset(&s, 0, sizeof(s));

	r.w.ax = 0x4f01;
	r.w.cx = (uint16_t)mode;
	s.es = FP_SEG(info);
	r.x.edi = FP_OFF(info);
	int386x(0x10, &r, &r, &s);

	return r.w.ax == VBE_SUCCESS;
}

static bool
find_vbe_mode(int *mode, struct vbe_mode_info_block *info)
{
	struct vbe_info_block vbe;
	uint16_t far *mode_list;
	uint16_t m;

	if (!get_vbe_info(&vbe)) {
		hal_log_error("VBE 2.0 is not available.");
		return false;
	}
	if (vbe.version < 0x0200) {
		hal_log_error("VBE version is too old: %x.%02x.", vbe.version >> 8, vbe.version & 0xff);
		return false;
	}

	mode_list = (uint16_t far *)MK_FP((uint16_t)(vbe.video_mode_ptr >> 16),
					    (uint16_t)(vbe.video_mode_ptr & 0xffff));

	while ((m = *mode_list++) != 0xffff) {
		struct vbe_mode_info_block mi;
		uint16_t attr;
		uint16_t scan_line;

		if (!get_vbe_mode_info(m, &mi))
			continue;
		attr = mi.mode_attributes;
		if ((attr & VBE_ATTR_SUPPORTED) == 0 ||
		    (attr & VBE_ATTR_COLOR) == 0 ||
		    (attr & VBE_ATTR_GRAPHICS) == 0 ||
		    (attr & VBE_ATTR_LFB) == 0)
			continue;
		if (mi.x_resolution != SCREEN_WIDTH || mi.y_resolution != SCREEN_HEIGHT)
			continue;
		if (mi.bits_per_pixel != 24)
			continue;
		if (mi.memory_model != VBE_MEMORY_DIRECTCOLOR &&
		    mi.memory_model != VBE_MEMORY_PACKEDPIXEL)
			continue;
		if (mi.phys_base_ptr == 0)
			continue;

		scan_line = mi.lin_bytes_per_scan_line != 0 ?
			mi.lin_bytes_per_scan_line : mi.bytes_per_scan_line;
		if (scan_line < SCREEN_WIDTH * BYTES_PER_PIXEL)
			continue;

		*mode = m;
		*info = mi;
		return true;
	}

	hal_log_error("No 640x480x24bpp VBE LFB mode found.");
	return false;
}

static bool
set_vbe_mode(int mode)
{
	union REGS r;

	memset(&r, 0, sizeof(r));
	r.w.ax = 0x4f02;
	r.w.bx = (uint16_t)(mode | VBE_MODE_LFB);
	int386(0x10, &r, &r);
	return r.w.ax == VBE_SUCCESS;
}

static uint32_t
map_physical_address(uint32_t phys_addr, uint32_t size)
{
	union REGS regs;
	struct SREGS sregs;

	memset(&regs, 0, sizeof(regs));
	memset(&sregs, 0, sizeof(sregs));
	regs.w.ax = 0x0800;
	regs.w.bx = (uint16_t)(phys_addr >> 16);
	regs.w.cx = (uint16_t)(phys_addr & 0xffff);
	regs.w.si = (uint16_t)(size >> 16);
	regs.w.di = (uint16_t)(size & 0xffff);
	int386x(0x31, &regs, &regs, &sregs);
	if (regs.x.cflag)
		return 0;
	return ((uint32_t)regs.w.bx << 16) | regs.w.cx;
}

static void
unmap_physical_address(uint32_t linear_addr)
{
	union REGS regs;
	struct SREGS sregs;

	if (linear_addr == 0)
		return;
	memset(&regs, 0, sizeof(regs));
	memset(&sregs, 0, sizeof(sregs));
	regs.w.ax = 0x0801;
	regs.w.bx = (uint16_t)(linear_addr >> 16);
	regs.w.cx = (uint16_t)(linear_addr & 0xffff);
	int386x(0x31, &regs, &regs, &sregs);
}

static void
init_vram(void)
{
	if (!find_vbe_mode(&vbe_mode, &active_mode_info)) {
		printf("Failed to find VBE mode.\n");
		exit(1);
	}

	pitch = active_mode_info.lin_bytes_per_scan_line != 0 ?
		active_mode_info.lin_bytes_per_scan_line :
		active_mode_info.bytes_per_scan_line;
	lfb_size = (uint32_t)pitch * SCREEN_HEIGHT;
	lfb_linear = map_physical_address(active_mode_info.phys_base_ptr, lfb_size);
	if (lfb_linear == 0) {
		hal_log_error("Failed to map VBE LFB: phys=%lx size=%lx.",
			      active_mode_info.phys_base_ptr, lfb_size);
		exit(1);
	}
	lfb = (uint8_t *)lfb_linear;

	if (!set_vbe_mode(vbe_mode)) {
		hal_log_error("Failed to set VBE mode: %x.", vbe_mode);
		unmap_physical_address(lfb_linear);
		lfb = NULL;
		lfb_linear = 0;
		exit(1);
	}
	memset(lfb, 0, lfb_size);
}

static void
cleanup_vram(void)
{
	union REGS r;

	if (lfb_linear != 0) {
		unmap_physical_address(lfb_linear);
		lfb_linear = 0;
		lfb = NULL;
	}
	memset(&r, 0, sizeof(r));
	r.w.ax = 0x0003;
	int386(0x10, &r, &r);
}

static INLINE void
put_pixel_24(uint8_t *dst, uint32_t pix)
{
	uint8_t r, g, b;

	r = (uint8_t)(pix & 0xff);
	g = (uint8_t)((pix >> 8) & 0xff);
	b = (uint8_t)((pix >> 16) & 0xff);

	/* VBE 24bpp is usually BGR. */
	dst[0] = b;
	dst[1] = g;
	dst[2] = r;
}

static void
flip(void)
{
	uint32_t *pixels;
	int x, y;
	int copy_width;
	int copy_height;

	if (lfb == NULL)
		return;
	pixels = back_image->pixels;
	copy_width = game_width < SCREEN_WIDTH ? game_width : SCREEN_WIDTH;
	copy_height = game_height < SCREEN_HEIGHT ? game_height : SCREEN_HEIGHT;

	for (y = 0; y < copy_height; y++) {
		uint8_t *dst = lfb + (uint32_t)y * pitch;
		uint32_t *src = pixels + (uint32_t)y * game_width;
		for (x = 0; x < copy_width; x++)
			put_pixel_24(dst + x * BYTES_PER_PIXEL, src[x]);
	}
}

static void process_input(void)
{
	static bool is_return_key_pressed = false;
	static bool is_space_key_pressed = false;
	static bool is_up_key_pressed = false;
	static bool is_down_key_pressed = false;
	static bool is_left_key_pressed = false;
	static bool is_right_key_pressed = false;
	bool next_is_return_key_pressed = false;
	bool next_is_space_key_pressed = false;
	bool next_is_up_key_pressed = false;
	bool next_is_down_key_pressed = false;
	bool next_is_left_key_pressed = false;
	bool next_is_right_key_pressed = false;

	while (1) {
		int ch, keycode;

		if (!kbhit())
			break;

		ch = getch();
		switch (ch) {
		case '\r':
			next_is_return_key_pressed = true;
			break;
		case ' ':
			next_is_space_key_pressed = true;
			break;
		case 0x00:
		case 0xe0:
			/* Extended key. */
			if (!kbhit())
				break;
			ch = getch();
			switch (ch) {
			case 0x48:
				next_is_up_key_pressed = true;
				break;
			case 0x50:
				next_is_down_key_pressed = true;
				break;
			case 0x4b:
				next_is_left_key_pressed = true;
				break;
			case 0x4d:
				next_is_right_key_pressed = true;
				break;
			}
			break;
		}
	}

	if (!is_return_key_pressed && next_is_return_key_pressed)
		hal_callback.on_key_press(HAL_KEY_RETURN);
	if (is_return_key_pressed && !next_is_return_key_pressed)
		hal_callback.on_key_release(HAL_KEY_RETURN);
	is_return_key_pressed = next_is_return_key_pressed;

	if (!is_space_key_pressed && next_is_space_key_pressed)
		hal_callback.on_key_press(HAL_KEY_SPACE);
	if (is_space_key_pressed && !next_is_space_key_pressed)
		hal_callback.on_key_release(HAL_KEY_SPACE);
	is_space_key_pressed = next_is_space_key_pressed;

	if (!is_up_key_pressed && next_is_up_key_pressed)
		hal_callback.on_key_press(HAL_KEY_UP);
	if (is_up_key_pressed && !next_is_up_key_pressed)
		hal_callback.on_key_release(HAL_KEY_UP);
	is_up_key_pressed = next_is_up_key_pressed;
		
	if (!is_down_key_pressed && next_is_down_key_pressed)
		hal_callback.on_key_press(HAL_KEY_DOWN);
	if (is_down_key_pressed && !next_is_down_key_pressed)
		hal_callback.on_key_release(HAL_KEY_DOWN);
	is_down_key_pressed = next_is_down_key_pressed;

	if (!is_left_key_pressed && next_is_left_key_pressed)
		hal_callback.on_key_press(HAL_KEY_LEFT);
	if (is_left_key_pressed && !next_is_left_key_pressed)
		hal_callback.on_key_release(HAL_KEY_LEFT);
	is_left_key_pressed = next_is_left_key_pressed;

	if (!is_right_key_pressed && next_is_right_key_pressed)
		hal_callback.on_key_press(HAL_KEY_RIGHT);
	if (is_right_key_pressed && !next_is_right_key_pressed)
		hal_callback.on_key_release(HAL_KEY_RIGHT);
	is_right_key_pressed = next_is_right_key_pressed;
}

/*
 * HAL
 */

void hal_notify_image_update(struct hal_image *img)
{
	UNUSED_PARAMETER(img);
}

void hal_notify_image_free(struct hal_image *img)
{
	UNUSED_PARAMETER(img);
}

void
hal_render_image_normal(
	int dst_left,			/* The X coordinate of the screen */
	int dst_top,			/* The Y coordinate of the screen */
	int dst_width,			/* The width of the destination rectangle */
	int dst_height,			/* The height of the destination rectangle */
	struct hal_image *src_image,	/* [IN] The image to be rendered */
	int src_left,			/* The X coordinate of a source image */
	int src_top,			/* The Y coordinate of a source image */
	int src_width,			/* The width of the source rectangle */
	int src_height,			/* The height of the source rectangle */
	int alpha)			/* The alpha value (0 to 255) */
{
	if (dst_width == -1)
		dst_width = src_image->width;
	if (dst_height == -1)
		dst_height = src_image->height;
	if (src_width == -1)
		src_width = src_image->width;
	if (src_height == -1)
		src_height = src_image->height;

	hal_draw_image_alpha(
		back_image,
		dst_left,
		dst_top,
		src_image,
		src_width,
		src_height,
		src_left,
		src_top,
		alpha);
}

void
hal_render_image_add(
	int dst_left,			/* The X coordinate of the screen */
	int dst_top,			/* The Y coordinate of the screen */
	int dst_width,			/* The width of the destination rectangle */
	int dst_height,			/* The width of the destination rectangle */
	struct hal_image *src_image,	/* [IN] The image to be rendered */
	int src_left,			/* The X coordinate of a source image */
	int src_top,			/* The Y coordinate of a source image */
	int src_width,			/* The width of the source rectangle */
	int src_height,			/* The height of the source rectangle */
	int alpha)			/* The alpha value (0 to 255) */
{
	if (dst_width == -1)
		dst_width = src_image->width;
	if (dst_height == -1)
		dst_height = src_image->height;
	if (src_width == -1)
		src_width = src_image->width;
	if (src_height == -1)
		src_height = src_image->height;

	hal_draw_image_add(
		back_image,
		dst_left,
		dst_top,
		src_image,
		src_width,
		src_height,
		src_left,
		src_top,
		alpha);
}

void
hal_render_image_sub(
	int dst_left,			/* The X coordinate of the screen */
	int dst_top,			/* The Y coordinate of the screen */
	int dst_width,			/* The width of the destination rectangle */
	int dst_height,			/* The width of the destination rectangle */
	struct hal_image *src_image,	/* [IN] The image to be rendered */
	int src_left,			/* The X coordinate of a source image */
	int src_top,			/* The Y coordinate of a source image */
	int src_width,			/* The width of the source rectangle */
	int src_height,			/* The height of the source rectangle */
	int alpha)			/* The alpha value (0 to 255) */
{
	if (dst_width == -1)
		dst_width = src_image->width;
	if (dst_height == -1)
		dst_height = src_image->height;
	if (src_width == -1)
		src_width = src_image->width;
	if (src_height == -1)
		src_height = src_image->height;

	hal_draw_image_sub(
		back_image,
		dst_left,
		dst_top,
		src_image,
		src_width,
		src_height,
		src_left,
		src_top,
		alpha);
}

void
hal_render_image_dim(
	int dst_left,			/* The X coordinate of the screen */
	int dst_top,			/* The Y coordinate of the screen */
	int dst_width,			/* The width of the destination rectangle */
	int dst_height,			/* The height of the destination rectangle */
	struct hal_image *src_image,	/* [IN] The image to be rendered */
	int src_left,			/* The X coordinate of a source image */
	int src_top,			/* The Y coordinate of a source image */
	int src_width,			/* The width of the source rectangle */
	int src_height,			/* The height of the source rectangle */
	int alpha)			/* The alpha value (0 to 255) */
{
	if (dst_width == -1)
		dst_width = src_image->width;
	if (dst_height == -1)
		dst_height = src_image->height;
	if (src_width == -1)
		src_width = src_image->width;
	if (src_height == -1)
		src_height = src_image->height;

	hal_draw_image_dim(
		back_image,
		dst_left,
		dst_top,
		src_image,
		src_width,
		src_height,
		src_left,
		src_top,
		alpha);
}

void
hal_render_image_rule(
	struct hal_image *src_img,	/* [IN] The source image */
	struct hal_image *rule_img,	/* [IN] The rule image */
	int threshold)			/* The threshold (0 to 255) */
{
	hal_draw_image_rule(back_image, src_img, rule_img, threshold);
}

void
hal_render_image_melt(
	struct hal_image *src_img,	/* [IN] The source image */
	struct hal_image *rule_img,	/* [IN] The rule image */
	int progress)			/* The progress (0 to 255) */
{
	hal_draw_image_melt(back_image, src_img, rule_img, progress);
}

void
hal_render_image_cross(
	struct hal_image *src1_img,
	struct hal_image *src2_img,
	float src1_left,
	float src1_top,
	float src2_left,
	float src2_top,
	int alpha)
{
	hal_draw_image_cross(back_image,
			     src1_img,
			     src2_img,
			     src1_left,
			     src1_top,
			     src2_left,
			     src2_top,
			     alpha);
}

void
hal_render_image_3d_normal(
	float x1,			/* x1 */
	float y1,			/* y1 */
	float x2,			/* x2 */
	float y2,			/* y2 */
	float x3,			/* x3 */
	float y3,			/* y3 */
	float x4,			/* x4 */
	float y4,			/* y4 */
	struct hal_image *src_image,	/* [IN] The source image */
	int src_left,			/* The X coordinate of a source image */
	int src_top,			/* The Y coordinate of a source image */
	int src_width,			/* The width of the source rectangle */
	int src_height,			/* The height of the source rectangle */
	int alpha)			/* The alpha value (0 to 255) */
{
	hal_draw_image_3d_alpha(back_image,
				(float)x1,
				(float)y1,
				(float)x2,
				(float)y2,
				(float)x3,
				(float)y3,
				(float)x4,
				(float)y4,
				src_image,
				src_left,
				src_top,
				src_width,
				src_height,
				alpha);
}

void
hal_render_image_3d_add(
	float x1,			/* x1 */
	float y1,			/* y1 */
	float x2,			/* x2 */
	float y2,			/* y2 */
	float x3,			/* x3 */
	float y3,			/* y3 */
	float x4,			/* x4 */
	float y4,			/* y4 */
	struct hal_image *src_image,	/* [IN] The source image */
	int src_left,			/* The X coordinate of a source image */
	int src_top,			/* The Y coordinate of a source image */
	int src_width,			/* The width of the source rectangle */
	int src_height,			/* The height of the source rectangle */
	int alpha)			/* The alpha value (0 to 255) */
{
	hal_draw_image_3d_alpha(back_image,
				(float)x1,
				(float)y1,
				(float)x2,
				(float)y2,
				(float)x3,
				(float)y3,
				(float)x4,
				(float)y4,
				src_image,
				src_left,
				src_top,
				src_width,
				src_height,
				alpha);
}

void
hal_render_image_3d_sub(
	float x1,			/* x1 */
	float y1,			/* y1 */
	float x2,			/* x2 */
	float y2,			/* y2 */
	float x3,			/* x3 */
	float y3,			/* y3 */
	float x4,			/* x4 */
	float y4,			/* y4 */
	struct hal_image *src_image,	/* [IN] The source image */
	int src_left,			/* The X coordinate of a source image */
	int src_top,			/* The Y coordinate of a source image */
	int src_width,			/* The width of the source rectangle */
	int src_height,			/* The height of the source rectangle */
	int alpha)			/* The alpha value (0 to 255) */
{
	hal_draw_image_3d_sub(back_image,
			      (float)x1,
			      (float)y1,
			      (float)x2,
			      (float)y2,
			      (float)x3,
			      (float)y3,
			      (float)x4,
			      (float)y4,
			      src_image,
			      src_left,
			      src_top,
			      src_width,
			      src_height,
			      alpha);
}

void
hal_render_image_3d_dim(
	float x1,			/* x1 */
	float y1,			/* y1 */
	float x2,			/* x2 */
	float y2,			/* y2 */
	float x3,			/* x3 */
	float y3,			/* y3 */
	float x4,			/* x4 */
	float y4,			/* y4 */
	struct hal_image *src_image,	/* [IN] The source image */
	int src_left,			/* The X coordinate of a source image */
	int src_top,			/* The Y coordinate of a source image */
	int src_width,			/* The width of the source rectangle */
	int src_height,			/* The height of the source rectangle */
	int alpha)			/* The alpha value (0 to 255) */
{
	hal_draw_image_3d_dim(back_image,
			      (float)x1,
			      (float)y1,
			      (float)x2,
			      (float)y2,
			      (float)x3,
			      (float)y3,
			      (float)x4,
			      (float)y4,
			      src_image,
			      src_left,
			      src_top,
			      src_width,
			      src_height,
			      alpha);
}

void
hal_render_image_3d_cross(
	struct hal_image *src1_img,
	struct hal_image *src2_img,
	float src1_x1,
	float src1_y1,
	float src1_x2,
	float src1_y2,
	float src1_x3,
	float src1_y3,
	float src1_x4,
	float src1_y4,
	float src2_x1,
	float src2_y1,
	float src2_x2,
	float src2_y2,
	float src2_x3,
	float src2_y3,
	float src2_x4,
	float src2_y4,
	int alpha)
{
	hal_draw_image_3d_cross(back_image,
				src1_img,
				src2_img,
				src1_x1,
				src1_y1,
				src1_x2,
				src1_y2,
				src1_x3,
				src1_y3,
				src1_x4,
				src1_y4,
				src2_x1,
				src2_y1,
				src2_x2,
				src2_y2,
				src2_x3,
				src2_y3,
				src2_x4,
				src2_y4,
				alpha);
}

static uint32_t get_time(void)
{
	union REGS r;
	uint32_t tick;

	r.h.al = 0xff;
	r.h.ah = 0x80;
	int386(0x1c, &r, &r);

	tick = (r.w.cx << 16) | r.w.dx;

	return tick * 1000 / 32;
}

void
hal_reset_lap_timer(
	uint64_t *t)
{
	*t = (uint64_t)get_time();
}

uint64_t
hal_get_lap_timer_millisec(
	uint64_t *t)
{
	uint64_t end;
	
	end = (uint64_t)get_time();

	return (uint64_t)((end - *t));
}

bool
hal_play_video(
	const char *fname,
	bool is_skippable)
{
	UNUSED_PARAMETER(fname);
	UNUSED_PARAMETER(is_skippable);
	return true;
}

void
hal_stop_video(void)
{
}

bool
hal_is_video_playing(void)
{
	return false;
}

bool
hal_is_full_screen_supported(void)
{
	return false;
}

bool
hal_is_full_screen_mode(void)
{
	return false;
}

void
hal_enter_full_screen_mode(void)
{
}

void
hal_leave_full_screen_mode(void)
{
}

bool
make_save_directory(void)
{
	return true;
}

char *
make_real_path(const char *fname)
{
	char *s, *t;

	s = strdup(fname);
	if (s == NULL) {
		hal_log_out_of_memory();
		return NULL;
	}

	t = s;
	while (*t != '\0') {
		if (*t == '/')
			*t = '\\';
		t++;
	}

	return s;
}

bool
hal_log_info(
	const char *s,
	...)
{
	char buf[1024];
	va_list ap;

	va_start(ap, s);
	vsnprintf(buf, sizeof(buf), s, ap);
	va_end(ap);

	open_log_file();
	if (log_fp != NULL) {
		fprintf(log_fp, "%s\n", buf);
		fflush(log_fp);
		if (ferror(log_fp))
			return false;
	}
	printf("%s\n", buf);

	return true;
}

bool
hal_log_warn(
	const char *s,
	...)
{
	char buf[1024];
	va_list ap;

	va_start(ap, s);
	vsnprintf(buf, sizeof(buf), s, ap);
	va_end(ap);

	open_log_file();
	if (log_fp != NULL) {
		fprintf(log_fp, "%s\n", buf);
		fflush(log_fp);
		if (ferror(log_fp))
			return false;
	}
	printf("%s\n", buf);

	return true;
}

bool
hal_log_error(
	const char *s,
	...)
{
	char buf[1024];
	va_list ap;

	va_start(ap, s);
	vsnprintf(buf, sizeof(buf), s, ap);
	va_end(ap);

	open_log_file();
	if (log_fp != NULL) {
		fprintf(log_fp, "%s\n", buf);
		fflush(log_fp);
		if (ferror(log_fp))
			return false;
	}
	printf("%s\n", buf);
	
	return true;
}

static bool
open_log_file(void)
{
	if (log_fp == NULL) {
		log_fp = fopen(LOG_FILE, "w");
		if (log_fp == NULL) {
			printf("Can't open log file.\n");
			return false;
		}
	}
	return true;
}

bool
hal_log_out_of_memory(void)
{
	hal_log_error("Out of memory.\n");
	return true;
}

const char *
hal_get_system_language(void)
{
	return "ja";
}

void
hal_set_continuous_swipe_enabled(
	bool is_enabled)
{
	UNUSED_PARAMETER(is_enabled);
}

bool
hal_play_sound(
	int stream,		/* A sound stream index */
	struct hal_wave *w)	/* [IN] A sound object, ownership will be delegated to the callee */
{
	UNUSED_PARAMETER(stream);
	UNUSED_PARAMETER(w);
	return true;
}

bool
hal_stop_sound(
	int stream)
{
	UNUSED_PARAMETER(stream);
	return true;
}

bool
hal_set_sound_volume(
	int stream,
	float vol)
{
	UNUSED_PARAMETER(stream);
	UNUSED_PARAMETER(vol);
	return true;
}

bool
hal_is_sound_finished(
	int stream)
{
	UNUSED_PARAMETER(stream);
	return true;
}
