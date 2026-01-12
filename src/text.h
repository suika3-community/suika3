/* -*- coding: utf-8; indent-tabs-mode: t; tab-width: 8; c-basic-offset: 8; -*- */

/*
 * Suika3
 * Copyright (c) 2001-2026 The Suika3 Authors
 */

/*
 * Text Rendering
 */

#ifndef SUIKA3_TEXT_H
#define SUIKA3_TEXT_H

#include <suika3/suika3.h>

/* Unicode Codepoints */
#define CHAR_TOUTEN		(0x3001)
#define CHAR_KUTEN		(0x3002)
#define CHAR_YENSIGN		(0x00a5)

/* Font Types */
#define FONT_SELECT1		(0)
#define FONT_SELECT2		(1)
#define FONT_SELECT3		(2)
#define FONT_SELECT4		(3)
#define FONT_COUNT		(4)

/*
 * Context for message drawing.
 */
struct draw_msg_context {
/* private: */
	/* Will be copied in the constructor. */
	struct s3_image *image;
	const char *msg;	/* Updated on a draw. */
	int font;
	int font_size;
	int base_font_size;
	int ruby_size;
	int outline_size;
	int pen_x;		/* Updated on a draw. */
	int pen_y;		/* Updated on a draw. */
	int area_width;
	int area_height;
	int left_margin;
	int right_margin;
	int top_margin;
	int bottom_margin;
	int line_margin;
	int char_margin;
	s3_pixel_t default_color;
	s3_pixel_t color;
	s3_pixel_t outline_color;
	s3_pixel_t bg_color;
	bool is_dimming;
	bool ignore_linefeed;
	bool ignore_font;
	bool ignore_outline;
	bool ignore_color;
	bool ignore_size;
	bool ignore_position;
	bool ignore_ruby;
	bool ignore_wait;
	bool fill_bg;
	void (*inline_wait_hook)(float);
	bool use_tategaki;

	/* Internal: updated on a draw. */
	struct image *layer_image;
	bool runtime_is_after_space;
	bool runtime_is_inline_wait;
	int runtime_ruby_x;
	int runtime_ruby_y;
	bool runtime_is_line_top;
	bool runtime_is_gyoto_kinsoku;
	bool runtime_is_gyoto_kinsoku_second;
	bool is_quoted;
};

/* Initialize the glyph subsystem. */
bool init_text(void);

/* Cleanup the glyph subsystem. */
void cleanup_text(void);

/* Check if c is an escape sequence character. */
bool is_escape_sequence_char(char c);

#endif
