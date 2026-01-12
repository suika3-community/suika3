/* -*- Coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

/*
 * Animation Subsystem
 */

#include "opennovel.h"

#define INVALID_ACCEL_TYPE	(0)

/* レイヤごとのアニメーションシーケンスの最大数 */
#define SEQUENCE_COUNT		(1024)

/* アニメーションシーケンスの構造 */
struct sequence {
	int layer;
	float start_time;
	float end_time;
	float from_x;
	float from_y;
	float from_a;
	float from_scale_x;
	float from_scale_y;
	float to_x;
	float to_y;
	float to_a;
	float to_scale_x;
	float to_scale_y;
	float center_x;
	float center_y;
	float from_rotate;
	float to_rotate;
	int frame;	/* 目パチ用 */
	int blend;
	int accel;
	bool loop;
	bool hide_special;
	bool show_special;
};

/* アニメーションシーケンス(レイヤxシーケンス長) */
static struct sequence sequence[STAGE_LAYERS][SEQUENCE_COUNT];

/* レイヤごとのアニメーションの状況 */
struct layer_context {
	/* props */
	int seq_count;
	bool is_running;
	bool is_finished;
	uint64_t loop_len;
	uint64_t loop_ofs;

	/* prop (deprecated) */
	char *file;

	/* runtime */
	uint64_t sw;
	float cur_lap;
};
static struct layer_context context[STAGE_LAYERS];

/* レイヤー名とレイヤーのインデックスのマップ */
struct layer_name_map {
	const char *name;
	int index;
};
static struct layer_name_map layer_name_map[] = {
	{"bg", LAYER_BG},
	{"bg2", LAYER_BG2},
	{"effect5", LAYER_EFFECT5},
	{"effect6", LAYER_EFFECT6},
	{"effect7", LAYER_EFFECT7},
	{"effect8", LAYER_EFFECT8},
	{"chb", LAYER_CHB},
	{"chb-eye", LAYER_CHB_EYE},
	{"chb-lip", LAYER_CHB_LIP},
	{"chl", LAYER_CHL},
	{"chl-eye", LAYER_CHL_EYE},
	{"chl-lip", LAYER_CHL_LIP},
	{"chlc", LAYER_CHLC},
	{"chlc-eye", LAYER_CHLC_EYE},
	{"chlc-lip", LAYER_CHLC_LIP},
	{"chr", LAYER_CHR},
	{"chr-eye", LAYER_CHR_EYE},
	{"chr-lip", LAYER_CHR_LIP},
	{"chrc", LAYER_CHRC},
	{"chrc-eye", LAYER_CHRC_EYE},
	{"chrc-lip", LAYER_CHRC_LIP},
	{"chc", LAYER_CHC},
	{"chc-eye", LAYER_CHC_EYE},
	{"chc-lip", LAYER_CHC_LIP},
	{"effect1", LAYER_EFFECT1},
	{"effect2", LAYER_EFFECT2},
	{"effect3", LAYER_EFFECT3},
	{"effect4", LAYER_EFFECT4},
	{"msg", LAYER_MSG},
	{"name", LAYER_NAME},
	{"face", LAYER_CHF},
	{"text1", LAYER_TEXT1},
	{"text2", LAYER_TEXT2},
	{"text3", LAYER_TEXT3},
	{"text4", LAYER_TEXT4},
	{"text5", LAYER_TEXT5},
	{"text6", LAYER_TEXT6},
	{"text7", LAYER_TEXT7},
	{"text8", LAYER_TEXT8},
};

/* Registered Anime Files */
static char *reg_anime_file[REG_ANIME_COUNT];

/* ロード中の情報 */
static int cur_seq_layer;
static uint64_t cur_sw;
static bool *used_layer_tbl;

/*
 * 前方参照
 */
static bool start_sequence(const char *name);
static bool on_key_value(const char *key, const char *val);
static int layer_name_to_index(const char *name);
static float calc_pos_x_from(int anime_layer, int index, const char *value);
static float calc_pos_x_to(int anime_layer, int index, const char *value);
static float calc_pos_y_from(int anime_layer, int index, const char *value);
static float calc_pos_y_to(int anime_layer, int index, const char *value);
static bool load_anime_file(const char *file);
static bool update_layer_params(int layer);
static void synthesis_eye_anime(int chpos);

/*
 * アニメーションサブシステムに関する初期化処理を行う
 */
bool init_anime(void)
{
#ifdef USE_DLL
	/* DLLが再利用されたときのために初期化する */
	cleanup_anime();
#endif
	return true;
}

/*
 * アニメーションサブシステムに関する終了処理を行う
 */
void cleanup_anime(void)
{
	int i, j;

	for (i = 0; i < STAGE_LAYERS; i++) {
		memset(sequence, 0, sizeof(sequence));
		for (j = 0 ; j < SEQUENCE_COUNT; j++) {
			sequence[i][j].from_scale_x = 1.0f;
			sequence[i][j].from_scale_y = 1.0f;
			sequence[i][j].to_scale_x = 1.0f;
			sequence[i][j].to_scale_y = 1.0f;
		}
	}
	memset(context, 0, sizeof(context));

	for (i = 0; i < REG_ANIME_COUNT; i++) {
		if (reg_anime_file[i] != NULL) {
			free(reg_anime_file[i]);
			reg_anime_file[i] = NULL;
		}
	}
}

/*
 * アニメーションファイルを読み込む
 */
bool load_anime_from_file(const char *fname, int reg_index, bool *used_layer)
{
	/* Load the anime file. */
	cur_seq_layer = -1;
	reset_lap_timer(&cur_sw);
	used_layer_tbl = used_layer;
	if (used_layer_tbl != NULL)
		memset(used_layer_tbl, 0, sizeof(bool) * STAGE_LAYERS);
	if (!load_anime_file(fname))
		return false;

	/* Register an anime file for looping. */
	if (reg_index != -1) {
		if (reg_anime_file[reg_index] != NULL) {
			free(reg_anime_file[reg_index]);
			reg_anime_file[reg_index] = NULL;
		}
		reg_anime_file[reg_index] = strdup(fname);
		if (reg_anime_file[reg_index] == NULL) {
			log_memory();
			return false;
		}
	}

	return true;
}

/*
 * アニメーションシーケンスをクリアする
 */
void clear_layer_anime_sequence(int layer)
{
	int i;

	assert(layer >= 0 && layer < STAGE_LAYERS);

	if (context[layer].seq_count > 0) {
		struct sequence *s;
		s = &sequence[layer][context[layer].seq_count - 1];
		set_layer_position(layer, (int)s->to_x, (int)s->to_y);
		set_layer_scale(layer, s->to_scale_x, s->to_scale_y);
		set_layer_alpha(layer, (int)s->to_a);
		set_layer_center(layer, (int)s->center_x, (int)s->center_y);
		set_layer_rotate(layer, s->to_rotate * (3.14159265f / 180.0f));
		set_layer_blend(layer, s->blend);
	}

	context[layer].seq_count = 0;
	context[layer].is_running = false;
	context[layer].is_finished = false;
	context[layer].loop_len = 0;
	context[layer].loop_ofs = 0;
	if (context[layer].file != NULL) {
		free(context[layer].file);
		context[layer].file = NULL;
	}

	for (i = 0 ; i < SEQUENCE_COUNT; i++) {
		memset(&sequence[layer][i], 0, sizeof(struct sequence));
		sequence[layer][i].from_scale_x = 1.0f;
		sequence[layer][i].from_scale_y = 1.0f;
		sequence[layer][i].to_scale_x = 1.0f;
		sequence[layer][i].to_scale_y = 1.0f;
	}
}

/*
 * アニメーションシーケンスをクリアする
 */
void clear_all_anime_sequence(void)
{
	int i;

	for (i = 0; i < STAGE_LAYERS; i++)
		clear_layer_anime_sequence(i);
}

/*
 * アニメーションシーケンスを開始する
 */
bool new_anime_sequence(int layer)
{
	struct sequence *s;

	assert(layer >= 0 && layer < STAGE_LAYERS);

	if (context[layer].seq_count >= SEQUENCE_COUNT)
		return false;

	cur_seq_layer = layer;

	s = &sequence[layer][context[layer].seq_count];
	memset(s, 0, sizeof(struct sequence));
	s->from_scale_x = 1.0f;
	s->from_scale_y = 1.0f;
	s->to_scale_x = 1.0f;
	s->to_scale_y = 1.0f;

	context[layer].seq_count++;
	context[layer].is_running = false;
	context[layer].is_finished = false;
	context[layer].loop_ofs = 0;
	context[layer].loop_len = 0;

	return true;
}

/*
 * アニメーションシーケンスにプロパティを追加する(float)
 */
bool add_anime_sequence_property_f(const char *key, float val)
{
	char s[128];

	snprintf(s, sizeof(s), "%f", val);

	if (!on_key_value(key, s))
		return false;

	return true;
}

/*
 * アニメーションシーケンスにプロパティを追加する(int)
 */
bool add_anime_sequence_property_i(const char *key, int val)
{
	char s[128];

	snprintf(s, sizeof(s), "%d", val);

	if (!on_key_value(key, s))
		return false;

	return true;
}

/*
 * 指定したレイヤのアニメーションを開始する
 */
bool start_layer_anime(int layer)
{
	assert(layer >= 0 && layer < STAGE_LAYERS);

	if (context[layer].seq_count == 0)
		return true;

	reset_lap_timer(&context[layer].sw);
	context[layer].is_running = true;
	context[layer].is_finished = false;
	context[layer].cur_lap = 0;
	context[layer].loop_ofs = 0;
	context[layer].loop_len =
		sequence[layer][context[layer].seq_count - 1].loop ?
		(uint64_t)(sequence[layer][context[layer].seq_count - 1].end_time * 1000.0f) :
		0;

	return true;
}

/*
 * 実行中のアニメーションがあるか調べる
 */
bool is_anime_running(void)
{
	int i;

	for (i = 0; i < STAGE_LAYERS; i++) {
		if (context[i].is_running)
			return true;
	}
	return false;
}

/*
 * 指定したレイヤーの中にアニメが実行中のものがあるか調べる
 */
bool is_anime_running_with_layer_mask(bool *used_layers)
{
	int i;

	for (i = 0; i < STAGE_LAYERS; i++) {
		if (!used_layers[i])
			continue;
		if (context[i].is_running)
			return true;
	}
	return false;
}

/*
 * 指定したレイヤーのアニメーションが実行中であるか調べる
 */
bool is_anime_finished_for_layer(int layer)
{
	return context[layer].is_finished;
}

/*
 * アニメーションのフレーム時刻を更新し、完了したレイヤにフラグをセットする
 */
void update_anime_frame(void)
{
	int i, last_seq;
	uint64_t lap;

	/* 経過時刻を更新するとともに、アニメの完了を検出する */
	for (i = 0; i < STAGE_LAYERS; i++) {
		if (!context[i].is_running)
			continue;
		if (context[i].is_finished)
			continue;

		lap = get_lap_timer_millisec(&context[i].sw);
		if (context[i].loop_len > 0 &&
		    lap > context[i].loop_ofs + context[i].loop_len) {
			lap -= context[i].loop_ofs;
			lap %= context[i].loop_len;
			lap += context[i].loop_ofs;
		}
		context[i].cur_lap = (float)lap / 1000.0f;

		last_seq = context[i].seq_count - 1;
		assert(last_seq >= 0);

		if (context[i].loop_len == 0 &&
		    context[i].cur_lap >= sequence[i][last_seq].end_time) {
			context[i].is_running = false;
			context[i].is_finished = true;
		}
	}

	/* 各レイヤの描画パラメータを更新する */
	for (i = 0; i < STAGE_LAYERS; i++) {
		/* 下記3つのレイヤはアニメレイヤがない */
		if (i == LAYER_CLICK || i == LAYER_AUTO || i == LAYER_SKIP)
			continue;

		/* レイヤの情報を更新する */
		update_layer_params(i);
	}
}

/*
 * レイヤのパラメータを取得する
 */
static bool update_layer_params(int layer)
{
	struct sequence *s;
	float progress;
	int i, x, y, alpha;
	float scale_x, scale_y, rotate;

	assert(layer >= 0 && layer < STAGE_LAYERS);

	/* シーケンスが定義されていない場合 */
	if (context[layer].seq_count == 0)
		return true;

	/* すでに完了している場合 */
	if (context[layer].is_finished &&
	    context[layer].seq_count > 0) {
		s = &sequence[layer][context[layer].seq_count - 1];
		x = (int)s->to_x;
		y = (int)s->to_y;
		alpha = (int)s->to_a;
		rotate = s->to_rotate;
		set_layer_position(layer, (int)s->to_x, (int)s->to_y);
		set_layer_scale(layer, s->to_scale_x, s->to_scale_y);
		set_layer_alpha(layer, alpha);
		set_layer_center(layer, (int)s->center_x, (int)s->center_y);
		set_layer_rotate(layer, rotate * (3.14159265f / 180.0f));
		set_layer_blend(layer, s->blend);
		if (layer == LAYER_MSG && s->hide_special)
			show_msgbox(false);
		if (layer == LAYER_MSG && s->show_special)
			show_msgbox(true);
		if (layer == LAYER_NAME && s->hide_special)
			show_namebox(false);
		if (layer == LAYER_NAME && s->show_special)
			show_namebox(true);
		return true;
	}

	/* 補間を行う */
	for (i = 0; i < context[layer].seq_count; i++) {
		s = &sequence[layer][i];
		if (context[layer].cur_lap < s->start_time)
			continue;
		if (i != context[layer].seq_count - 1 &&
		    context[layer].cur_lap > s->end_time)
			continue;

		/* 進捗率を計算する */
		progress = (context[layer].cur_lap - s->start_time) /
			(s->end_time - s->start_time);
		if (progress > 1.0f)
			progress = 1.0f;

		/* 加速を処理する */
		switch (s->accel) {
		case ANIME_ACCEL_UNIFORM:
			break;
		case ANIME_ACCEL_ACCEL:
			progress = progress * progress;
			break;
		case ANIME_ACCEL_DEACCEL:
			progress = sqrtf(progress);
			break;
		default:
			assert(INVALID_ACCEL_TYPE);
			break;
		}

		/* パラメータを計算する */
		x = (int)(s->from_x + (s->to_x - s->from_x) * progress);
		y = (int)(s->from_y + (s->to_y - s->from_y) * progress);
		scale_x = s->from_scale_x + (s->to_scale_x - s->from_scale_x) * progress;
		scale_y = s->from_scale_y + (s->to_scale_y - s->from_scale_y) * progress;
		rotate = s->from_rotate + (s->to_rotate - s->from_rotate) * progress;
		alpha = (int)(s->from_a + (s->to_a - s->from_a) * progress);
		set_layer_position(layer, x, y);
		set_layer_center(layer, (int)s->center_x, (int)s->center_y);
		set_layer_alpha(layer, alpha);
		set_layer_scale(layer, scale_x, scale_y);
		set_layer_rotate(layer, rotate * (3.14159265f / 180.0f));
		set_layer_blend(layer, s->blend);
		set_layer_frame(layer, s->frame);
		if (layer == LAYER_MSG && s->hide_special)
			show_msgbox(false);
		if (layer == LAYER_MSG && s->show_special)
			show_msgbox(true);
		if (layer == LAYER_NAME && s->hide_special)
			show_namebox(false);
		if (layer == LAYER_NAME && s->show_special)
			show_namebox(true);
		break;
	}

	return true;
}

/*
 * アニメーションファイルの読み込み
 */

/* シーケンス(ブロック)が開始されたときに呼び出される */
static bool start_sequence(const char *name)
{
	UNUSED_PARAMETER(name);
	cur_seq_layer = -1;
	return true;
}

/* キーバリューペアが出現したときに呼び出される */
static bool on_key_value(const char *key, const char *val)
{
	struct sequence *s;
	int top;

	/* 最初にレイヤのキーが指定される必要がある */
	if (strcmp(key, "layer") == 0) {
		/* レイヤを求める */
		cur_seq_layer = layer_name_to_index(val);
		if (cur_seq_layer == -1) {
			log_invalid_layer_name(val);
			return false;
		}

		/* レイヤの最初のシーケンスであれば、シーケンスをクリアする */
		if (used_layer_tbl != NULL) {
			if (!used_layer_tbl[cur_seq_layer])
				clear_layer_anime_sequence(cur_seq_layer);
			used_layer_tbl[cur_seq_layer] = true;
		}

		/* トップのシーケンスを求めて、スケールを1.0fにする */
		s = &sequence[cur_seq_layer][context[cur_seq_layer].seq_count];
		s->from_scale_x = 1.0f;
		s->from_scale_y = 1.0f;
		s->to_scale_x = 1.0f;
		s->to_scale_y = 1.0f;

		/* レイヤの実行状態を設定する */
		context[cur_seq_layer].seq_count++;
		context[cur_seq_layer].sw = cur_sw;
		context[cur_seq_layer].is_running = true;
		context[cur_seq_layer].is_finished = false;
		context[cur_seq_layer].loop_len = 0;
		context[cur_seq_layer].loop_ofs = 0;

		return true;
	}
	if (cur_seq_layer == -1) {
		log_anime_layer_not_specified(key);
		return false;
	}

	/* レイヤのシーケンス長をチェックする */
	top = context[cur_seq_layer].seq_count - 1;
	if (top == SEQUENCE_COUNT) {
		log_anime_long_sequence();
		return false;
	}
	assert(top >= 0);
	assert(top < SEQUENCE_COUNT);

	/* その他のキーのとき */
	s = &sequence[cur_seq_layer][top];
	if (strcmp(key, "start") == 0) {
		s->start_time = (float)atof(val);
	} else if (strcmp(key, "end") == 0) {
		s->end_time = (float)atof(val);
	} else if (strcmp(key, "from-x") == 0) {
		s->from_x = calc_pos_x_from(cur_seq_layer, top, val);
	} else if (strcmp(key, "from-y") == 0) {
		s->from_y = calc_pos_y_from(cur_seq_layer, top, val);
	} else if (strcmp(key, "from-a") == 0) {
		s->from_a = (float)atoi(val);
	} else if (strcmp(key, "from-scale-x") == 0) {
		s->from_scale_x = (float)atof(val);
	} else if (strcmp(key, "from-scale-y") == 0) {
		s->from_scale_y = (float)atof(val);
	} else if (strcmp(key, "to-x") == 0) {
		s->to_x = calc_pos_x_to(cur_seq_layer, top, val);
	} else if (strcmp(key, "to-y") == 0) {
		s->to_y = calc_pos_y_to(cur_seq_layer, top, val);
	} else if (strcmp(key, "to-a") == 0) {
		s->to_a = (float)atoi(val);
	} else if (strcmp(key, "to-scale-x") == 0) {
		s->to_scale_x = (float)atof(val);
	} else if (strcmp(key, "to-scale-y") == 0) {
		s->to_scale_y = (float)atof(val);
	} else if (strcmp(key, "from-rotate") == 0) {
		s->from_rotate = (float)atof(val);
	} else if (strcmp(key, "to-rotate") == 0) {
		s->to_rotate = (float)atof(val);
	} else if (strcmp(key, "center-x") == 0) {
		s->center_x = (float)atof(val);
	} else if (strcmp(key, "center-y") == 0) {
		s->center_y = (float)atof(val);
	} else if (strcmp(key, "blend") == 0) {
		s->blend = atoi(val);
	} else if (strcmp(key, "accel") == 0) {
		if (strcmp(val, "normal") == 0)
			s->accel = 0;
		else if (strcmp(val, "accel") == 0)
			s->accel = 1;
		else if (strcmp(val, "brake") == 0)
			s->accel = 2;
		else
			s->accel = atoi(val);
	} else if (strcmp(key, "loop") == 0) {
		s->loop = true;
		context[cur_seq_layer].loop_ofs = (uint64_t)(atof(val) * 1000.0f);
		context[cur_seq_layer].loop_len = (uint64_t)(s->end_time * 1000.0f) - context[cur_seq_layer].loop_ofs;
	} else if (strcmp(key, "frame") == 0) {
		s->frame = atoi(val);
	} else if (strcmp(key, "hide-special") == 0) {
		s->hide_special = true;
	} else if (strcmp(key, "show-special") == 0) {
		s->show_special = true;
	} else {
		log_anime_unknown_key(key);
		return false;
	}

	return true;
}

/* レイヤ名をレイヤインデックスに変換する */
static int layer_name_to_index(const char *name)
{
	int i;

	for (i = 0;
	     i < (int)(sizeof(layer_name_map) / sizeof(struct layer_name_map));
	     i++) {
		if (strcmp(layer_name_map[i].name, name) == 0)
			return layer_name_map[i].index;
	}
	return -1;
}

/* 座標を計算する */
static float calc_pos_x_from(int layer, int index, const char *value)
{
	float ret;

	assert(value != NULL);

	if (value[0] == '+') {
		if (index == 0)
			ret = (float)get_layer_x(layer);
		else
			ret = sequence[layer][index - 1].to_x;
		ret += (float)atoi(value + 1);
	} else {
		ret = (float)atoi(value);
	}

	return ret;
}

/* 座標を計算する */
static float calc_pos_x_to(int layer, int index, const char *value)
{
	float ret;

	assert(value != NULL);

	if (value[0] == '+')
		ret = sequence[layer][index].from_x + (float)atoi(value + 1);
	else
		ret = (float)atoi(value);

	return ret;
}

/* 座標を計算する */
static float calc_pos_y_from(int anime_layer, int index, const char *value)
{
	float ret;

	assert(value != NULL);

	if (value[0] == '+') {
		if (index == 0)
			ret = (float)get_layer_y(anime_layer);
		else
			ret = sequence[anime_layer][index - 1].to_y;
		ret += (float)atoi(value + 1);
	} else {
		ret = (float)atoi(value);
	}

	return ret;
}

/* 座標を計算する */
static float calc_pos_y_to(int anime_layer, int index, const char *value)
{
	float ret;

	assert(value != NULL);

	if (value[0] == '+')
		ret = sequence[anime_layer][index].from_y + (float)atoi(value + 1);
	else
		ret = (float)atoi(value);

	return ret;
}

/*
 * ループアニメの登録を解除する
 */
void unregister_anime(int reg_index)
{
	free(reg_anime_file[reg_index]);
	reg_anime_file[reg_index] = NULL;
}

/*
 * ループアニメファイル名を取得する
 */
const char *get_reg_anime_file_name(int reg_index)
{
	return reg_anime_file[reg_index];
}

/*
 * 目パチ
 */

/*
 * 目パチ画像をロードする
 */
bool load_eye_image_if_exists(int chpos, const char *fname)
{
	char eye_fname[1024], *slash, *dot;
	struct image *eye_img;
	int eye_layer;

	/* Firstly, disable the eye layer. */
	eye_layer = chpos_to_eye_layer(chpos);
	set_layer_file_name(eye_layer, NULL);
	set_layer_image(eye_layer, NULL);

	/* In case that we erase the character. */
	if (fname == NULL || strcmp(fname, "none") == 0 || strcmp(fname, U8("消去")) == 0) {
		/* Finish the existing eye anime. */
		clear_layer_anime_sequence(eye_layer);
		return true;
	}

	/* Make an eye file name. (base + "/eye/" + file) */
	slash = strrchr(fname, '/');
	if (slash != NULL) {
		char base[1024], file[1024];

		strcpy(base, fname);
		slash = strrchr(base, '/');
		*slash = '\0';
		strcpy(file, slash + 1);

		strcpy(eye_fname, base);
		strcat(eye_fname, "/eye/");
		strcat(eye_fname, file);
	} else {
		strcpy(eye_fname, "eye/");
		strcat(eye_fname, fname);
	}

	/* Check if the file name includes extension. */
	dot = strstr(eye_fname, ".");
	if (dot != NULL) {
		/* There is an extension. */
		if (!check_file_exist(CH_DIR, eye_fname)) {
			/* No such file. */
			return true;
		}
	} else {
		/* There is no extension. */
		do {
			char tmp[1024];

			strcpy(tmp, eye_fname);
			strcat(tmp, ".png");
			if (check_file_exist(CH_DIR, tmp)) {
				strcpy(eye_fname, tmp);
				break;
			}
			strcpy(tmp, eye_fname);
			strcat(tmp, ".webp");
			if (check_file_exist(CH_DIR, tmp)) {
				strcpy(eye_fname, tmp);
				break;
			}

			/* No such file. */
			return true;
		} while (0);
	}

	/* Load an image. */
	eye_img = create_image_from_file(CH_DIR, eye_fname);
	if (eye_img == NULL) {
		log_script_exec_footer();
		return false;
	}

	/* Initialize the layer. */
	set_layer_file_name(eye_layer, eye_fname);
	set_layer_image(eye_layer, eye_img);
	set_layer_alpha(eye_layer, 0);
	set_layer_scale(eye_layer, 1.0f, 1.0f);

	/* Make an eye anime. */
	synthesis_eye_anime(chpos);

	return true;
}

/*
 * 目パチアニメを再ロードする
 */
bool reload_eye_anime(int chpos)
{
	int eye_layer;

	/* キャラがない場合 */
	eye_layer = chpos_to_eye_layer(chpos);
	if (get_layer_image(eye_layer) == NULL)
		return true;

	/* 目パチのアニメを合成する */
	synthesis_eye_anime(chpos);

	return true;
}

/* 目パチのアニメを合成する */
static void synthesis_eye_anime(int chpos)
{
	float ofs_time, base_time;
	int x, y, i, frame_count, repeat_count;
	int base_layer, eye_layer;

	base_layer = chpos_to_layer(chpos);
	eye_layer = chpos_to_eye_layer(chpos);

	if (get_layer_image(base_layer) == NULL)
		return;
	if (get_layer_image(eye_layer) == NULL)
		return;

	/* キャラの座標を取得する */
	x = get_layer_x(base_layer);
	y = get_layer_y(base_layer);

	/* 目パチレイヤーの位置を設定する */
	set_layer_position(eye_layer, x, y);

	/* 目パチのアニメを開始する */
	frame_count = get_layer_image(eye_layer)->width / get_layer_image(base_layer)->width;
	base_time = conf_character_eyeblink_interval + conf_character_eyeblink_interval * 0.3f * (2.0f * (float)rand() / (float)RAND_MAX - 1.0f);
	ofs_time = base_time;
	clear_layer_anime_sequence(eye_layer);
	new_anime_sequence(eye_layer);
	add_anime_sequence_property_f("start",	0);
	add_anime_sequence_property_f("end",	ofs_time);
	add_anime_sequence_property_i("from-x",	x);
	add_anime_sequence_property_i("from-y",	y);
	add_anime_sequence_property_i("to-x",	x);
	add_anime_sequence_property_i("to-y",	y);
	for (repeat_count = 10; repeat_count > 0; repeat_count--) {
		int blink_times;

		/* 10回に2回程度は2回まばたき、それ以外は1回まばたきする */
		for (blink_times = rand() % 10 >= 2 ? 1 : 2; blink_times > 0; blink_times--) {
			for (i = 0; i < frame_count; i++) {
				new_anime_sequence(eye_layer);
				add_anime_sequence_property_f("start",	ofs_time);
				ofs_time += conf_character_eyeblink_frame * (float)(i + 1);
				add_anime_sequence_property_f("end",	ofs_time);
				add_anime_sequence_property_i("from-x",	x);
				add_anime_sequence_property_i("from-y",	y);
				add_anime_sequence_property_i("from-a",	255);
				add_anime_sequence_property_i("to-x",	x);
				add_anime_sequence_property_i("to-y",	y);
				add_anime_sequence_property_i("to-a",	255);
				add_anime_sequence_property_i("frame",	i);
			}
		}

		/* 目を開ける */
		if (repeat_count != 1) {
			new_anime_sequence(eye_layer);
			add_anime_sequence_property_f("start",	ofs_time);
			ofs_time += base_time;
			add_anime_sequence_property_f("end",	ofs_time);
			add_anime_sequence_property_i("from-x",	x);
			add_anime_sequence_property_i("from-y",	y);
			add_anime_sequence_property_i("to-x",	x);
			add_anime_sequence_property_i("to-y",	y);
			add_anime_sequence_property_i("frame",	0);
		}
	}
	add_anime_sequence_property_i("loop", 0);

	/* アニメを開始する */
	start_layer_anime(eye_layer);
}

/*
 * 口パク
 */

/*
 * 口パク画像をロードする
 */
bool load_lip_image_if_exists(int chpos, const char *fname)
{
	char lip_fname[1024], *slash, *dot;
	struct image *lip_img;
	int lip_layer;

	/* Firstly, disable the lip layer. */
	lip_layer = chpos_to_lip_layer(chpos);
	set_layer_file_name(lip_layer, NULL);
	set_layer_image(lip_layer, NULL);

	/* In case that we erase the character. */
	if (fname == NULL || strcmp(fname, "none") == 0 || strcmp(fname, U8("消去")) == 0) {
		/* Finish the existing lip anime. */
		clear_layer_anime_sequence(lip_layer);
		return true;
	}

	/* Make a lip file name. (base + "/lip/" + file) */
	slash = strrchr(fname, '/');
	if (slash != NULL) {
		char base[1024], file[1024];

		strcpy(base, fname);
		slash = strrchr(base, '/');
		*slash = '\0';
		strcpy(file, slash + 1);

		strcpy(lip_fname, base);
		strcat(lip_fname, "/lip/");
		strcat(lip_fname, file);
	} else {
		strcpy(lip_fname, "lip/");
		strcat(lip_fname, fname);
	}

	/* Check if the file name includes extension. */
	dot = strstr(lip_fname, ".");
	if (dot != NULL) {
		/* There is an extension. */
		if (!check_file_exist(CH_DIR, lip_fname)) {
			/* No such file. */
			return true;
		}
	} else {
		/* There is no extension. */
		do {
			char tmp[1024];

			strcpy(tmp, lip_fname);
			strcat(tmp, ".png");
			if (check_file_exist(CH_DIR, tmp)) {
				strcpy(lip_fname, tmp);
				break;
			}
			strcpy(tmp, lip_fname);
			strcat(tmp, ".webp");
			if (check_file_exist(CH_DIR, tmp)) {
				strcpy(lip_fname, tmp);
				break;
			}

			/* No such file. */
			return true;
		} while (0);
	}

	/* Load an image. */
	lip_img = create_image_from_file(CH_DIR, lip_fname);
	if (lip_img == NULL) {
		log_script_exec_footer();
		return false;
	}

	/* レイヤを設定する */
	set_layer_file_name(lip_layer, lip_fname);
	set_layer_image(lip_layer, lip_img);
	set_layer_alpha(lip_layer, 0);
	set_layer_scale(lip_layer, 1.0f, 1.0f);

	return true;
}

/*
 * 口パクのアニメを実行する
 */
void run_lip_anime(int chpos, const char *msg)
{
	float ofs_time, base_time;
	int i, x, y, base_layer, lip_layer, frame_count, word_count;
	int WORD_COUNT;

	base_layer = chpos_to_layer(chpos);
	lip_layer = chpos_to_lip_layer(chpos);

	if (get_layer_image(base_layer) == NULL)
		return;
	if (get_layer_image(lip_layer) == NULL)
		return;

	/* キャラの座標を取得する */
	x = get_layer_x(base_layer);
	y = get_layer_y(base_layer);

	/* 口パクレイヤーの位置を設定する */
	set_layer_position(lip_layer, x, y);

	/* 何文字ごとに口パクするかを決める */
	WORD_COUNT = conf_character_lipsync_chars == 0 ? 3 : conf_character_lipsync_chars;

	/* 口パクのアニメを開始する */
	frame_count = get_layer_image(lip_layer)->width / get_layer_image(base_layer)->width;
	base_time = conf_character_lipsync_frame == 0 ? 0.02f : conf_character_lipsync_frame;
	ofs_time = 0;
	word_count = WORD_COUNT;
	clear_layer_anime_sequence(lip_layer);
	while (msg) {
		uint32_t wc;
		int n;

		n = utf8_to_utf32(msg, &wc);
		if (n <= 0)
			break;
		msg += n;

		if (wc == U32_C('、', 0x3001) || wc == U32_C('。', 0x3002)) {
			ofs_time += base_time * 10;
			word_count = WORD_COUNT;
			continue;
		}

		if (wc == U32_C('、', 0x3001) || wc == U32_C('。', 0x3002) ||
		    wc == U32_C('！', 0xff01) || wc == U32_C('？', 0xff1f) ||
		    wc == U32_C('・', 0x30fb) || wc == U32_C('…', 0x2026) ||
		    wc == U32_C('―', 0x2015)) {
			ofs_time += base_time * 20;
			word_count = WORD_COUNT;
			continue;
		}

		/* (WORD_COUNT)文字ごとに口パクする */
		if (word_count == 0)
			word_count = WORD_COUNT;
		if (word_count-- != WORD_COUNT)
			continue;

		/* 正順で口パクを表示する */
		for (i = 0; i < frame_count; i++) {
			new_anime_sequence(lip_layer);
			add_anime_sequence_property_f("start",	ofs_time);
			ofs_time += base_time;
			add_anime_sequence_property_f("end",	ofs_time);
			add_anime_sequence_property_i("from-x",	x);
			add_anime_sequence_property_i("from-y",	y);
			add_anime_sequence_property_i("from-a",	255);
			add_anime_sequence_property_i("to-x",	x);
			add_anime_sequence_property_i("to-y",	y);
			add_anime_sequence_property_i("to-a",	255);
			add_anime_sequence_property_i("frame",	i);
			ofs_time += base_time;
		}

		/* 3フレーム以上ある場合、逆順で口パクを表示する */
		if (frame_count > 2) {
			for (i = frame_count - 1; i >= 0; i--) {
				new_anime_sequence(lip_layer);
				add_anime_sequence_property_f("start",	ofs_time);
				ofs_time += base_time;
				add_anime_sequence_property_f("end",	ofs_time);
				add_anime_sequence_property_i("from-x",	x);
				add_anime_sequence_property_i("from-y",	y);
				add_anime_sequence_property_i("from-a",	255);
				add_anime_sequence_property_i("to-x",	x);
				add_anime_sequence_property_i("to-y",	y);
				add_anime_sequence_property_i("to-a",	255);
				add_anime_sequence_property_i("frame",	i);
				ofs_time += base_time;
			}
		}
	}

	/* 口パクを非表示にする */
	new_anime_sequence(lip_layer);
	add_anime_sequence_property_f("start",	ofs_time);
	ofs_time += base_time;
	add_anime_sequence_property_f("end",	ofs_time);
	add_anime_sequence_property_i("from-x",	x);
	add_anime_sequence_property_i("from-y",	y);
	add_anime_sequence_property_i("from-a",	0);
	add_anime_sequence_property_i("to-x",	x);
	add_anime_sequence_property_i("to-y",	y);
	add_anime_sequence_property_i("to-a",	0);
	add_anime_sequence_property_i("frame",	0);

	/* アニメを開始する */
	start_layer_anime(lip_layer);
}

/*
 * 口パクのアニメを停止する
 */
void stop_lip_anime(int chpos)
{
	int lip_layer;

	lip_layer = chpos_to_lip_layer(chpos);

	clear_layer_anime_sequence(lip_layer);
}

/*
 * アニメファイルのパース
 */

/* アニメーションファイルをロードする */
static bool load_anime_file(const char *file)
{
	enum {
		ST_SCOPE,
		ST_OPEN,
		ST_KEY,
		ST_COLON,
		ST_VALUE,
		ST_VALUE_DQ,
		ST_SEMICOLON,
		ST_ERROR
	};

	char word[256], key[256];
	struct rfile *rf;
	char *buf;
	size_t fsize, pos;
	int st, len, line;
	char c;
	bool is_comment;

	assert(file != NULL);

	/* ファイルをオープンする */
	rf = open_rfile(ANIME_DIR, file, false);
	if (rf == NULL)
		return false;

	/* ファイルサイズを取得する */
	fsize = get_rfile_size(rf);

	/* メモリを確保する */
	buf = malloc(fsize);
	if (buf == NULL) {
		log_memory();
		close_rfile(rf);
		return false;
	}

	/* ファイルを読み込む */
	if (read_rfile(rf, buf, fsize) < fsize) {
		log_file_read(GUI_DIR, file);
		close_rfile(rf);
		free(buf);
		return false;
	}

	/* コメントをスペースに変換する */
	is_comment = false;
	for (pos = 0; pos < fsize; pos++) {
		if (!is_comment) {
			if (buf[pos] == '#') {
				buf[pos] = ' ';
				is_comment = true;
			}
		} else {
			if (buf[pos] == '\n')
				is_comment = false;
			else
				buf[pos] = ' ';
		}
	}

	/* ファイルをパースする */
	st = ST_SCOPE;
	line = 0;
	len = 0;
	pos = 0;
	while (pos < fsize) {
		/* 1文字読み込む */
		c = buf[pos++];

		/* ステートに応じて解釈する */
		switch (st) {
		case ST_SCOPE:
			if (len == 0) {
				if (c == ' ' || c == '\t' || c == '\r' ||
				    c == '\n') {
					st = ST_SCOPE;
					break;
				}
				if (c == ':' || c == '{' || c == '}') {
					log_anime_parse_char(c);
					st = ST_ERROR;
					break;
				}
			}
			if (c == '}' || c == ':') {
				log_anime_parse_char(c);
				st = ST_ERROR;
				break;
			}
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n' ||
			    c == '{') {
				assert(len > 0);
				word[len] = '\0';
				if (!start_sequence(word)) {
					st = ST_ERROR;
					break;
				}
				if (c == '{')
					st = ST_KEY;
				else
					st = ST_OPEN;
				len = 0;
				break;
			}
			if (len == sizeof(word) - 1) {
				log_anime_parse_long_word();
				st = ST_ERROR;
				break;
			}
			word[len++] = c;
			st = ST_SCOPE;
			break;
		case ST_OPEN:
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
				st = ST_OPEN;
				break;
			}
			if (c == '{') {
				st = ST_KEY;
				len = 0;
				break;
			}
			log_anime_parse_char(c);
			st = ST_ERROR;
			break;
		case ST_KEY:
			if (len == 0) {
				if (c == ' ' || c == '\t' || c == '\r' ||
				    c == '\n') {
					st = ST_KEY;
					break;
				}
				if (c == ':') {
					log_anime_parse_char(c);
					st = ST_ERROR;
					break;
				}
				if (c == '}') {
					st = ST_SCOPE;
					break;
				}
			}
			if (c == '{' || c == '}') {
				log_anime_parse_char(c);
				st = ST_ERROR;
				break;
			}
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
				word[len] = '\0';
				strcpy(key, word);
				st = ST_COLON;
				len = 0;
				break;
			}
			if (c == ':') {
				word[len] = '\0';
				strcpy(key, word);
				st = ST_VALUE;
				len = 0;
				break;
			}
			if (len == sizeof(word) - 1) {
				log_anime_parse_long_word();
				st = ST_ERROR;
				break;
			}
			word[len++] = c;
			st = ST_KEY;
			break;
		case ST_COLON:
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
				st = ST_COLON;
				break;
			}
			if (c == ':') {
				st = ST_VALUE;
				len = 0;
				break;
			}
			log_anime_parse_char(c);
			st = ST_ERROR;
			break;
		case ST_VALUE:
			if (len == 0) {
				if (c == ' ' || c == '\t' || c == '\r' ||
				    c == '\n') {
					st = ST_VALUE;
					break;
				}
				if (c == '\"') {
					st = ST_VALUE_DQ;
					break;
				}
			}
			if (c == ':' || c == '{') {
				log_anime_parse_char(c);
				st = ST_ERROR;
				break;
			}
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n' ||
			    c == ';' || c == '}') {
				word[len] = '\0';
				if (!on_key_value(key, word)) {
					st = ST_ERROR;
					break;
				}
				if (c == ';')
					st = ST_KEY;
				else if (c == '}')
					st = ST_SCOPE;
				else
					st = ST_SEMICOLON;
				len = 0;
				break;
			}
			if (len == sizeof(word) - 1) {
				log_anime_parse_long_word();
				st = ST_ERROR;
				break;
			}
			word[len++] = c;
			st = ST_VALUE;
			break;
		case ST_VALUE_DQ:
			if (c == '\"') {
				word[len] = '\0';
				if (!on_key_value(key, word)) {
					st = ST_ERROR;
					break;
				}
				st = ST_SEMICOLON;
				len = 0;
				break;
			}
			if (c == '\r' || c == '\n') {
				log_anime_parse_char(c);
				st = ST_ERROR;
				break;
			}
			if (len == sizeof(word) - 1) {
				log_anime_parse_long_word();
				st = ST_ERROR;
				break;
			}
			word[len++] = c;
			st = ST_VALUE_DQ;
			break;
		case ST_SEMICOLON:
			if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
				st = ST_SEMICOLON;
				break;
			}
			if (c == ';') {
				st = ST_KEY;
				len = 0;
				break;
			}
			if (c == '}') {
				st = ST_SCOPE;
				len = 0;
				break;
			}
			log_anime_parse_char(c);
			st = ST_ERROR;
			break;
		}

		/* エラー時 */
		if (st == ST_ERROR)
			break;

		/* FIXME: '\r'のみの改行を処理する */
		if (c == '\n')
			line++;
	}

	/* エラーが発生した場合 */
	if (st == ST_ERROR) {
		log_anime_parse_footer(file, line);
	} else if (st != ST_SCOPE || len > 0) {
		log_anime_parse_invalid_eof();
	}

	/* バッファを解放する */
	free(buf);

	/* ファイルをクローズする */
	close_rfile(rf);

	return st != ST_ERROR;
}
