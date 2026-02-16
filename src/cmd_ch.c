/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * "ch" command implementation
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
#include "command.h"
#include "conf.h"

struct params {
	int layer;
	const char *file_arg;
	const char *x_arg;
	const char *y_arg;
	const char *a_arg;
	const char *scale_x_arg;
	const char *scale_y_arg;
	const char *center_x_arg;
	const char *center_y_arg;
	const char *rotate_arg;
} params[] = {
	{
		S3_STAGE_LAYER_BG,
		"bg",
		"bg-x",
		"bg-y",
		"bg-a",
		"bg-scale-x",
		"bg-scale-y",
		"bg-bg-x",
		"bg-bg-y",
		"bg-rotate",
	},
	{
		S3_STAGE_LAYER_CHB,
		"back",
		"back-x",
		"back-y",
		"back-a",
		"back-scale-x",
		"back-scale-y",
		"back-back-x",
		"back-back-y",
		"back-rotate",
	},
	{
		S3_STAGE_LAYER_CHL,
		"left",
		"left-x",
		"left-y",
		"left-a",
		"left-scale-x",
		"left-scale-y",
		"left-left-x",
		"left-left-y",
		"left-rotate",
	},
	{
		S3_STAGE_LAYER_CHLC,
		"left-center",
		"left-center-x",
		"left-center-y",
		"left-center-a",
		"left-center-scale-x",
		"left-center-scale-y",
		"left-center-left-center-x",
		"left-center-left-center-y",
		"left-center-rotate",
	},
	{
		S3_STAGE_LAYER_CHR,
		"right",
		"right-x",
		"right-y",
		"right-a",
		"right-scale-x",
		"right-scale-y",
		"right-right-x",
		"right-right-y",
		"right-rotate",
	},
	{
		S3_STAGE_LAYER_CHRC,
		"right-center",
		"right-center-x",
		"right-center-y",
		"right-center-a",
		"right-center-scale-x",
		"right-center-scale-y",
		"right-center-right-center-x",
		"right-center-right-center-y",
		"right-center-rotate",
	},
	{
		S3_STAGE_LAYER_CHC,
		"center",
		"center-x",
		"center-y",
		"center-a",
		"center-scale-x",
		"center-scale-y",
		"center-center-x",
		"center-center-y",
		"center-rotate",
	},
	{
		S3_STAGE_LAYER_CHF,
		"face",
		"face-x",
		"face-y",
		"face-a",
		"face-scale-x",
		"face-scale-y",
		"face-face-x",
		"face-face-y",
		"face-rotate",
	},
};


static uint64_t sw;
static float span;
static int fade_method;

static bool init(void);
static void get_offset_x(const char *s, int layer, int *ofs_x, bool *keep);
static void get_offset_y(const char *s, int layer, int *ofs_y, bool *keep);
static int get_alpha(const char *alpha_s);
static int get_dim(const char *dim_s);
static void get_position(int *xpos, int *ypos, int chpos, struct image *img, bool ofs_keep_x, bool ofs_keep_y, int ofs_x, int ofs_y);
static void focus_character(int chpos, const char *fname);
static void draw(void);
static bool cleanup(void);

/*
 * ch command
 */
bool
s3i_ch_command(void)
{
	/* Is the first frame? */
	if (!is_in_command_repetition()) {
		/* Initialize a multiple frame execution. */
		if (!init())
			return false;
	}

	/* Process a frame. */
	process_frame();

	/* Is finished? */
	if (!is_in_command_repetition()) {
		/* Cleanup the multiple frame execution. */
		if (!cleanup())
			return false;
	}

	return true;
}

/*
 * Initialize the command execution.
 */
static bool
init(void)
{
	struct s3_fade_desc desc[S3_FADE_DESC_COUNT];
	const char *s;
	int i;

	for (i = 0; i < S3_FADE_DESC_COUNT; i++) {
		const int LAYER_INDEX = param[i].layer;
		const char *FILE_ARG = params[i].file_arg;
		const char *X_ARG = params[i].x_arg;
		const char *Y_ARG = params[i].y_arg;
		const char *SCALE_X_ARG = params[i].scale_x_arg;
		const char *SCALE_Y_ARG = params[i].scale_y_arg;
		const char *CENTER_X_ARG = params[i].center_x_arg;
		const char *CENTER_Y_ARG = params[i].center_y_arg;
		const char *ROTATE_ARG = params[i].rotate_arg;

		/* Has a file argument? */
		if (s3_check_tag_arg(FILE_ARG)) {
			/* There is a file name. */
			s = s3_get_tag_arg(FILE_ARG);

			/* Is "none" specified? */
			if (strcmp(s, "none") == 0) {
				/* Erase the existing image. */
				desc[i].fname = NULL;
				desc[i].image = NULL;
			} else {
				/* Load an image. */
				desc[i].fname = s;
				desc[i].image = s3_create_image_from_file(s);
				if (desc[i].image == NULL)
					return false;
			}
		} else {
			/* Not specified, leave the image as is. */
			desc[i].fname = s3_get_layer_file_name(LAYER_INDEX);
			desc[i].image = s3_get_layer_image(LAYER_INDEX);
		}

		/* Has a x position argument? */
		if (s3_check_tag_arg(X_ARG)) {
			s = s3_get_tag_arg(X_ARG);
		}

	}

	s3_get_arg_string("center")

	struct image *img[PARAM_SIZE], *rule_img;
	const char *fname[PARAM_SIZE];
	bool stay[PARAM_SIZE];
	bool ofs_keep_x[PARAM_SIZE];
	bool ofs_keep_y[PARAM_SIZE];
	int ofs_x[PARAM_SIZE];
	int ofs_y[PARAM_SIZE];
	int alpha[PARAM_SIZE];
	int x[PARAM_SIZE];
	int y[PARAM_SIZE];
	int dim[PARAM_SIZE - 1];
	const char *method;
	int i, layer;

	/* パラメータを取得する */
	fname[CH_CENTER] = get_string_param(CHCH_PARAM_C);
	fname[CH_RIGHT] = get_string_param(CHCH_PARAM_R);
	fname[CH_RIGHT_CENTER] = get_string_param(CHCH_PARAM_RC);
	fname[CH_LEFT] = get_string_param(CHCH_PARAM_L);
	fname[CH_LEFT_CENTER] = get_string_param(CHCH_PARAM_LC);
	fname[CH_BACK] = get_string_param(CHCH_PARAM_B);
	fname[BG_INDEX] = get_string_param(CHCH_PARAM_BG);
	get_offset_x(get_string_param(CHCH_PARAM_CX), LAYER_CHC, &ofs_x[CH_CENTER], &ofs_keep_x[CH_CENTER]);
	get_offset_y(get_string_param(CHCH_PARAM_CY), LAYER_CHC, &ofs_y[CH_CENTER], &ofs_keep_y[CH_CENTER]);
	get_offset_x(get_string_param(CHCH_PARAM_RX), LAYER_CHR, &ofs_x[CH_RIGHT], &ofs_keep_x[CH_RIGHT]);
	get_offset_y(get_string_param(CHCH_PARAM_RY), LAYER_CHR, &ofs_y[CH_RIGHT], &ofs_keep_y[CH_RIGHT]);
	get_offset_x(get_string_param(CHCH_PARAM_LX), LAYER_CHL, &ofs_x[CH_LEFT], &ofs_keep_x[CH_LEFT]);
	get_offset_y(get_string_param(CHCH_PARAM_LY), LAYER_CHL, &ofs_y[CH_LEFT], &ofs_keep_y[CH_LEFT]);
	get_offset_x(get_string_param(CHCH_PARAM_RCX), LAYER_CHRC, &ofs_x[CH_RIGHT_CENTER], &ofs_keep_x[CH_RIGHT_CENTER]);
	get_offset_y(get_string_param(CHCH_PARAM_RCY), LAYER_CHRC, &ofs_y[CH_RIGHT_CENTER], &ofs_keep_y[CH_RIGHT_CENTER]);
	get_offset_x(get_string_param(CHCH_PARAM_LCX), LAYER_CHLC, &ofs_x[CH_LEFT_CENTER], &ofs_keep_x[CH_LEFT_CENTER]);
	get_offset_y(get_string_param(CHCH_PARAM_LCY), LAYER_CHLC, &ofs_y[CH_LEFT_CENTER], &ofs_keep_y[CH_LEFT_CENTER]);
	get_offset_x(get_string_param(CHCH_PARAM_BX), LAYER_CHB, &ofs_x[CH_BACK], &ofs_keep_x[CH_BACK]);
	get_offset_y(get_string_param(CHCH_PARAM_BY), LAYER_CHB, &ofs_y[CH_BACK], &ofs_keep_y[CH_BACK]);
	get_offset_x(get_string_param(CHCH_PARAM_BGX), LAYER_BG, &ofs_x[BG_INDEX], &ofs_keep_x[BG_INDEX]);
	get_offset_y(get_string_param(CHCH_PARAM_BGY), LAYER_BG, &ofs_y[BG_INDEX], &ofs_keep_y[BG_INDEX]);
	alpha[CH_CENTER] = get_alpha(get_string_param(CHCH_PARAM_CA));
	alpha[CH_RIGHT] = get_alpha(get_string_param(CHCH_PARAM_RA));
	alpha[CH_LEFT] = get_alpha(get_string_param(CHCH_PARAM_LA));
	alpha[CH_BACK] = get_alpha(get_string_param(CHCH_PARAM_BA));
	alpha[CH_RIGHT_CENTER] = get_alpha(get_string_param(CHCH_PARAM_RCA));
	alpha[CH_LEFT_CENTER] = get_alpha(get_string_param(CHCH_PARAM_LCA));
	alpha[BG_INDEX] = get_alpha(get_string_param(CHCH_PARAM_BGA));
	dim[CH_CENTER] = get_dim(get_string_param(CHCH_PARAM_CD));
	dim[CH_RIGHT] = get_dim(get_string_param(CHCH_PARAM_RD));
	dim[CH_LEFT] = get_dim(get_string_param(CHCH_PARAM_LD));
	dim[CH_RIGHT_CENTER] = get_dim(get_string_param(CHCH_PARAM_RCD));
	dim[CH_LEFT_CENTER] = get_dim(get_string_param(CHCH_PARAM_LCD));
	dim[CH_BACK] = get_dim(get_string_param(CHCH_PARAM_BD));
	span = get_float_param(CHCH_PARAM_SPAN);
	method = get_string_param(CHCH_PARAM_METHOD);

	/* 描画メソッドを識別する */
	fade_method = get_fade_method(method);
	if (fade_method == FADE_METHOD_INVALID) {
		log_script_fade_method(method);
		log_script_exec_footer();
		return false;
	}

	/* 各キャラと背景について */
	for (i = 0; i < PARAM_SIZE; i++) {
		stay[i] = false;
		img[i] = NULL;
		x[i] = 0;
		y[i] = 0;

		if (i != BG_INDEX)
			layer = chpos_to_layer(i);
		else
			layer = LAYER_BG;

		/* 変更なしが指定された場合 */
		if (i != BG_INDEX) {
			if (strcmp(fname[i], "stay") == 0 || strcmp(fname[i], "") == 0) {
				/* 変更なしフラグをセットする */
				stay[i] = true;
				get_position(&x[i], &y[i], i, get_layer_image(layer), ofs_keep_x[i], ofs_keep_y[i], ofs_x[i], ofs_y[i]);
				continue;
			}
		} else {
			if (strcmp(fname[i], "stay") == 0 || strcmp(fname[i], "") == 0) {
				/* 変更なしフラグをセットする */
				stay[i] = true;
				get_position(&x[i], &y[i], i, get_layer_image(layer), ofs_keep_x[i], ofs_keep_y[i], ofs_x[i], ofs_y[i]);
				continue;
			}
		}

		/* イメージの消去が指定された場合 */
		if (i != BG_INDEX &&
		    (strcmp(fname[i], "none") == 0 || strcmp(fname[i], "") == 0)) {
			fname[i] = NULL;
			x[i] = get_layer_x(layer);
			y[i] = get_layer_y(layer);
			continue;
		}

		/* 背景の色指定の場合 */
		if (i == BG_INDEX && fname[i][0] == '#') {
			/* 色を指定してイメージを作成する */
			img[i] = create_image_from_color_string(conf_game_width, conf_game_height, &fname[i][1]);
		} else {
			/* イメージを読み込む */
			img[i] = create_image_from_file(i != BG_INDEX ? CH_DIR : BG_DIR, fname[i]);
		}
		if (img[i] == NULL) {
			log_script_exec_footer();
			return false;
		}

		/* ファイル名を設定する */
		if (!set_layer_file_name(layer, fname[i]))
			return false;

		/* 表示位置を取得する */
		if (i != BG_INDEX) {
			get_position(&x[i], &y[i], i, img[i], ofs_keep_x[i], ofs_keep_y[i], ofs_x[i], ofs_y[i]);
		} else {
			x[i] = ofs_x[i];
			y[i] = ofs_y[i];
		}

		/* キャラを暗くしない */
		if (i != BG_INDEX)
			focus_character(i, fname[i]);
	}

	/* 発話中のキャラをなしにする */
	if (conf_character_focus == 1)
		set_ch_talking(-1);

	/* キャラのdim状態を発話中のキャラを元に更新する */
	if (conf_character_focus != 0)
		update_ch_dim_by_talking_ch();

	/* 手動でキャラのdim状態を設定する */
	for (i = 0; i < CH_BASIC_LAYERS; i++) {
		if (dim[i] == 1)
			force_ch_dim(i, false);
		else if (dim[i] == -1)
			force_ch_dim(i, true);
	}

	/* ルールが使用される場合 */
	if (fade_method == FADE_METHOD_RULE ||
	    fade_method == FADE_METHOD_MELT) {
		/* ルールファイルが指定されていない場合 */
		if (strcmp(&method[5], "") == 0) {
			log_script_rule();
			log_script_exec_footer();
			return false;
		}

		/* イメージを読み込む */
		rule_img = create_image_from_file(RULE_DIR, &method[5]);
		if (rule_img == NULL) {
			log_script_exec_footer();
			return false;
		}
	} else {
		rule_img = NULL;
	}

	/* 繰り返し動作を開始する */
	start_command_repetition();

	/* キャラフェードモードを有効にする */
	if (!start_fade_for_chs(stay, fname, img, x, y, alpha, fade_method, rule_img)) {
		log_script_exec_footer();
		return false;
	}

	/* 目パチのイメージをロードする */
	for (i = 0; i < CH_BASIC_LAYERS; i++) {
		if (stay[i])
			continue;
		if (!load_eye_image_if_exists(i, fname[i]))
			return false;
		if (!load_lip_image_if_exists(i, fname[i]))
			return false;
	}

	/* 時間計測を開始する */
	reset_lap_timer(&sw);

	/* メッセージボックスを消す */
	if (!conf_msgbox_show_on_ch) {
		show_namebox(false);
		show_msgbox(false);
	}
	show_click(false);

	return true;
}

/* Xオフセットを取得する */
static void get_offset_x(const char *s, int layer, int *ofs_x, bool *keep)
{
	if (strcmp(s, "keep") == 0) {
		*keep = true;
		*ofs_x = get_layer_x(layer);
	} else {
		*keep = false;
		*ofs_x = atoi(s);
	}
}

/* Yオフセットを取得する */
static void get_offset_y(const char *s, int layer, int *ofs_y, bool *keep)
{
	if (strcmp(s, "keep") == 0) {
		*keep = true;
		*ofs_y = get_layer_y(layer);
	} else {
		*keep = false;
		*ofs_y = atoi(s);
	}
}

/* 文字列のアルファ値を整数に変換する */
static int get_alpha(const char *alpha_s)
{
	int ret;

	/* 省略された場合は255にする */
	if (strcmp(alpha_s, "") == 0)
		return 255;

	ret = atoi(alpha_s);
	if (ret < 0)
		ret = 0;
	if (ret > 255)
		ret = 255;
	return ret;
}

/* 文字列の明暗を整数に変換する */
static int get_dim(const char *dim_s)
{
	/* 未指定の場合は変更しない */
	if (strcmp(dim_s, "") == 0)
		return 0;

	/* 暗くすることが指定された場合 */
	if (strcmp(dim_s, "dark") == 0 ||
	    strcmp(dim_s, "yes") == 0 ||
	    strcmp(dim_s, U8("暗")) == 0)
	    return -1;

	/* 明るくすることが指定された場合 */
	if (strcmp(dim_s, "light") == 0 ||
	    strcmp(dim_s, "no") == 0 ||
	    strcmp(dim_s, U8("明")) == 0)
	    return 1;

	/* 指定が誤っている場合は変更しない */
	return 0;
}

/* キャラの横方向の位置を取得する */
static void get_position(int *xpos, int *ypos, int chpos, struct image *img, bool ofs_keep_x, bool ofs_keep_y, int ofs_x, int ofs_y)
{
	int center, right;

	*xpos = 0;

	switch (chpos) {
	case CH_BACK:
		/* 中央に配置する */
		if (img != NULL) {
			if (!ofs_keep_x)
				*xpos = (conf_game_width - img->width) / 2 + ofs_x;
			else
				*xpos = get_layer_x(LAYER_CHB);
			if (!ofs_keep_y)
				*ypos = conf_game_height - img->height - conf_stage_ch_margin_bottom + ofs_y;
			else
				*ypos = get_layer_y(LAYER_CHB);
		} else {
			*xpos = 0;
			*ypos = 0;
		}
		break;
	case CH_CENTER:
		/* 中央に配置する */
		if (img != NULL) {
			if (!ofs_keep_x)
				*xpos = (conf_game_width - img->width) / 2 + ofs_x;
			else
				*xpos = get_layer_x(LAYER_CHC);
			if (!ofs_keep_y)
				*ypos = conf_game_height - img->height - conf_stage_ch_margin_bottom + ofs_y;
			else
				*ypos = get_layer_y(LAYER_CHC);
		} else {
			*xpos = 0;
			*ypos = 0;
		}
		break;
	case CH_LEFT:
		/* 左に配置する */
		if (img != NULL) {
			if (!ofs_keep_x)
				*xpos = conf_stage_ch_margin_left + ofs_x;
			else
				*xpos = get_layer_x(LAYER_CHL);
			if (!ofs_keep_y)
				*ypos = conf_game_height - img->height - conf_stage_ch_margin_bottom + ofs_y;
			else
				*ypos = get_layer_y(LAYER_CHL);
		} else {
			*xpos = 0;
			*ypos = 0;
		}
		break;
	case CH_LEFT_CENTER:
		/* 左中に配置する */
		if (img != NULL) {
			if (!ofs_keep_x)
				*xpos = (conf_game_width - img->width) / 4 + ofs_x;
			else
				*xpos = get_layer_x(LAYER_CHLC);
			if (!ofs_keep_y)
				*ypos = conf_game_height - img->height - conf_stage_ch_margin_bottom + ofs_y;
			else
				*ypos = get_layer_y(LAYER_CHLC);
		} else {
			*xpos = 0;
			*ypos = 0;
		}
		break;
	case CH_RIGHT:
		/* 右に配置する */
		if (img != NULL) {
			if (!ofs_keep_x)
				*xpos = conf_game_width - img->width - conf_stage_ch_margin_right + ofs_x;
			else
				*xpos = get_layer_x(LAYER_CHR);
			if (!ofs_keep_y)
				*ypos = conf_game_height - img->height - conf_stage_ch_margin_bottom + ofs_y;
			else
				*ypos = get_layer_y(LAYER_CHR);
		} else {
			*xpos = 0;
			*ypos = 0;
		}
		break;
	case CH_RIGHT_CENTER:
		/* 右に配置する */
		if (img != NULL) {
			if (!ofs_keep_x) {
				center = (conf_game_width - img->width) / 2;
				right = conf_game_width - img->width - conf_stage_ch_margin_right;
				*xpos = (center + right) / 2 + ofs_x;
			} else {
				*xpos = get_layer_x(LAYER_CHRC);
			}
			if (!ofs_keep_y)
				*ypos = conf_game_height - img->height - conf_stage_ch_margin_bottom + ofs_y;
			else
				*ypos = get_layer_y(LAYER_CHRC);
		} else {
			*xpos = 0;
			*ypos = 0;
		}
		break;
	}
}

/* キャラクタのフォーカスを行う */
static void focus_character(int chpos, const char *fname)
{
	int i;

	/* 名前が登録されているキャラクタであるかチェックする */
	for (i = 0; i < CHARACTER_MAP_COUNT; i++) {
		if (conf_character_name[i] == NULL)
			continue;
		if (conf_character_image[i] == NULL)
			continue;
		if (fname == NULL)
			continue;
		if (strncmp(conf_character_image[i], fname, strlen(conf_character_image[i])) == 0)
			break;
	}
	if (i == CHARACTER_MAP_COUNT)
		i = -1;

	set_ch_name_mapping(chpos, i);
}

/* 描画を行う */
static void draw(void)
{
	float lap;

	/* 経過時間を取得する */
	lap = (float)get_lap_timer_millisec(&sw) / 1000.0f;
	if (lap >= span)
		lap = span;

	/* 入力に反応する */
	if (is_auto_mode() &&
	    (is_control_pressed || is_return_pressed ||
	     is_left_clicked || is_down_pressed)) {
		/* 入力によりオートモードを終了する */
		stop_auto_mode();
		show_automode_banner(false);

		/* 繰り返し動作を停止する */
		stop_command_repetition();

		/* フェードを完了する */
		finish_fade();
	} else if (is_skip_mode() &&
		   (is_control_pressed || is_return_pressed ||
		    is_left_clicked || is_down_pressed)) {
		/* 入力によりスキップモードを終了する */
		stop_skip_mode();
		show_skipmode_banner(false);

		/* 繰り返し動作を停止する */
		stop_command_repetition();

		/* フェードを完了する */
		finish_fade();
	} else if ((lap >= span)
		   ||
		   is_skip_mode()
		   ||
		   (!is_non_interruptible() &&
		    (is_control_pressed || is_return_pressed ||
		     is_left_clicked || is_down_pressed))) {
		/*
		 * 経過時間が一定値を超えた場合と、
		 * スキップモードの場合と、
		 * 入力により省略された場合
		 */

		/* 繰り返し動作を終了する */
		stop_command_repetition();

		/* フェードを終了する */
		finish_fade();

		/* 入力があればスキップとオートを終了する */
		if (is_control_pressed || is_return_pressed ||
		    is_left_clicked || is_down_pressed) {
			if (is_skip_mode()) {
				stop_skip_mode();
				show_skipmode_banner(false);
			} else if (is_auto_mode()) {
				stop_auto_mode();
				show_automode_banner(false);
			}
		}
	} else {
		/* 進捗を設定する */
		set_fade_progress(lap / span);
	}

	/* ステージを描画する */
	if (is_in_command_repetition())
		render_fade();
	else
		render_stage();

	/* 折りたたみシステムメニューを描画する */
	if (conf_sysbtn_transition && !is_non_interruptible())
		render_sysbtn(false);
}

/* 終了処理を行う */
static bool cleanup(void)
{
	int i;

	/* 目パチレイヤーの再設定を行う */
	for (i = 0; i < CH_BASIC_LAYERS; i++)
		reload_eye_anime(i);

	/* 次のコマンドに移動する */
	if (!move_to_next_command())
		return false;

	return true;
}
