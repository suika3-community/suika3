/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (c) 2001-2026 The Suika3 Authors
 */

#include <suika3/suika3.h>

/*
 * False Assertions
 */
#define BAD_POSITION		(0)
#define INVALID_FADE_METHOD	(0)

/*
 * Stage Operation Mode
 */

static int stage_mode;

enum stage_mode {
	STAGE_MODE_IDLE,
	STAGE_MODE_BG_FADE,
	STAGE_MODE_CH_FADE,
	STAGE_MODE_CHS_FADE,
	STAGE_MODE_CIEL_FADE,
	STAGE_MODE_SHAKE_FADE,
};

/*
 * Stage Images
 */

/* Layer images. */
static struct s3_image *layer_image[S3_STAGE_LAYERS];

/* Image for the message box background. */
static struct s3_image *msgbox_image;

/* Image for the name box background. */
static struct s3_image *namebox_image;

/* Images for the click animation. */
static struct s3_image *click_image[CLICK_FRAMES];

/* Image for the choose box (idle). */
static struct s3_image *choose_idle_image[S3_CHOOSE_BOXES];

/* Image for the choose box (hover). */
static struct s3_image *choose_hover_image[S3_CHOOSE_BOXES];

/* Image for the system button (idle). */
static struct s3_image *sysbtn_idle_image;

/* Image for the system button (hover). */
static struct s3_image *sysbtn_hover_image;

/* Image for the save slot "NEW". */
static struct s3_image *savenew_image;

/* Image for the save data thumbnail. */
static struct s3_image *thumb_image;

/* Rule image. */
static struct s3_image *fade_rule_img;

/* Images for Kira Kira Effect. */
static struct s3_image *kirakira_image[KIRAKIRA_FRAME_COUNT];

/*
 * Layer Visibility
 */

/* Whether to show the message box. */
static bool is_msgbox_visible;

/* Whether to show the name box. */
static bool is_namebox_visible;

/* Whether to show the click animation. */
static bool is_click_visible;

/* Whether to show the auto mode bannder. */
static bool is_auto_visible;

/* Whether to show the skip mode banner. */
static bool is_skip_visible;

/*
 * Layer Properties
 */

/* X positions for the layers. */
static int layer_x[S3_STAGE_LAYERS];

/* Y positions for the layers. */
static int layer_y[S3_STAGE_LAYERS];

/* Alpha values for the layers. */
static int layer_alpha[S3_STAGE_LAYERS];

/* Blending modes for the layers. */
static int layer_blend[S3_STAGE_LAYERS];

/* X scales for the layers. */
static float layer_scale_x[S3_STAGE_LAYERS];

/* Y scales for the layers. */
static float layer_scale_y[S3_STAGE_LAYERS];

/* X centers for the layers. */
static float layer_center_x[S3_STAGE_LAYERS];

/* Y centers for the layers. */
static float layer_center_y[S3_STAGE_LAYERS];

/* Rotations for the layers in radian. */
static float layer_rotate[S3_STAGE_LAYERS];

/* File names for the layers. */
static char *layer_file_name[S3_STAGE_LAYERS];

/* Frame index for the eye and lips layers. */
static int layer_frame[S3_STAGE_LAYERS];

/*
 * Text Layers
 */

/* Strings for the text layers.  */
static char *layer_text[S3_STAGE_LAYERS];

/*
 * Non-Speaker Dimming
 */

/* Whether to dim characters. */
static bool ch_dim[CH_BASIC_LAYERS];

/* Character name indices. */
static int ch_name_mapping[CH_BASIC_LAYERS] = {-1, -1, -1, -1, -1, -1};

/* Speaker character name index. */
static int ch_talking = -1;

/*
 * Fading Mode
 */

/* Fading method. */
static int fade_method;

/* Fading progress. */
static float fade_progress;

/* Offsets for the shake command. */
static int shake_offset_x;
static int shake_offset_y;

/*
 * Kira Kira Effect
 */

/* Position to show Kira Kira Effect. */
static int kirakira_x;
static int kirakira_y;

/* Start time of the last Kira Kira Effect. */
static uint64_t sw_kirakira;

/*
 * Forward Declarations
 */
static bool setup_namebox(void);
static bool setup_msgbox(void);
static bool setup_click(void);
static bool setup_choose(bool no_bg, bool no_fg, int index);
static bool setup_sysbtn(void);
static bool setup_banners(void);
static bool setup_kirakira(void);
static bool setup_savenew(void);
static bool setup_thumb(void);
static void restore_text_layers(void);
static bool create_fade_layer_images(void);
static void destroy_layer_image(int layer);
static void draw_fo_common(void);
static void draw_fi_common(bool show_msgbox);
static void render_fade_normal(void);
static void render_fade_rule(void);
static void render_fade_melt(void);
static void render_fade_curtain_right(void);
static void render_fade_curtain_left(void);
static void render_fade_curtain_up(void);
static void render_fade_curtain_down(void);
static void render_fade_slide_right(void);
static void render_fade_slide_left(void);
static void render_fade_slide_up(void);
static void render_fade_slide_down(void);
static void render_fade_shutter_right(void);
static void render_fade_shutter_left(void);
static void render_fade_shutter_up(void);
static void render_fade_shutter_down(void);
static void render_fade_eye_open(void);
static void render_fade_eye_close(void);
static void render_fade_eye_open_v(void);
static void render_fade_eye_close_v(void);
static void render_fade_slit_open(void);
static void render_fade_slit_close(void);
static void render_fade_slit_open_v(void);
static void render_fade_slit_close_v(void);
static void render_fade_shake(void);
static void render_layer_image(int layer);
static void draw_layer_image(struct image *target, int layer);

/*
 * Initialization
 */

/*
 * Initialize the stage subsystem.
 */
bool init_stage(void)
{
	int i;

	/* Cleanup for when our DLL is reused. */
	cleanup_stage();

	/* Load the stage images. */
	if (!s3_reload_stage_images())
		return false;

	/* Setup the image for the save data thumbnail. */
	if (!setup_thumb())
		return false;

	/* Create the image for our temporary background. */
	layer_image[S3_LAYER_BG] = create_initial_bg();
	if (layer_image[S3_LAYER_BG] == NULL)
		return false;

	/* Set the initial values. */
	for (i = 0; i < S3_STAGE_LAYERS; i++) {
		layer_scale_x[i] = 1.0f;
		layer_scale_y[i] = 1.0f;
		layer_alpha[i] = 255;
	}

	return true;
}

/*
 * Reload the stage images by the config.
 */
bool s3_reload_stage_images(void)
{
	/* Setup the name box. */
	if (!setup_namebox())
		return false;

	/* Setup the message box. */
	if (!setup_msgbox())
		return false;

	/* Setup the click animation. */
	if (!setup_click())
		return false;

	/* Setup the choose boxes. */
	if (!setup_choose(false, false, -1))
		return false;

	/* Setup the system button. */
	if (!setup_sysbtn())
		return false;

	/* Setup the auto and skip modes banners. */
	if (!setup_banners())
		return false;

	/* Setup Kira Kira Effect. */
	if (!setup_kirakira())
		return false;

	/* Setup the "NEW" image for the save lots. */
	if (!setup_savenew())
		return false;

	/* Restore the text layers. */
	restore_text_layers();

	return true;
}

/* Setup the name box.*/
static bool setup_namebox(void)
{
	is_namebox_visible = false;

	/* Destroy when reinitialized. */
	if (namebox_image != NULL) {
		s3_destroy_image(namebox_image);
		namebox_image = NULL;
	}
	if (layer_image[LAYER_NAME] != NULL) {
		s3_destroy_image(layer_image[LAYER_NAME]);
		layer_image[LAYER_NAME] = NULL;
	}

	/* Load the name box image. */
	namebox_image = s3_create_image_from_file(conf_namebox_image);
	if (namebox_image == NULL)
		return false;

	/* Create a name box layer image. */
	layer_image[LAYER_NAME] = s3_create_image(namebox_image->width, namebox_image->height);
	if (layer_image[LAYER_NAME] == NULL)
		return false;

	/* Layout the name box layer. */
	layer_x[LAYER_NAME] = conf_namebox_x;
	layer_y[LAYER_NAME] = conf_namebox_y;

	/* Transfer the name box image to the name box layer. */
	fill_namebox();

	return true;
}

/* Setup the message box. */
static bool setup_msgbox(void)
{
	is_msgbox_visible = false;

	/* Destroy when reinitialized. */
	if (msgbox_image != NULL) {
		s3_destroy_image(msgbox_image);
		msgbox_image = NULL;
	}
	if (msgbox_hover_image != NULL) {
		s3_destroy_image(msgbox_hover_image);
		msgbox_hover_image = NULL;
	}
	if (layer_image[LAYER_MSG] != NULL) {
		s3_destroy_image(layer_image[LAYER_MSG]);
		layer_image[LAYER_MSG] = NULL;
	}

	/* Load the message box image. */
	msgbox_image = s3_create_image_from_file(CG_DIR, conf_msgbox_image);
	if (msgbox_image == NULL)
		return false;

	/* Create a message box layer image. */
	layer_image[LAYER_MSG] = s3_create_image(msgbox_image->width, msgbox_image->height);
	if (layer_image[LAYER_MSG] == NULL)
		return false;

	/* Layout the message box layer. */
	layer_x[LAYER_MSG] = conf_msgbox_x;
	layer_y[LAYER_MSG] = conf_msgbox_y;

	/* Transfer the message box image to the message box layer. */
	fill_msgbox();

	return true;
}

/* Setup the click animation. */
static bool setup_click(void)
{
	int i;

	is_click_visible = false;

	/* Destroy when reinitialized. */
	for (i = 0; i < CLICK_FRAMES; i++) {
		if (click_image[i] != NULL) {
			destroy_image(click_image[i]);
			click_image[i] = NULL;
		}
	}

	/* Load the click animation images. */
	for (i = 0; i < click_frames; i++) {
		if (conf_click_image[i] != NULL) {
			/* Load the image if the file name is specified. */
			click_image[i] = create_image_from_file(CG_DIR, conf_click_image[i]);
			if (click_image[i] == NULL)
				return false;
		} else {
			/* Otherwise, create a transparent iamge. */
			click_image[i] = create_image(1, 1);
			if (click_image[i] == NULL)
				return false;
			clear_image_color(click_image[i], make_pixel(0, 0, 0, 0));
		}
	}

	/* Layout the click animation layer. */
	layer_x[LAYER_CLICK] = conf_click_x;
	layer_y[LAYER_CLICK] = conf_click_y;

	/* For now, let the layer point to NULL. */
	layer_image[LAYER_CLICK] = NULL;

	return true;
}

/* Setup the choose boxes. */
static bool setup_choose(int index)
{
	int i;

	/* Destroy when reinitialized. */
	for (i = 0; i < 10; i ++) {
		if (index == -1 || index == 0) {
			if (!no_bg) {
				if (choose_bg_image[i] != NULL) {
					destroy_image(choose_bg_image[i]);
					choose_bg_image[i] = NULL;
				}
			}
			if (!no_fg) {
				if (choose_fg_image[i] != NULL) {
					destroy_image(choose_fg_image[i]);
					choose_fg_image[i] = NULL;
				}
			}
		}
	}

	/* Load the images. */
	for (i = 0; i < S3_CHOOSE_BOXES; i ++) {
		if (i != 0 &&
		    (conf_choose_bg_image[i] == NULL || 
		     conf_choose_fg_image[i] == NULL))
			continue;

		assert(conf_choose_bg_image[i] != NULL);
		assert(conf_choose_bg_image[i] != NULL);

		/* Load the idle image. */
		choose_bg_image[i] = s3_create_image_from_file(CG_DIR, conf_choose_bg_image[i]);
		if (choose_bg_image[i] == NULL)
			return false;

		/* Load the hover image. */
		choose_fg_image[i] = s3_create_image_from_file(CG_DIR, conf_choose_fg_image[i]);
		if (choose_fg_image[i] == NULL)
			return false;
	}

	return true;
}

/* Setup the system button. */
static bool setup_sysbtn(void)
{
	/* Destroy when reinitialized. */
	if (sysbtn_idle_image != NULL) {
		s3_destroy_image(sysbtn_idle_image);
		sysbtn_idle_image = NULL;
	}
	if (sysbtn_hover_image != NULL) {
		s3_destroy_image(sysbtn_hover_image);
		sysbtn_hover_image = NULL;
	}

	/* Load the sysbtn idle image. */
	sysbtn_idle_image = s3_create_image_from_file(conf_sysbtn_idle_image);
	if (sysbtn_idle_image == NULL)
		return false;

	/* Load the sysbtn hover image. */
	sysbtn_hover_image = s3_create_image_from_file(conf_sysbtn_hover_image);
	if (sysbtn_hover_image == NULL)
		return false;

	return true;
}

/* バナーをセットアップする */
static bool setup_banners(void)
{
	is_auto_visible = false;
	is_skip_visible = false;

	/* 再初期化時に破棄する */
	if (layer_image[LAYER_AUTO] != NULL) {
		destroy_image(layer_image[LAYER_AUTO]);
		layer_image[LAYER_AUTO] = NULL;
	}
	if (layer_image[LAYER_SKIP] != NULL) {
		destroy_image(layer_image[LAYER_SKIP]);
		layer_image[LAYER_SKIP] = NULL;
	}

	/* オートモードバナーの画像を読み込む */
	layer_image[LAYER_AUTO] = create_image_from_file(CG_DIR, conf_automode_banner_image);
	if (layer_image[LAYER_AUTO] == NULL)
		return false;

	layer_x[LAYER_AUTO] = conf_automode_banner_x;
	layer_y[LAYER_AUTO] = conf_automode_banner_y;

	/* スキップモードバナーの画像を読み込む */
	layer_image[LAYER_SKIP] = create_image_from_file(CG_DIR, conf_skipmode_banner_image);
	if (layer_image[LAYER_SKIP] == NULL)
		return false;

	layer_x[LAYER_SKIP] = conf_skipmode_banner_x;
	layer_y[LAYER_SKIP] = conf_skipmode_banner_y;

	return true;
}

/* キラキラ画像をセットアップする */
static bool setup_kirakira(void)
{
	int i;

	/* 再初期化時に破棄する */
	for (i = 0; i < KIRAKIRA_FRAME_COUNT; i++) {
		if (kirakira_image[i] != NULL) {
			destroy_image(kirakira_image[i]);
			kirakira_image[i] = NULL;
		}
	}

	/* キラキラ画像を読み込む */
	for (i = 0; i < KIRAKIRA_FRAME_COUNT; i++) {
		if (conf_kirakira_image[i] == NULL)
			continue;
		kirakira_image[i] = create_image_from_file(CG_DIR, conf_kirakira_image[i]);
		if (kirakira_image[i] == NULL)
			return false;
	}

	return true;
}

/* セーブデータのサムネイル画像をセットアップする */
static bool setup_thumb(void)
{
	/* 再初期化時に破棄する */
	if (thumb_image != NULL) {
		destroy_image(thumb_image);
		thumb_image = NULL;
	}

	/* コンフィグの値がおかしければ補正する */
	if (conf_save_data_thumb_width <= 0)
		conf_save_data_thumb_width = 1;
	if (conf_save_data_thumb_height <= 0)
		conf_save_data_thumb_height = 1;

	/* イメージを作成する */
	thumb_image = create_image(conf_save_data_thumb_width,
				   conf_save_data_thumb_height);
	if (thumb_image == NULL)
		return false;

	return true;
}

/* セーブスロットのNEW画像をセットアップする */
static bool setup_savenew(void)
{
	/* 再初期化時に破棄する */
	if (savenew_image != NULL) {
		destroy_image(savenew_image);
		savenew_image = NULL;
	}

	/* コンフィグが指定されていない場合 */
	if (conf_save_data_new_image == NULL)
		return true;

	/* イメージを作成する */
	savenew_image = create_image_from_file(CG_DIR, conf_save_data_new_image);
	if (savenew_image == NULL)
		return false;

	return true;
}

/* テキストレイヤーの文字を復元する */
static void restore_text_layers(void)
{
	struct draw_msg_context context;
	pixel_t color, outline_color;
	int i, total_chars;

	/* デフォルト色をロードする */
	color = make_pixel(0xff,
			   (pixel_t)conf_msgbox_font_r,
			   (pixel_t)conf_msgbox_font_g,
			   (pixel_t)conf_msgbox_font_b);
	outline_color = make_pixel(0xff,
				   (pixel_t)conf_msgbox_font_outline_r,
				   (pixel_t)conf_msgbox_font_outline_g,
				   (pixel_t)conf_msgbox_font_outline_b);

	for (i = LAYER_TEXT1; i <= LAYER_TEXT8; i++) {
		if (layer_text[i] == NULL)
			continue;

		/* 描画する */
		construct_draw_msg_context(
			&context,
			i,
			layer_text[i],
			conf_msgbox_font,
			conf_msgbox_font_size,
			conf_msgbox_font_size,
			conf_msgbox_font_ruby,
			conf_msgbox_font_outline,
			0,	/* pen_x */
			0,	/* pen_y */
			layer_image[i]->width,
			layer_image[i]->height,
			0,	/* left_margin */
			0,	/* right_margin */
			0,	/* top_margin */
			0,	/* bottom_margin */
			0,	/* line_margin */
			conf_msgbox_margin_char,
			color,
			outline_color,
			0,	/* bg_color */
			false,	/* fill_bg */
			false,	/* is_dimming */
			false,	/* ignore_linefeed */
			false,	/* ignore_font */
			false,	/* ignore_outline */
			false,	/* ignore_color */
			false,	/* ignore_size */
			false,	/* ignore_position */
			false,	/* ignore_ruby */
			true,	/* ignore_wait */
			NULL,	/* inline_wait_hook */
			false);	/* use_tategaki */
		total_chars = count_chars_common(&context, NULL);
		draw_msg_common(&context, total_chars);
		notify_image_update(layer_image[i]);
	}
}

/* レイヤのイメージを作成する */
static bool create_fade_layer_images(void)
{
	/* 再初期化時に破棄する */
	if (fo_image != NULL)
		destroy_image(fo_image);
	if (fi_image != NULL)
		destroy_image(fi_image);

	/* フェードアウトのレイヤのイメージを作成する */
	fo_image = create_image(conf_game_width, conf_game_height);
	if (fo_image == NULL)
		return false;

	/* フェードインのレイヤのイメージを作成する */
	fi_image = create_image(conf_game_width, conf_game_height);
	if (fi_image == NULL)
		return false;

	return true;
}

/*
 * 起動直後の仮の背景イメージを作成する
 */
struct image *create_initial_bg(void)
{
	struct image *img;

	/* 背景レイヤのイメージを作成する */
	img = create_image(conf_game_width, conf_game_height);
	if (img == NULL)
		return NULL;

	/* 塗り潰す */
	clear_image_black(img);

	return img;
}

/*
 * メッセージボックスを更新する
 */
bool update_msgbox(void)
{
	/* メッセージボックスをセットアップする */
	if (!setup_msgbox())
		return false;

	return true;
}

/*
 * 名前ボックスを更新する
 */
bool update_namebox(void)
{
	/* 名前ボックスをセットアップする */
	if (!setup_namebox())
		return false;

	return true;
}

/*
 * 選択肢ボックスを更新する
 */
bool update_choosebox(bool is_fg, int index)
{
	if (!is_fg) {
		if (!setup_choose(false, true, index))
			return false;
	} else {
		if (!setup_choose(true, false, index))
			return false;
	}

	return true;
}

/*
 * ステージの終了処理を行う
 */
void cleanup_stage(void)
{
	int i;

	stage_mode = STAGE_MODE_IDLE;

	for (i = 0; i < S3_STAGE_LAYERS; i++) {
		if (i == LAYER_CLICK)
			layer_image[i] = NULL;
		else
			destroy_layer_image(i);
	}
	for (i = 0; i < CLICK_FRAMES; i++) {
		if (click_image[i] != NULL) {
			destroy_image(click_image[i]);
			click_image[i] = NULL;
		}
	}
	if (msgbox_image != NULL) {
		destroy_image(msgbox_image);
		msgbox_image = NULL;
	}
	if (namebox_image != NULL) {
		destroy_image(namebox_image);
		namebox_image = NULL;
	}
	for (i = 0; i < 8; i++) {
		if (choose_bg_image[i] != NULL) {
			destroy_image(choose_bg_image[i]);
			choose_bg_image[i] = NULL;
		}
		if (choose_fg_image[i] != NULL) {
			destroy_image(choose_fg_image[i]);
			choose_fg_image[i] = NULL;
		}
	}
	if (sysbtn_idle_image != NULL) {
		destroy_image(sysbtn_idle_image);
		sysbtn_idle_image = NULL;
	}
	if (sysbtn_hover_image != NULL) {
		destroy_image(sysbtn_hover_image);
		sysbtn_hover_image = NULL;
	}
	if (thumb_image != NULL) {
		destroy_image(thumb_image);
		thumb_image = NULL;
	}
	for (i = 0; i < S3_STAGE_LAYERS; i++) {
		if (layer_file_name[i] != NULL) {
			free(layer_file_name[i]);
			layer_file_name[i] = NULL;
		}
	}
	for (i = 0; i < S3_STAGE_LAYERS; i++) {
		if (layer_file_name[i] != NULL) {
			free(layer_file_name[i]);
			layer_file_name[i] = NULL;
		}
	}
	if (fo_image != NULL) {
		destroy_image(fo_image);
		fo_image = NULL;
	}
	if (fi_image != NULL) {
		destroy_image(fi_image);
		fi_image = NULL;
	}
	if (fade_rule_img != NULL) {
		destroy_image(fade_rule_img);
		fade_rule_img = NULL;
	}
}

/*
 * レイヤのイメージを破棄する
 */
static void destroy_layer_image(int layer)
{
	assert(layer >= 0 && layer < S3_STAGE_LAYERS);

	if (layer_image[layer] != NULL) {
		destroy_image(layer_image[layer]);
		layer_image[layer] = NULL;
	}
}

/*
 * Basic Functionality
 */

/*
 * Gets a layer x position.
 */
int get_layer_x(int layer)
{
	assert(layer >= 0 && layer < S3_STAGE_LAYERS);
	return layer_x[layer];
}

/*
 * Gets a layer y position.
 */
int get_layer_y(int layer)
{
	assert(layer >= 0 && layer < S3_STAGE_LAYERS);
	return layer_y[layer];
}

/*
 * Sets a layer position.
 */
void set_layer_position(int layer, int x, int y)
{
	assert(layer >= 0 && layer < S3_STAGE_LAYERS);

	layer_x[layer] = x;
	layer_y[layer] = y;

	switch (layer) {
	case LAYER_CLICK: return;
	case LAYER_AUTO: return;
	case LAYER_SKIP: return;
	default: break;
	}
}

/*
 * Updates layer positions by config.
 */
void update_layer_position_by_config(void)
{
	layer_x[LAYER_MSG] = conf_msgbox_x;
	layer_y[LAYER_MSG] = conf_msgbox_y;

	layer_x[LAYER_NAME] = conf_namebox_x;
	layer_y[LAYER_NAME] = conf_namebox_y;

	layer_x[LAYER_NAME] = conf_namebox_x;
	layer_y[LAYER_NAME] = conf_namebox_y;

	layer_x[LAYER_CLICK] = conf_click_x;
	layer_y[LAYER_CLICK] = conf_click_y;

	layer_x[LAYER_AUTO] = conf_automode_banner_x;
	layer_y[LAYER_AUTO] = conf_automode_banner_y;

	layer_x[LAYER_SKIP] = conf_skipmode_banner_x;
	layer_y[LAYER_SKIP] = conf_skipmode_banner_y;
}

/*
 * Sets a layer scale.
 */
void set_layer_scale(int layer, float scale_x, float scale_y)
{
	assert(layer >= 0 && layer < S3_STAGE_LAYERS);

	if (scale_x == 0)
		log_info("warning: scale_x = 0");
	if (scale_y == 0)
		log_info("warning: scale_y = 0");

	layer_scale_x[layer] = scale_x;
	layer_scale_y[layer] = scale_y;
}

/*
 * Gets a layer image width.
 */
int get_layer_width(int layer)
{
	assert(layer >= 0 && layer < S3_STAGE_LAYERS);
	assert(layer_image[layer] != NULL);
	return layer_image[layer]->width;
}

/*
 * Gets a layer image height.
 */
int get_layer_height(int layer)
{
	assert(layer >= 0 && layer < S3_STAGE_LAYERS);
	assert(layer_image[layer] != NULL);
	return layer_image[layer]->height;
}

/*
 * Gets a layer alpha.
 */
int get_layer_alpha(int layer)
{
	assert(layer >= 0 && layer < S3_STAGE_LAYERS);
	return layer_alpha[layer];
}

/*
 * Sets a layer alpha.
 */
void set_layer_alpha(int layer, int alpha)
{
	assert(layer >= 0 && layer < S3_STAGE_LAYERS);
	layer_alpha[layer] = alpha;
}

/*
 * Sets a layer belnd mode.
 */
void set_layer_blend(int layer, int blend)
{
	assert(layer >= 0 && layer < S3_STAGE_LAYERS);
	layer_blend[layer] = blend;
}

/*
 * Sets a layer center coordinate.
 */
void set_layer_center(int layer, int x, int y)
{
	assert(layer >= 0 && layer < S3_STAGE_LAYERS);
	layer_center_x[layer] = (float)x;
	layer_center_y[layer] = (float)y;
}

/*
 * Sets a layer rotation.
 */
void set_layer_rotate(int layer, float rad)
{
	assert(layer >= 0 && layer < S3_STAGE_LAYERS);
	layer_rotate[layer] = rad;
}

/*
 * Gets a layer file name.
 */
const char *get_layer_file_name(int layer)
{
	assert(layer >= 0 && layer < S3_STAGE_LAYERS);
	return layer_file_name[layer];
}

/*
 * Sets a layer file name.
 */
bool set_layer_file_name(int layer, const char *file_name)
{
	assert(layer >= 0 && layer < S3_STAGE_LAYERS);
	assert(layer != LAYER_CLICK);
	assert(layer != LAYER_MSG);
	assert(layer != LAYER_NAME);
	assert(layer != LAYER_AUTO);
	assert(layer != LAYER_SKIP);

	if (layer_file_name[layer] != NULL) {
		free(layer_file_name[layer]);
		layer_file_name[layer] = NULL;
	}
	if (file_name != NULL) {
		layer_file_name[layer] = strdup(file_name);
		if (layer_file_name[layer] == NULL) {
			log_memory();
			return false;
		}
	}
	return true;
}

/*
 * Gets a layer image.
 */
struct image *get_layer_image(int layer)
{
	assert(layer >= 0 && layer < S3_STAGE_LAYERS);
	return layer_image[layer];
}

/*
 * Sets a layer image.
 */
void set_layer_image(int layer, struct image *img)
{
	assert(layer >= 0 && layer < S3_STAGE_LAYERS);
	assert(layer != LAYER_CLICK);
	assert(layer != LAYER_MSG);
	assert(layer != LAYER_NAME);
	assert(layer != LAYER_AUTO);
	assert(layer != LAYER_SKIP);

	destroy_layer_image(layer);

	layer_image[layer] = img;
}

/*
 * Sets a layer frame for eye blinking and lip synchronizing.
 */
void set_layer_frame(int layer, int frame)
{
	layer_frame[layer] = frame;
}

/*
 * Clear basic layers.
 */
void clear_stage_basic(void)
{
	int i;

	for (i = 0; i < S3_STAGE_LAYERS; i++) {
		if (i == LAYER_BG)
			continue;
		if (i == LAYER_MSG)
			continue;
		if (i == LAYER_NAME)
			continue;
		if (i == LAYER_CLICK)
			continue;
		if (i == LAYER_AUTO)
			continue;
		if (i == LAYER_SKIP)
			continue;

		set_layer_file_name(i, NULL);
		set_layer_image(i, NULL);
	}

	for (i = LAYER_TEXT1; i <= LAYER_TEXT8; i++) {
		if (layer_text[i] != NULL) {
			free(layer_text[i]);
			layer_text[i] = NULL;
		}
	}}

/*
 * Clear the stage and make it initial state.
 */
void clear_stage(void)
{
	int i;

	for (i = 0; i < S3_STAGE_LAYERS; i++) {
		switch (i) {
		case LAYER_BG:
			set_layer_file_name(i, NULL);
			set_layer_image(i, create_initial_bg());
			set_layer_position(i, 0, 0);
			set_layer_alpha(i, 255);
			break;
		case LAYER_MSG:
			set_layer_position(i, conf_msgbox_x, conf_msgbox_y);
			set_layer_alpha(i, 255);
			show_msgbox(false);
			break;
		case LAYER_NAME:
			set_layer_position(i, conf_namebox_x, conf_namebox_y);
			set_layer_alpha(i, 255);
			show_namebox(false);
			break;
		case LAYER_BG2:		/* fall-thru */
		case LAYER_CHB:		/* fall-thru */
		case LAYER_CHB_EYE:	/* fall-thru */
		case LAYER_CHB_LIP:	/* fall-thru */
		case LAYER_CHL:		/* fall-thru */
		case LAYER_CHL_EYE:	/* fall-thru */
		case LAYER_CHL_LIP:	/* fall-thru */
		case LAYER_CHLC:	/* fall-thru */
		case LAYER_CHLC_EYE:	/* fall-thru */
		case LAYER_CHLC_LIP:	/* fall-thru */
		case LAYER_CHR:		/* fall-thru */
		case LAYER_CHR_EYE:	/* fall-thru */
		case LAYER_CHR_LIP:	/* fall-thru */
		case LAYER_CHRC:	/* fall-thru */
		case LAYER_CHRC_EYE:	/* fall-thru */
		case LAYER_CHRC_LIP:	/* fall-thru */
		case LAYER_CHC:		/* fall-thru */
		case LAYER_CHC_EYE:	/* fall-thru */
		case LAYER_CHC_LIP:	/* fall-thru */
		case LAYER_CHF:		/* fall-thru */
		case LAYER_CHF_EYE:	/* fall-thru */
		case LAYER_CHF_LIP:	/* fall-thru */
		case LAYER_EFFECT1:	/* fall-thru */
		case LAYER_EFFECT2:	/* fall-thru */
		case LAYER_EFFECT3:	/* fall-thru */
		case LAYER_EFFECT4:	/* fall-thru */
		case LAYER_EFFECT5:	/* fall-thru */
		case LAYER_EFFECT6:	/* fall-thru */
		case LAYER_EFFECT7:	/* fall-thru */
		case LAYER_EFFECT8:
			set_layer_file_name(i, NULL);
			set_layer_image(i, NULL);
			set_layer_position(i, 0, 0);
			set_layer_alpha(i, 255);
			break;
		case LAYER_CLICK:
			set_layer_position(i, conf_click_x, conf_click_y);
			set_layer_alpha(i, 255);
			show_click(false);
			break;
		case LAYER_AUTO:
			set_layer_position(i, conf_automode_banner_x, conf_automode_banner_y);
			set_layer_alpha(i, 255);
			show_automode_banner(false);
			break;
		case LAYER_SKIP:
			set_layer_position(i, conf_skipmode_banner_x, conf_skipmode_banner_y);
			set_layer_alpha(i, 255);
			show_skipmode_banner(false);
			break;
		case LAYER_TEXT1:	/* fall-thru */
		case LAYER_TEXT2:	/* fall-thru */
		case LAYER_TEXT3:	/* fall-thru */
		case LAYER_TEXT4:	/* fall-thru */
		case LAYER_TEXT5:	/* fall-thru */
		case LAYER_TEXT6:	/* fall-thru */
		case LAYER_TEXT7:	/* fall-thru */
		case LAYER_TEXT8:
			set_layer_file_name(i, NULL);
			set_layer_image(i, NULL);
			set_layer_position(i, 0, 0);
			set_layer_alpha(i, 255);
			set_layer_text(i, NULL);
			break;
		default:
			assert(0);
			break;
		}
	}
}

/*
 * Conversion of Layer Index and Character Position
 */

/*
 * Converts a character position to a stage layer index.
 */
int chpos_to_layer(int chpos)
{
	switch (chpos) {
	case CH_BACK:
		return LAYER_CHB;
	case CH_LEFT:
		return LAYER_CHL;
	case CH_LEFT_CENTER:
		return LAYER_CHLC;
	case CH_RIGHT:
		return LAYER_CHR;
	case CH_RIGHT_CENTER:
		return LAYER_CHRC;
	case CH_CENTER:
		return LAYER_CHC;
	case CH_FACE:
		return LAYER_CHF;
	default:
		assert(0);
		break;
	}
	return -1;
}

/*
 * Converts a character position to a stage layer index (character eye).
 */
int chpos_to_eye_layer(int chpos)
{
	switch (chpos) {
	case CH_BACK:
		return LAYER_CHB_EYE;
	case CH_LEFT:
		return LAYER_CHL_EYE;
	case CH_LEFT_CENTER:
		return LAYER_CHLC_EYE;
	case CH_RIGHT:
		return LAYER_CHR_EYE;
	case CH_RIGHT_CENTER:
		return LAYER_CHRC_EYE;
	case CH_CENTER:
		return LAYER_CHC_EYE;
	case CH_FACE:
		return LAYER_CHF_EYE;
	default:
		assert(0);
		break;
	}
	return -1;
}

/*
 * Converts a character position to a stage layer index (character lip).
 */
int chpos_to_lip_layer(int chpos)
{
	switch (chpos) {
	case CH_BACK:
		return LAYER_CHB_LIP;
	case CH_LEFT:
		return LAYER_CHL_LIP;
	case CH_LEFT_CENTER:
		return LAYER_CHLC_LIP;
	case CH_RIGHT:
		return LAYER_CHR_LIP;
	case CH_RIGHT_CENTER:
		return LAYER_CHRC_LIP;
	case CH_CENTER:
		return LAYER_CHC_LIP;
	case CH_FACE:
		return LAYER_CHF_LIP;
	default:
		assert(0);
		break;
	}
	return -1;
}

/*
 * Converts a stage layer index to a character position.
 */
int layer_to_chpos(int layer)
{
	assert(layer == LAYER_CHB || layer == LAYER_CHB_EYE || layer == LAYER_CHB_LIP ||
	       layer == LAYER_CHL || layer == LAYER_CHL_EYE || layer == LAYER_CHL_LIP ||
	       layer == LAYER_CHR || layer == LAYER_CHR_EYE || layer == LAYER_CHR_LIP ||
	       layer == LAYER_CHC || layer == LAYER_CHC_EYE || layer == LAYER_CHC_LIP ||
	       layer == LAYER_CHRC || layer == LAYER_CHRC_EYE || layer == LAYER_CHRC_LIP ||
	       layer == LAYER_CHLC || layer == LAYER_CHLC_EYE || layer == LAYER_CHLC_LIP ||
	       layer == LAYER_CHF || layer == LAYER_CHF_EYE || layer == LAYER_CHF_LIP);

	switch (layer) {
	case LAYER_CHB:
	case LAYER_CHB_EYE:
	case LAYER_CHB_LIP:
		return CH_BACK;
	case LAYER_CHL:
	case LAYER_CHL_EYE:
	case LAYER_CHL_LIP:
		return CH_LEFT;
	case LAYER_CHLC:
	case LAYER_CHLC_EYE:
	case LAYER_CHLC_LIP:
		return CH_LEFT_CENTER;
	case LAYER_CHR:
	case LAYER_CHR_EYE:
	case LAYER_CHR_LIP:
		return CH_RIGHT;
	case LAYER_CHRC:
	case LAYER_CHRC_EYE:
	case LAYER_CHRC_LIP:
		return CH_RIGHT_CENTER;
	case LAYER_CHC:
	case LAYER_CHC_EYE:
	case LAYER_CHC_LIP:
		return CH_CENTER;
	case LAYER_CHF:
	case LAYER_CHF_EYE:
	case LAYER_CHF_LIP:
		return CH_FACE;
	default:
		assert(0);
		break;
	}
	return -1;
}

/*
 * Stage rendering to the screen.
 */

/*
 * Renders the stage with all stage layers.
 */
void render_stage(void)
{
	/* We cannot use render_stage() in an FO/FI fading. */
	assert(stage_mode != STAGE_MODE_BG_FADE);
	assert(stage_mode != STAGE_MODE_CH_FADE);
	assert(stage_mode != STAGE_MODE_CHS_FADE);
	assert(stage_mode != STAGE_MODE_CIEL_FADE);
	assert(stage_mode != STAGE_MODE_SHAKE_FADE);

	/* Update an anime frame. */
	update_anime_frame();

	/* Render stage layers. */
	render_layer_image(LAYER_BG);
	render_layer_image(LAYER_BG2);
	render_layer_image(LAYER_EFFECT5);
	render_layer_image(LAYER_EFFECT6);
	render_layer_image(LAYER_EFFECT7);
	render_layer_image(LAYER_EFFECT8);
	render_layer_image(LAYER_CHB);
	render_layer_image(LAYER_CHB_EYE);
	render_layer_image(LAYER_CHB_LIP);
	render_layer_image(LAYER_CHL);
	render_layer_image(LAYER_CHL_EYE);
	render_layer_image(LAYER_CHL_LIP);
	render_layer_image(LAYER_CHLC);
	render_layer_image(LAYER_CHLC_EYE);
	render_layer_image(LAYER_CHLC_LIP);
	render_layer_image(LAYER_CHR);
	render_layer_image(LAYER_CHR_EYE);
	render_layer_image(LAYER_CHR_LIP);
	render_layer_image(LAYER_CHRC);
	render_layer_image(LAYER_CHRC_EYE);
	render_layer_image(LAYER_CHRC_LIP);
	render_layer_image(LAYER_CHC);
	render_layer_image(LAYER_CHC_EYE);
	render_layer_image(LAYER_CHC_LIP);
	render_layer_image(LAYER_EFFECT1);
	render_layer_image(LAYER_EFFECT2);
	render_layer_image(LAYER_EFFECT3);
	render_layer_image(LAYER_EFFECT4);
	if (is_msgbox_visible)
		render_layer_image(LAYER_MSG);
	if (is_namebox_visible && !conf_namebox_hide)
		render_layer_image(LAYER_NAME);
	if (is_msgbox_visible) {
		render_layer_image(LAYER_CHF);
		render_layer_image(LAYER_CHF_EYE);
		render_layer_image(LAYER_CHF_LIP);
	}
	if (is_click_visible)
		render_layer_image(LAYER_CLICK);
	if (is_auto_visible)
		render_layer_image(LAYER_AUTO);
	if (is_skip_visible)
		render_layer_image(LAYER_SKIP);
	render_layer_image(LAYER_TEXT1);
	render_layer_image(LAYER_TEXT2);
	render_layer_image(LAYER_TEXT3);
	render_layer_image(LAYER_TEXT4);
	render_layer_image(LAYER_TEXT5);
	render_layer_image(LAYER_TEXT6);
	render_layer_image(LAYER_TEXT7);
	render_layer_image(LAYER_TEXT8);
}

/*
 * System Menu Rendering
 */

/*
 * Checks if the system button is pointed with a absolute mouse position.
 */
bool is_sysbtn_pointed(void)
{
	if (mouse_pos_x >= conf_sysbtn_x &&
	    mouse_pos_x < conf_sysbtn_x + sysbtn_idle_image->width &&
	    mouse_pos_y >= conf_sysbtn_y &&
	    mouse_pos_y < conf_sysbtn_y + sysbtn_idle_image->height)
		return true;

	return false;
}

/*
 * Renders the system button.
 */
void render_sysbtn(bool is_pointed)
{
	if (!is_pointed) {
		render_image_normal(conf_sysbtn_x,
				    conf_sysbtn_y,
				    -1, -1,
				    sysbtn_idle_image,
				    0, 0,
				    -1, -1,
				    255);
	} else {
		render_image_normal(conf_sysbtn_x,
				    conf_sysbtn_y,
				    -1, -1,
				    sysbtn_hover_image,
				    0, 0,
				    -1, -1,
				    255);
	}

#if defined(USE_EDITOR)
	prev_sysbtn_drawn = true;
#endif
}

/*
 * セーブデータ用サムネイルの描画
 */

/*
 * セーブデータ用サムネイル画像にステージ全体を描画する
 */
void draw_stage_to_thumb(void)
{
	int i;

	for (i = 0; i < S3_STAGE_LAYERS; i++) {
		if (i == LAYER_MSG)
			if (!is_msgbox_visible)
				continue;
		if (i== LAYER_NAME)
			if (!is_namebox_visible || conf_namebox_hide)
				continue;
		if (i == LAYER_AUTO)
			continue;
		if (i == LAYER_SKIP)
			continue;
		if (layer_image[i] == NULL)
			continue;
		if (layer_alpha[i] == 0)
			continue;
		draw_image_scale(thumb_image,
				 conf_game_width,
				 conf_game_height,
				 layer_x[i],
				 layer_y[i],
				 layer_image[i]);
	}
}

/*
 * セーブデータ用サムネイル画像にswitchの画像を描画する
 */
void draw_choose_to_thumb(struct image *img, int x, int y)
{
	draw_image_scale(thumb_image, conf_game_width, conf_game_height, x, y, img);
}

/*
 * セーブデータ用サムネイル画像を取得する
 */
struct image *get_thumb_image(void)
{
	return thumb_image;
}

/*
 * フェードモード
 */

/*
 * 文字列からフェードメソッドを取得する (@bg, @ch)
 */
int get_fade_method(const char *method)
{
	/*
	 * ノーマルフェード
	 */

	if (strcmp(method, "normal") == 0 ||
	    strcmp(method, "n") == 0 ||
	    strcmp(method, "") == 0 ||
	    strcmp(method, "mask") == 0 ||
	    strcmp(method, "m") == 0 ||
	    strcmp(method, U8("標準")) == 0)
		return FADE_METHOD_NORMAL;

	/*
	 * カーテンフェード
	 */

	/* カーテンが右方向だけだった頃との互換性のため、省略形が複数ある */
	if (strcmp(method, "curtain-right") == 0 ||
	    strcmp(method, "curtain") == 0 ||
	    strcmp(method, "cr") == 0 ||
	    strcmp(method, "c") == 0 ||
	    strcmp(method, U8("右カーテン")) == 0)
		return FADE_METHOD_CURTAIN_RIGHT;

	if (strcmp(method, "curtain-left") == 0 ||
	    strcmp(method, "cl") == 0 ||
	    strcmp(method, U8("左カーテン")) == 0)
		return FADE_METHOD_CURTAIN_LEFT;

	if (strcmp(method, "curtain-up") == 0 ||
	    strcmp(method, "cu") == 0 ||
	    strcmp(method, U8("上カーテン")) == 0)
		return FADE_METHOD_CURTAIN_UP;

	if (strcmp(method, "curtain-down") == 0 ||
	    strcmp(method, "cd") == 0 ||
	    strcmp(method, U8("下カーテン")) == 0)
		return FADE_METHOD_CURTAIN_DOWN;

	/*
	 * スライドフェード
	 */

	if (strcmp(method, "slide-right") == 0 ||
	    strcmp(method, "sr") == 0 ||
	    strcmp(method, U8("右スライド")) == 0)
		return FADE_METHOD_SLIDE_RIGHT;

	if (strcmp(method, "slide-left") == 0 ||
	    strcmp(method, "sl") == 0 ||
	    strcmp(method, U8("左スライド")) == 0)
		return FADE_METHOD_SLIDE_LEFT;

	if (strcmp(method, "slide-up") == 0 ||
	    strcmp(method, "su") == 0 ||
	    strcmp(method, U8("上スライド")) == 0)
		return FADE_METHOD_SLIDE_UP;

	if (strcmp(method, "slide-down") == 0 ||
	    strcmp(method, "sd") == 0 ||
	    strcmp(method, U8("下スライド")) == 0)
		return FADE_METHOD_SLIDE_DOWN;

	/*
	 * シャッターフェード Shutter fade
	 */

	if (strcmp(method, "shutter-right") == 0 ||
	    strcmp(method, "shr") == 0 ||
	    strcmp(method, U8("右シャッター")) == 0)
		return FADE_METHOD_SHUTTER_RIGHT;

	if (strcmp(method, "shutter-left") == 0 ||
	    strcmp(method, "shl") == 0 ||
	    strcmp(method, U8("左シャッター")) == 0)
		return FADE_METHOD_SHUTTER_LEFT;

	if (strcmp(method, "shutter-up") == 0 ||
	    strcmp(method, "shu") == 0 ||
	    strcmp(method, U8("上シャッター")) == 0)
		return FADE_METHOD_SHUTTER_UP;

	if (strcmp(method, "shutter-down") == 0 ||
	    strcmp(method, "shd") == 0 ||
	    strcmp(method, U8("下シャッター")) == 0)
		return FADE_METHOD_SHUTTER_DOWN;

	/*
	 * 目開き/目閉じフェード eye-open/eye-close
	 */

	if (strcmp(method, "eye-open") == 0)
		return FADE_METHOD_EYE_OPEN;

	if (strcmp(method, "eye-close") == 0)
		return FADE_METHOD_EYE_CLOSE;

	if (strcmp(method, "eye-open-v") == 0)
		return FADE_METHOD_EYE_OPEN_V;

	if (strcmp(method, "eye-close-v") == 0)
		return FADE_METHOD_EYE_CLOSE_V;

	/*
	 * スリット開き/スリット閉じフェード slit-open/slit-close
	 */

	if (strcmp(method, "slit-open") == 0)
		return FADE_METHOD_SLIT_OPEN;

	if (strcmp(method, "slit-close") == 0)
		return FADE_METHOD_SLIT_CLOSE;

	if (strcmp(method, "slit-open-v") == 0)
		return FADE_METHOD_SLIT_OPEN_V;

	if (strcmp(method, "slit-close-v") == 0)
		return FADE_METHOD_SLIT_CLOSE_V;

	/*
	 * ルール
	 */

	if (strncmp(method, "rule:", 5) == 0)
		return FADE_METHOD_RULE;

	if (strncmp(method, "melt:", 5) == 0)
		return FADE_METHOD_MELT;

	/* 不正なフェード指定 */
	return FADE_METHOD_INVALID;
}

/*
 * bg用のフェードを開始する
 */
bool start_fade_for_bg(const char *fname, struct image *img, int x, int y,
		       int alpha, int method, struct image *rule_img)
{
	assert(stage_mode == STAGE_MODE_IDLE);

	/* 背景フェードを有効にする */
	stage_mode = STAGE_MODE_BG_FADE;
	fade_method = method;

	/* フェードアウト用のレイヤにステージを描画する */
	draw_fo_common();

	/* 背景レイヤの情報をセットする */
	if (!set_layer_file_name(LAYER_BG, fname))
		return false;
	set_layer_image(LAYER_BG, img);
	set_layer_position(LAYER_BG, x, y);
	set_layer_alpha(LAYER_BG, alpha);

	/* 無効になるキャラクタを破棄する */
	set_layer_file_name(LAYER_CHB, NULL);
	set_layer_file_name(LAYER_CHL, NULL);
	set_layer_file_name(LAYER_CHLC, NULL);
	set_layer_file_name(LAYER_CHR, NULL);
	set_layer_file_name(LAYER_CHRC, NULL);
	set_layer_file_name(LAYER_CHC, NULL);
	set_layer_image(LAYER_CHB, NULL);
	set_layer_image(LAYER_CHL, NULL);
	set_layer_image(LAYER_CHLC, NULL);
	set_layer_image(LAYER_CHR, NULL);
	set_layer_image(LAYER_CHRC, NULL);
	set_layer_image(LAYER_CHC, NULL);

	/* フェードイン用のレイヤに背景を描画する */
	draw_fi_common(conf_msgbox_show_on_bg);

	/* ルールイメージを保持する */
	fade_rule_img = rule_img;

	return true;
}

/*
 * ch用のフェードを開始する
 */
bool start_fade_for_ch(int chpos, const char *fname, struct image *img,
		       int x, int y, int alpha, int method,
		       struct image *rule_img)
{
	int layer;

	assert(stage_mode == STAGE_MODE_IDLE);
	assert(chpos >= 0 && chpos < CH_ALL_LAYERS);

	stage_mode = STAGE_MODE_CH_FADE;
	fade_method = method;

	/* キャラフェードアウトレイヤにステージを描画する */
	draw_fo_common();

	/* キャラを入れ替える */
	layer = chpos_to_layer(chpos);
	if (!set_layer_file_name(layer, fname))
		return false;
	set_layer_image(layer, img);
	set_layer_alpha(layer, alpha);
	set_layer_position(layer, x, y);

	/* キャラフェードインレイヤにステージを描画する */
	draw_fi_common(conf_msgbox_show_on_ch);

	/* ルールイメージを保持する */
	fade_rule_img = rule_img;

	return true;
}

/*
 * chs用のフェードモードを開始する
 */
bool start_fade_for_chs(const bool *stay, const char **fname,
			struct image **img, const int *x, const int *y,
			const int *alpha, int method, struct image *rule_img)
{
	int i, layer;
	const int BG_INDEX = CH_BASIC_LAYERS;

	assert(stage_mode == STAGE_MODE_IDLE);

	/* このフェードではSTAGE_MODE_CHS_FADEを利用する */
	stage_mode = STAGE_MODE_CHS_FADE;
	fade_method = method;

	/* キャラフェードアウトレイヤにステージを描画する */
	draw_fo_common();

	/* 画像を入れ替える */
	for (i = 0; i <= BG_INDEX; i++) {
		layer = i == BG_INDEX ? LAYER_BG : chpos_to_layer(i);
		if (!stay[i]) {
			if (!set_layer_file_name(layer, fname[i]))
				return false;
			set_layer_image(layer, img[i]);
		}
		set_layer_position(layer, x[i], y[i]);
		set_layer_alpha(layer, alpha[i]);
	}

	/* キャラフェードインレイヤにステージを描画する */
	draw_fi_common(conf_msgbox_show_on_ch);

	/* ルールイメージを保持する */
	fade_rule_img = rule_img;

	return true;
}

/*
 * shake用のフェードモードを開始する
 */
void start_fade_for_shake(void)
{
	assert(stage_mode == STAGE_MODE_IDLE);

	stage_mode = STAGE_MODE_SHAKE_FADE;

	/* フェードアウト用のレイヤをクリアする */
	clear_image_black(fo_image);

	/* フェードイン用のレイヤにステージを描画する */
	draw_fi_common(conf_msgbox_show_on_ch);
}

/*
 * フェードの進捗率を設定する
 */
void set_fade_progress(float progress)
{
	assert(stage_mode == STAGE_MODE_BG_FADE ||
	       stage_mode == STAGE_MODE_CH_FADE ||
	       stage_mode == STAGE_MODE_CHS_FADE ||
	       stage_mode == STAGE_MODE_CIEL_FADE);

	/* 進捗率を保存する */
	fi_fo_fade_progress = progress;
}

/*
 * shakeの表示オフセットを設定する
 */
void set_shake_offset(int x, int y)
{
	assert(stage_mode == STAGE_MODE_SHAKE_FADE);

	shake_offset_x = x;
	shake_offset_y = y;
}

/*
 * フェードモードを終了する
 */
void finish_fade(void)
{
	assert(stage_mode == STAGE_MODE_BG_FADE ||
	       stage_mode == STAGE_MODE_CH_FADE ||
	       stage_mode == STAGE_MODE_CHS_FADE ||
	       stage_mode == STAGE_MODE_CIEL_FADE ||
	       stage_mode == STAGE_MODE_SHAKE_FADE);

	/* ルールイメージを破棄する */
	if (fade_rule_img != NULL) {
		destroy_image(fade_rule_img);
		fade_rule_img = NULL;
	}

	stage_mode = STAGE_MODE_IDLE;
}

/* FOにステージの内容を描画する */
static void draw_fo_common(void)
{
	clear_image_black(fo_image);
	draw_layer_image(fo_image, LAYER_BG);
	draw_layer_image(fo_image, LAYER_BG2);
	draw_layer_image(fo_image, LAYER_EFFECT5);
	draw_layer_image(fo_image, LAYER_EFFECT6);
	draw_layer_image(fo_image, LAYER_EFFECT7);
	draw_layer_image(fo_image, LAYER_EFFECT8);
	draw_layer_image(fo_image, LAYER_CHB);
	draw_layer_image(fo_image, LAYER_CHL);
	draw_layer_image(fo_image, LAYER_CHLC);
	draw_layer_image(fo_image, LAYER_CHR);
	draw_layer_image(fo_image, LAYER_CHRC);
	draw_layer_image(fo_image, LAYER_CHC);
	draw_layer_image(fo_image, LAYER_EFFECT1);
	draw_layer_image(fo_image, LAYER_EFFECT2);
	draw_layer_image(fo_image, LAYER_EFFECT3);
	draw_layer_image(fo_image, LAYER_EFFECT4);
	if (is_msgbox_visible)
		draw_layer_image(fo_image, LAYER_MSG);
	if (is_namebox_visible && !conf_namebox_hide)
		draw_layer_image(fo_image, LAYER_NAME);
	if (is_msgbox_visible)
		draw_layer_image(fo_image, LAYER_CHF);
	if (is_auto_visible)
		draw_layer_image(fo_image, LAYER_AUTO);
	if (is_skip_visible)
		draw_layer_image(fo_image, LAYER_SKIP);
	draw_layer_image(fo_image, LAYER_TEXT1);
	draw_layer_image(fo_image, LAYER_TEXT2);
	draw_layer_image(fo_image, LAYER_TEXT3);
	draw_layer_image(fo_image, LAYER_TEXT4);
	draw_layer_image(fo_image, LAYER_TEXT5);
	draw_layer_image(fo_image, LAYER_TEXT6);
	draw_layer_image(fo_image, LAYER_TEXT7);
	draw_layer_image(fo_image, LAYER_TEXT8);
}

/* FIにステージの内容を描画する */
static void draw_fi_common(bool show_msgbox)
{
	clear_image_black(fi_image);
	draw_layer_image(fi_image, LAYER_BG);
	draw_layer_image(fi_image, LAYER_BG2);
	draw_layer_image(fi_image, LAYER_EFFECT5);
	draw_layer_image(fi_image, LAYER_EFFECT6);
	draw_layer_image(fi_image, LAYER_EFFECT7);
	draw_layer_image(fi_image, LAYER_EFFECT8);
	draw_layer_image(fi_image, LAYER_CHB);
	draw_layer_image(fi_image, LAYER_CHL);
	draw_layer_image(fi_image, LAYER_CHLC);
	draw_layer_image(fi_image, LAYER_CHR);
	draw_layer_image(fi_image, LAYER_CHRC);
	draw_layer_image(fi_image, LAYER_CHC);
	draw_layer_image(fi_image, LAYER_EFFECT1);
	draw_layer_image(fi_image, LAYER_EFFECT2);
	draw_layer_image(fi_image, LAYER_EFFECT3);
	draw_layer_image(fi_image, LAYER_EFFECT4);
	if (show_msgbox) {
		if (is_msgbox_visible)
			draw_layer_image(fi_image, LAYER_MSG);
		if (is_namebox_visible && !conf_namebox_hide)
			draw_layer_image(fi_image, LAYER_NAME);
		if (is_msgbox_visible)
			draw_layer_image(fi_image, LAYER_CHF);
	}
	if (is_auto_visible)
		draw_layer_image(fi_image, LAYER_AUTO);
	if (is_skip_visible)
		draw_layer_image(fi_image, LAYER_SKIP);
	draw_layer_image(fi_image, LAYER_TEXT1);
	draw_layer_image(fi_image, LAYER_TEXT2);
	draw_layer_image(fi_image, LAYER_TEXT3);
	draw_layer_image(fi_image, LAYER_TEXT4);
	draw_layer_image(fi_image, LAYER_TEXT5);
	draw_layer_image(fi_image, LAYER_TEXT6);
	draw_layer_image(fi_image, LAYER_TEXT7);
	draw_layer_image(fi_image, LAYER_TEXT8);
}

/*
 * フェードのレンダリングを行う
 */
void render_fade(void)
{
	assert(stage_mode == STAGE_MODE_BG_FADE ||
	       stage_mode == STAGE_MODE_CH_FADE ||
	       stage_mode == STAGE_MODE_CHS_FADE ||
	       stage_mode == STAGE_MODE_CIEL_FADE ||
	       stage_mode == STAGE_MODE_SHAKE_FADE);

	if (stage_mode == STAGE_MODE_SHAKE_FADE) {
		render_fade_shake();
		return;
	}

	switch (fade_method) {
	case FADE_METHOD_NORMAL:
		render_fade_normal();
		break;
	case FADE_METHOD_RULE:
		render_fade_rule();
		break;
	case FADE_METHOD_MELT:
		render_fade_melt();
		break;
	case FADE_METHOD_CURTAIN_RIGHT:
		render_fade_curtain_right();
		break;
	case FADE_METHOD_CURTAIN_LEFT:
		render_fade_curtain_left();
		break;
	case FADE_METHOD_CURTAIN_UP:
		render_fade_curtain_up();
		break;
	case FADE_METHOD_CURTAIN_DOWN:
		render_fade_curtain_down();
		break;
	case FADE_METHOD_SLIDE_RIGHT:
		render_fade_slide_right();
		break;
	case FADE_METHOD_SLIDE_LEFT:
		render_fade_slide_left();
		break;
	case FADE_METHOD_SLIDE_UP:
		render_fade_slide_up();
		break;
	case FADE_METHOD_SLIDE_DOWN:
		render_fade_slide_down();
		break;
	case FADE_METHOD_SHUTTER_RIGHT:
		render_fade_shutter_right();
		break;
	case FADE_METHOD_SHUTTER_LEFT:
		render_fade_shutter_left();
		break;
	case FADE_METHOD_SHUTTER_UP:
		render_fade_shutter_up();
		break;
	case FADE_METHOD_SHUTTER_DOWN:
		render_fade_shutter_down();
		break;
	case FADE_METHOD_EYE_OPEN:
		render_fade_eye_open();
		break;
	case FADE_METHOD_EYE_CLOSE:
		render_fade_eye_close();
		break;
	case FADE_METHOD_EYE_OPEN_V:
		render_fade_eye_open_v();
		break;
	case FADE_METHOD_EYE_CLOSE_V:
		render_fade_eye_close_v();
		break;
	case FADE_METHOD_SLIT_OPEN:
		render_fade_slit_open();
		break;
	case FADE_METHOD_SLIT_CLOSE:
		render_fade_slit_close();
		break;
	case FADE_METHOD_SLIT_OPEN_V:
		render_fade_slit_open_v();
		break;
	case FADE_METHOD_SLIT_CLOSE_V:
		render_fade_slit_close_v();
		break;
	default:
		assert(INVALID_FADE_METHOD);
		break;
	}
}

/* デフォルトの背景フェードの描画を行う  */
static void render_fade_normal(void)
{
	render_image_normal(0, 0, -1, -1, fo_image, 0, 0, -1, -1, 255);
	render_image_normal(0, 0, -1, -1, fi_image, 0, 0, -1, -1, (int)(fi_fo_fade_progress * 255.0f));
}

/* ルール描画を行う */
static void render_fade_rule(void)
{
	int threshold;

	assert(stage_mode == STAGE_MODE_BG_FADE ||
	       stage_mode == STAGE_MODE_CH_FADE ||
	       stage_mode == STAGE_MODE_CHS_FADE ||
	       stage_mode == STAGE_MODE_CIEL_FADE);
	assert(fade_rule_img != NULL);

	/* テンプレートの閾値を求める */
	threshold = (int)(255.0f * fi_fo_fade_progress);

	/* フェードアウトする画像をコピーする */
	render_image_normal(0, 0, -1, -1, fo_image, 0, 0, -1, -1, 255);

	/* フェードインする画像をレンダリングする */
	render_image_rule(fi_image, fade_rule_img, threshold);
}

/* ルール描画(メルト)を行う */
static void render_fade_melt(void)
{
	int threshold;

	assert(stage_mode == STAGE_MODE_BG_FADE ||
	       stage_mode == STAGE_MODE_CH_FADE ||
	       stage_mode == STAGE_MODE_CHS_FADE ||
	       stage_mode == STAGE_MODE_CIEL_FADE);
	assert(fade_rule_img != NULL);

	/* テンプレートの閾値を求める */
	threshold = (int)(255.0f * fi_fo_fade_progress);

	/* フェードアウトする画像をコピーする */
	render_image_normal(0, 0, -1, -1, fo_image, 0, 0, -1, -1, 255);

	/* フェードインする画像をレンダリングする */
	render_image_melt(fi_image, fade_rule_img, threshold);
}

/* 右方向カーテンフェードの描画を行う */
static void render_fade_curtain_right(void)
{
	int right, alpha, i;

	/*
	 * カーテンの右端を求める
	 *  - カーテンの右端は0からconf_game_width+CURTAIN_WIDTHになる
	 */
	right = (int)((float)(conf_game_width + CURTAIN_WIDTH) * fi_fo_fade_progress);

	/* カーテンが通り過ぎる前の背景をコピーする */
	if (right < conf_game_width) {
		render_image_normal(right,
				    0,
				    conf_game_width - right,
				    conf_game_height,
				    fo_image,
				    right,
				    0,
				    conf_game_width - right,
				    conf_game_height,
				    255);
	}

	/* カーテンの部分の背景をコピーする */
	render_image_normal(right - CURTAIN_WIDTH,
			    0,
			    CURTAIN_WIDTH,
			    conf_game_height,
			    fo_image,
			    right - CURTAIN_WIDTH,
			    0,
			    CURTAIN_WIDTH,
			    conf_game_height,
			    255);

	/* カーテンが通り過ぎた後の背景を描画する */
	if (right >= CURTAIN_WIDTH) {
		render_image_normal(0,
				    0,
				    right - CURTAIN_WIDTH,
				    conf_game_height,
				    fi_image,
				    0,
				    0,
				    right - CURTAIN_WIDTH,
				    conf_game_height,
				    255);
	}

	/* カーテンを描画する */
	for (alpha = 0, i = right; i >= right - CURTAIN_WIDTH; i--, alpha++) {
		if (i < 0 || i >= conf_game_width)
			continue;
		if (alpha > 255)
			alpha = 255;
		render_image_normal(i,
				    0,
				    2,
				    conf_game_height,
				    fi_image,
				    i,
				    0,
				    2,
				    conf_game_height,
				    alpha);
	}
}

/* 左方向カーテンフェードの描画を行う */
static void render_fade_curtain_left(void)
{
	int left, alpha, i;

	/*
	 * カーテンの左端を求める
	 *  - カーテンの左端はconf_game_widthから-CURTAIN_WIDTになる
	 */
	left = conf_game_width -
		(int)((float)(conf_game_width + CURTAIN_WIDTH) *
		      fi_fo_fade_progress);

	/* カーテンが通り過ぎる前の背景をコピーする */
	render_image_normal(0,
			    0,
			    left + CURTAIN_WIDTH,
			    conf_game_height,
			    fo_image,
			    0,
			    0,
			    left + CURTAIN_WIDTH,
			    conf_game_height,
			    255);

	/* カーテンが通り過ぎた後の背景を描画する */
	if (left <= conf_game_width - CURTAIN_WIDTH) {
		render_image_normal(left + CURTAIN_WIDTH,
				    0,
				    conf_game_width - left - CURTAIN_WIDTH,
				    conf_game_height,
				    fi_image,
				    left + CURTAIN_WIDTH,
				    0,
				    conf_game_width - left - CURTAIN_WIDTH,
				    conf_game_height,
				    255);
	}

	/* カーテンを描画する */
	for (alpha = 0, i = left; i <= left + CURTAIN_WIDTH; i++, alpha++) {
		if (i < 0 || i >= conf_game_width)
			continue;
		if (alpha > 255)
			alpha = 255;
		render_image_normal(i,
				    0,
				    1,
				    conf_game_height,
				    fi_image,
				    i,
				    0,
				    1,
				    conf_game_height,
				    alpha);
	}
}

/* 上方向カーテンフェードの描画を行う */
static void render_fade_curtain_up(void)
{
	int top, alpha, i;

	/*
	 * カーテンの左端を求める
	 *  - カーテンの上端はconf_game_heightから-CURTAIN_WIDTHになる
	 */
	top = conf_game_height -
		(int)((float)(conf_game_height + CURTAIN_WIDTH) *
		      fi_fo_fade_progress);

	/* カーテンが通り過ぎる前の背景をコピーする */
	render_image_normal(0,
			    0,
			    conf_game_width,
			    top + CURTAIN_WIDTH,
			    fo_image,
			    0,
			    0,
			    conf_game_width,
			    top + CURTAIN_WIDTH,
			    255);

	/* カーテンが通り過ぎた後の背景を描画する */
	if (top <= conf_game_height - CURTAIN_WIDTH) {
		render_image_normal(0,
				    top + CURTAIN_WIDTH,
				    conf_game_width,
				    conf_game_height - top - CURTAIN_WIDTH,
				    fi_image,
				    0,
				    top + CURTAIN_WIDTH,
				    conf_game_width,
				    conf_game_height - top - CURTAIN_WIDTH,
				    255);
	}

	/* カーテンを描画する */
	for (alpha = 0, i = top; i <= top + CURTAIN_WIDTH; i++, alpha++) {
		if (i < 0 || i >= conf_game_height)
			continue;
		if (alpha > 255)
			alpha = 255;
		render_image_normal(0,
				    i,
				    conf_game_width,
				    1,
				    fi_image,
				    0,
				    i,
				    conf_game_width,
				    1,
				    alpha);
	}
}

/* 下方向カーテンフェードの描画を行う */
static void render_fade_curtain_down(void)
{
	int bottom, alpha, i;

	/*
	 * カーテンの下端を求める
	 *  - カーテンの下端は0からconf_game_height+CURTAIN_WIDTHになる
	 */
	bottom = (int)((float)(conf_game_height + CURTAIN_WIDTH) *
		       fi_fo_fade_progress);

	/* カーテンが通り過ぎる前の背景をコピーする */
	if (bottom < conf_game_height) {
		render_image_normal(0,
				    bottom,
				    conf_game_width,
				    conf_game_height - bottom,
				    fo_image,
				    0,
				    bottom,
				    conf_game_width,
				    conf_game_height - bottom,
				    255);
	}

	/* カーテンの部分の背景をコピーする */
	render_image_normal(0,
			    bottom - CURTAIN_WIDTH,
			    conf_game_width,
			    CURTAIN_WIDTH,
			    fo_image,
			    0,
			    bottom - CURTAIN_WIDTH,
			    conf_game_width,
			    CURTAIN_WIDTH,
			    255);

	/* カーテンが通り過ぎた後の背景を描画する */
	if (bottom >= CURTAIN_WIDTH) {
		render_image_normal(0,
				    0,
				    conf_game_width,
				    bottom - CURTAIN_WIDTH,
				    fi_image,
				    0,
				    0,
				    conf_game_width,
				    bottom - CURTAIN_WIDTH,
				    255);
	}

	/* カーテンを描画する */
	for (alpha = 0, i = bottom; i >= bottom - CURTAIN_WIDTH;
	     i--, alpha++) {
		if (i < 0 || i >= conf_game_height)
			continue;
		if (alpha > 255)
			alpha = 255;
		render_image_normal(0,
				    i,
				    conf_game_width,
				    1,
				    fi_image,
				    0,
				    i,
				    conf_game_width,
				    1,
				    alpha);
	}
}

/* 右方向スライドフェードの描画を行う */
static void render_fade_slide_right(void)
{
	int shift;

	/*
	 * スライドの右端を求める
	 *  - スライドの右端は0からconf_game_widthになる
	 */
	shift = (int)((float)conf_game_width * fi_fo_fade_progress);

	/* 左側の背景を表示する */
	render_image_normal(-(conf_game_width - shift),
			    0,
			    conf_game_width,
			    conf_game_height,
			    fi_image,
			    0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    255);

	/* 右側の背景を表示する */
	render_image_normal(shift,
			    0,
			    conf_game_width,
			    conf_game_height,
			    fo_image,
			    0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    255);
}

/* 左方向スライドフェードの描画を行う */
static void render_fade_slide_left(void)
{
	int shift, left;

	/*
	 * スライドの左端を求める
	 *  - スライドの左端はconf_game_widthから0になる
	 */
	shift = (int)((float)conf_game_width * fi_fo_fade_progress);
	left = conf_game_width - shift;

	/* 右側の背景を表示する */
	render_image_normal(left,
			    0,
			    conf_game_width - left,
			    conf_game_height,
			    fi_image,
			    0,
			    0,
			    conf_game_width - left,
			    conf_game_height,
			    255);

	/* 左側の背景を表示する */
	render_image_normal(-shift,
			    0,
			    conf_game_width,
			    conf_game_height,
			    fo_image,
			    0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    255);
}

/* 上方向スライドフェードの描画を行う */
static void render_fade_slide_up(void)
{
	int shift, top;

	/*
	 * スライドの上端を求める
	 *  - スライドの上端はconf_game_heightから0になる
	 */
	shift = (int)((float)conf_game_height * fi_fo_fade_progress);
	top = conf_game_height - shift;

	/* 上側の背景を表示する */
	render_image_normal(0,
			    -shift,
			    conf_game_width,
			    conf_game_height,
			    fo_image,
			    0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    255);

	/* 下側の背景を表示する */
	render_image_normal(0,
			    top,
			    conf_game_width,
			    conf_game_height,
			    fi_image,
			    0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    255);
}

/* 下方向スライドフェードの描画を行う */
static void render_fade_slide_down(void)
{
	int bottom;

	/*
	 * スライドの下端を求める
	 *  - スライドの下端は0からconf_game_heightになる
	 */
	bottom = (int)((float)conf_game_height * fi_fo_fade_progress);

	/* 上側の背景を表示する */
	render_image_normal(0,
			    0,
			    conf_game_width,
			    bottom,
			    fi_image,
			    0,
			    conf_game_height - bottom,
			    conf_game_width,
			    bottom,
			    255);

	/* 下側の背景を表示する */
	render_image_normal(0,
			    bottom,
			    conf_game_width,
			    conf_game_height - bottom,
			    fo_image,
			    0,
			    0,
			    conf_game_width,
			    conf_game_height - bottom,
			    255);
}

/* 右方向シャッターフェードの描画を行う Right direction shutter fade */
static void render_fade_shutter_right(void)
{
	int right;

	/*
	 * スライドの右端を求める
	 *  - スライドの右端は0からconf_game_widthになる
	 */
	right = (int)((float)conf_game_width * fi_fo_fade_progress);

	/* 左側の背景を表示する */
	render_image_normal(0,
			    0,
			    right,
			    conf_game_height,
			    fi_image,
			    conf_game_width - right,
			    0,
			    right,
			    conf_game_height,
			    255);

	/* 右側の背景を表示する */
	render_image_normal(right,
			    0,
			    conf_game_width - right,
			    conf_game_height,
			    fo_image,
			    right,
			    0,
			    conf_game_width - right,
			    conf_game_height,
			    255);
}

/* 左方向シャッターフェードの描画を行う Left direction shutter fade */
static void render_fade_shutter_left(void)
{
	int left;

	/*
	 * スライドの左端を求める
	 *  - スライドの左端はconf_game_widthから0になる
	 */
	left = conf_game_width - (int)((float)conf_game_width * fi_fo_fade_progress);

	/* 右側の背景を表示する */
	render_image_normal(left,
			    0,
			    conf_game_width - left,
			    conf_game_height,
			    fi_image,
			    0,
			    0,
			    conf_game_width - left,
			    conf_game_height,
			    255);

	/* 左側の背景を表示する */
	render_image_normal(0,
			    0,
			    left,
			    conf_game_height,
			    fo_image,
			    0,
			    0,
			    left,
			    conf_game_height,
			    255);
}

/* 上方向シャッターフェードの描画を行う Up direction shutter fade */
static void render_fade_shutter_up(void)
{
	int top;

	/*
	 * スライドの上端を求める
	 *  - スライドの上端はconf_game_heightから0になる
	 */
	top = conf_game_height - (int)((float)conf_game_height * fi_fo_fade_progress);

	/* 上側の背景を表示する */
	render_image_normal(0,
			    0,
			    conf_game_width,
			    top,
			    fo_image,
			    0,
			    0,
			    conf_game_width,
			    top,
			    255);

	/* 下側の背景を表示する */
	render_image_normal(0,
			    top,
			    conf_game_width,
			    conf_game_height - top,
			    fi_image,
			    0,
			    0,
			    conf_game_width,
			    conf_game_height - top,
			    255);
}

/* 下方向シャッターフェードの描画を行う Down direction shutter fade */
static void render_fade_shutter_down(void)
{
	int bottom;

	/*
	 * スライドの下端を求める
	 *  - スライドの下端は0からconf_game_heightになる
	 */
	bottom = (int)((float)conf_game_height * fi_fo_fade_progress);

	/* 上側の背景を表示する */
	render_image_normal(0,
			    0,
			    conf_game_width,
			    bottom,
			    fi_image,
			    0,
			    conf_game_height - bottom,
			    conf_game_width,
			    bottom,
			    255);

	/* 下側の背景を表示する */
	render_image_normal(0,
			    bottom,
			    conf_game_width,
			    conf_game_height - bottom,
			    fo_image,
			    0,
			    bottom,
			    conf_game_width,
			    conf_game_height - bottom,
			    255);
}

/* 目開きフェードの描画を行う */
static void render_fade_eye_open(void)
{
	int up, down, i, a;
	const int ALPHA_STEP = 4;

	/* 上幕の下端を求める */
	up = (int)((float)(conf_game_height / 2 - 1) -
		   (float)(conf_game_height / 2 - 1) * fi_fo_fade_progress);

	/* 下幕の上端を求める */
	down = (int)((float)(conf_game_height / 2) +
		     (float)(conf_game_height / 2 - 1) * fi_fo_fade_progress);

	/* 幕が通り過ぎた後の背景をコピーする */
	render_image_normal(0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    fi_image,
			    0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    255);

	/* 上幕の描画を行う */
	for (i = up, a = 0; i >= 0; i--) {
		render_image_normal(0,
				    i,
				    conf_game_width,
				    1,
				    fo_image,
				    0,
				    i,
				    conf_game_width,
				    1,
				    a);
		a += ALPHA_STEP;
		if (a > 255)
			a = 255;
	}

	/* 下幕の描画を行う */
	for (i = down, a = 0; i <= conf_game_height - 1; i++) {
		render_image_normal(0,
				    i,
				    conf_game_width,
				    1,
				    fo_image,
				    0,
				    i,
				    conf_game_width,
				    1,
				    a);
		a += ALPHA_STEP;
		if (a > 255)
			a = 255;
	}
}

/* 目閉じフェードの描画を行う */
static void render_fade_eye_close(void)
{
	int up, down, i, a;
	const int ALPHA_STEP = 4;

	/* 上幕の下端を求める */
	up = (int)((float)(conf_game_height / 2 - 1) * fi_fo_fade_progress);

	/* 下幕の上端を求める */
	down = (int)((float)(conf_game_height - 1) -
		     (float)(conf_game_height / 2 - 1) * fi_fo_fade_progress);

	/* 幕が通り過ぎる前の背景をコピーする */
	render_image_normal(0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    fo_image,
			    0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    255);

	/* 上幕の描画を行う */
	for (i = up, a = 0; i >= 0; i--) {
		render_image_normal(0,
				    i,
				    conf_game_width,
				    1,
				    fi_image,
				    0,
				    i,
				    conf_game_width,
				    1,
				    a);
		a += ALPHA_STEP;
		if (a > 255)
			a = 255;
	}

	/* 下幕の描画を行う */
	for (i = down, a = 0; i <= conf_game_height - 1; i++) {
		render_image_normal(0,
				    i,
				    conf_game_width,
				    1,
				    fi_image,
				    0,
				    i,
				    conf_game_width,
				    1,
				    a);
		a += ALPHA_STEP;
		if (a > 255)
			a = 255;
	}
}

/* 目開きフェード(垂直)の描画を行う */
static void render_fade_eye_open_v(void)
{
	int left, right, i, a;
	const int ALPHA_STEP = 4;

	/* 左幕の右端を求める */
	left = (int)((float)(conf_game_width / 2 - 1) -
		     (float)(conf_game_width / 2 - 1) * fi_fo_fade_progress);

	/* 右幕の左端を求める */
	right = (int)((float)(conf_game_width / 2) +
		      (float)(conf_game_width / 2 - 1) * fi_fo_fade_progress);

	/* 幕が通り過ぎた後の背景をコピーする */
	render_image_normal(0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    fi_image,
			    0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    255);

	/* 左幕の描画を行う */
	for (i = left, a = 0; i >= 0; i--) {
		render_image_normal(i,
				    0,
				    1,
				    conf_game_height,
				    fo_image,
				    i,
				    0,
				    1,
				    conf_game_height,
				    a);
		a += ALPHA_STEP;
		if (a > 255)
			a = 255;
	}

	/* 下幕の描画を行う */
	for (i = right, a = 0; i <= conf_game_width - 1; i++) {
		render_image_normal(i,
				    0,
				    1,
				    conf_game_height,
				    fo_image,
				    i,
				    0,
				    1,
				    conf_game_height,
				    a);
		a += ALPHA_STEP;
		if (a > 255)
			a = 255;
	}
}

/* 目閉じフェード(垂直)の描画を行う */
static void render_fade_eye_close_v(void)
{
	int left, right, i, a;
	const int ALPHA_STEP = 4;

	/* 左幕の右端を求める */
	left = (int)((float)(conf_game_width / 2 - 1) * fi_fo_fade_progress);

	/* 右幕の左端を求める */
	right = (int)((float)(conf_game_width - 1) -
		      (float)(conf_game_width / 2 - 1) * fi_fo_fade_progress);

	/* 幕が通り過ぎる前の背景をコピーする */
	render_image_normal(0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    fo_image,
			    0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    255);

	/* 左幕の描画を行う */
	for (i = left, a = 0; i >= 0; i--) {
		render_image_normal(i,
				    0,
				    1,
				    conf_game_height,
				    fi_image,
				    i,
				    0,
				    1,
				    conf_game_height,
				    a);
		a += ALPHA_STEP;
		if (a > 255)
			a = 255;
	}

	/* 右幕の描画を行う */
	for (i = right, a = 0; i <= conf_game_width - 1; i++) {
		render_image_normal(i,
				    0,
				    1,
				    conf_game_height,
				    fi_image,
				    i,
				    0,
				    1,
				    conf_game_height,
				    a);
		a += ALPHA_STEP;
		if (a > 255)
			a = 255;
	}
}

/* スリット開きフェードの描画を行う */
static void render_fade_slit_open(void)
{
	int up, down;

	/* 上幕の下端を求める */
	up = (int)((float)(conf_game_height / 2 - 1) -
		   (float)(conf_game_height / 2 - 1) * fi_fo_fade_progress);

	/* 下幕の上端を求める */
	down = (int)((float)(conf_game_height / 2) +
		     (float)(conf_game_height / 2 - 1) * fi_fo_fade_progress);

	/* 幕が通り過ぎた後の背景をコピーする */
	render_image_normal(0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    fi_image,
			    0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    255);

	/* 上幕の描画を行う */
	render_image_normal(0,
			    0,
			    conf_game_width,
			    up + 1,
			    fo_image,
			    0,
			    0,
			    conf_game_width,
			    up + 1,
			    255);

	/* 下幕の描画を行う */
	render_image_normal(0,
			    down,
			    conf_game_width,
			    conf_game_height - down + 1,
			    fo_image,
			    0,
			    down,
			    conf_game_width,
			    conf_game_height - down + 1,
			    255);
}

/* スリット閉じフェードの描画を行う */
static void render_fade_slit_close(void)
{
	int up, down;

	/* 上幕の下端を求める */
	up = (int)((float)(conf_game_height / 2 - 1) * fi_fo_fade_progress);

	/* 下幕の上端を求める */
	down = (int)((float)(conf_game_height - 1) -
		     (float)(conf_game_height / 2 - 1) * fi_fo_fade_progress);

	/* 幕が通り過ぎる前の背景をコピーする */
	render_image_normal(0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    fo_image,
			    0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    255);

	/* 上幕の描画を行う */
	render_image_normal(0,
			    0,
			    conf_game_width,
			    up + 1,
			    fi_image,
			    0,
			    0,
			    conf_game_width,
			    up + 1,
			    255);

	/* 下幕の描画を行う */
	render_image_normal(0,
			    down,
			    conf_game_width,
			    conf_game_height - down + 1,
			    fi_image,
			    0,
			    down,
			    conf_game_width,
			    conf_game_height - down + 1,
			    255);
}

/* スリット開きフェード(垂直)の描画を行う */
static void render_fade_slit_open_v(void)
{
	int left, right;

	/* 左幕の右端を求める */
	left = (int)((float)(conf_game_width / 2 - 1) -
		     (float)(conf_game_width / 2 - 1) * fi_fo_fade_progress);

	/* 右幕の左端を求める */
	right = (int)((float)(conf_game_width / 2) +
		      (float)(conf_game_width / 2 - 1) * fi_fo_fade_progress);

	/* 幕が通り過ぎた後の背景をコピーする */
	render_image_normal(0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    fi_image,
			    0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    255);

	/* 左幕の描画を行う */
	render_image_normal(0,
			    0,
			    left + 1,
			    conf_game_height,
			    fo_image,
			    0,
			    0,
			    left + 1,
			    conf_game_height,
			    255);

	/* 下幕の描画を行う */
	render_image_normal(right,
			    0,
			    conf_game_width - right + 1,
			    conf_game_height,
			    fo_image,
			    right,
			    0,
			    conf_game_width - right + 1,
			    conf_game_height,
			    255);
}

/* スリット開きフェード(垂直)の描画を行う */
static void render_fade_slit_close_v(void)
{
	int left, right;

	/* 左幕の右端を求める */
	left = (int)((float)(conf_game_width / 2 - 1) * fi_fo_fade_progress);

	/* 右幕の左端を求める */
	right = (int)((float)(conf_game_width - 1) -
		      (float)(conf_game_width / 2 - 1) * fi_fo_fade_progress);

	/* 幕が通り過ぎる前の背景をコピーする */
	render_image_normal(0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    fo_image,
			    0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    255);

	/* 左幕の描画を行う */
	render_image_normal(0,
			    0,
			    left + 1,
			    conf_game_height,
			    fi_image,
			    0,
			    0,
			    left + 1,
			    conf_game_height,
			    255);

	/* 下幕の描画を行う */
	render_image_normal(right,
			    0,
			    conf_game_width - right + 1,
			    conf_game_height,
			    fi_image,
			    right,
			    0,
			    conf_game_width - right + 1,
			    conf_game_height,
			    255);
}

/* 画面揺らしモードが有効な際のステージ描画を行う */
static void render_fade_shake(void)
{
	/* FOレイヤを描画する */
	render_image_normal(0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    fo_image,
			    0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    255);

	/* FIレイヤを描画する */
	render_image_normal(shake_offset_x,
			    shake_offset_y,
			    conf_game_width,
			    conf_game_height,
			    fi_image,
			    0,
			    0,
			    conf_game_width,
			    conf_game_height,
			    255);

	if (is_auto_visible)
		render_layer_image(LAYER_AUTO);
	if (is_skip_visible)
		render_layer_image(LAYER_SKIP);
}

/*
 * キャラクタの描画
 */

/*
 * キャラ位置にキャラ番号を指定する
 */
void set_ch_name_mapping(int pos, int ch_name_index)
{
	assert(pos >= 0 && pos < CH_ALL_LAYERS);

	ch_name_mapping[pos] = ch_name_index;
}

/*
 * 発話キャラを設定する
 *  - ch_name_index == -1 であれば誰も発話していない
 */
void set_ch_talking(int ch_name_index)
{
	ch_talking = ch_name_index;
}

/* 発話キャラを取得する */
int get_talking_chpos(void)
{
	int i;

	if (ch_talking == -1)
		return -1;

	for (i = 0; i < CH_BASIC_LAYERS; i++) {
		if (ch_name_mapping[i] == ch_talking)
			return i;
	}
	return -1;
}

/*
 * キャラの自動明暗を発話キャラを元に更新する
 */
void update_ch_dim_by_talking_ch(void)
{
	int i;

	for (i = 0; i < CH_BASIC_LAYERS; i++) {
		if (ch_talking == -1)
			ch_dim[i] = conf_character_focus;
		else if (ch_name_mapping[i] == -1)
			ch_dim[i] = conf_character_focus;
		else if (ch_name_mapping[i] == ch_talking)
			ch_dim[i] = false;
		else if (strncmp(conf_character_image[ch_name_mapping[i]],
				 conf_character_image[ch_talking],
				 strlen(conf_character_image[ch_talking])) == 0)
			ch_dim[i] = false;
		else
			ch_dim[i] = true;
	}
}

/*
 * キャラの明暗を手動で設定する
 */
void force_ch_dim(int chpos, bool is_dim)
{
	assert(chpos >= 0 && chpos < CH_BASIC_LAYERS);

	ch_dim[chpos] = is_dim;
}

/*
 * 名前ボックスの描画
 */

/*
 * 名前ボックスの矩形を取得する
 */
void get_namebox_rect(int *x, int *y, int *w, int *h)
{
	*x = layer_x[LAYER_NAME];
	*y = layer_y[LAYER_NAME];
	*w = layer_image[LAYER_NAME]->width;
	*h = layer_image[LAYER_NAME]->width;
}

/*
 * 名前ボックスを名前ボックス画像で埋める
 */
void fill_namebox(void)
{
	if (namebox_image == NULL)
		return;

	draw_image_copy(layer_image[LAYER_NAME],
			0, 0,
			namebox_image,
			layer_image[LAYER_NAME]->width,
			layer_image[LAYER_NAME]->height,
			0, 0);
}

/*
 * 名前ボックスの表示・非表示を設定する
 */
void show_namebox(bool show)
{
	is_namebox_visible = show;
}

/*
 * メッセージボックスの描画
 */

/*
 * メッセージボックスの矩形を取得する
 */
void get_msgbox_rect(int *x, int *y, int *w, int *h)
{
	*x = layer_x[LAYER_MSG];
	*y = layer_y[LAYER_MSG];
	*w = layer_image[LAYER_MSG]->width;
	*h = layer_image[LAYER_MSG]->height;
}

/*
 * メッセージボックスの背景を描画する
 */
void fill_msgbox(void)
{
	if (msgbox_image == NULL)
		return;

	draw_image_copy(layer_image[LAYER_MSG],
			0, 0,
			msgbox_image,
			layer_image[LAYER_MSG]->width,
			layer_image[LAYER_MSG]->height,
			0, 0);
}

/*
 * メッセージボックスの背景の矩形を描画する
 */
void fill_msgbox_rect_with_bg(int x, int y, int w, int h)
{
	if (msgbox_image == NULL)
		return;

	draw_image_copy(layer_image[LAYER_MSG], x, y, msgbox_image, w, h, x, y);
}

/*
 * メッセージボックスの前景の矩形を描画する
 */
void fill_msgbox_rect_with_fg(int x, int y, int w, int h)
{
	if (msgbox_hover_image == NULL)
		return;

	draw_image_copy(layer_image[LAYER_MSG], x, y, msgbox_hover_image, w, h, x, y);
}

/*
 * メッセージボックスの表示・非表示を設定する
 */
void show_msgbox(bool show)
{
	is_msgbox_visible = show;
}

/*
 * クリックアニメーションの描画
 */

/*
 * クリックアニメーションの矩形を取得する
 */
void get_click_rect(int *x, int *y, int *w, int *h)
{
	*x = layer_x[LAYER_CLICK];
	*y = layer_y[LAYER_CLICK];
	*w = layer_image[LAYER_CLICK]->width;
	*h = layer_image[LAYER_CLICK]->height;
}

/*
 * クリックアニメーションの位置を設定する
 */
void set_click_position(int x, int y)
{
	layer_x[LAYER_CLICK] = x;
	layer_y[LAYER_CLICK] = y;
}

/*
 * クリックアニメーションの表示・非表示を設定する
 */
void show_click(bool show)
{
	is_click_visible = show;
}

/*
 * クリックアニメーションのフレーム番号を指定する
 */
void set_click_index(int index)
{
	assert(index >= 0 && index < CLICK_FRAMES);
	assert(index < click_frames);

	layer_image[LAYER_CLICK] = click_image[index];
}

/*
 * 選択肢の描画
 */

/*
 * 選択肢の矩形を取得する
 */
void get_choose_rect(int index, int *x, int *y, int *w, int *h)
{
	int use_image_index, width, height;

	assert(index >= 0 && index < 10);

	if (index != 0 && choose_bg_image[index] == NULL)
		use_image_index = 0;
	else
		use_image_index = index;

	width = choose_bg_image[use_image_index]->width;
	height = choose_bg_image[use_image_index]->height;

	if (use_image_index == 0) {
		*x = conf_choose_x[0];
		*y = conf_choose_y[0] + (height + conf_choose_margin_y) * index;
		*w = width;
		*h = height;
	} else {
		*x = conf_choose_x[use_image_index];
		*y = conf_choose_y[use_image_index];
		*w = width;
		*h = height;
	}
}

/*
 * 選択肢の非選択イメージを描画する
 */
void draw_choose_bg_image(struct image *target, int index)
{
	if (choose_bg_image[index] == NULL)
		index = 0;
	if (choose_bg_image[index] == NULL)
		return;

	draw_image_copy(target, 0, 0,
			choose_bg_image[index],
			choose_bg_image[index]->width,
			choose_bg_image[index]->height,
			0, 0);
}

/*
 * 選択肢の選択イメージを描画する
 */
void draw_choose_fg_image(struct image *target, int index)
{
	if (choose_fg_image[index] == NULL)
		index = 0;
	if (choose_fg_image[index] == NULL)
		return;

	draw_image_copy(target, 0, 0,
			choose_fg_image[index],
			choose_fg_image[index]->width,
			choose_fg_image[index]->height,
			0, 0);
}

/*
 * バナーの描画
 */

/*
 * オートモードバナーの表示・非表示を設定する
 */
void show_automode_banner(bool show)
{
	is_auto_visible = show;
}

/*
 * スキップモードバナーの表示・非表示を設定する
 */
void show_skipmode_banner(bool show)
{
	is_skip_visible = show;
}

/*
 * 共通ルーチン
 */

/* レイヤをレンダリングする */
static void render_layer_image(int layer)
{
	struct image *base_img;
	int src_x, src_width;

	assert(layer >= 0 && layer < S3_STAGE_LAYERS);

	/* 背景イメージは必ずセットされている必要がある */
	if (layer == LAYER_BG)
		assert(layer_image[LAYER_BG] != NULL);

	/* イメージがセットされていなければ描画しない */
	if (layer_image[layer] == NULL)
		return;

	/* 目パチ/口パクのフレームを計算する */
	if (layer == LAYER_CHB_EYE || layer == LAYER_CHL_EYE ||
	    layer == LAYER_CHLC_EYE || layer == LAYER_CHC_EYE ||
	    layer == LAYER_CHRC_EYE || layer == LAYER_CHR_EYE ||
	    layer == LAYER_CHB_LIP || layer == LAYER_CHL_LIP ||
	    layer == LAYER_CHLC_LIP || layer == LAYER_CHC_LIP ||
	    layer == LAYER_CHRC_LIP || layer == LAYER_CHR_LIP) {
		base_img = layer_image[chpos_to_layer(layer_to_chpos(layer))];
		if (base_img == NULL)
			return;
		src_width = base_img->width;
		src_x = src_width * layer_frame[layer];
	} else {
		src_width = layer_image[layer]->width;
		src_x = 0;
	}

	/* 3Dの場合 */
	if (layer_rotate[layer] != 0 ||
	    layer_scale_x[layer] != 1.0f ||
	    layer_scale_y[layer] != 1.0f) {
		float x1 = 0;
		float y1 = 0;
		float x2 = (float)layer_image[layer]->width - 1.0f;
		float y2 = 0;
		float x3 = 0;
		float y3 = (float)layer_image[layer]->height - 1.0f;;
		float x4 = (float)layer_image[layer]->width - 1.0f;
		float y4 = (float)layer_image[layer]->height - 1.0f;
		float center_x = (float)layer_center_x[layer];
		float center_y = (float)layer_center_y[layer];
		float rad = (float)layer_rotate[layer];

		/* 1. Shift for the centering. */
		x1 -= center_x;
		y1 -= center_y;
		x2 -= center_x;
		y2 -= center_y;
		x3 -= center_x;
		y3 -= center_y;
		x4 -= center_x;
		y4 -= center_y;

		/* 2. Scale. */
		x1 *= layer_scale_x[layer];
		y1 *= layer_scale_y[layer];
		x2 *= layer_scale_x[layer];
		y2 *= layer_scale_y[layer];
		x3 *= layer_scale_x[layer];
		y3 *= layer_scale_y[layer];
		x4 *= layer_scale_x[layer];
		y4 *= layer_scale_y[layer];

		/* 3. Rotate. */
		if (rad != 0) {
			float tmp_x, tmp_y;

			tmp_x = x1;
			tmp_y = y1;
			x1 = tmp_x * cosf(rad) - tmp_y * sinf(rad);
			y1 = tmp_x * sinf(rad) + tmp_y * cosf(rad);

			tmp_x = x2;
			tmp_y = y2;
			x2 = tmp_x * cosf(rad) - tmp_y * sinf(rad);
			y2 = tmp_x * sinf(rad) + tmp_y * cosf(rad);

			tmp_x = x3;
			tmp_y = y3;
			x3 = tmp_x * cosf(rad) - tmp_y * sinf(rad);
			y3 = tmp_x * sinf(rad) + tmp_y * cosf(rad);

			tmp_x = x4;
			tmp_y = y4;
			x4 = tmp_x * cosf(rad) - tmp_y * sinf(rad);
			y4 = tmp_x * sinf(rad) + tmp_y * cosf(rad);
		}

		/* 4. Shift again for the centering. */
		x1 += center_x;
		y1 += center_y;
		x2 += center_x;
		y2 += center_y;
		x3 += center_x;
		y3 += center_y;
		x4 += center_x;
		y4 += center_y;

		/* 5. Shift for the layer position. */
		x1 += (float)layer_x[layer];
		y1 += (float)layer_y[layer];
		x2 += (float)layer_x[layer];
		y2 += (float)layer_y[layer];
		x3 += (float)layer_x[layer];
		y3 += (float)layer_y[layer];
		x4 += (float)layer_x[layer];
		y4 += (float)layer_y[layer];

		/* Render. */
		switch (layer_blend[layer]) {
		case BLENDMODE_NORMAL:
			render_image_3d_normal(x1, y1, x2, y2, x3, y3, x4, y4,
					       layer_image[layer],
					       0, 0,
					       layer_image[layer]->width,
					       layer_image[layer]->height,
					       layer_alpha[layer]);
			break;
		case BLENDMODE_ADD:
			render_image_3d_add(x1, y1, x2, y2, x3, y3, x4, y4,
					    layer_image[layer],
					    0, 0,
					    layer_image[layer]->width,
					    layer_image[layer]->height,
					    layer_alpha[layer]);
			break;
		default:
			break;
		}
		return;
	}

	/* キャラクタレイヤを暗く描画する場合 */
	if (layer >= LAYER_CHB && layer <= LAYER_CHC &&
	    ch_dim[layer_to_chpos(layer)]) {
		render_image_dim(layer_x[layer],
				 layer_y[layer],
				 (int)((float)src_width * layer_scale_x[layer]),
				 (int)((float)layer_image[layer]->height * layer_scale_y[layer]),
				 layer_image[layer],
				 src_x,
				 0,
				 src_width,
				 layer_image[layer]->height,
				 layer_alpha[layer]);
		return;
	}

	/* それ以外の描画の場合 */
	render_image_normal(layer_x[layer],
			    layer_y[layer],
			    (int)((float)src_width * layer_scale_x[layer]),
			    (int)((float)layer_image[layer]->height * layer_scale_y[layer]),
			    layer_image[layer],
			    src_x,
			    0,
			    src_width,
			    layer_image[layer]->height,
			    layer_alpha[layer]);
}

/* レイヤを描画する */
static void draw_layer_image(struct image *target, int layer)
{
	assert(layer >= 0 && layer < S3_STAGE_LAYERS);

	/* 背景イメージは必ずセットされている必要がある */
	if (layer == LAYER_BG)
		assert(layer_image[LAYER_BG] != NULL);

	/* イメージがセットされていなければ描画しない */
	if (layer_image[layer] == NULL)
		return;

	/* 背景レイヤの場合 */
	if (layer == LAYER_BG) {
		draw_image_copy(target,
				layer_x[layer],
				layer_y[layer],
				layer_image[layer],
				layer_image[layer]->width,
				layer_image[layer]->height,
				0, 0);
		return;
	}

	/* キャラクタレイヤを暗く描画する場合 */
	if (layer >= LAYER_CHB && layer <= LAYER_CHC &&
	    ch_dim[layer_to_chpos(layer)]) {
		draw_image_dim(target,
			       layer_x[layer],
			       layer_y[layer],
			       layer_image[layer],
			       layer_image[layer]->width,
			       layer_image[layer]->height,
			       0, 0,
			       layer_alpha[layer]);
		return;
	}

	/* 普通に描画する */
	draw_image_fast(target,
			layer_x[layer],
			layer_y[layer],
			layer_image[layer],
			layer_image[layer]->width,
			layer_image[layer]->height,
			0, 0,
			layer_alpha[layer]);
}

/*
 * セーブスロットのNEW画像の描画
 */

/* Renders a NEW image of save slots. */
void render_savenew(int x, int y, int alpha)
{
	if (savenew_image == NULL)
		return;

	render_image_normal(x,
			    y,
			    savenew_image->width,
			    savenew_image->height,
			    savenew_image,
			    0,
			    0,
			    savenew_image->width,
			    savenew_image->height,
			    alpha);
}

/*
 * Kirakira Effect
 */

/*
 * Starts the Kirakira effect.
 */
void start_kirakira(int x, int y)
{
	int w, h;

	kirakira_x = x;
	kirakira_y = y;

	if (kirakira_image[0] != NULL) {
		w = kirakira_image[0]->width;
		h = kirakira_image[0]->height;
		kirakira_x -= w / 2;
		kirakira_y -= h / 2;
	}

	reset_lap_timer(&sw_kirakira);
}

/*
 * Renders a Kirakira effect frame.
 */
void render_kirakira(void)
{
	float lap, frame_time;
	int index;

	frame_time = conf_kirakira_frame == 0 ? 0.333f : conf_kirakira_frame;

	lap = (float)get_lap_timer_millisec(&sw_kirakira) / 1000.0f;
	index = (int)(lap / frame_time);
	if (index < 0 || index >= KIRAKIRA_FRAME_COUNT)
		return;
	if (kirakira_image[index] == NULL)
		return;

	if (!conf_kirakira_add) {
		render_image_normal(kirakira_x,
				    kirakira_y,
				    -1,
				    -1,
				    kirakira_image[index],
				    0,
				    0,
				    -1,
				    -1,
				    255);
	} else {
		render_image_add(kirakira_x,
				 kirakira_y,
				 -1,
				 -1,
				 kirakira_image[index],
				 0,
				 0,
				 -1,
				 -1,
				 255);
	}
}

/*
 * Text Layers
 */

/*
 * テキストレイヤのテキストを取得する
 */
const char *get_layer_text(int layer)
{
	assert(layer >= LAYER_TEXT1);
	assert(layer <= LAYER_TEXT8);

	return layer_text[layer];
}

/*
 * テキストレイヤのテキストを設定する
 */
bool set_layer_text(int layer, const char *msg)
{
	assert(layer >= LAYER_TEXT1);
	assert(layer <= LAYER_TEXT8);

	if (layer_text[layer] != NULL) {
		free(layer_text[layer]);
		layer_text[layer] = NULL;
	}

	if (msg != NULL && strcmp(msg, "") != 0) {
		layer_text[layer] = strdup(msg);
		if (layer_text[layer] == NULL) {
			log_memory();
			return false;
		}
	}

	return true;
}

#if defined(USE_EDITOR)
void clear_sysbtn_drawn(void)
{
	prev_sysbtn_drawn = false;
}
#endif

#if defined(USE_EDITOR)
bool get_sysbtn_drawn(void)
{
	if (!prev_sysbtn_drawn)
		return false;

	return true;
}
#endif

/*
 * for debug
 */

#define PNG_DEBUG 3
#if defined(OPENNOVEL_TARGET_WASM) || defined(OPENNOVEL_TARGET_ANDROID) || defined(OPENNOVEL_TARGET_POSIX)
#include <png.h>
#else
#include <png/png.h>
#endif

#if defined(__GNUC__) && !defined(__llvm__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wclobbered"
#endif

void write_layers_to_files(void)
{
	char fname[128];
	png_structp png;
	png_infop info;
	FILE *png_fp;
	static png_bytep *row_pointers;
	int y, i;

	for (i = 0; i < S3_STAGE_LAYERS; i++) {
		sprintf(fname, "debug-layer-%02d.png", i);
		remove(fname);

		if (layer_image[i] == NULL)
			continue;

		row_pointers = malloc(sizeof(png_bytep) * (size_t)layer_image[i]->height);
		if (row_pointers == NULL) {
			log_memory();
			return;
		}
		for (y = 0; y < layer_image[i]->height; y++)
			row_pointers[y] = (png_bytep)&layer_image[i]->pixels[layer_image[i]->width * y];

		/* PNGファイルをオープンする */
		png_fp = fopen(fname, "wb");
		if (png_fp == NULL) {
			log_file_open(fname);
			return;
		}

		/* PNGを書き出す */
		png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png == NULL) {
			log_api_error("png_create_write_struct");
			fclose(png_fp);
			return;
		}
		info = png_create_info_struct(png);
		if (info == NULL) {
			log_api_error("png_create_info_struct");
			png_destroy_write_struct(&png, NULL);
			return;
		}
		if (setjmp(png_jmpbuf(png))) {
			log_error("Failed to write png file.");
			png_destroy_write_struct(&png, &info);
			return;
		}

		png_init_io(png, png_fp);
		png_set_IHDR(png, info,
			     (png_uint_32)layer_image[i]->width,
			     (png_uint_32)layer_image[i]->height,
			     8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
			     PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
		png_set_bgr(png);
		png_write_info(png, info);
		png_write_image(png, row_pointers);
		png_write_end(png, NULL);
		png_destroy_write_struct(&png, &info);

		/* PNGファイルをクローズする */
		fclose(png_fp);

		free(row_pointers);
	}
}
#if defined(__GNUC__) && !defined(__llvm__)
#pragma GCC diagnostic pop
#endif
