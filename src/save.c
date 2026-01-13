/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Save Subsystem
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
#include "save.h"

/* Save data compatibility version. */
#define SAVE_VER	(0x00000001)

/* False assertion */
#define CONFIG_TYPE_ERROR	(0)

/* Terminator of the config. */
#define END_OF_CONFIG		"eoc"

/* Flag to show if right after load. */
static bool load_flag;

/*
 * Save data that are loaded onto the memory.
 */

/* Timestamp of the save data. (0 for no data) */
static uint64_t save_time[ALL_SAVE_SLOTS];

/* Index of the last saved. */
static int latest_index;

/* Chapter title of the save data. */
static char *save_title[ALL_SAVE_SLOTS];

/* Last message of the save data. */
static char *save_message[ALL_SAVE_SLOTS];

/* Thumbnail of the save data. */
static struct s3_image *save_thumb[ALL_SAVE_SLOTS];

/* Timestamp of the quick save data. */
static uint64_t quick_save_time;

/* Message to restored right after load. */
static char *pending_message;

/*
 * Temporary Buffers
 */

/* Buffer for string read. */
static char tmp_str[4096];

/* Buffer for thumbnail read and write. */
static unsigned char *tmp_pixels;

/*
 * Forward declaration.
 */
static void load_basic_save_data(void);
static void load_basic_save_data_file(struct rfile *rf, int index);
static bool serialize_all(const char *fname, uint64_t *timestamp, int index);
static bool serialize_title(struct wfile *wf, int index);
static bool serialize_message(struct wfile *wf, int index);
static bool serialize_thumb(struct wfile *wf, int index);
static bool serialize_command(struct wfile *wf);
static bool serialize_stage(struct wfile *wf);
static bool serialize_anime(struct wfile *wf);
static bool serialize_sound(struct wfile *wf);
static bool serialize_volumes(struct wfile *wf);
static bool serialize_vars(struct wfile *wf);
static bool serialize_name_vars(struct wfile *wf);
static bool serialize_local_config(struct wfile *wf);
static bool serialize_config_helper(struct wfile *wf, bool is_global);
static bool deserialize_all(const char *fname);
static bool deserialize_message(struct rfile *rf);
static bool deserialize_command(struct rfile *rf);
static bool deserialize_stage(struct rfile *rf);
static bool deserialize_anime(struct rfile *rf);
static bool deserialize_sound(struct rfile *rf);
static bool deserialize_volumes(struct rfile *rf);
static bool deserialize_vars(struct rfile *rf);
static bool deserialize_name_vars(struct rfile *rf);
static bool deserialize_config_common(struct rfile *rf);
static void load_global_data(void);

/*
 * Initialization
 */

/*
 * Initialize the save subsystem.
 */
bool
s3i_init_save(void)
{
	int i;

	/* Clear the save slots. */
	for (i = 0; i < ALL_SAVE_SLOTS; i++) {
		save_time[i] = 0;
		if (save_title[i] != NULL) {
			free(save_title[i]);
			save_title[i] = NULL;
		}
		if (save_message[i] != NULL) {
			free(save_message[i]);
			save_message[i] = NULL;
		}
		if (save_thumb[i] != NULL) {
			destroy_image(save_thumb[i]);
			save_thumb[i] = NULL;
		}
	}

	/* Allocate the buffer for reading/writing thubnail data. */
	if (tmp_pixels != NULL)
		free(tmp_pixels);
	tmp_pixels = malloc((size_t)(conf_save_data_thumb_width *
				     conf_save_data_thumb_height * 3));
	if (tmp_pixels == NULL) {
		s3_log_out_of_memory();
		return false;
	}

	/* Load the basic data from the local save files. */
	load_basic_save_data();

	/* Load the global save file. */
	load_global_data();

	return true;
}

/*
 * Cleanup the save subsystem.
 */
void
s3i_cleanup_save(void)
{
	int i;

	for (i = 0; i < SAVE_SLOTS; i++) {
		if (save_title[i] != NULL) {
			free(save_title[i]);
			save_title[i] = NULL;
		}
		if (save_message[i] != NULL) {
			free(save_message[i]);
			save_message[i] = NULL;
		}
		if (save_thumb[i] != NULL) {
			destroy_image(save_thumb[i]);
			save_thumb[i] = NULL;
		}
	}

	free(tmp_pixels);
	tmp_pixels = NULL;

	save_global_data();
}

/*
 * Check if right after load.
 */
bool
s3_check_if_right_after_load(void)
{
	if (load_flag) {
		load_flag = false;
		return true;
	}
	return false;
}

/*
 * Get the timestamp of a save data.
 */
uint64_t
s3_get_save_date(
	int index)
{
	assert(index >= 0);
	if (index >= ALL_SAVE_SLOTS)
		return 0;

	return save_time[index];
}

/*
 * Get the latest save index.
 */
int
s3_get_latest_save_index(void)
{
	return latest_index;
}

/*
 * Get the chapter title of a save data.
 */
const char *
s3_get_save_chapter_name(
	int index)
{
	assert(index >= 0);
	if (index >= SAVE_SLOTS)
		return 0;

	return save_title[index];
}

/*
 * Get the last message of a save data.
 */
const char *
s3_get_save_last_message(
	int index)
{
	assert(index >= 0);
	if (index >= SAVE_SLOTS)
		return 0;

	return save_message[index];
}

/*
 * Get the thumbnail of a save data.
 */
struct s3_image *
s3_get_save_thumbnail(
	int index)
{
	assert(index >= 0);
	if (index >= SAVE_SLOTS)
		return NULL;

	return save_thumb[index];
}

/*
 * Execute a save.
 */
bool
s3_execute_save(
	int index)
{
	char s[128];
	uint64_t timestamp;

	/* Save the seen flags for the current tag file. */
	s3_save_seen();

	/* Save the global save data. */
	s3_execute_save_global();

	/* Get the file name for the local save data. */
	snprintf(s, sizeof(s), "%03d.sav", index);

	/* Serialize the local save data. */
	if (!serialize_all(s, &timestamp, index))
		return false;

	/* Store the timestamp. */
	save_time[index] = timestamp;

	/* Update the latest index. */
	latest_index = index;

	return true;
}

/* Serialize the local save data. */
static bool
serialize_all(
	const char *fname,
	uint64_t *timestamp,
	int index)
{
	uint64_t t;
	uint32_t ver;
	bool success;

	/* Open a buffer for streaming. */
	if (!open_write_stream())
		return false;

	success = false;
	t = 0;
	do {
		/* Write the save format version. */
		ver = (uint32_t)SAVE_VER;
		if (!write32(&ver))
			break;

		/* Write the timestamp. */
		t = (uint64_t)time(NULL);
		if (!write64(&t, sizeof(t)))
			break;

		/* Write the chapter name. */
		if (!write_string(s3_get_chapter_name()))
			break;

		/* Write the last message. */
		if (!write_string(s3_get_last_message(false)))
		return false;

		/* Write the previous last message. */
		if (!write_string(s3_get_last_message(true)))
		return false;

		/* Serialize the thumbnail. */
		if (!serialize_thumb(index))
			break;

		/* Serialize the command position. */
		if (!serialize_command())
			break;

		/* Serialize the stage. */
		if (!serialize_stage())
			break;

		/* Serialize the anime. */
		if (!serialize_anime())
			break;

		/* Serialize the sound. */
		if (!serialize_sound())
			break;

		/* Serialize the variables. */
		if (!serialize_vars())
			break;

		/* Serialize the temporary stage of Ciel. */
		/*
		 * if (!ciel_serialize_hook(wf))
		 *      break;
		 */

		/* Serialize the config. */
		if (!serialize_local_config(wf))
			break;

		/* Succeeded. */
		success = true;
	} while (0);

	/* Close the stream. */
	close_write_stream(fname);

	/* Store the timestamp. */
	*timestamp = t;

	if (!success)
		s3_log_error(S3_TR("Failed to write save data."));

	return success;
}

/* Serialize the chapter title. */
static bool
serialize_title(int index)
{
	size_t len;


	/* Save章題を保存する */
	if (index != -1) {
		if (save_title[index] != NULL)
			free(save_title[index]);
		save_title[index] = strdup(tmp_str);
		if (save_title[index] == NULL) {
			log_memory();
			return false;
		}
	}

	return true;
}

/* Serialize the thumbnail. */
static bool
serialize_thumb(struct wfile *wf, int index)
{
	pixel_t *src, pix;
	unsigned char *dst;
	size_t len;
	int x, y;

	/* stage.cのサムネイルをsave.cのイメージにコピーする */
	if (save_thumb[index] == NULL) {
		save_thumb[index] = create_image(conf_save_data_thumb_width,
						 conf_save_data_thumb_height);
		if (save_thumb[index] == NULL)
			return false;
	}
	draw_image_copy(save_thumb[index], 0, 0, get_thumb_image(),
			conf_save_data_thumb_width, conf_save_data_thumb_height, 0, 0);
	notify_image_update(save_thumb[index]);

	/* ピクセル列を準備する */
	src = get_thumb_image()->pixels;
	dst = tmp_pixels;
	for (y = 0; y < conf_save_data_thumb_height; y++) {
		for (x = 0; x < conf_save_data_thumb_width; x++) {
			pix = *src++;
			*dst++ = (unsigned char)get_pixel_r(pix);
			*dst++ = (unsigned char)get_pixel_g(pix);
			*dst++ = (unsigned char)get_pixel_b(pix);
		}
	}

	/* 書き出す */
	len = (size_t)(conf_save_data_thumb_width * conf_save_data_thumb_height * 3);
	if (write_wfile(wf, tmp_pixels, len) < len)
		return false;

	return true;
}

/* コマンド位置をシリアライズする */
static bool serialize_command(struct wfile *wf)
{
	const char *s;
	int n, m;

	/* スクリプトファイル名を取得してシリアライズする */
	s = get_script_file_name();
	if (write_wfile(wf, s, strlen(s) + 1) < strlen(s) + 1)
		return false;

	/* コマンドインデックスを取得してシリアライズする */
	n = get_command_index();
	if (last_en_command != -1 &&
	    n >= last_en_command && n < last_en_command + 10)
		n = last_en_command;
	if (write_wfile(wf, &n, sizeof(n)) < sizeof(n))
		return false;

	/* '@gosub'のリターンポイントを取得してシリアライズする */
	m = get_return_point();
	if (write_wfile(wf, &m, sizeof(m)) < sizeof(m))
		return false;

	return true;
}

/* ステージをシリアライズする */
static bool serialize_stage(struct wfile *wf)
{
	const char *file, *text;
	size_t len;
	int i, x, y, alpha;

	for (i = 0; i < STAGE_LAYERS; i++) {
		/* Exclude the following layers. */
		switch (i) {
		case LAYER_MSG: continue;
		case LAYER_NAME: continue;
		case LAYER_CLICK: continue;
		case LAYER_AUTO: continue;
		case LAYER_SKIP: continue;
		case LAYER_CHB_EYE: continue;
		case LAYER_CHL_EYE: continue;
		case LAYER_CHLC_EYE: continue;
		case LAYER_CHR_EYE: continue;
		case LAYER_CHRC_EYE: continue;
		case LAYER_CHC_EYE: continue;
		case LAYER_CHF_EYE: continue;
		case LAYER_CHB_LIP: continue;
		case LAYER_CHL_LIP: continue;
		case LAYER_CHLC_LIP: continue;
		case LAYER_CHR_LIP: continue;
		case LAYER_CHRC_LIP: continue;
		case LAYER_CHC_LIP: continue;
		case LAYER_CHF_LIP: continue;
		default: break;
		}

		file = get_layer_file_name(i);
		if (file == NULL)
			file = "none";
		if (strcmp(file, "") == 0)
			file = "none";
		if (write_wfile(wf, file, strlen(file) + 1) < strlen(file) + 1)
			return false;

		x = get_layer_x(i);
		y = get_layer_y(i);
		if (write_wfile(wf, &x, sizeof(x)) < sizeof(x))
			return false;
		if (write_wfile(wf, &y, sizeof(y)) < sizeof(y))
			return false;

		alpha = get_layer_alpha(i);
		if (write_wfile(wf, &alpha, sizeof(alpha)) < sizeof(alpha))
			return false;

		if (i >= LAYER_TEXT1 && i <= LAYER_TEXT8) {
			text = get_layer_text(i);
			if (text == NULL)
				text = "";

			len = strlen(text) + 1;
			if (write_wfile(wf, text, len) < len)
				return false;
		}
	}

	return true;
}

/* アニメをシリアライズする */
static bool serialize_anime(struct wfile *wf)
{
	const char *file;
	int i;

	for (i = 0; i < REG_ANIME_COUNT; i++) {
		file = get_reg_anime_file_name(i);
		if (file == NULL)
			file = "none";
		if (strcmp(file, "") == 0)
			file = "none";
		if (write_wfile(wf, file, strlen(file) + 1) < strlen(file) + 1)
			return false;
	}

	return true;
}

/* サウンドをシリアライズする */
static bool serialize_sound(struct wfile *wf)
{
	const char *s;
	float vol;
	int n;

	/* BGMをシリアライズする */
	s = get_bgm_file_name();
	if (s == NULL)
		s = "none";
	if (write_wfile(wf, s, strlen(s) + 1) < strlen(s) + 1)
		return false;

	/* SEをシリアライズする(ループ再生時のみ) */
	s = get_se_file_name();
	if (s == NULL)
		s = "none";
	if (write_wfile(wf, s, strlen(s) + 1) < strlen(s) + 1)
		return false;

	/* Serialize the volumes. */
	for (n = 0; n < MIXER_STREAMS; n++) {
		vol = get_mixer_volume(n);
		if (write_wfile(wf, &vol, sizeof(vol)) < sizeof(vol))
			return false;
	}

	return true;
}

/* ローカル変数をシリアライズする */
static bool serialize_vars(struct wfile *wf)
{
	size_t len;

	len = LOCAL_VAR_SIZE * sizeof(int32_t);
	if (write_wfile(wf, get_local_variables_pointer(), len) < len)
		return false;

	return true;
}

/* 名前変数をシリアライズする */
static bool serialize_name_vars(struct wfile *wf)
{
	size_t len;
	const char *name;
	int i;

	for (i = 0; i < NAME_VAR_SIZE; i++) {
		name = get_name_variable(i);
		assert(name != NULL);

		len = strlen(name) + 1;
		if (write_wfile(wf, name, len) < len)
			return false;
	}

	return true;
}

/* ローカルコンフィグをシリアライズする */
static bool serialize_local_config(struct wfile *wf)
{
	if (!serialize_config_helper(wf, false))
		return false;

	return true;
}

/* コンフィグをシリアライズする */
static bool serialize_config_helper(struct wfile *wf, bool is_global)
{
	char val[1024];
	const char *key, *val_s;
	size_t len;
	int key_index;

	/* 保存可能なキーを列挙してループする */
	key_index = 0;
	while (1) {
		/* セーブするキーを取得する */
		key = get_config_key_for_save_data(key_index++);
		if (key == NULL) {
			/* キー列挙が終了した */
			break;
		}

		/* グローバル/ローカルの対象をチェックする */
		if (!is_global) {
			if (is_config_key_global(key))
				continue;
		} else {
			if (!is_config_key_global(key))
				continue;
		}

		/* キーを出力する */
		len = strlen(key) + 1;
		if (write_wfile(wf, key, len) < len)
			return false;

		/* 型ごとに値を出力する */
		switch (get_config_type_for_key(key)) {
		case 's':
			val_s = get_string_config_value_for_key(key);
			if (val_s == NULL)
				val_s = "";
			len = strlen(val_s) + 1;
			if (write_wfile(wf, val_s, len) < len)
				return false;
			break;
		case 'i':
			snprintf(val, sizeof(val), "%d", get_int_config_value_for_key(key));
			len = strlen(val) + 1;
			if (write_wfile(wf, &val, len) < len)
				return false;
			break;
		case 'b':
			if (get_bool_config_value_for_key(key))
				snprintf(val, sizeof(val), "yes");
			else
				snprintf(val, sizeof(val), "no");
			len = strlen(val) + 1;
			if (write_wfile(wf, &val, len) < len)
				return false;
			break;
		case 'f':
			snprintf(val, sizeof(val), "%f",
				 get_float_config_value_for_key(key));
			len = strlen(val) + 1;
			if (write_wfile(wf, &val, len) < len)
				return false;
			break;
		default:
			assert(CONFIG_TYPE_ERROR);
			break;
		}
	}

	/* 終端記号を出力する */
	len = strlen(END_OF_CONFIG);
	if (write_wfile(wf, END_OF_CONFIG, len) < len)
		return false;

	return true;
}

/*
 * ロードの実際の処理
 */

/*
 * クイックセーブデータがあるか
 */
bool have_quick_save_data(void)
{
	if (quick_save_time == 0)
		return false;

	return true;
}

/*
 * クイックロードを行う Do quick load
 */
bool quick_load(bool extra)
{
	const char *fname;

	/* 既読フラグのセーブを行う */
	save_seen();

	/* グローバル変数のセーブを行う */
	save_global_data();

	/* ステージをクリアする */
	clear_stage();

	/* アニメを停止する */
	cleanup_anime();

	/* ローカルデータのデシリアライズを行う */
	fname = !extra ? QUICK_SAVE_FILE : QUICK_SAVE_EXTRA_FILE;
	if (!deserialize_all(fname))
		return false;

	/* ステージを初期化する */
	if (!reload_stage())
		abort();

	/* 名前ボックス、メッセージボックス、選択ボックスを非表示とする */
	show_namebox(false);
	show_msgbox(false);

	/* オートモードを解除する */
	if (is_auto_mode())
		stop_auto_mode();

	/* スキップモードを解除する */
	if (is_skip_mode())
		stop_skip_mode();

#ifdef USE_EDITOR
	clear_variable_changed();
	on_load_script();
	on_change_position();
#endif

	load_flag = true;

	if (is_message_active())
		clear_message_active();

	/* ロード画面から戻ったという状態をクリアする */
	(void)is_return_from_sysmenu_gosub();

	return true;
}

/*
 * ロードを処理する
 */
bool execute_load(int index)
{
	char s[128];

	/* ファイル名を求める */
	snprintf(s, sizeof(s), "%03d.sav", index);

	/* 既読フラグのセーブを行う */
	save_seen();

	/* グローバル変数のセーブを行う */
	save_global_data();

	/* ステージをクリアする */
	clear_stage();

	/* アニメを停止する */
	cleanup_anime();

	/* ローカルデータのデシリアライズを行う */
	if (!deserialize_all(s))
		return false;

	/* ステージを初期化する */
	if (!reload_stage())
		abort();

	/* 名前ボックス、メッセージボックス、選択ボックスを非表示とする */
	show_namebox(false);
	show_msgbox(false);

#ifdef USE_EDITOR
	clear_variable_changed();
	on_load_script();
	on_change_position();
#endif

	load_flag = true;

	if (is_message_active())
		clear_message_active();

	/* ロード画面から戻ったという状態をクリアする */
	(void)is_return_from_sysmenu_gosub();

	return true;
}

/* すべてをデシリアライズする */
static bool deserialize_all(const char *fname)
{
	struct rfile *rf;
	uint64_t t;
	size_t img_size;
	uint32_t ver;
	bool success;

	/* ファイルを開く */
	rf = open_rfile(SAVE_DIR, fname, true);
	if (rf == NULL)
		return false;

	success = false;
	do {
		/* セーブデータバージョンを読み込む */
		if (read_rfile(rf, &ver, sizeof(ver)) < sizeof(ver))
			break;
		if (ver != SAVE_VER) {
			log_save_ver();
			break;
		}

		/* 日付を読み込む (読み飛ばす) */
		if (read_rfile(rf, &t, sizeof(t)) < sizeof(t))
			break;

		/* 章題を読み込む */
		if (gets_rfile(rf, tmp_str, sizeof(tmp_str)) != NULL)
			if (!set_chapter_name(tmp_str))
				break;

		/* メッセージを読み込む */
		if (!deserialize_message(rf))
			break;

		/* 直前の継続メッセージを読み込む */
		gets_rfile(rf, tmp_str, sizeof(tmp_str));
		if (strcmp(tmp_str, "") != 0) {
			pending_message = strdup(tmp_str);
			if (pending_message == NULL) {
				log_memory();
				break;
			}
		} else {
			if (pending_message != NULL)
				free(pending_message);
			pending_message = NULL;
		}

		/* サムネイルを読み込む (読み飛ばす) */
		img_size = (size_t)(conf_save_data_thumb_width *
				    conf_save_data_thumb_height * 3);
		if (read_rfile(rf, tmp_pixels, img_size) < img_size)
			break;

		/* コマンド位置のデシリアライズを行う */
		if (!deserialize_command(rf))
			break;

		/* ステージのデシリアライズを行う */
		if (!deserialize_stage(rf))
			break;

		/* アニメのデシリアライズを行う */
		if (!deserialize_anime(rf))
			break;

		/* サウンドのデシリアライズを行う */
		if (!deserialize_sound(rf))
			break;

		/* ボリュームのデシリアライズを行う */
		if (!deserialize_volumes(rf))
			break;

		/* 変数のデシリアライズを行う */
		if (!deserialize_vars(rf))
			break;
		
		/* 名前変数のデシリアライズを行う */
		if (!deserialize_name_vars(rf))
			break;

		/* Cielの仮ステージのデシリアライズを行う */
		if (!ciel_deserialize_hook(rf))
			break;

		/* コンフィグのデシリアライズを行う */
		if (!deserialize_config_common(rf))
			break;

		/* ヒストリをクリアする */
		clear_history();

		/* 成功 */
		success = true;
	} while (0);

	/* ファイルをクローズする */
	close_rfile(rf);

	return success;
}

/* メッセージのデシリアライズを行う */
static bool deserialize_message(struct rfile *rf)
{
	if (gets_rfile(rf, tmp_str, sizeof(tmp_str)) == NULL)
		return false;

	return true;
}

/* コマンド位置のデシリアライズを行う */
static bool deserialize_command(struct rfile *rf)
{
	char s[1024];
	int n, m;

	if (gets_rfile(rf, s, sizeof(s)) == NULL)
		return false;

	if (!load_script(s))
		return false;

	if (read_rfile(rf, &n, sizeof(n)) < sizeof(n))
		return false;

	if (read_rfile(rf, &m, sizeof(m)) < sizeof(m))
		return false;

	if (!move_to_command_index(n))
		return false;

	if (!set_return_point(m))
		return false;

	return true;
}

/* ステージのデシリアライズを行う */
static bool deserialize_stage(struct rfile *rf)
{
	char text[4096];
	struct image *img;
	const char *fname;
	int i, x, y, alpha, layer;

	for (i = 0; i < STAGE_LAYERS; i++) {
		/* Exclude the following layers. */
		switch (i) {
		case LAYER_MSG: continue;
		case LAYER_NAME: continue;
		case LAYER_CLICK: continue;
		case LAYER_AUTO: continue;
		case LAYER_SKIP: continue;
		case LAYER_CHB_EYE: continue;
		case LAYER_CHL_EYE: continue;
		case LAYER_CHLC_EYE: continue;
		case LAYER_CHR_EYE: continue;
		case LAYER_CHRC_EYE: continue;
		case LAYER_CHC_EYE: continue;
		case LAYER_CHF_EYE: continue;
		case LAYER_CHB_LIP: continue;
		case LAYER_CHL_LIP: continue;
		case LAYER_CHLC_LIP: continue;
		case LAYER_CHR_LIP: continue;
		case LAYER_CHRC_LIP: continue;
		case LAYER_CHC_LIP: continue;
		case LAYER_CHF_LIP: continue;
		default: break;
		}

		/* File name. */
		text[0] = '\0';
		if (gets_rfile(rf, text, sizeof(text)) == NULL)
			strcpy(text, "none");
		if (i == LAYER_BG) {
			if (strcmp(text, "none") == 0 ||
			    strcmp(text, "") == 0) {
				fname = NULL;
				img = create_initial_bg();
				if (img == NULL)
					return false;;
			} else if (text[0] == '#') {
				fname = &text[0];
				img = create_image_from_color_string(conf_game_width, conf_game_height, &text[1]);
				if (img == NULL)
					return false;
			} else {
				fname = &text[0];
				if (strncmp(text, "image/", 3) == 0) {
					img = create_image_from_file(CG_DIR, &text[6]);
				} else {
					img = create_image_from_file(BG_DIR, text);
				}
				if (img == NULL)
					return false;
			}
		} else {
			const char *dir;
			switch (i) {
			case LAYER_BG2:
				dir = BG_DIR;
				break;
			case LAYER_CHB:
			case LAYER_CHL:
			case LAYER_CHLC:
			case LAYER_CHR:
			case LAYER_CHRC:
			case LAYER_CHC:
			case LAYER_CHF:
				dir = CH_DIR;
				break;
			default:
				dir = CG_DIR;
				break;
			}

			if (strcmp(text, "none") == 0 || strcmp(text, "") == 0) {
				fname = NULL;
				img = NULL;
			} else {
				fname = &text[0];
				img = create_image_from_file(dir, text);
				if (img == NULL)
					return false;
			}
		}
		set_layer_file_name(i, fname);
		set_layer_image(i, img);

		/* Position. */
		if (read_rfile(rf, &x, sizeof(x)) < sizeof(x))
			return false;
		if (read_rfile(rf, &y, sizeof(y)) < sizeof(y))
			return false;
		set_layer_position(i, x, y);

		/* Alpha. */
		if (read_rfile(rf, &alpha, sizeof(alpha)) < sizeof(alpha))
			return false;
		set_layer_alpha(i, alpha);

		/* Text. */
		if (i >= LAYER_TEXT1 && i <= LAYER_TEXT8) {
			if (gets_rfile(rf, text, sizeof(text)) != NULL)
				set_layer_text(i, text);
			else
				set_layer_text(i, NULL);
		}
	}

	for (i = 0; i < CH_ALL_LAYERS; i++) {
		layer = chpos_to_layer(i);
		fname = get_layer_file_name(layer);
		if (!load_eye_image_if_exists(i, fname))
			return false;
		if (!load_lip_image_if_exists(i, fname))
			return false;
	}

	return true;
}

/* アニメをデシリアライズする */
static bool deserialize_anime(struct rfile *rf)
{
	char text[4096];
	int i;

	for (i = 0; i < REG_ANIME_COUNT; i++) {
		if (gets_rfile(rf, text, sizeof(text)) == NULL)
			continue;
		if (strcmp(text, "none") == 0)
			continue;
		if (!load_anime_from_file(text, i, NULL))
			return false;
	}

	return true;
}

/* サウンドをデシリアライズする */
static bool deserialize_sound(struct rfile *rf)
{
	char s[1024];
	struct wave *w;

	/* BGMをデシリアライズする */
	if (gets_rfile(rf, s, sizeof(s)) == NULL)
		return false;
	if (strcmp(s, "none") == 0) {
		set_bgm_file_name(NULL);
		w = NULL;
	} else {
		set_bgm_file_name(s);
		w = create_wave_from_file(BGM_DIR, s, true);
		if (w == NULL)
			return false;
	}
	set_mixer_input(BGM_STREAM, w);

	/* SEをデシリアライズする */
	if (gets_rfile(rf, s, sizeof(s)) == NULL)
		return false;
	if (strcmp(s, "none") == 0) {
		set_se_file_name(NULL);
		w = NULL;
	} else {
		set_se_file_name(s);
		w = create_wave_from_file(SE_DIR, s, true);
		if (w == NULL)
			return false;
	}
	set_mixer_input(SE_STREAM, w);

	/* VOICEを停止する */
	set_bgvoice_playing(false);
	set_mixer_input(VOICE_STREAM, NULL);

	return true;
}

/* ボリュームをデシリアライズする */
static bool deserialize_volumes(struct rfile *rf)
{
	float vol;
	int n;

	for (n = 0; n < MIXER_STREAMS; n++) {
		if (read_rfile(rf, &vol, sizeof(vol)) < sizeof(vol))
			return false;
		set_mixer_volume(n, vol, 0);
	}

	return true;
}

/* ローカル変数をデシリアライズする */
static bool deserialize_vars(struct rfile *rf)
{
	size_t len;

	len = LOCAL_VAR_SIZE * sizeof(int32_t);
	if (read_rfile(rf, get_local_variables_pointer(), len) < len)
		return false;

	return true;
}

/* 名前変数をデシリアライズする */
static bool deserialize_name_vars(struct rfile *rf)
{
	char name[1024];
	int i;

	for (i = 0; i < NAME_VAR_SIZE; i++) {
		if (gets_rfile(rf, name, sizeof(name)) == NULL)
			return false;
		set_name_variable(i, name);
	}

	return true;
}

/* コンフィグをデシリアライズする */
static bool deserialize_config_common(struct rfile *rf)
{
	char key[1024];
	char val[1024];

	/* 終端記号が現れるまでループする */
	while (1) {
		/* ロードするキーを取得する */
		if (gets_rfile(rf, key, sizeof(key)) == NULL)
			return false;

		/* 終端記号の場合はループを終了する */
		if (strcmp(key, END_OF_CONFIG) == 0)
			break;

		/* 値を取得する(文字列として保存されている) */
		if (gets_rfile(rf, val, sizeof(val)) == NULL)
			return false;

		/* コンフィグを上書きする */
		if (!overwrite_config(key, val))
			return false;
	}

	return true;
}

/* セーブデータから基本情報を読み込む */
static void load_basic_save_data(void)
{
	struct rfile *rf;
	char buf[128];
	uint64_t t;
	int i;

	latest_index = -1;

	/* セーブスロットごとに読み込む */
	for (i = 0; i < SAVE_SLOTS; i++) {
		/* セーブデータファイルを開く */
		snprintf(buf, sizeof(buf), "%03d.sav", i);
		rf = open_rfile(SAVE_DIR, buf, true);
		if (rf != NULL) {
			/* 読み込む */
			load_basic_save_data_file(rf, i);
			close_rfile(rf);
		}
	}

	/* セーブデータファイルを開く */
	rf = open_rfile(SAVE_DIR, QUICK_SAVE_FILE, true);
	if (rf != NULL) {
		/* セーブ時刻を取得する */
		if (read_rfile(rf, &t, sizeof(t)) == sizeof(t))
			quick_save_time = (time_t)t;
		close_rfile(rf);
	}
}

/* セーブデータファイルから基本情報を読み込む */
static void load_basic_save_data_file(struct rfile *rf, int index)
{
	uint64_t t;
	size_t img_size;
	uint32_t ver;
	pixel_t *dst;
	const unsigned char *src;
	uint32_t r, g, b;
	int x, y;

	/* セーブデータのバージョンを読む */
	read_rfile(rf, &ver, sizeof(uint32_t));
	if (ver != SAVE_VER) {
		/* セーブデータの互換性がないので読み込まない */
		return;
	}

	/* セーブ時刻を取得する */
	if (read_rfile(rf, &t, sizeof(t)) < sizeof(t))
		return;
	save_time[index] = (time_t)t;
	if (latest_index == -1)
		latest_index = index;
	else if ((time_t)t > save_time[latest_index])
		latest_index = index;

	/* 章題を取得する */
	if (gets_rfile(rf, tmp_str, sizeof(tmp_str)) == NULL)
		return;
	save_title[index] = strdup(tmp_str);
	if (save_title[index] == NULL) {
		log_memory();
		return;
	}

	/* メッセージを取得する */
	if (gets_rfile(rf, tmp_str, sizeof(tmp_str)) == NULL)
		return;
	save_message[index] = strdup(tmp_str);
	if (save_message[index] == NULL) {
		log_memory();
		return;
	}

	/* 直前の継続メッセージを取得する */
	if (gets_rfile(rf, tmp_str, sizeof(tmp_str)) == NULL)
		return;

	/* サムネイルを取得する */
	img_size = (size_t)(conf_save_data_thumb_width *
			    conf_save_data_thumb_height * 3);
	if (read_rfile(rf, tmp_pixels, img_size) < img_size)
		return;

	/* サムネイルの画像を生成する */
	save_thumb[index] = create_image(conf_save_data_thumb_width,
					 conf_save_data_thumb_height);
	if (save_thumb[index] == NULL)
		return;
	dst = save_thumb[index]->pixels;
	src = tmp_pixels;
	for (y = 0; y < conf_save_data_thumb_height; y++) {
		for (x = 0; x < conf_save_data_thumb_width; x++) {
			r = *src++;
			g = *src++;
			b = *src++;
			*dst++ = make_pixel(0xff, r, g, b);
		}
	}
	notify_image_update(save_thumb[index]);
}

/*
 * グローバル変数
 */

/* グローバルデータのロードを行う */
static void load_global_data(void)
{
	struct rfile *rf;
	float f;
	uint32_t ver;
	int i;

	/* ファイルを開く */
	rf = open_rfile(SAVE_DIR, GLOBAL_SAVE_FILE, true);
	if (rf == NULL)
		return;

	/* セーブデータのバージョンを読む */
	read_rfile(rf, &ver, sizeof(uint32_t));
	if (ver != SAVE_VER) {
		/* セーブデータの互換性がないので読み込まない */
		log_save_ver();
		close_rfile(rf);
		return;
	}

	/* グローバル変数をデシリアライズする */
	read_rfile(rf, get_global_variables_pointer(),
		   GLOBAL_VAR_SIZE * sizeof(int32_t));

	/*
	 * load_global_data()はinit_mixer()より後に呼ばれる
	 */

	/* マスターボリュームをデシリアライズする */
	if (read_rfile(rf, &f, sizeof(f)) < sizeof(f))
		return;
	f = (f < 0 || f > 1.0f) ? 1.0f : f;
	set_master_volume(f);

	/* グローバルボリュームをデシリアライズする */
	for (i = 0; i < MIXER_STREAMS; i++) {
		if (read_rfile(rf, &f, sizeof(f)) < sizeof(f))
			break;
		f = (f < 0 || f > 1.0f) ? 1.0f : f;
		set_mixer_global_volume(i, f);
	}

	/* キャラクタボリュームをデシリアライズする */
	for (i = 0; i < CH_VOL_SLOTS; i++) {
		if (read_rfile(rf, &f, sizeof(f)) < sizeof(f))
			break;
		f = (f < 0 || f > 1.0f) ? 1.0f : f;
		set_character_volume(i, f);
	}

	/* テキストスピードをデシリアライズする */
	read_rfile(rf, &msg_text_speed, sizeof(f));
	msg_text_speed =
		(msg_text_speed < 0 || msg_text_speed > 10000.0f) ?
		1.0f : msg_text_speed;

	/* オートモードスピードをデシリアライズする */
	read_rfile(rf, &msg_auto_speed, sizeof(f));
	msg_auto_speed =
		(msg_auto_speed < 0 || msg_auto_speed > 10000.0f) ?
		1.0f : msg_auto_speed;

	/* コンフィグをデシリアライズする */
	deserialize_config_common(rf);

	/* ファイルを閉じる */
	close_rfile(rf);
}

/*
 * グローバルデータのセーブを行う
 */
void save_global_data(void)
{
	struct wfile *wf;
	uint32_t ver;
	float f;
	int i;

	/* セーブディレクトリを作成する */
	make_sav_dir();

	/* ファイルを開く */
	wf = open_wfile(SAVE_DIR, GLOBAL_SAVE_FILE);
	if (wf == NULL)
		return;

	/* セーブデータのバージョンを書き出す */
	ver = SAVE_VER;
	write_wfile(wf, &ver, sizeof(uint32_t));

	/* グローバル変数をシリアライズする */
	write_wfile(wf, get_global_variables_pointer(),
		    GLOBAL_VAR_SIZE * sizeof(int32_t));

	/* マスターボリュームをシリアライズする */
	f = get_master_volume();
	if (write_wfile(wf, &f, sizeof(f)) < sizeof(f))
		return;

	/* グローバルボリュームをシリアライズする */
	for (i = 0; i < MIXER_STREAMS; i++) {
		f = get_mixer_global_volume(i);
		if (write_wfile(wf, &f, sizeof(f)) < sizeof(f))
			break;
	}

	/* キャラクタボリュームをシリアライズする */
	for (i = 0; i < CH_VOL_SLOTS; i++) {
		f = get_character_volume(i);
		if (write_wfile(wf, &f, sizeof(f)) < sizeof(f))
			break;
	}

	/* テキストスピードをシリアライズする */
	write_wfile(wf, &msg_text_speed, sizeof(f));
	
	/* オートモードスピードをシリアライズする */
	write_wfile(wf, &msg_auto_speed, sizeof(f));

	/* コンフィグをデシリアライズする */
	serialize_config_helper(wf, true);

	/* ファイルを閉じる */
	close_wfile(wf);
}

/*
 * ローカルセーブデータの削除を行う
 */
void delete_local_save(int index)
{
	char s[128];

	if (index == -1)
		index = QUICK_SAVE_INDEX;

	/* セーブデータがない場合、何もしない */
	if (save_time[index] == 0)
		return;

	/* ファイル名を求める */
	snprintf(s, sizeof(s), "%03d.sav", index);

	/* セーブファイルを削除する */
	remove_file(SAVE_DIR, s);

	/* セーブデータを消去する */
	save_time[index] = 0;
	if (save_title[index] != NULL) {
		free(save_title[index]);
		save_title[index] = NULL;
	}
	if (save_message[index] != NULL) {
		free(save_message[index]);
		save_message[index] = NULL;
	}
	if (save_thumb[index] != NULL) {
		destroy_image(save_thumb[index]);
		save_thumb[index] = NULL;
	}
}

/*
 * グローバルセーブデータの削除を処理する
 */
void delete_global_save(void)
{
	/* セーブファイルを削除する */
	remove_file(SAVE_DIR, GLOBAL_SAVE_FILE);
}

/*
 * Get the pending message (message box content right after load)
 */
char *get_pending_message(void)
{
	char *ret;

	if (pending_message != NULL) {
		ret = strdup(pending_message);
		if (ret == NULL) {
			log_memory();
			return NULL;
		}
		free(pending_message);
		pending_message = NULL;
	} else {
		ret = NULL;
	}

	return ret;
}

/* */
static bool
open_write_buffer(void)
{
}

static bool
close_write_buffer(
	const char *file)
{
}

static bool
write_to_buffer(
	const void *data,
	size_t size)
{
}

/* */
static bool
open_read_buffer(
	const char *file)
{
}

static bool
close_read_buffer(void)
{
}

static bool
read_from_buffer(
	void *data,
	size_t size)
{
}
