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
 * Stage layers. (stage.c)
 */
#define S3_LAYER_BG		(0)	/* Background */
#define S3_LAYER_BG2		(1)	/* Background2 */
#define S3_LAYER_EFB1		(2)	/* Back effect 1 */
#define S3_LAYER_EFB2		(3)	/* Back effect 2 */
#define S3_LAYER_EFB3		(4)	/* Back effect 3 */
#define S3_LAYER_EFB4		(5)	/* Back effect 4 */
#define S3_LAYER_CHB		(6)	/* Character "back" */
#define S3_LAYER_CHB_EYE	(7)	/*   Eyes */
#define S3_LAYER_CHB_LIP	(8)	/*   Lips */
#define S3_LAYER_CHL		(9)	/* Character "left" */
#define S3_LAYER_CHL_EYE	(10)	/*   Eyes */
#define S3_LAYER_CHL_LIP	(11)	/*   Lips */
#define S3_LAYER_CHLC		(12)	/* Character "left-center" */
#define S3_LAYER_CHLC_EYE	(13)	/*   Eyes */
#define S3_LAYER_CHLC_LIP	(14)	/*   Lips */
#define S3_LAYER_CHR		(15)	/* Character "right" */
#define S3_LAYER_CHR_EYE	(16)	/*   Eyes */
#define S3_LAYER_CHR_LIP	(17)	/*   Lips */
#define S3_LAYER_CHRC		(18)	/* Character "right-center" */
#define S3_LAYER_CHRC_EYE	(19)	/*   Eyes */
#define S3_LAYER_CHRC_LIP	(20)	/*   Lips */
#define S3_LAYER_CHC		(21)	/* Character "right-center" */
#define S3_LAYER_CHC_EYE	(22)	/*   Eyes */
#define S3_LAYER_CHC_LIP	(23)	/*   Lips */
#define S3_LAYER_EFF1		(24)	/* Front effect 1 */
#define S3_LAYER_EFF2		(25)	/* Front effect 2 */
#define S3_LAYER_EFF3		(26)	/* Front effect 3 */
#define S3_LAYER_EFF4		(27)	/* Front effect 4 */
#define S3_LAYER_MSGBOX		(28)	/* Message box (text layer) */
#define S3_LAYER_NAMEBOX	(29)	/* Name box (text layer) */
#define S3_LAYER_CHOOSE1	(30)	/* Choose box 1 (text layer) */
#define S3_LAYER_CHOOSE2	(31)	/* Choose box 2 (text layer) */
#define S3_LAYER_CHOOSE3	(32)	/* Choose box 3 (text layer) */
#define S3_LAYER_CHOOSE4	(33)	/* Choose box 4 (text layer) */
#define S3_LAYER_CHOOSE5	(34)	/* Choose box 5 (text layer) */
#define S3_LAYER_CHOOSE6	(35)	/* Choose box 6 (text layer) */
#define S3_LAYER_CHOOSE7	(36)	/* Choose box 7 (text layer) */
#define S3_LAYER_CHOOSE8	(37)	/* Choose box 8 (text layer) */
#define S3_LAYER_CHF		(38)	/* Character "face" */
#define S3_LAYER_CHF_EYE	(39)	/*   Eyes */
#define S3_LAYER_CHF_LIP	(40)	/*   Lips */
#define S3_LAYER_CLICK		(41)	/* Click animation */
#define S3_LAYER_AUTO		(42)	/* Auto mode banner */
#define S3_LAYER_SKIP		(43)	/* Skip mode banner */
#define S3_LAYER_TEXT1		(44)	/* Text layer 1 */
#define S3_LAYER_TEXT2		(44)	/* Text layer 2 */
#define S3_LAYER_TEXT3		(44)	/* Text layer 3 */
#define S3_LAYER_TEXT4		(44)	/* Text layer 4 */
#define S3_LAYER_TEXT5		(44)	/* Text layer 5 */
#define S3_LAYER_TEXT6		(44)	/* Text layer 6 */
#define S3_LAYER_TEXT7		(44)	/* Text layer 7 */
#define S3_LAYER_TEXT8		(44)	/* Text layer 8 */
#define S3_STAGE_LAYERS		(45)	/* Total layers */

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
#define S3_CHOOSE_BOXES		(8)

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
#define S3_BLEND_NORMAL		(0)
#define S3_BLEND_ADD		(1)

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
#define CH_VOL_SLOTS		(16)
#define CH_VOL_SLOT_DEFAULT	(0)

/*
 * Types
 */

/*
 * Image
 */
struct s3_image;

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
bool s3_is_left_button_pressed(void);

/* Check if mouse right button is pressed. */
bool s3_is_right_button_pressed(void);

/* Check if mouse left button is pressed then released. */
bool s3_is_left_clicked(void);

/* Check if mouse right button is pressed then released. */
bool s3_is_right_clicked(void);

/* Check if mouse is dragging. */
bool s3_is_mouse_dragging(void);

/* Check if return key is pressed. */
bool s3_is_return_pressed(void);

/* Check if space key is pressed. */
bool s3_is_space_pressed(void);

/* Check if escape key is pressed. */
bool s3_is_escape_pressed(void);

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

struct s3_image *s3_create_image_from_file(const char *file);
struct s3_image *s3_create_image(int width, int height);
void s3_destroy_image(struct s3_image *image);

/*
 * Stage Layer Management (stage.c)
 */

/* Reload the stage images by the config. */
bool s3_reload_stage_images(void);

/* Reload the stage positions by the config. */
void s3_reload_stage_positions(void);

/* Gets the layer X position. */
int s3_get_layer_x(int layer);

/* Gets the layer Y position. */
int s3_get_layer_y(int layer);

/* Sets the layer position. */
void s3_set_layer_position(int layer, int x, int y);

/* Sets the layer scale. */
void s3_set_layer_scale(int layer, float scale_x, float scale_y);

/* Gets the layer image width. */
int s3_get_layer_width(int layer);

/* Gets the layer image height. */
int s3_get_layer_height(int layer);

/* Gets the layer alpha. */
int s3_get_layer_alpha(int layer);

/* Sets the layer alpha. */
void s3_set_layer_alpha(int layer, int alpha);

/* Sets the layer belnd mode. */
void s3_set_layer_blend(int layer, int blend);

/* Sets the layer center coordinate. */
void s3_set_layer_center(int layer, int x, int y);

/* Sets the layer rotation. */
void s3_set_layer_rotate(int layer, float rad);

/* Gets the layer file name. */
const char *s3_get_layer_file_name(int layer);

/* Sets the layer file name. */
bool s3_set_layer_file_name(int layer, const char *file_name);

/* Gets the layer image for a glyph drawing. */
int s3_get_layer_image(int layer);

/* Sets the layer image for a load.*/
void s3_set_layer_image(int layer, struct image *img);

/* Sets the layer frame for eye blinking and lip synchronization. */
void s3_set_layer_frame(int layer, int frame);

/* Clears the basic layers. */
void s3_clear_stage_basic(void);

/* Clears the stage and make it initial state. */
void s3_clear_stage(void);

/*
 * Conversion of Layer Index and Character Position (stage.c)
 */

/* Converts a character position to a stage layer index. */
int s3_chpos_to_layer(int chpos);

/* Converts a character position to a stage layer index (character eye). */
int s3_chpos_to_eye_layer(int chpos);

/* Converts a character position to a stage layer index (character lip). */
int s3_chpos_to_lip_layer(int chpos);

/* Converts a stage layer index to a character position. */
int s3_layer_to_chpos(int chpos);

/*
 * Stage Rendering (stage.c)
 */

/* Renders the stage with all stage layers. */
void s3_render_stage(void);

/*
 * Save Data Thumbnail Drawing (stage.c)
 */

/* Draws entire stage into the save data thumbnail. */
void s3_draw_stage_to_thumb(void);

/* Get the image of the save data thumbnail */
int get_thumb_image(void);

/*
 * Fading Control (stage.c)
 */

/* Gets a fading method from a string. */
int s3_get_fade_method(const char *method);

/* Starts a fading for the bg command. */
bool s3_start_fade_for_bg(const char *fname, struct image *img, int x, int y,
			  int alpha, int method, struct image *rule_img);

/* Starts a fading for the ch command. */
bool s3_start_fade_for_ch(int chpos, const char *fname, struct image *img,
			  int x, int y, int alpha, int method,
			  struct image *rule_img);

/* Starts a fading for the all command. */
bool s3_start_fade_for_chs(const bool *stay, const char **fname,
			   struct image **img, const int *x, const int *y,
			   const int *alpha, int method, struct image *rule_img);

/* Starts a fading for the cl command. */
bool s3_start_fade_for_ciel(const bool *stay, const char **fname,
			    struct image **img, const int *x, const int *y,
			    const int *alpha, int method, struct image *rule_img);

/* Starts a fading for the shake command. */
void s3_start_fade_for_shake(void);

/* Sets the fading progress. */
void s3_set_fade_progress(float progress);

/* Sets the offset for the shake command. */
void s3_set_shake_offset(int x, int y);

/* Ends the fading. */
void s3_finish_fade(void);

/*
 * Character Change (stage.c)
 */

/* Specifies a character index for a character position. */
void s3_set_ch_name_mapping(int chpos, int ch_name_index);

/* Sets the talker character name index. */
void s3_set_ch_talking(int ch_name_index);

/* Gets the talker character position. */
int s3_get_talking_chpos(void);

/* Updates the character dimming automatically. */
void s3_update_ch_dim_by_talking_ch(void);

/* Updates the character dimming manually. */
void s3_force_ch_dim(int chpos, bool is_dim);

/*
 * Name Box Control
 */

/* Fills the name box by the name box image. */
void s3_fill_namebox(void);

/* Shows or hides the name box. */
void s3_show_namebox(bool show);

/*
 * Message Box Control
 */

/* Fills the message box by the message box image. */
void s3_fill_msgbox(void);

/* Shows or hides the message box. */
void s3_show_msgbox(bool show);

/*
 * Click Animation Control
 */

/* Sets the position of the click animation. */
void s3_set_click_position(int x, int y);

/* Shows or hides the click animation. */
void s3_show_click(bool show);

/*
 * Choose Box Control
 */

/* Fills the choose box by the choose box bg image. */
void s3_fill_choose_bg_image(int index);

/* Fills the choose box by the choose box fg image. */
void s3_fill_choose_fg_image(int index);

/*
 * Character Drawing
 */

/* Draws a character on a layer. */
bool s3_draw_char_on_layer(int layer, int x, int y, uint32_t wc, pixel_t color,
			   pixel_t outline_color, int base_font_size,
			   bool is_dimming, int *ret_width, int *ret_height,
			   int *union_x, int *union_y, int *union_w, int *union_h);

/*
 * Banner Control
 */

/* Shows or hides the auto mode banner. */
void s3_show_automode_banner(bool show);

/* Shows or hides the skip mode banner. */
void s3_show_skipmode_banner(bool show);

/*
 * GUI Rendering
 */

/* Render an image. */
void s3_render_image(int dst_left, int dst_top, int dst_width, int dst_height,
		     int image, int src_x, int src_y, eint src_width,
		     int src_height, int alpha);

/* Render a texture. */
void s3_render_image_3d(float x1, float y1, float x2, float y2,
			float x3, float y3, efloat x4, float y4,
			int image, int src_left, int src_top, int src_width,
			int src_height, int alpha);

/*
 * Text Layers Control (stage.c)
 */

/* Gets the text layer string. */
const char *s3_get_layer_text(int text_layer_index);

/* Sets the text layer string. */
bool s3_set_layer_text(int textlayer_index, const char *msg);

/*
 * Mixer (mixer.c)
 */

/* Sets the BGM file name. */
bool s3_set_bgm_file_name(const char *file);

/* Gets the BGM file name. */
const char *s3_get_bgm_file_name(void);

/* Sets the SE file name. */
bool s3_set_se_file_name(const char *file);

/* Gets the SE file name. (only when loopback-playing) */
const char *s3_get_se_file_name(void);

/* Plays a sound file on a mixer track. */
void s3_set_mixer_input_file(int track, const char *file);

/* Sets the volume for a mixer track. */
void s3_set_mixer_volume(int track, float vol, float span);

/* Gets the volume for a mixer track. */
float s3_get_mixer_volume(int n);

/* Sets the master volume. */
void s3_set_master_volume(float vol);

/* Gets the master volume. */
float s3_get_master_volume(void);

/* Sets the track volume. */
void s3_set_mixer_track_volume(int track, float vol);

/* Gets the track volume. */
float s3_get_mixer_global_volume(int track);

/* Sets the character volume. */
void s3_set_character_volume(int ch_index, float vol);

/* Gets the character volume. */
float s3_get_character_volume(int ch_index);

/* Check if the track playback is finished. */
bool s3_is_mixer_sound_finished(int track);

#endif
