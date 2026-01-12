/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (C) 2001-2026 The Suika3 Authors
 */

/*
 * Public C API
 */

#ifndef SUIKA3_SUIKA3_H
#define SUIKA3_SUIKA3_H

#include <suika3/c89compat.h>

/*
 * Constants
 */

/*
 * Name of the first tag file. (game.c)
 */
#define S3_FIRST_TAG_FILE	"start.s3"

/*
 * Name of the config file (conf.c)
 */
#define S3_CONFIG_FILE		"config.ini"

/*
 * Stage layers. (stage.c)
 */
#define S3_LAYER_BG		(0)	/* Background */
#define S3_LAYER_BG_FO		(1)	/*   Fade-Out */
#define S3_LAYER_BG2		(2)	/* Background2 */
#define S3_LAYER_EFB1		(3)	/* Back effect 1 */
#define S3_LAYER_EFB2		(4)	/* Back effect 2 */
#define S3_LAYER_EFB3		(5)	/* Back effect 3 */
#define S3_LAYER_EFB4		(6)	/* Back effect 4 */
#define S3_LAYER_CHB		(7)	/* Character "back" */
#define S3_LAYER_CHB_EYE	(8)	/*   Eyes */
#define S3_LAYER_CHB_LIP	(9)	/*   Lips */
#define S3_LAYER_CHB_FO		(10)	/*   Fade-Out */
#define S3_LAYER_CHL		(11)	/* Character "left" */
#define S3_LAYER_CHL_EYE	(12)	/*   Eyes */
#define S3_LAYER_CHL_LIP	(13)	/*   Lips */
#define S3_LAYER_CHL_FO		(14)	/*   Fade-Out */
#define S3_LAYER_CHLC		(15)	/* Character "left-center" */
#define S3_LAYER_CHLC_EYE	(16)	/*   Eyes */
#define S3_LAYER_CHLC_LIP	(17)	/*   Lips */
#define S3_LAYER_CHLC_FO	(18)	/*   Fade-Out */
#define S3_LAYER_CHR		(19)	/* Character "right" */
#define S3_LAYER_CHR_EYE	(20)	/*   Eyes */
#define S3_LAYER_CHR_LIP	(21)	/*   Lips */
#define S3_LAYER_CHR_FO		(22)	/*   Fade-Out */
#define S3_LAYER_CHRC		(23)	/* Character "right-center" */
#define S3_LAYER_CHRC_EYE	(24)	/*   Eyes */
#define S3_LAYER_CHRC_LIP	(25)	/*   Lips */
#define S3_LAYER_CHRC_FO	(26)	/*   Fade-Out */
#define S3_LAYER_CHC		(27)	/* Character "right-center" */
#define S3_LAYER_CHC_EYE	(28)	/*   Eyes */
#define S3_LAYER_CHC_LIP	(29)	/*   Lips */
#define S3_LAYER_CHC_FO		(30)	/*   Fade-Out */
#define S3_LAYER_EFF1		(31)	/* Front effect 1 */
#define S3_LAYER_EFF2		(32)	/* Front effect 2 */
#define S3_LAYER_EFF3		(33)	/* Front effect 3 */
#define S3_LAYER_EFF4		(34)	/* Front effect 4 */
#define S3_LAYER_MSGBOX		(35)	/* Message box (text layer) */
#define S3_LAYER_NAMEBOX	(36)	/* Name box (text layer) */
#define S3_LAYER_CHOOSE1_IDLE	(37)	/* Choose box 1 (idle, text layer) */
#define S3_LAYER_CHOOSE1_HOVER	(38)	/* Choose box 1 (hover, text layer) */
#define S3_LAYER_CHOOSE2_IDLE	(39)	/* Choose box 2 (idle, text layer) */
#define S3_LAYER_CHOOSE2_HOVER	(40)	/* Choose box 2 (hover, text layer) */
#define S3_LAYER_CHOOSE3_IDLE	(41)	/* Choose box 3 (idle, text layer) */
#define S3_LAYER_CHOOSE3_HOVER	(42)	/* Choose box 3 (hover, text layer) */
#define S3_LAYER_CHOOSE4_IDLE	(43)	/* Choose box 4 (idle, text layer) */
#define S3_LAYER_CHOOSE4_HOVER	(44)	/* Choose box 4 (hover, text layer) */
#define S3_LAYER_CHOOSE5_IDLE	(45)	/* Choose box 5 (idle, text layer) */
#define S3_LAYER_CHOOSE5_HOVER	(46)	/* Choose box 5 (hover, text layer) */
#define S3_LAYER_CHOOSE6_IDLE	(47)	/* Choose box 6 (idle, text layer) */
#define S3_LAYER_CHOOSE6_HOVER	(48)	/* Choose box 6 (hover, text layer) */
#define S3_LAYER_CHOOSE7_IDLE	(49)	/* Choose box 7 (idle, text layer) */
#define S3_LAYER_CHOOSE7_HOVER	(50)	/* Choose box 7 (hover, text layer) */
#define S3_LAYER_CHOOSE8_IDLE	(51)	/* Choose box 8 (idle, text layer) */
#define S3_LAYER_CHOOSE8_HOVER	(52)	/* Choose box 8 (hover, text layer) */
#define S3_LAYER_CHF		(53)	/* Character "face" */
#define S3_LAYER_CHF_EYE	(54)	/*   Eyes */
#define S3_LAYER_CHF_LIP	(55)	/*   Lips */
#define S3_LAYER_CHF_FO		(56)	/*   Fade-Out */
#define S3_LAYER_CLICK		(57)	/* Click animation */
#define S3_LAYER_AUTO		(58)	/* Auto mode banner */
#define S3_LAYER_SKIP		(59)	/* Skip mode banner */
#define S3_LAYER_TEXT1		(60)	/* Text layer 1 */
#define S3_LAYER_TEXT2		(61)	/* Text layer 2 */
#define S3_LAYER_TEXT3		(62)	/* Text layer 3 */
#define S3_LAYER_TEXT4		(63)	/* Text layer 4 */
#define S3_LAYER_TEXT5		(64)	/* Text layer 5 */
#define S3_LAYER_TEXT6		(65)	/* Text layer 6 */
#define S3_LAYER_TEXT7		(66)	/* Text layer 7 */
#define S3_LAYER_TEXT8		(67)	/* Text layer 8 */
#define S3_STAGE_LAYERS		(68)	/* Total layers */

/*
 * Number of the text layers. (stage.c)
 */
#define S3_TEXT_LAYERS		(8)

/*
 * Number of the effect layers. (stage.c)
 */
#define S3_EFFECT_LAYERS	(4)

/*
 * Maximum number of the click animation frames (stage.c)
 */
#define S3_CLICK_FRAMES		(16)

/*
 * Number of the choose box
 */
#define S3_CHOOSE_COUNT		(8)

/*
 * Character positions. (stage.c)
 */
#define S3_CH_BACK		(0)
#define S3_CH_LEFT		(1)
#define S3_CH_LEFT_CENTER	(2)
#define S3_CH_RIGHT		(3)
#define S3_CH_RIGHT_CENTER	(4)
#define S3_CH_CENTER		(5)
#define S3_CH_FACE		(6)
#define S3_CH_BASIC_LAYERS	(6)	/* Excluding S3_CH_FACE */
#define S3_CH_ALL_LAYERS	(7)	/* Including S3_CH_FACE */

/*
 * Fading methods. (stage.c)
 */
#define S3_FADE_INVALID		(0)
#define S3_FADE_NORMAL		(1)
#define S3_FADE_RULE		(2)
#define S3_FADE_MELT		(3)

/*
 * Blending mode. (stage.c)
 */
#define S3_BLEND_ALPHA		(0)
#define S3_BLEND_ADD		(1)
#define S3_BLEND_SUB		(2)

/*
 * Fading Descriptor Index
 */
#define S3_FADE_DESC_BG		(0)
#define S3_FADE_DESC_CHB	(1)
#define S3_FADE_DESC_CHL	(2)
#define S3_FADE_DESC_CHLC	(3)
#define S3_FADE_DESC_CHR	(4)
#define S3_FADE_DESC_CHRC	(5)
#define S3_FADE_DESC_CHC	(6)
#define S3_FADE_DESC_CHF	(7)
#define S3_FADE_DESC_COUNT	(8)

/*
 * Kira Kira Effect Frames
 */
#define S3_KIRAKIRA_FRAMES	(16)

/*
 * Mixer Tracks
 */
#define S3_MIXER_TRACKS		(4)
#define S3_TRACK_BGM		(0)
#define S3_TRACK_VOICE		(1)
#define S3_TRACK_SE		(2)
#define S3_TRACK_SYS		(3)

/*
 * Character Volume Slots
 */
#define S3_CH_VOL_SLOTS		(33)
#define S3_CH_VOL_SLOT_DEFAULT	(0)

/*
 * Character Map Count
 */
#define S3_CHARACTER_MAP_COUNT	(32)

/*
 * Emoji Count
 */
#define S3_EMOJI_COUNT		(32)

/*
 * Call Arguments for GUI and Anime
 */
#define S3_CALL_ARGS		(32)

/*
 * Fonts
 */
#define S3_FONT_SELECT1		(0)
#define S3_FONT_SELECT2		(1)
#define S3_FONT_SELECT3		(2)
#define S3_FONT_SELECT4		(3)
#define S3_FONT_COUNT		(4)

/*
 * Types
 */

/*
 * Pixel (image.c)
 */

typedef uint32_t s3_pixel_t;

/*
 * Image (image.c)
 */
struct s3_image;

/*
 * Fade Descriptor (stage.c)
 */
struct s3_fade_desc {
	bool stay;
	const char *fname;
	struct s3_image *image;
	int x;
	int y;
	int alpha;
	float scale_x;
	float scale_y;
	int center_x;
	int center_y;
	float rotate;
};

/*
 * Message Drawing
 */

/*
 * Context for message drawing.
 */
struct draw_msg_context;

/*
 * Functions
 */

/*
 * Argument Access (game.c)
 */

/* Check if an argument exists. */
bool s3_check_arg(const char *name);

/* Get an integer argument. */
int s3_get_arg_int(const char *name);

/* Get a float argument. */
float s3_get_arg_float(const char *name);

/* Get a string argument. */
const char *s3_get_arg_string(const char *name);

/*
 * Input State (game.c)
 */

/* Get the mouse X position. */
int s3_get_mouse_pos_x(void);

/* Get the mouse Y position. */
int s3_get_mouse_pos_y(void);

/* Check if mouse left button is pressed. */
bool s3_is_mouse_left_pressed(void);

/* Check if mouse right button is pressed. */
bool s3_is_mouse_right_pressed(void);

/* Check if mouse left button is pressed then released. */
bool s3_is_mouse_left_clicked(void);

/* Check if mouse right button is pressed then released. */
bool s3_is_mouse_right_clicked(void);

/* Check if mouse is dragging. */
bool s3_is_mouse_dragging(void);

/* Check if return key is pressed. */
bool s3_is_return_key_pressed(void);

/* Check if space key is pressed. */
bool s3_is_space_key_pressed(void);

/* Check if escape key is pressed. */
bool s3_is_escape_key_pressed(void);

/* Check if up key is pressed. */
bool s3_is_up_key_pressed(void);

/* Check if down key is pressed. */
bool s3_is_down_key_pressed(void);

/* Check if left key is pressed. */
bool s3_is_left_key_pressed(void);

/* Check if right key is pressed. */
bool s3_is_right_key_pressed(void);

/* Check if pageup key is pressed. */
bool s3_is_pageup_key_pressed(void);

/* Check if pagedown key is pressed. */
bool s3_is_pagedown_key_pressed(void);

/* Check if control key is pressed. */
bool s3_is_control_key_pressed(void);

/* Check if S key is pressed. */
bool s3_is_s_key_pressed(void);

/* Check if L key is pressed. */
bool s3_is_l_key_pressed(void);

/* Check if H key is pressed. */
bool s3_is_h_key_pressed(void);

/* Check if touch is canceled. */
bool s3_is_touch_canceled(void);

/* Check if swiped. */
bool s3_is_swiped(void);

/* Clear input states to avoid further input processing in the current frame. */
void s3_clear_input_state(void);

/*
 * Multiple-Frame Command State (game.c)
 */

/* Start a multiple-frame command execution. */
void s3_start_command_repetition(void);

/* Stop a multiple-frame command execution. */
void s3_stop_command_repetition(void);

/* Check whether we are in a multiple-frame command execution or not. */
bool s3_is_in_command_repetition(void);

/*
 * Message Showing State (game.c)
 *  - Set active when a message shown.
 *  - Keep active when move to a GUI.
 *  - Cleared when move to a next command or loaded a save file.
 */

/* Set the message showing state. */
void s3_set_message_active(void);

/* Reset the message showing state. */
void s3_clear_message_active(void);

/* Check whether the message showing state is set or not. */
bool s3_is_message_active(void);

/*
 * Auto-Mode State (game.c)
 */

/* Start the auto-mode. */
void s3_start_auto_mode(void);

/* Stop the auto-mode. */
void s3_stop_auto_mode(void);

/* Check whether we are in the auto-mode or not. */
bool s3_is_auto_mode(void);

/*
 * Skip-Mode State (game.c)
 */

/* Start the skip-mode. */
void s3_start_skip_mode(void);

/* Stop the skip-mode. */
void s3_stop_skip_mode(void);

/* Check whether we are in the skip-mode or not. */
bool s3_is_skip_mode(void);

/*
 * Save/Load Enable State (game.c)
 */

/* Set the save/load enable setting. */
void s3_set_save_load(bool enable);

/* Get the save/load enable setting. */
bool s3_is_save_load_enabled(void);

/*
 * Non-Interruptible State (game.c)
 */

/* Set the non-interruptible mode setting. */
void s3_set_non_interruptible(bool mode);

/* Get the non-interruptible mode setting. */
bool s3_is_non_interruptible(void);

/*
 * Pen State (game.c)
 */

/* Set the pen position. */
void s3_set_pen_position(int x, int y);

/* Get the pen X position. */
int s3_get_pen_position_x(void);

/* Get the pen Y position. */
int s3_get_pen_position_y(void);

/*
 * Call Arguments (game.c)
 */

/* Set a calling argument. */
bool s3_set_call_argument(int index, const char *val);

/* Get a calling argument. */
const char *s3_get_call_argument(int index);

/*
 * Page Mode (game.c)
 */

/* Set the script page mode. */
bool s3_is_page_mode(void);

/* Append a string to the page mode buffer string. */
bool s3_append_buffered_message(const char *msg);

/* Get the page mode buffer string. */
const char *s3_get_buffered_message(void);

/* Clear the page mode buffer string. */
void s3_clear_buffered_message(void);

/* Reset the message line count in a page. */
void s3_reset_page_line(void);

/* Increment the line count in a page. */
void s3_inc_page_line(void);

/* Check we are at the first line in a page. */
bool s3_is_page_top(void);

/*
 * BGVoice (game.c)
 */

/* Register a BGVoice. */
bool s3_register_bgvoice(const char *file);

/* Get the BGVoice. */
const char *s3_get_bgvoice(void);

/* Set the BGVoice state playing. */
void s3_set_bgvoice_playing(bool is_playing);

/* Check whether the BGVoice is playing or not. */
bool s3_is_bgvoice_playing(void);

/*
 * Image Management (image.c)
 */

/* Load an image from a file. */
struct s3_image *
s3_create_image_from_file(
	const char *file);

/* Create an image. */
struct s3_image *
s3_create_image(int width,
		int height);

/* Load a glyph image. */
struct s3_image *
s3_load_glyph_image(
	int slot,
	int codepoint,
	int size,
	s3_pixel_t color,
	int outline_width,
	s3_pixel_t outline_color);

/* Destroy an image. */
void s3_destroy_image(
	struct s3_image *image);

/* Copy an image to an image. */
void s3_draw_image_copy(
	struct s3_image *dst,
	int dst_left,
	int dst_top,
	struct s3_image *src,
	int dst_width,
	int dst_height,
	int src_left,
	int src_top);

/* Draw an image to an image with alpha blending. */
void s3_draw_image_alpha(
	struct s3_image *dst,
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	struct s3_image *src,
	int src_left,
	int src_top,
	int alpha);

/* Draw an image to an image with add blending. */
void s3_draw_image_add(
	struct s3_image *dst,
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	struct s3_image *src,
	int src_left,
	int src_top,
	int alpha);

/* Draw an image to an image with sub blending. */
void s3_draw_image_sub(
	struct s3_image *dst,
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	struct s3_image *src,
	int src_left,
	int src_top,
	int alpha);

/* Draw an image to an image with sub blending. */
void s3_draw_image_dim(
	struct s3_image *dst,
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	struct s3_image *src,
	int src_left,
	int src_top,
	int alpha);

/* Draw a glyph image to an image. */
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
	int alpha);

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
	int alpha);

/* Draw an image with scaling. */
void
s3_draw_image_scale(
	struct s3_image *dst,
	int virtual_dst_width,
	int virtual_dst_height,
	int virtual_dst_left,
	int virtual_dst_top,
	struct s3_image *src);

/* Make a pixel value. */
s3_pixel_t
s3_make_pixel(
	int r,
	int g,
	int b,
	int a);

/* Fill a rectangle on an image */
void
s3_fill_image_rect(
	struct s3_image *image,
	int left,
	int top,
	int width,
	int height,
	s3_pixel_t color);

/*
 * Stage Layer Management (stage.c)
 */

/* Reload the stage images by the config. */
bool s3_reload_stage_images(void);

/* Reload the stage positions by the config. */
void s3_reload_stage_positions(void);

/* Get the layer X position. */
int s3_get_layer_x(int layer);

/* Get the layer Y position. */
int s3_get_layer_y(int layer);

/* Set the layer position. */
void s3_set_layer_position(int layer, int x, int y);

/* Set the layer scale. */
void s3_set_layer_scale(int layer, float scale_x, float scale_y);

/* Get the layer image width. */
int s3_get_layer_width(int layer);

/* Get the layer image height. */
int s3_get_layer_height(int layer);

/* Get the layer alpha. */
int s3_get_layer_alpha(int layer);

/* Set the layer alpha. */
void s3_set_layer_alpha(int layer, int alpha);

/* Set the layer belnd mode. */
void s3_set_layer_blend(int layer, int blend);

/* Set the layer center coordinate. */
void s3_set_layer_center(int layer, int x, int y);

/* Set the layer rotation. */
void s3_set_layer_rotate(int layer, float rad);

/* Get the layer file name. */
const char *s3_get_layer_file_name(int layer);

/* Set the layer file name. */
bool s3_set_layer_file_name(int layer, const char *file_name);

/* Get the layer image for a glyph drawing. */
struct s3_image *s3_get_layer_image(int layer);

/* Set the layer image for a load.*/
void s3_set_layer_image(int layer, struct s3_image *img);

/* Set the layer frame for eye blinking and lip synchronization. */
void s3_set_layer_frame(int layer, int frame);

/* Get the text layer string. */
const char *
s3_get_layer_text(
	int text_layer_index);

/* Set the text layer string. */
bool
s3_set_layer_text(
	int textlayer_index,
	const char *msg);

/* Get the sysbtn idle image. */
struct s3_image *
s3_get_sysbtn_idle_image(void);

/* Get the sysbtn hover image. */
struct s3_image *
s3_get_sysbtn_hover_image(void);

/* Clear the basic layers. */
void s3_clear_stage_basic(void);

/* Clear the stage and make it initial state. */
void s3_clear_stage(void);

/*
 * Conversion of Layer Index and Character Position (stage.c)
 */

/* Convert a character position to a stage layer index. */
int s3_chpos_to_layer(int chpos);

/* Convert a character position to a stage layer index (character eye). */
int s3_chpos_to_eye_layer(int chpos);

/* Convert a character position to a stage layer index (character lip). */
int s3_chpos_to_lip_layer(int chpos);

/* Convert a stage layer index to a character position. */
int s3_layer_to_chpos(int chpos);

/*
 * Stage Rendering (stage.c)
 */

/* Render the stage with all stage layers. */
void s3_render_stage(void);

/*
 * Save Data Thumbnail Drawing (stage.c)
 */

/* Draw entire stage into the save data thumbnail. */
void s3_draw_stage_to_thumb(void);

/* Get the image of the save data thumbnail */
struct s3_image *get_thumb_image(void);

/*
 * Fading Control (stage.c)
 */

/* Get a fading method from a string. */
int
s3_get_fade_method(
	const char *method);

/* Start a fading. */
bool s3_start_fade(
	struct s3_fade_desc *desc,
	int method,
	struct s3_image *rule_img);

/* Set the fading progress. */
void s3_set_fade_progress(float progress);

/* Set the offset for the shake command. */
void s3_set_shake_offset(int x, int y);

/* End the fading. */
void s3_finish_fade(void);

/*
 * Character Control (stage.c)
 */

/* Specify a character index for a character position. */
void s3_set_ch_name_mapping(int chpos, int ch_name_index);

/* Set the talker character name index. (-1 for no speaker) */
void s3_set_ch_talking(int ch_name_index);

/* Get the talker character position. */
int s3_get_talking_chpos(void);

/* Update the character dimming automatically. */
void s3_update_ch_dim_by_talking_ch(void);

/* Update the character dimming manually. */
void s3_force_ch_dim(int chpos, bool is_dim);

/*
 * Name Box Control
 */

/* Fill the name box by the name box image. */
void s3_fill_namebox(void);

/* Show or hides the name box. */
void s3_show_namebox(bool show);

/*
 * Message Box Control
 */

/* Fill the message box by the message box image. */
void s3_fill_msgbox(void);

/* Show or hides the message box. */
void s3_show_msgbox(bool show);

/*
 * Click Animation Control
 */

/* Set the position of the click animation. */
void s3_set_click_position(int x, int y);

/* Show or hides the click animation. */
void s3_show_click(bool show);

/* Set the click animation frame index. */
void s3_set_click_index(int index);

/*
 * Choose Box Control
 */

/* Fill the choose box by the choose box bg image. */
void s3_fill_cbox_idle_image(int index);

/* Fill the choose box by the choose box fg image. */
void s3_fill_cbox_hover_image(int index);

/*
 * Banner Control
 */

/* Show or hides the auto mode banner. */
void s3_show_automode_banner(bool show);

/* Show or hides the skip mode banner. */
void s3_show_skipmode_banner(bool show);

/*
 * GUI Rendering (stage.c)
 */

/* Render an image. */
void
s3_render_image(
	int dst_left,
	int dst_top,
	int dst_width,
	int dst_height,
	struct s3_image *image,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha);

/* Render a image with free transform. */
void
s3_render_image_3d(
	float x1,
	float y1,
	float x2,
	float y2,
	float x3,
	float y3,
	float x4,
	float y4,
	struct s3_image *image,
	int src_left,
	int src_top,
	int src_width,
	int src_height,
	int alpha);

/*
 * System Button (sysbtn.c)
 */

/* Show or hide the system button. */
bool
s3_show_sysbtn(
	bool is_visible);

/* Check if the system button is visible. */
bool
s3_is_sysbtn_visible(void);

/* Update the system button's pointed state by the mouse position. */
void
s3_update_sysbtn_state(void);

/* Check if the system bytton is pointed. */
bool
s3_is_sysbtn_pointed(void);

/* Check if the system bytton is clicked. */
bool
s3_is_sysbtn_clicked(void);

/*
 * Glyph (glyph.c)
 */

/* Convert the top character of a utf-8 string to utf-32. */
int
s3_utf8_to_utf32(
	const char *mbs,
	uint32_t *wc);

/* Count the characters of a utf-8 string. */
int
s3_count_utf8_chars(
	const char *mbs);

/* Get the glyph width. */
int
s3_get_glyph_width(
	int font_type,
	int font_size,
	uint32_t codepoint);

/* Get the glyph height. */
int
s3_get_glyph_height(
	int font_type,
	int font_size,
	uint32_t codepoint);

/* Get the string width. */
int
s3_get_string_width(
	int font_type,
	int font_size,
	const char *mbs);

/* Get the string height. */
int
s3_get_string_height(
	int font_type,
	int font_size,
	const char *mbs);

/* Draw a glyph. */
bool
s3_draw_glyph(
	struct s3_image *img,
	int font_type,
	int font_size,
	int base_font_size,
	int outline_size,
	int x,
	int y,
	s3_pixel_t color,
	s3_pixel_t outline_color,
	uint32_t codepoint,
	int *ret_w,
	int *ret_h,
	bool is_dim);

/*
 * Message Drawing (glyph.c)
 */

/* Initialize a message drawing context. */
void
s3_construct_draw_msg_context(
	struct draw_msg_context *context,
	struct s3_image *dst,
	const char *msg,
	int font,
	int font_size,
	int base_font_size,
	int ruby_size,
	int outline_size,
	int pen_x,
	int pen_y,
	int area_width,
	int area_height,
	int left_margin,
	int right_margin,
	int top_margin,
	int bottom_margin,
	int line_margin,
	int char_margin,
	s3_pixel_t color,
	s3_pixel_t outline_color,
	s3_pixel_t bg_color,
	bool fill_bg,
	bool is_dimming,
	bool ignore_linefeed,
	bool ignore_font,
	bool ignore_outline,
	bool ignore_color,
	bool ignore_size,
	bool ignore_position,
	bool ignore_ruby,
	bool ignore_wait,
	void (*inline_wait_hook)(float),
	bool use_tategaki);

/* Count the remaining characters excluding escape sequences. */
int
s3_count_chars_common(
	struct draw_msg_context *context, int *width);

/* Draw characters in a message up to (max_chars) characters. */
int
s3_draw_msg_common(
	struct draw_msg_context *context,
	int max_chars);

/* Get a pen position. */
void
s3_get_pen_position_common(
	struct draw_msg_context *context,
	int *pen_x,
	int *pen_y);

/* Check if a message is a quoted serif. (TODO: delete) */
bool
s3_is_quoted_serif(
	const char *msg);

/*
 * Mixer (mixer.c)
 */

/* Set the BGM file name. */
bool s3_set_bgm_file_name(const char *file);

/* Get the BGM file name. */
const char *s3_get_bgm_file_name(void);

/* Set the SE file name. */
bool s3_set_se_file_name(const char *file);

/* Get the SE file name. (only when loopback-playing) */
const char *s3_get_se_file_name(void);

/* Play a sound file on a mixer track. */
void s3_set_mixer_input_file(int track, const char *file);

/* Set the volume for a mixer track. */
void s3_set_mixer_volume(int track, float vol, float span);

/* Get the volume for a mixer track. */
float s3_get_mixer_volume(int n);

/* Set the master volume. */
void s3_set_master_volume(float vol);

/* Get the master volume. */
float s3_get_master_volume(void);

/* Set the global volume for a track. */
void s3_set_mixer_global_volume(int track, float vol);

/* Get the global volume for a track. */
float s3_get_mixer_global_volume(int track);

/* Set the character volume. */
void s3_set_character_volume(int ch_index, float vol);

/* Get the character volume. */
float s3_get_character_volume(int ch_index);

/* Check if the track playback is finished. */
bool s3_is_mixer_sound_finished(int track);

/*
 * Tag File (tag.c)
 */

/* Move to a tag file. */
bool s3_move_to_tag_file(const char *file);

/* Move to a next tag. */
bool s3_move_to_next_tag(void);

/* Move to a label. */
bool s3_move_to_label(const char *label);

/*
 * Logging (game.c)
 */

/*
 * Print a debug message.
 */
void
s3_log_info(
	const char *msg,
	...);

/*
 * Print a warning message.
 */
void
s3_log_warn(
	const char *msg,
	...);

/*
 * Print an error message.
 */
void
s3_log_error(
	const char *msg,
	...);

/*
 * Print an out-of-memory error message.
 */
void
s3_log_out_of_memory(void);

#endif
