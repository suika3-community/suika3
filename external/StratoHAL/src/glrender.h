/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * OpenGL Rendering
 */

#ifndef PLATFORM_GLRENDER_H
#define PLATFORM_GLRENDER_H

#include "stratohal/platform.h"

bool init_opengl(int width, int height);
void cleanup_opengl(void);
void opengl_start_rendering(void);
void opengl_end_rendering(void);
void opengl_notify_image_update(struct image *img);
void opengl_notify_image_free(struct image *img);

void
opengl_render_image_normal(
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	struct image *src_image,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha);

void
opengl_render_image_add(
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	struct image *src_image,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha);

void
opengl_render_image_sub(
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	struct image *src_image,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha);

void
opengl_render_image_dim(
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	struct image *src_image,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha);

void
opengl_render_image_rule(
	struct image *src_image,
	struct image *rule_image,
	int threshold);

void
opengl_render_image_melt(
	struct image *src_image,
	struct image *rule_image,
	int progress);

void
opengl_render_image_3d_normal(
	float x1,
	float y1,
	float x2,
	float y2,
	float x3,
	float y3,
	float x4,
	float y4,
	struct image *src_image,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha);

void
opengl_render_image_3d_add(
	float x1,
	float y1,
	float x2,
	float y2,
	float x3,
	float y3,
	float x4,
	float y4,
	struct image *src_image,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha);

void
opengl_render_image_3d_sub(
	float x1,
	float y1,
	float x2,
	float y2,
	float x3,
	float y3,
	float x4,
	float y4,
	struct image *src_image,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha);

void
opengl_render_image_3d_dim(
	float x1,
	float y1,
	float x2,
	float y2,
	float x3,
	float y3,
	float x4,
	float y4,
	struct image *src_image,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha);

void opengl_set_screen(int x, int y, int w, int h);

#endif
