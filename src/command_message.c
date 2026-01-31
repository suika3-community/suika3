/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * OpenNovel
 * Copyright (c) 2001-2024, OpenNovel.org. All rights reserved.
 */

/*
 * The implementation of the message and the line commands.
 */

/*
 * [Memo]
 *  - システムGUIから戻った場合は、すでに描画されているメッセージレイヤをそのまま流用する (gui_sys_flag)
 */

#include "opennovel.h"

/* Message speed (x letters/second) */
#define MESSAGE_SPEED	(80.0f)

/*
 * オートモードでボイスありのときの待ち時間
 *  - 表示と再生の完了からどれくらい待つかの基本時間
 *  - 実際にはオートモードスピードの係数を乗算して使用する
 */
#define AUTO_MODE_VOICE_WAIT		(4000)

/*
 * オートモードでボイスなしのときの待ち時間係数のデフォルト値
 *  - automode.speed=0 のときに使用される
 *  - 文字数に乗算して使用する
 */
#define AUTO_MODE_TEXT_WAIT_SCALE	(0.15f)

/* メッセージボックス内のボタンのインデックス */
#define BTN_NONE		(-1)
#define BTN_SAVE		(0)
#define BTN_LOAD		(1)
#define BTN_AUTO		(2)
#define BTN_SKIP		(3)
#define BTN_HISTORY		(4)
#define BTN_CONFIG		(5)
#define BTN_HIDE		(6)
#define MIN_BTN			(0)
#define MAX_BTN			(6)

/*
 * 文字列バッファ
 */

/* 描画する名前 */
static char *name_top;

/* 変数展開前の名前 */
static const char *raw_name;

/*
 * 描画する本文 (バッファの先頭)
 *  - 文字列の内容は変数の値を展開した後のもの
 *  - 行継続の場合、先頭の'\\'は含まれない
 *  - 行継続の場合、先頭の連続する"\\n"は含まれない
 */
static char *msg_top;

/*
 * 描画状態
 */

/* 本文の描画状態 */
static struct draw_msg_context msgbox_context;

/* スキップによりすべて描画するか(インラインウェイトの回避) */
static bool do_draw_all;

/*
 * 描画する文字の総数
 *  - エスケープシーケンスを除外した合計の表示文字数
 *  - Unicodeの合成はサポートしていない
 *    - 基底文字+結合文字はそれぞれ1文字としてカウントする
 */
static int total_chars;

/* すでに描画した文字数 (0 <= drawn_chars <= total_chars) */
static int drawn_chars;

/*
 * 描画位置
 */

/*
 * 現在の描画位置
 *  - メッセージボックス内の座標
 *  - 他のコマンドに移ったり、GUIから戻ってきた場合も、保持される
 */
static int pen_x;
static int pen_y;

/* 描画開始位置(重ね塗りのため) */
static int orig_pen_x;
static int orig_pen_y;

/* メッセージボックスの位置とサイズ */
static int msgbox_x;
static int msgbox_y;
static int msgbox_w;
static int msgbox_h;

/*
 * 色
 */

/* 本文の文字の色 */
static pixel_t body_color;

/* 本文の文字の縁取りの色 */
static pixel_t body_outline_color;

/* 名前の文字の色 */
static pixel_t name_color;

/* 名前の文字の縁取りの色 */
static pixel_t name_outline_color;

/* 背景色 */
static pixel_t bg_color;

/*
 * 行継続モード
 */

/* 行継続モードであるか */
static bool is_continue_mode;

/* 重ね塗りしているか (将来、色のエスケープシーケンスを無視するため) */
static bool is_dimming;

/* dimを回避するか(ロード時) */
static bool avoid_dimming;

/*
 * ボイス
 */

/* ボイスがあるか */
static bool have_voice;

/* ボイスファイル */
static char *voice_file;

/* ボイスを停止したか */
static bool is_voice_stopped;

/*
 * クリックアニメーション
 */

/* クリックアニメーションの初回描画か */
static bool is_click_first;

/* クリックアニメーションの表示状態 */
static bool is_click_visible;

/* コマンドの経過時刻を表すストップウォッチ */
static uint64_t click_sw;

/*
 * オートモード
 */

/* オートモードでメッセージ表示とボイス再生の完了後の待ち時間中か */
static bool is_auto_mode_wait;

/* オートモードの経過時刻を表すストップウォッチ */
static uint64_t auto_sw;

/*
 * インラインウェイト
 */

/* インラインウェイト中であるか */
static bool is_inline_wait;

/* インラインウェイトの待ち時間 */
static float inline_wait_time;

/* インラインウェイトで待ったトータルの時間 */
static float inline_wait_time_total;

/* インラインウェイトの経過時刻を表すストップウォッチ */
static uint64_t inline_sw;

/*
 * コマンドが開始されたときの状態 
 */

/* ロードによって開始されたか */
static bool load_flag;

/* GUIコマンド終了後の最初のコマンドか */
static bool gui_cmd_flag;

/* システムGUIから復帰したか */
static bool gui_sys_flag;

/* システムGUIのgosubから復帰したか */
static bool gui_gosub_flag;

/*
 * 非表示
 */

/* スペースキーによる非表示が実行中であるか */
static bool is_hidden;

/*
 * メッセージボックス内ボタンの選択
 */
static int pointed_index;

/*
 * システム遷移フラグ
 *  - TODO: cmd_switch.cと共通化する
 */

/* システムメニューモードに遷移するか */
static bool need_sysmenu_mode;

/* セーブ画面に遷移するか */
static bool need_save_mode;

/* ロード画面に遷移するか */
static bool need_load_mode;

/* ヒストリ画面に遷移するか */
static bool need_history_mode;

/* コンフィグ画面に遷移するか */
static bool need_config_mode;

/*
 * dimming
 */

static bool need_dimming;

/*
 * その他
 */

/* ヒストリに残すが表示しない場合 */
static bool no_show;

/*
 * 前方参照
 */

/* 主な処理 */
static bool pre_process(void);
static bool blit_process(void);
static void render_process(void);
static void post_process(void);

/* 初期化 */
static bool init(bool *cont);
static void init_flags_and_vars(void);
static void init_auto_mode(void);
static void init_skip_mode(void);
static void init_colors(void);
static bool init_name_top(void);
static bool init_voice_file(void);
static bool init_msg_top(void);
static const char *skip_lf(const char *m, int *lf);
static void put_space(void);
static bool register_message_for_history(const char *msg);
static char *concat_serif(const char *name, const char *serif);
static void init_msgbox(void);
static bool init_serif(void);
static bool check_play_voice(void);
static bool play_voice(void);
static void set_character_volume_by_name(const char *name);
static void blit_namebox(void);
static void focus_character(void);
static void init_click(void);
static void init_pointed_index(void);
static int get_pointed_button(void);
static void adjust_pointed_index(void);
static void get_button_rect(int btn, int *x, int *y, int *w, int *h);
static void init_repetition(void);
static void speak(void);
static void init_lip_sync(void);
static void cleanup_lip_sync(void);

/* フレーム処理 */
static bool frame_auto_mode(void);
static bool check_auto_play_condition(void);
static int get_wait_time(void);
static bool frame_buttons(void);
static bool process_button_click(void);
static bool draw_buttons(void);
static void action_toggle_hide(void);
static bool frame_sysbtn(void);

/* blit描画処理 */
static void blit_frame(void);
static void blit_pending_message(void);
static bool is_end_of_msg(void);
static void blit_msgbox(void);
static void inline_wait_hook(float wait_time);
static int get_frame_chars(void);
static bool is_canceled_by_skip(void);
static bool is_fast_forward_by_click(void);
static int calc_frame_chars_by_lap(void);
static void set_click(void);
static bool check_stop_click_animation(void);

/* sysbtn */
static void render_sysbtn_extended(void);

/* dimming */
static void blit_dimming(void);

/* その他 */
static void play_se(const char *file);
static bool is_skippable(void);
void check_and_play_bgvoice(void);

/* 終了処理 */
static void stop(void);
static bool cleanup(void);

/*
 * メッセージ・セリフコマンド
 */
bool message_command(bool *cont)
{
	/* 初期化処理を行う */
	if (!is_in_command_repetition()) {
		if (!init(cont))
			return false;
		if (*cont)
			return move_to_next_command();
	}
	if (no_show) {
		render_stage();
		if (!conf_sysbtn_hide)
			render_sysbtn(false);
		move_to_next_command();
		return true;
	}

	if (!pre_process())
		return false;
	if (!blit_process())
		return false;
	render_process();
	post_process();

	/* 終了処理を行う */
	if (!is_in_command_repetition())
		if (!cleanup())
			return false;

	/* 次のフレームへ */
	return true;
}

static bool pre_process(void)
{
	/* インラインウェイトのキャンセルを処理する */
	if (is_inline_wait) {
		if (is_left_clicked || is_right_clicked ||
		    is_control_pressed || is_space_pressed ||
		    is_return_pressed || is_up_pressed || is_down_pressed ||
		    is_page_up_pressed || is_page_down_pressed ||
		    is_escape_pressed) {
			is_inline_wait = false;
			clear_input_state();
			return true;
		}
	}

	/* オートモードを処理する */
	if (frame_auto_mode()) {
		/* 入力がキャプチャされたので、ここでリターンする */
		return true;
	}

	/* メッセージボックス内のボタンを処理する */
	if (frame_buttons()) {
		/* 入力がキャプチャされたので、ここでリターンする */
		return true;
	}

	/* システムボタンを処理する */
	if (frame_sysbtn()) {
		/* 入力がキャプチャされたので、ここでリターンする */
		return true;
	}

	/* 入力はキャプチャされなかったが、エラーはない */
	return true;
}

static bool blit_process(void)
{
	/* dimmingを行う場合 */
	if (need_dimming) {
		blit_dimming();
		/* オート/スキップモードでなければ */
		if (is_in_command_repetition())
			stop_command_repetition();
		return true;
	}

	/* 文字の描画/クリックアニメーションの制御を行う */
	blit_frame();

	/* 必要な場合はステージのサムネイルを作成する (システムGUI遷移) */
	if (need_sysmenu_mode)
		draw_stage_to_thumb();

	/* システムボタンが押された場合 */
	if (need_sysmenu_mode) {
		if (!prepare_gui_mode(SYSMENU_GUI_FILE, true))
			return false;
		start_gui_mode();
		return true;
	}

	/* セーブ/ロード/ヒストリ/コンフィグのボタンが押された場合 */
	if (need_save_mode) {
		if (!prepare_gui_mode(SAVE_GUI_FILE, true))
			return false;
		start_gui_mode();
		need_save_mode = false;
		return true;
	}
	if (need_load_mode) {
		if (!prepare_gui_mode(LOAD_GUI_FILE, true))
			return false;
		start_gui_mode();
		need_load_mode = false;
		return true;
	}
	if (need_history_mode) {
		if (!prepare_gui_mode(HISTORY_GUI_FILE, true))
			return false;
		start_gui_mode();
		need_history_mode = false;
		return true;
	}
	if (need_config_mode) {
		if (!prepare_gui_mode(CONFIG_GUI_FILE, true))
			return false;
		start_gui_mode();
		need_config_mode = false;
		return true;
	}

	return true;
}

static void render_process(void)
{
	/*
	 * GUIへ遷移する場合、このフレームでは、
	 * メッセージコマンドのレンダリングではなく、
	 * GUIのレンダリングを行う
	 */
	if (need_sysmenu_mode) {
		run_gui_mode();
		return;
	}

	render_stage();

	/* システムメニューを描画する */
	if (!conf_sysbtn_hide && !is_hidden) {
		if (!is_auto_mode() && !is_skip_mode())
			render_sysbtn_extended();
	}
}

static void post_process(void)
{
	/* ロードされた場合のフラグをクリアする */
	if (load_flag)
		load_flag = false;

	/* システムGUIへ遷移する場合 */
	if (need_sysmenu_mode)
		stop();

	/* 準備ができた場合BGVoiceを再生する */
	check_and_play_bgvoice();
}

/* 準備ができた場合BGVoiceを再生する */
void check_and_play_bgvoice(void)
{
	struct wave *w;
	const char *file;

	/* 再生が必要ない場合 */
	if (is_bgvoice_playing())
		return;
	if (!have_voice)
		return;
	if (!is_voice_stopped)
		if (!is_mixer_sound_finished(VOICE_STREAM))
			return;

	/* ファイルを取得する */
	file = get_bgvoice();
	if (file == NULL)
		return;

	/* ファイルを抹消する */
	register_bgvoice(NULL);

	/* ファイルをオープンする */
	w = create_wave_from_file(CV_DIR, file, true);
	if (w == NULL)
		return;

	/* 再生する */
	set_mixer_input(VOICE_STREAM, w);

	/* 再生中とする */
	set_bgvoice_playing(true);
}

/*
 * 初期化
 */

/* 初期化処理を行う */
static bool init(bool *cont)
{
	/* ページモードの場合 */
	if (is_page_mode()) {
		const char *s = get_string_param(MESSAGE_PARAM_MESSAGE);
		if (strcasecmp(s, "\\page") == 0 || strcmp(s, "\\P") == 0 || strcmp(s, "\\===") == 0 ||
		    strcasecmp(s, "\\erase") == 0 || strcmp(s, "\\E") == 0) {
			int msgbox_x, msgbox_y, msgbox_w, msgbox_h;
			clear_buffered_message();
			reset_page_line();
			fill_msgbox();
			get_msgbox_rect(&msgbox_x, &msgbox_y, &msgbox_w, &msgbox_h);
			if (!conf_msgbox_font_tategaki) {
				pen_x = conf_msgbox_margin_left;
				pen_y = conf_msgbox_margin_top;
			} else {
				pen_x = msgbox_w - conf_msgbox_margin_right - conf_msgbox_font_size;
				pen_y = conf_msgbox_margin_top;
			}
			*cont = true;
			return true;
		} else if (strcasecmp(s, "\\click") == 0 || strcmp(s, "\\C") == 0 || strcmp(s, "\\---") == 0) {
			// Do normal initialization.
		} else {
			if (!is_page_top()) {
				if (!append_buffered_message("\\n"))
					return false;
			}
			if (!append_buffered_message(s))
				return false;
			inc_page_line();
			*cont = true;
			return true;
		}
	}

	/* フラグを初期化する */
	init_flags_and_vars();

	/* オートモードの場合の初期化を行う */
	init_auto_mode();

	/* スキップモードの場合の初期化を行う */
	init_skip_mode();

	/* Initialize the colors. */
	init_colors();

	/* 名前を取得する */
	if (!init_name_top())
		return false;

	/* ボイスファイルを取得する */
	if (!init_voice_file())
		return false;

	/* メッセージを取得する */
	if (!init_msg_top())
		return false;

	/* ヒストリに登録するが表示しない場合 */
	if (conf_msgbox_history_control != NULL &&
	    strcmp(conf_msgbox_history_control, "only-history") == 0) {
		no_show = true;
		return true;
	}

	/* メッセージボックスを初期化する */
	init_msgbox();

	/* セリフ固有の初期化を行う */
	if (!init_serif())
		return false;

	/*
	 * メッセージの表示中状態をセットする
	 *  - システムGUIに入っても保持される
	 *  - メッセージから次のコマンドに移行するときにクリアされる
	 *  - ロードされてもクリアされる
	 *  - タイトルに戻るときにもクリアされる
	 */
	if (!gui_sys_flag) {
		if (!is_message_active())
			set_message_active();
	}

	/* クリックアニメーションを非表示の状態にする */
	init_click();

	/* ボタンの選択状態を初期化する */
	init_pointed_index();

	/* 繰り返し動作を設定する */
	init_repetition();

	/* テキスト読み上げを行う */
	speak();

	/* 口パクを実行する */
	init_lip_sync();

	/* 連続スワイプによるスキップ動作を有効にする */
	set_continuous_swipe_enabled(true);

	return true;
}

/* フラグを初期化する */
static void init_flags_and_vars(void)
{
	/* GUIから戻ったばかりかチェックする */
	if (check_gui_flag()) {
		if (is_message_active()) {
			/*
			 * メッセージがアクティブ状態のままシステムGUIに
			 * 移行して、その後戻ってきた場合
			 *  - 瞬間表示を行うためにフラグをセットする
			 */
			gui_sys_flag = true;
			gui_cmd_flag = false;
			gui_gosub_flag = false;
		} else {
			/*
			 * GUIコマンドの直後の場合
			 *  - 画面全体の更新を行うためにフラグをセットする
			 */
			gui_sys_flag = false;
			gui_cmd_flag = true;
			gui_gosub_flag = false;
		}
	} else if (is_return_from_sysmenu_gosub()) {
		/*
		 * GUIコマンドの直後の場合
		 *  - 画面全体の更新を行うためにフラグをセットする
		 */
		gui_sys_flag = false;
		gui_cmd_flag = false;
		gui_gosub_flag = true;
	} else {
		gui_sys_flag = false;
		gui_cmd_flag = false;
		gui_gosub_flag = false;
	}

	/* ロードされたばかりかチェックする */
	load_flag = check_load_flag();

	/* スペースキーによる非表示でない状態にする */
	is_hidden = false;

	/* オートモードの状態設定を行う */
	is_auto_mode_wait = false;

	/* メニュー遷移の状態をクリアする */
	need_sysmenu_mode = false;
	need_save_mode = false;
	need_load_mode = false;
	need_history_mode = false;
	need_config_mode = false;

	/* 重ね塗り(dimming)でない状態にする */
	need_dimming = false;
	is_dimming = false;
	avoid_dimming = false;

	/* インラインウェイトでない状態にする */
	is_inline_wait = false;
	inline_wait_time_total = 0;
	do_draw_all = false;

	/* ボイスを停止する前の状態にする */
	is_voice_stopped = false;

	/* ヒストリに残すが表示しない場合 */
	no_show = false;
}

/* オートモードの場合の初期化処理を行う */
static void init_auto_mode(void)
{
	/* システムGUIから戻った場合は処理しない */
	if (gui_sys_flag)
		return;

	/* ヒストリに残して表示しない場合は処理しない */
	if (conf_msgbox_history_control != NULL &&
	    strcmp(conf_msgbox_history_control, "only-history") == 0)
		return;

	/* オートモードの場合 */
	if (is_auto_mode()) {
		/* リターンキー、下キーの入力を無効にする */
		is_return_pressed = false;
		is_down_pressed = false;
	}
}

/* スキップモードの場合の初期化処理を行う */
static void init_skip_mode(void)
{
	/* システムGUIから戻った場合は処理しない */
	if (gui_sys_flag)
		return;

	/* ヒストリに残して表示しない場合は処理しない */
	if (conf_msgbox_history_control != NULL &&
	    strcmp(conf_msgbox_history_control, "only-history") == 0)
		return;

	/* スキップモードの場合 */
	if (is_skip_mode()) {
		/* 未読に到達した場合、スキップモードを終了する */
		if (!is_skippable()) {
			stop_skip_mode();
			show_skipmode_banner(false);
			return;
		}

		/* クリックされた場合 */
		if (is_right_clicked || is_left_clicked ||
		    is_up_pressed || is_down_pressed ||
		    is_escape_pressed) {
			/* SEを再生する */
			play_se(conf_skipmode_leave_se);

			/* スキップモードを終了する */
			stop_skip_mode();
			show_skipmode_banner(false);

			/* 以降のクリック処理を行わない */
			clear_input_state();
		}
	}
}

/* Initialize the colors. */
static void init_colors(void)
{
	body_color = make_pixel(0xff,
				(uint32_t)conf_msgbox_font_r,
				(uint32_t)conf_msgbox_font_g,
				(uint32_t)conf_msgbox_font_b);

	body_outline_color = make_pixel(0xff,
					(uint32_t)conf_msgbox_font_outline_r,
					(uint32_t)conf_msgbox_font_outline_g,
					(uint32_t)conf_msgbox_font_outline_b);

	name_color = make_pixel(0xff,
				(uint32_t)conf_namebox_font_r,
				(uint32_t)conf_namebox_font_g,
				(uint32_t)conf_namebox_font_b);

	name_outline_color = make_pixel(0xff,
					(uint32_t)conf_namebox_font_outline_r,
					(uint32_t)conf_namebox_font_outline_g,
					(uint32_t)conf_namebox_font_outline_b);

	bg_color = make_pixel(0xff,
			      (uint32_t)conf_msgbox_fill_r,
			      (uint32_t)conf_msgbox_fill_g,
			      (uint32_t)conf_msgbox_fill_b);
}

/* 名前を取得する */
static bool init_name_top(void)
{
	const char *exp;

	/* システムGUIから戻った場合 */
	if (gui_sys_flag)
		return true;

	/* ロード/タイトルへ戻った場合はname_topが残っているので解放する */
	if (name_top != NULL) {
		free(name_top);
		name_top = NULL;
	}

	/* 名前を取得する */
	if (get_command_type() == COMMAND_SERIF) {
		raw_name = get_string_param(SERIF_PARAM_NAME);
		exp = expand_variable(raw_name);
		name_top = strdup(exp);
		if (name_top == NULL) {
			log_memory();
			return false;
		}
	} else {
		name_top = NULL;
		raw_name = NULL;
	}

	return true;
}

/* ボイスファイル名を取得する */
static bool init_voice_file(void)
{
	const char *voice;

	if (get_command_type() != COMMAND_SERIF) {
		voice_file = NULL;
		return true;
	}

	/* ロード/タイトルへ戻った場合はvoice_fileが残っているので解放する */
	if (voice_file != NULL) {
		free(voice_file);
		voice_file = NULL;
		return true;
	}

	/* ボイスのファイル名を取得する */
	voice = get_string_param(SERIF_PARAM_VOICE);

	/* 変数展開する */
	voice_file = strdup(expand_variable_with_increment(voice, 1));
	if (voice_file == NULL) {
		log_memory();
		return false;
	}

	return true;
}

/* メッセージを取得する */
static bool init_msg_top(void)
{
	const char *raw_msg;
	char *exp_msg;
	int lf;
	bool is_serif;

	/* システムGUIから戻った場合 */
	if (gui_sys_flag)
		return true;

	/* ロード/タイトルへ戻った場合はmsg_topが残っているので解放する */
	if (msg_top != NULL) {
		free(msg_top);
		msg_top = NULL;
	}

	/* ページモードの場合 */
	if (is_page_mode()) {
		raw_msg = get_buffered_message();
		is_serif = false;
		is_continue_mode = true;
	} else {
		/* 引数を取得する */
		is_serif = get_command_type() == COMMAND_SERIF;
		if (is_serif)
			raw_msg = get_string_param(SERIF_PARAM_MESSAGE);
		else
			raw_msg = get_string_param(MESSAGE_PARAM_MESSAGE);

		/* 継続行かチェックする */
		if (*raw_msg == '\\' && !is_escape_sequence_char(*(raw_msg + 1))) {
			is_continue_mode = true;

			/* 先頭の改行をスキップする */
			raw_msg = skip_lf(raw_msg + 1, &lf);

			/* 日本語以外のロケールで、改行がない場合 */
			if (conf_game_locale != NULL && strcmp(conf_game_locale, "ja") != 0 && lf == 0)
				put_space();
		} else {
			is_continue_mode = false;
		}
	}

	/* 変数を展開する */
	exp_msg = strdup(expand_variable(raw_msg));
	if (exp_msg == NULL) {
		log_memory();
		return false;
	}

	/* ヒストリ画面用にメッセージ履歴を登録する */
	if (!register_message_for_history(exp_msg)) {
		free(exp_msg);
		return false;
	}

	/* セーブ用にメッセージを保存する */
	if (!load_flag) {
		if (!set_last_message(exp_msg, is_continue_mode)) {
			free(exp_msg);
			return false;
		}
	}

	/* セリフの場合、実際に表示するメッセージを修飾する */
	if (is_serif) {
		if (conf_namebox_hide) {
			/* 名前とカギカッコを付加する */
			msg_top = concat_serif(name_top, exp_msg);
			if (msg_top == NULL) {
				log_memory();
				free(exp_msg);
				return false;
			}
			free(exp_msg);
		} else {
			/* 何も付加しない場合 */
			msg_top = exp_msg;
		}
	} else {
		/* メッセージの場合はそのまま表示する */
		msg_top = exp_msg;
	}

	/* ロードで開始された場合は、行継続モードを解除する */
#if 0
	if (load_flag && is_continue_mode)
		is_continue_mode = false;
#endif

	return true;
}

/* 継続行の先頭の改行をスキップする */
static const char *skip_lf(const char *m, int *lf)
{
	assert(is_continue_mode);

	*lf = 0;
	while (*m == '\\') {
		if (*(m + 1) == 'n') {
			(*lf)++;
			m += 2;

			/* ロードで開始されたときは、先頭の改行を行わない */
			if (load_flag)
				continue;

			/* ペンを改行する */
			if (!conf_msgbox_font_tategaki) {
				pen_x = conf_msgbox_margin_left;
				pen_y += conf_msgbox_margin_line;
			} else {
				pen_x -= conf_msgbox_margin_line;
				pen_y = conf_msgbox_margin_top;
			}
		} else {
			break;
		}
	}
	return m;
}

/* 空白文字の分だけカーソルを移動する */
static void put_space(void)
{
	int cw, ch;

	if (!conf_msgbox_font_tategaki) {
		cw = get_glyph_width(conf_msgbox_font, conf_msgbox_font_size, ' ');
		if (pen_x + cw >= msgbox_w - conf_msgbox_margin_right) {
			pen_y += conf_msgbox_margin_line;
			pen_x = conf_msgbox_margin_left;
		} else {
			pen_x += cw;
		}
	} else {
		ch = get_glyph_height(conf_msgbox_font, conf_msgbox_font_size, ' ');
		if (pen_y + ch >= msgbox_h - conf_msgbox_margin_bottom) {
			pen_y = conf_msgbox_margin_top;
			pen_x -= conf_msgbox_margin_line;
		} else {
			pen_y += ch;
		}
	}
}

/* ヒストリ画面用にメッセージ履歴を登録する */
static bool register_message_for_history(const char *msg)
{
	const char *voice;

	assert(msg != NULL);
	assert(!gui_sys_flag);

	/* ヒストリに登録しない場合 */
	if (gui_gosub_flag)
		return true;
	if (conf_msgbox_history_control != NULL &&
	    strcmp(conf_msgbox_history_control, "no-history") == 0)
		return true;

	/* メッセージ履歴を登録する */
	if (get_command_type() == COMMAND_SERIF) {
		assert(name_top != NULL);

		/* ビープ音は履歴画面で再生しない */
		voice = voice_file;
		if (voice != NULL && voice[0] == '@')
			voice = NULL;

		if (!is_continue_mode || is_page_mode()) {
			/* セリフをヒストリ画面用に登録する */
			if (!register_message(name_top,
					      msg,
					      voice,
					      body_color,
					      body_outline_color,
					      name_color,
					      name_outline_color))
				return false;
		} else {
			/* セリフのメッセージ部分のみをヒストリ画面用に登録(追記)する */
			if (!append_message(msg))
				return false;
		}
	} else {
		if (!is_continue_mode || is_page_mode()) {
			/* メッセージをヒストリ画面用に登録する */
			if (!register_message(NULL,
					      msg,
					      NULL,
					      body_color,
					      body_outline_color,
					      0,
					      0))
				return false;
		} else {
			/* メッセージをヒストリ画面用に登録(追記)する */
			if (!append_message(msg))
				return false;
		}
	}

	/* 成功 */
	return true;
}

/* 名前とメッセージを連結する */
static char *concat_serif(const char *name, const char *serif)
{
	char *ret;
	size_t len, lf, i;
	const char *prefix;
	const char *suffix;

	assert(name != NULL);
	assert(serif != NULL);

	/* 日本語ロケールかどうかでセリフの囲いを分ける */
	if (strcmp(conf_game_locale, "ja") == 0) {
		prefix = U8("「");
		suffix = U8("」");
	} else {
		prefix = ": ";
		suffix = "";
	}

	/* 先頭の'\\' 'n'をカウントする */
	lf = 0;
	while (*serif == '\\' && *(serif + 1) == 'n') {
		lf++;
		serif += 2;
	}

	/* 先頭の改行の先の文字列を作る */
	if (is_quoted_serif(serif)) {
		/* クオート済みの文字列の場合 */
		len = lf * 2 + strlen(name) + strlen(serif) + 1;
		ret = malloc(len);
		if (ret == NULL) {
			log_memory();
			return NULL;
		}
		snprintf(ret + lf * 2, len, "%s%s", name, serif);
	} else {
		/* クオートされていない文字列の場合 */
		len = lf * 2 + strlen(name) + strlen(prefix) +
			strlen(serif) + strlen(suffix) + 1;
		ret = malloc(len);
		if (ret == NULL) {
			log_memory();
			return NULL;
		}
		snprintf(ret + lf * 2, len, "%s%s%s%s", name, prefix, serif,
			 suffix);
	}

	/* 先頭の改行を埋める */
	for (i = 0; i < lf; i++) {
		ret[i * 2] = '\\';
		ret[i * 2 + 1] = 'n';
	}

	return ret;
}

/* メッセージボックスを初期化する */
static void init_msgbox(void)
{
	/* システムGUIから戻った場合 */
	if (gui_sys_flag)
		return;

	/* メッセージボックスの矩形を取得する */
	get_msgbox_rect(&msgbox_x, &msgbox_y, &msgbox_w, &msgbox_h);

	/* 継続行でなければ、メッセージの描画位置を初期化する */
	if (!is_continue_mode) {
		if (!conf_msgbox_font_tategaki) {
			pen_x = conf_msgbox_margin_left;
			pen_y = conf_msgbox_margin_top;
		} else {
			pen_x = msgbox_w - conf_msgbox_margin_right - conf_msgbox_font_size;
			pen_y = conf_msgbox_margin_top;
		}
	}

	/* 重ね塗りをする場合のためにペン位置を保存する */
	orig_pen_x = pen_x;
	orig_pen_y = pen_y;

	/* 行継続でなければ、メッセージレイヤをクリアする */
	if (!is_continue_mode)
		fill_msgbox();
	else
		blit_pending_message();

	/* メッセージレイヤを可視にする */
	show_msgbox(true);

	/* メッセージ描画のコンテキストを構築する */
	construct_draw_msg_context(
		&msgbox_context,
		LAYER_MSG,
		msg_top,
		conf_msgbox_font,
		conf_msgbox_font_size,
		conf_msgbox_font_size,
		conf_msgbox_font_ruby,
		conf_msgbox_font_outline,
		pen_x,
		pen_y,
		msgbox_w,
		msgbox_h,
		conf_msgbox_margin_left,
		conf_msgbox_margin_right,
		conf_msgbox_margin_top,
		conf_msgbox_margin_bottom,
		conf_msgbox_margin_line,
		conf_msgbox_margin_char,
		body_color,
		body_outline_color,
		bg_color,
		conf_msgbox_fill,
		false,	/* is_dimming */
		false,	/* ignore_linefeed */
		false,	/* ignore_font */
		false,	/* ignore_outline */
		false,	/* ignore_color */
		false,	/* ignore_size */
		false,	/* ignore_position */
		false,	/* ignore_ruby */
		false,	/* ignore_wait */
		inline_wait_hook,
		conf_msgbox_font_tategaki);

	/* 文字数をカウントする */
	total_chars = count_chars_common(&msgbox_context, NULL);
	drawn_chars = 0;
}

/* セリフコマンドを処理する */
static bool init_serif(void)
{
	/* システムGUIから戻った場合 */
	if (gui_sys_flag)
		return true;

	/* セリフコマンドではない場合 */
	if (get_command_type() != COMMAND_SERIF) {
		/* ボイスはない */
		have_voice = false;

		/* 名前ボックスを表示しない(継続行の場合は表示状態を変えない) */
		if (!is_continue_mode)
			show_namebox(false);

		return true;
	}

	/* ボイスを再生する */
	if (check_play_voice()) {
		/* いったんボイスなしの判断にしておく(あとで変更する) */
		have_voice = false;

		/* ボイスを再生する */
		if (!play_voice())
			return false;
	}

	/* 名前を描画する */
	if (!conf_namebox_hide)
		blit_namebox();

	/* 名前ボックスを表示する */
	show_namebox(true);

	/* フォーカスを設定する */
	focus_character();

	return true;
}

/* ボイスを再生するかを判断する */
static bool check_play_voice(void)
{
	/* システムGUIから戻った場合は再生しない */
	if (gui_sys_flag)
		return false;

	/* 割り込み不可モードの場合は他の条件を考慮せず再生する */
	if (is_non_interruptible())
		return true;

	/* スキップモードで、かつ、既読の場合は再生しない */
	if (is_skip_mode() && is_skippable())
		return false;

	/* オートモードの場合は再生する */
	if (is_auto_mode())
		return true;

	/* 未読の場合は再生する */
	if (!is_skippable())
		return true;

	/* コントロールキーが押下されている場合は再生しない */
	if (is_control_pressed) {
		/* BGVoiceを止める */
		set_mixer_input(VOICE_STREAM, NULL);
		set_bgvoice_playing(false);

		/* BGVoiceが指定されていれば再生する */
		is_voice_stopped = true;
		have_voice = true;
		check_and_play_bgvoice();
		return false;
	}

	/* その他の場合は再生する */
	return true;
}

/* ボイスを再生する */
static bool play_voice(void)
{
	struct wave *w;

	if (voice_file == NULL || strcmp(voice_file, "") == 0)
		return true;

	/* PCMストリームを開く */
	w = create_wave_from_file(CV_DIR, voice_file, false);
	if (w == NULL) {
		log_script_exec_footer();
		return false;
	}

	/* キャラクタ音量を設定する */
	set_character_volume_by_name(get_string_param(SERIF_PARAM_NAME));

	/* PCMストリームを再生する */
	set_mixer_input(VOICE_STREAM, w);

	/* BGVoiceを停止状態と記録する */
	set_bgvoice_playing(false);

	/* ボイスありの判断にする */
	have_voice = true;

	return true;
}

/* キャラクタ音量を設定する */
static void set_character_volume_by_name(const char *name)
{
	int i;

	for (i = 1; i < CH_VOL_SLOTS; i++) {
		/* キャラクタ名を探す */
		if (conf_sound_character_name[i] == NULL)
			continue;
		if (strcmp(conf_sound_character_name[i], name) == 0) {
			/* みつかった場合 */
			apply_character_volume(i);
			return;
		}
	}

	/* その他のキャラクタのボリュームを適用する */
	apply_character_volume(CH_VOL_SLOT_DEFAULT);
}

/* 名前ボックスを描画する */
static void blit_namebox(void)
{
	struct draw_msg_context context;
	int pen_x, pen_y, char_count;
	int namebox_x, namebox_y, namebox_width, namebox_height;

	/* 名前ボックスのサイズを取得する */
	get_namebox_rect(&namebox_x, &namebox_y, &namebox_width, &namebox_height);

	/* 描画位置を決める */
	if (!conf_namebox_leftify) {
		if (!conf_namebox_font_tategaki) {
			pen_x = (namebox_width - get_string_width(conf_namebox_font, conf_namebox_font_size, name_top)) / 2;
			pen_y = conf_namebox_margin_top;
		} else {
			pen_x = conf_namebox_margin_left;
			pen_y = (namebox_height - get_string_height(conf_namebox_font, conf_namebox_font_size, name_top)) / 2;
		}
	} else {
		pen_x = conf_namebox_margin_left;
		pen_y = conf_namebox_margin_top;
	}

	/* 名前ボックスレイヤを名前ボックス画像で埋める */
	fill_namebox();

	/* 文字描画コンテキストを作成する */
	construct_draw_msg_context(
		&context,
		LAYER_NAME,
		name_top,
		conf_namebox_font,
		conf_namebox_font_size,
		conf_namebox_font_size,		/* base_font_size */
		conf_namebox_font_ruby,
		conf_namebox_font_outline,
		pen_x,
		pen_y,
		namebox_width,
		namebox_height,
		conf_namebox_margin_left,
		0,			/* right_margin */
		conf_namebox_margin_top,
		0,			/* bottom_margin */
		0,			/* line_margin */
		conf_msgbox_margin_char,
		name_color,
		name_outline_color,
		0,			/* bg_color */
		false,			/* fill_bg */
		false,			/* is_dimming */
		false,			/* ignore_linefeed */
		false,			/* ignore_font*/
		false,			/* ignore_outline */
		false,			/* ignore_color */
		false,			/* ignore_size */
		false,			/* ignore_position */
		false,			/* ignore_ruby */
		true,			/* ignore_wait */
		NULL,			/* inline_wait_hook */
		conf_namebox_font_tategaki);

	/* 名前の文字数を取得する */
	char_count = count_chars_common(&context, NULL);

	/* 文字描画する */
	draw_msg_common(&context, char_count);
}

/* キャラクタのフォーカスを行う */
static void focus_character(void)
{
	int i;

	/* 名前が登録されているキャラクタであるかチェックする */
	for (i = 0; i < CHARACTER_MAP_COUNT; i++) {
		if (conf_character_name[i] == NULL)
			continue;
		if (conf_character_image[i] == NULL)
			continue;
		if (strcmp(conf_character_name[i], raw_name) == 0)
			break;
	}

	/* 発話キャラを設定する */
	set_ch_talking(i < CHARACTER_MAP_COUNT ? i : -1);

	/* キャラクタのオートフォーカスが有効なとき */
	if (conf_character_focus) {
		/* 発話キャラを元に明暗を更新する */
		update_ch_dim_by_talking_ch();
	}
}

/* クリックアニメーションを初期化する */
static void init_click(void)
{
	/* クリックレイヤを不可視にする */
	show_click(false);

	/* クリックアニメーションの初回表示フラグをセットする */
	is_click_first = true;

	/* クリックアニメーションの表示状態を保存する */
	is_click_visible = false;
}

/* 初期化処理においてポイントされているボタンを求め描画する */
static void init_pointed_index(void)
{
	int i, btn_x, btn_y, btn_w, btn_h;

	/* ポイントされているボタンを求める */
	pointed_index = get_pointed_button();
	adjust_pointed_index();

	/* ボタンがポイントされていなければ、すべてのボタンをbgで塗りつぶす */
	if (pointed_index == BTN_NONE) {
		for (i = MIN_BTN; i <= MAX_BTN; i++) {
			get_button_rect(i, &btn_x, &btn_y, &btn_w, &btn_h);
			fill_msgbox_rect_with_bg(btn_x, btn_y, btn_w, btn_h);
		}
		return;
	}

	/* ポイントされているボタンのfgを描画する */
	get_button_rect(pointed_index, &btn_x, &btn_y, &btn_w, &btn_h);
	fill_msgbox_rect_with_fg(btn_x, btn_y, btn_w, btn_h);
}

/* 選択中のボタンを取得する */
static int get_pointed_button(void)
{
	int rx, ry, btn_x, btn_y, btn_w, btn_h, i;

#ifdef USE_EDITOR
	/* シングルステップか停止要求中の場合、ボタンを選択できなくする */
	if (dbg_is_stop_requested())
		return BTN_NONE;
#endif

	/* マウス座標からメッセージボックス内座標に変換する */
	rx = mouse_pos_x - conf_msgbox_x;
	ry = mouse_pos_y - conf_msgbox_y;

	/* ボタンを順番に見ていく */
	btn_x = btn_y = btn_w = btn_h = 0;
	for (i = 0; i <= MAX_BTN; i++) {
		/* ボタンの座標を取得する */
		get_button_rect(i, &btn_x, &btn_y, &btn_w, &btn_h);

		/* マウスがボタンの中にあればボタンの番号を返す */
		if ((rx >= btn_x && rx < btn_x + btn_w) &&
		    (ry >= btn_y && ry < btn_y + btn_h))
			return i;
	}

	/* ボタンがポイントされていない */
	return BTN_NONE;
}

/* 状況に応じてメッセージボックス内のボタンのポイントを無効化する */
static void adjust_pointed_index(void)
{
	/* メッセージボックスを隠している間はボタンを選択しない */
	if (is_hidden) {
		pointed_index = BTN_NONE;
		return;
	}

	/* オートモード中とスキップモード中はボタンを選択しない */
	if (is_auto_mode() || is_skip_mode()) {
		pointed_index = BTN_NONE;
		return;
	}

	/* スキップモードに入れないときにSKIPボタンを無効化する */
	if (!is_skippable() && pointed_index == BTN_SKIP) {
		pointed_index = BTN_NONE;
		return;
	}
}

/* ボタンの座標を取得する */
static void get_button_rect(int btn, int *x, int *y, int *w, int *h)
{
	switch (btn) {
	case BTN_SAVE:
		*x = conf_msgbox_btn_save_x;
		*y = conf_msgbox_btn_save_y;
		*w = conf_msgbox_btn_save_width;
		*h = conf_msgbox_btn_save_height;
		break;
	case BTN_LOAD:
		*x = conf_msgbox_btn_load_x;
		*y = conf_msgbox_btn_load_y;
		*w = conf_msgbox_btn_load_width;
		*h = conf_msgbox_btn_load_height;
		break;
	case BTN_AUTO:
		*x = conf_msgbox_btn_auto_x;
		*y = conf_msgbox_btn_auto_y;
		*w = conf_msgbox_btn_auto_width;
		*h = conf_msgbox_btn_auto_height;
		break;
	case BTN_SKIP:
		*x = conf_msgbox_btn_skip_x;
		*y = conf_msgbox_btn_skip_y;
		*w = conf_msgbox_btn_skip_width;
		*h = conf_msgbox_btn_skip_height;
		break;
	case BTN_HISTORY:
		*x = conf_msgbox_btn_history_x;
		*y = conf_msgbox_btn_history_y;
		*w = conf_msgbox_btn_history_width;
		*h = conf_msgbox_btn_history_height;
		break;
	case BTN_CONFIG:
		*x = conf_msgbox_btn_config_x;
		*y = conf_msgbox_btn_config_y;
		*w = conf_msgbox_btn_config_width;
		*h = conf_msgbox_btn_config_height;
		break;
	case BTN_HIDE:
		*x = conf_msgbox_btn_hide_x;
		*y = conf_msgbox_btn_hide_y;
		*w = conf_msgbox_btn_hide_width;
		*h = conf_msgbox_btn_hide_height;
		break;
	default:
		assert(0);
		break;
	}
}

/* 初期化処理において、繰り返し動作を設定する */
static void init_repetition(void)
{
	if (is_skippable() && !is_non_interruptible() &&
	    (is_skip_mode() || (!is_auto_mode() && is_control_pressed))) {
		/* 繰り返し動作せず、すぐに表示する */
	} else {
		/* コマンドが繰り返し呼び出されるようにする */
		start_command_repetition();

		/* 時間計測を開始する */
		reset_lap_timer(&click_sw);
	}
}

/*
 * フレームの処理
 */

/* オートモード制御を処理する */
static bool frame_auto_mode(void)
{
	uint64_t lap;

	/* オートモードでない場合、何もしない */
	if (!is_auto_mode())
		return false;

	/* オートモード中はスキップモードにできない */
	assert(!is_skip_mode());

	/* オートモード中はメッセージボックスを非表示にできない */
	assert(!is_hidden);

#ifdef USE_EDITOR
	/* オートモード中に停止要求があった場合 */
	if (dbg_is_stop_requested()) {
		/* オートモード終了アクションを処理する */
		stop_auto_mode();
		show_automode_banner(false);

		/* クリックされたものとして処理する */
		return true;
	}
#endif

	/* クリックされた場合 */
	if (is_left_clicked || is_right_clicked || is_escape_pressed ||
	    is_return_pressed || is_down_pressed) {
		/* SEを再生する */
		play_se(conf_automode_leave_se);

		/* オートモードを終了する */
		stop_auto_mode();

		/* オートモードバナーを非表示にする */
		show_automode_banner(false);

		/* メッセージ表示とボイス再生を未完了にする */
		is_auto_mode_wait = false;

		/* 以降のクリック処理を行わない */
		clear_input_state();

		/* クリックされた */
		return true;
	} else if (!is_auto_mode_wait) {
		/* メッセージ表示とボイス再生が未完了の場合 */

		/* すでに表示が完了していれば */
		if (check_auto_play_condition()) {
			/* 待ち時間に入る */
			is_auto_mode_wait = true;

			/* 時間計測を開始する */
			reset_lap_timer(&auto_sw);
		}
	} else {
		/* 待ち時間の場合 */

		/* 時間を計測する */
		lap = get_lap_timer_millisec(&auto_sw);

		/* 時間が経過していれば、コマンドの終了処理に移る */
		if (lap >= (uint64_t)get_wait_time()) {
			if (conf_msgbox_dim)
				need_dimming = true;

			stop();

			/* コマンドを終了する */
			return true;
		}
	}

	/* クリックされず、コマンドも終了しない */
	return false;
}

/* オートモード開始アクションを処理する */
static void action_auto_start(void)
{
	/* SEを再生する */
	play_se(conf_msgbox_btn_auto_enter_se);

	/* オートモードを開始する */
	start_auto_mode();

	/* オートモードバナーを表示する */
	show_automode_banner(true);

	/* メッセージ表示とボイス再生を未完了にする */
	is_auto_mode_wait = false;
}

/* スキップモード開始アクションを処理する */
static void action_skip_start(void)
{
	/* SEを再生する */
	play_se(conf_msgbox_btn_skip_enter_se);

	/* スキップモードを開始する */
	start_skip_mode();

	/* スキップモードバナーを表示する */
	show_skipmode_banner(true);
}

/* オートプレイ用の表示完了チェックを行う */
static bool check_auto_play_condition(void)
{
	/*
	 * システムGUIから戻ったコマンドである場合
	 *  - すでに表示完了している
	 */
	if (gui_sys_flag)
		return true;

	/*
	 * ボイスありの場合
	 *  - ボイスの再生完了と文字の表示完了をチェックする
	 */
	if (have_voice) {
		/* 表示完了かつ再生完了しているか */
		if (is_mixer_sound_finished(VOICE_STREAM) && is_end_of_msg())
			return true;

		return false;
	}

	/*
	 * ボイスなしの場合
	 *  - 文字の表示完了をチェックする
	 */
	if (is_end_of_msg())
		return true;

	return false;
}

/* オートモードの待ち時間を取得する */
static int get_wait_time(void)
{
	float scale;

	/* ボイスありのとき */
	if (have_voice)
		return (int)(AUTO_MODE_VOICE_WAIT * get_auto_speed());

	/* ボイスなしのとき、スケールを求める */
	scale = AUTO_MODE_TEXT_WAIT_SCALE;

	return (int)((float)total_chars * scale * get_auto_speed() * 1000.0f);
}

/* メッセージボックス内のボタンを処理する (非表示中も呼ばれる) */
static bool frame_buttons(void)
{
	/* ボタンを描画する */
	if (draw_buttons()) {
		/* アクティブなボタンが変わったのでSEを再生する */
		/* play_se(conf_msgbox_btn_change_se); */
	}

#ifdef USE_EDITOR
	/* シングルステップか停止要求中はボタンのクリックを処理しない */
	if (dbg_is_stop_requested())
		return false;
#endif

	/* オートモードの場合、クリックを処理しない */
	if (is_auto_mode())
		return false;

	/* ボタンのクリックを処理する (非表示のキャンセルもここで処理する) */
	if (process_button_click())
		return true;	/* クリックされた */

	/* クリックされなかった */
	return false;
}

/* メッセージボックス内のボタンを描画する */
static bool draw_buttons(void)
{
	int last_pointed_index, btn_x, btn_y, btn_w, btn_h;

	/* メッセージボックス非表示中は処理しない */
	if (is_hidden)
		return false;

	/* ポイントされているボタンを取得する */
	last_pointed_index = pointed_index;
	pointed_index = get_pointed_button();
	adjust_pointed_index();

	/* ポイント状態に変更がない場合 */
	if (pointed_index == last_pointed_index)
		return false;

	/* 非アクティブになるボタンのbgを描画する */
	if (last_pointed_index != BTN_NONE) {
		get_button_rect(last_pointed_index, &btn_x, &btn_y, &btn_w, &btn_h);
		fill_msgbox_rect_with_bg(btn_x, btn_y, btn_w, btn_h);
	}

	/* アクティブになるボタンのfgを描画する */
	if (pointed_index != BTN_NONE) {
		get_button_rect(pointed_index, &btn_x, &btn_y, &btn_w, &btn_h);
		fill_msgbox_rect_with_fg(btn_x, btn_y, btn_w, btn_h);
	}

	/* ポイントが解除された場合 */
	if (pointed_index == BTN_NONE)
		return false;

	/* SEを再生する */
	return true;
}

/* メッセージボックス内のボタンのクリックを処理する */
static bool process_button_click(void)
{
	/* メッセージボックス非表示中は以降処理しない */
	if (is_hidden)
		return false;

	/* ボタンがポイントされていない場合は以降処理しない */
	if (pointed_index == BTN_NONE)
		return false;

	/* ボタンがクリックされていない場合は以降処理しない */
	if (!is_left_clicked)
		return false;

	/* ボタンの押下を処理する */
	switch (pointed_index) {
	case BTN_SAVE:
		need_save_mode = true;
		break;
	case BTN_LOAD:
		need_load_mode = true;
		break;
	case BTN_AUTO:
		action_auto_start();
		break;
	case BTN_SKIP:
		action_skip_start();
		break;
	case BTN_HISTORY:
		need_history_mode = true;
		break;
	case BTN_HIDE:
		action_toggle_hide();
		break;
	case BTN_CONFIG:
		need_config_mode = true;
		break;
	default:
		assert(0);
		break;
	}

	/* 入力は処理済みなので、以降は入力を処理しない */
	clear_input_state();

	/* ボタンがクリックされた */
	return true;
}

/* メッセージボックスの非表示/復帰を開始する */
static void action_toggle_hide(void)
{
	if (!is_hidden) {
		/* メッセージボックスを非表示にする */
		is_hidden = true;
		if (get_command_type() == COMMAND_SERIF)
			show_namebox(false);
		show_msgbox(false);
		show_click(false);
		play_se(conf_msgbox_btn_hide_enter_se);
	} else {
		/* メッセージボックスを表示する */
		is_hidden = false;
		if (get_command_type() == COMMAND_SERIF)
			show_namebox(true);
		show_msgbox(true);
		show_click(is_click_visible);
		play_se(conf_msgbox_btn_hide_leave_se);
	}
}

/* システムボタンの処理を行う */
static bool frame_sysbtn(void)
{
	bool enter_sysmenu;

#ifdef USE_EDITOR
	/* シングルステップか停止要求中の場合 */
	if (dbg_is_stop_requested()) {
		return false;
	}
#endif

	/* キー操作を受け付ける */
	if (is_space_pressed) {
		action_toggle_hide();
		clear_input_state();
		return true;
	}

	/* メッセージボックス非表示時のクリックを受け付ける */
	if (is_hidden && is_left_clicked) {
		action_toggle_hide();
		clear_input_state();
		return true;
	}

	/* コンフィグでシステムボタンを表示しない場合 */
	if (conf_sysbtn_hide)
		return false;

	/* メッセージボックス非表示中は処理しない */
	if (is_hidden)
		return false;

	/* オートモード中は処理しない */
	if (is_auto_mode())
		return false;

	/* スキップモード中は処理しない */
	if (is_skip_mode())
		return false;

	/* システムメニューに入るかを求める */
	enter_sysmenu = false;
	if (is_right_clicked || is_escape_pressed) {
		/* 右クリックされた場合と、エスケープキーが押下された場合 */
		enter_sysmenu = true;
		clear_input_state();
	} else if (is_left_clicked && is_sysbtn_pointed()) {
		/* システムボタンがクリックされたとき */
		enter_sysmenu = true;
		clear_input_state();
	}

	/* システムメニューに入らないとき */
	if (!enter_sysmenu)
		return false;

	/* システムメニューに入る */
	play_se(conf_sysbtn_se);

	/* システムメニューを表示する */
	need_sysmenu_mode = true;

	return true;
}

/*
 * メイン描画処理
 */

/* フレーム描画を行う */
static void blit_frame(void)
{
	/* メッセージボックス非表示中は処理しない */
	if (is_hidden)
		return;

	/* 以下、メインの表示処理を行う */

	/* 入力があったらボイスを止める */
	if (is_canceled_by_skip()) {
		/* BGVoiceの再生中を除く */
		if (!is_bgvoice_playing()) {
			/* ボイスを停止する */
			set_mixer_input(VOICE_STREAM, NULL);
			is_voice_stopped = true;

			/* BGVoiceが登録されていれば再生する */
			check_and_play_bgvoice();
		}
	}

	/* 本文の描画中であるか */
	if (!is_end_of_msg()) {
		/* インラインウェイトを処理する */
		if (is_inline_wait) {
			if ((float)get_lap_timer_millisec(&inline_sw) / 1000.0f >
			    inline_wait_time)
				is_inline_wait = false;
		}
		if (!is_inline_wait) {
			/* 本文を描画する */
			blit_msgbox();
		}
		if (is_end_of_msg()) {
			/* 口パクを止める */
			/*cleanup_lip_sync();*/
		}
	} else {
		/*
		 * 本文の描画が完了してさらに描画しようとしているので、
		 * クリックアニメーションを描画する
		 */
		set_click();
	}
}

static void blit_pending_message(void)
{
	struct draw_msg_context text_context;
	char *text;
	pixel_t save_body_color, save_body_outline_color;
	int len;

	text = get_pending_message();
	if (text == NULL)
		return;

	fill_msgbox();

	if (!conf_msgbox_font_tategaki) {
		pen_x = conf_msgbox_margin_left;
		pen_y = conf_msgbox_margin_top;
	} else {
		pen_x = msgbox_w - conf_msgbox_margin_right - conf_msgbox_font_size;
		pen_y = conf_msgbox_margin_top;
	}
	orig_pen_x = pen_x;
	orig_pen_y = pen_y;

	save_body_color = body_color;
	save_body_outline_color = body_outline_color;
	body_color = make_pixel(0xff,
				(uint32_t)conf_msgbox_dim_r,
				(uint32_t)conf_msgbox_dim_g,
				(uint32_t)conf_msgbox_dim_b);
	body_outline_color = make_pixel(0xff,
					(uint32_t)conf_msgbox_dim_outline_r,
					(uint32_t)conf_msgbox_dim_outline_g,
					(uint32_t)conf_msgbox_dim_outline_b);

	construct_draw_msg_context(
		&text_context,
		LAYER_MSG,
		text,
		conf_msgbox_font,
		conf_msgbox_font_size,
		conf_msgbox_font_size,
		conf_msgbox_font_ruby,
		conf_msgbox_font_outline,
		pen_x,
		pen_y,
		msgbox_w,
		msgbox_h,
		conf_msgbox_margin_left,
		conf_msgbox_margin_right,
		conf_msgbox_margin_top,
		conf_msgbox_margin_bottom,
		conf_msgbox_margin_line,
		conf_msgbox_margin_char,
		body_color,
		body_outline_color,
		0,	/* bg_color */
		false,	/* fill_bg */
		conf_msgbox_dim,
		false,	/* ignore_linefeed */
		false,	/* ignore_font */
		false,	/* ignore_outline */
		false,	/* ignore_color */
		false,	/* ignore_size */
		false,	/* ignore_position */
		false,	/* ignore_ruby */
		false,	/* ignore_wait */
		inline_wait_hook,
		conf_msgbox_font_tategaki);

	len = count_chars_common(&text_context, NULL);
	draw_msg_common(&text_context, len);

	get_pen_position_common(&text_context, &pen_x, &pen_y);

	free(text);

	body_color = save_body_color;
	body_outline_color = save_body_outline_color;
	avoid_dimming = true;
}

/* メッセージ本文の描画が完了しているか */
static bool is_end_of_msg(void)
{
	/* システムGUIから戻った場合 */
	if (gui_sys_flag)
		return true;

	/* 完了している場合 */
	if (drawn_chars == total_chars)
		return true;

	/* 完了していない場合 */
	return false;
}

/* メッセージボックスの描画を行う */
static void blit_msgbox(void)
{
	int char_count, ret;

	/*
	 * システムGUIから戻った場合の描画ではないはず
	 *  - ただしディミングの場合はシステムGUIから戻った場合でも描画する
	 */
	if (!is_dimming)
		assert(!gui_sys_flag);

	/* 今回のフレームで描画する文字数を取得する */
	char_count = get_frame_chars();
	if (char_count == 0)
		return;

	/* 描画を行う */
	ret = draw_msg_common(&msgbox_context, char_count);
	if (is_inline_wait) {
		/* インラインウェイトが現れた場合 */
		drawn_chars += ret;
		return;
	}

	/* 描画した文字数を記録する */
	drawn_chars += char_count;

	/* ペンの描画終了位置を取得する */
	if (drawn_chars == total_chars)
		get_pen_position_common(&msgbox_context, &pen_x, &pen_y);
}

/* インラインウェイトのエスケープシーケンス出現時のコールバック */
static void inline_wait_hook(float wait_time)
{
	/* インラインウェイトを開始する */
	if (wait_time > 0 && !do_draw_all) {
		is_inline_wait = true;
		inline_wait_time = wait_time;
		inline_wait_time_total += inline_wait_time;
		reset_lap_timer(&inline_sw);
	}
}

/*
 * 今回のフレームで描画する文字数を取得する
 *  - クリックやキー入力、システムGUIへの移行などすべてを加味する
 */
static int get_frame_chars(void)
{
	if (!is_dimming)
		assert(!gui_sys_flag);

	/* 繰り返し動作しない場合 (dimmingを含む) */
	if (!is_in_command_repetition()) {
		/* すべての文字を描画する */
		do_draw_all = true;
		return total_chars;
	}

	/* コンフィグで瞬間表示が設定されている場合 */
	if (conf_msgbox_nowait) {
		do_draw_all = true;
		return total_chars;
	}

	/* セーブのサムネイルを作成するために全文字描画する場合 */
	if (need_sysmenu_mode || need_save_mode || need_load_mode || need_history_mode || need_config_mode) {
		/* 残りの文字をすべて描画する */
		do_draw_all = true;
		return total_chars - drawn_chars;
	}

	/* スキップ処理する場合 */
	if (is_canceled_by_skip()) {
		if (conf_msgbox_dim)
			need_dimming = true;

		/* 繰り返し動作を停止する */
		stop();

		/* 口パクを止める */
		cleanup_lip_sync();

		/* 残りの文字をすべて描画する */
		do_draw_all = true;
		return total_chars - drawn_chars;
	}

	/* 全部描画してクリック待ちに移行する場合 */
	if (is_fast_forward_by_click()) {
		/* 口パクを止める */
		cleanup_lip_sync();

#ifdef USE_EDITOR
		/*
		 * デバッガの停止ボタンが押されている場合は、
		 * クリック待ちに移行せずにコマンドを終了する流れにする
		 */
		if (dbg_is_stop_requested())
			stop_command_repetition();
#endif

		/* 残りの文字をすべて描画する */
		do_draw_all = true;
		set_ignore_inline_wait(&msgbox_context);
		return total_chars - drawn_chars;
	}

	/* 経過時間を元に今回描画する文字数を計算する */
	return calc_frame_chars_by_lap();
}

/* スキップによりキャンセルされたかチェックする */
static bool is_canceled_by_skip(void)
{
	/* 未読ならそもそもスキップできない */
	if (!is_skippable())
		return false;

	/* 割り込み不可ならスキップしない */
	if (is_non_interruptible())
		return false;

	/* スキップモードならスキップする */
	if (is_skip_mode())
		return true;

	/* Controlキーが押下されたらスキップする */
	if (is_control_pressed) {
		/* ただしオートモードならスキップしない */
		if (is_auto_mode())
			return false;
		else
			return true;
	}

	/* スキップしない */
	return false;
}

/* 全部描画してクリック待ちに移行する場合 */
static bool is_fast_forward_by_click(void)
{
	/* 割り込み不可ならクリック待ちに移行しない */
	if (is_non_interruptible())
		return false;

	/* Returnキーが押下されたらクリック待ちに移行する */
	if (is_return_pressed)
		return true;

	/* 下キーが押下されたらクリック待ちに移行する */
	if (is_down_pressed)
		return true;

	/* クリックされたらクリック待ちに移行する */
	if (is_left_clicked) {
		/* クリック待ちに移行する */
		return true;
	}

	/* クリック待ちに移行しない */
	return false;
}

/* 経過時間を元に今回描画する文字数を計算する */
static int calc_frame_chars_by_lap(void)
{
	float lap;
	float progress;
	int char_count;

	/*
	 * システムGUIのコンフィグでテキストスピードが最大のときは、
	 * ノーウェイトで全部描画する。
	 */
	if (get_text_speed() == 1.0f)
		return total_chars - drawn_chars;

	/*
	 * 経過時間(秒)を取得する
	 *  - インラインウェイトの分を差し引く
	 */
	lap = (float)get_lap_timer_millisec(&click_sw) / 1000.0f -
		inline_wait_time_total;

	/* 進捗(文字数)を求める */
	progress = MESSAGE_SPEED * lap;

	/* ユーザ設定のテキストスピードを乗算する (0にならないよう0.1足す) */
	progress *= get_text_speed() + 0.1f;

	/* 整数にする */
	char_count = (int)ceil(progress);

	/* 残りの文字数にする */
	char_count -= drawn_chars;

	/* 残りの文字数を越えた場合は、残りの文字数にする */
	if (char_count > total_chars - drawn_chars)
		char_count = total_chars - drawn_chars;

	return char_count;
}

/* クリックアニメーションを設定する */
static void set_click(void)
{
	uint64_t lap;
	int click_x, click_y, click_w, click_h;
	int index;

	/* 継続行で、改行のみの場合、クリック待ちを行わない */
	if (is_continue_mode && total_chars == 0) {
		if (is_in_command_repetition())
			stop();
		return;
	}

	/* 入力があったら繰り返しを終了する */
	if (check_stop_click_animation()) {
		if (conf_msgbox_dim && !avoid_dimming)
			need_dimming = true;
		stop();
	}

	/* クリックアニメーションの初回表示のとき */
	if (is_click_first) {
		is_click_first = false;

		/* 表示位置を設定する */
		if (conf_click_move) {
			int cur_pen_x, cur_pen_y;
			set_click_index(0);
			get_click_rect(&click_x, &click_y, &click_w, &click_h);
			get_pen_position_common(&msgbox_context, &cur_pen_x, &cur_pen_y);
			set_click_position(cur_pen_x + conf_msgbox_x,
					   cur_pen_y + conf_msgbox_y);
		} else {
			set_click_position(conf_click_x, conf_click_y);
		}

		/* 時間計測を開始する */
		reset_lap_timer(&click_sw);
	}

	/* 経過時間を取得する */
	lap = get_lap_timer_millisec(&click_sw);

	/* クリックアニメーションの表示を行う */
	index = (int)((lap % (uint64_t)(conf_click_interval * 1000)) /
		((uint64_t)(conf_click_interval * 1000) / (uint64_t)click_frames) %
		      (uint64_t)click_frames);
	index = index < 0 ? 0 : index;
	index = index >= click_frames ? 0 : index;
	set_click_index(index);
	show_click(true);
	is_click_visible = true;
}

/* クリックアニメーションで入力があったら繰り返しを終了する */
static bool check_stop_click_animation(void)
{
	/* 本文の描画が完了していなければここに来ない */
	assert(is_end_of_msg());

#ifdef USE_EDITOR
	/* デバッガから停止要求がある場合 */
	if (dbg_is_stop_requested()) {
		/* ボイスがなければ停止する */
		if (!have_voice)
			return true;

		/* ボイスが再生完了したフレームで停止する */
		if (is_mixer_sound_finished(VOICE_STREAM))
			return true;

		/* 何もないところをクリックされた場合は停止する */
		if (is_left_clicked && !is_sysbtn_pointed())
			return true;

		/* キーが押下された場合 */
		if (is_down_pressed || is_return_pressed)
			return true;
	}
#endif

	/* スキップモードの場合 */
	if (is_skip_mode()) {
		assert(!is_auto_mode());

		/* 既読でない場合はスキップできないので停止しない */
		if (!is_skippable())
			return false;

		/* 割り込み不可ではない場合はスキップにより停止する */
		if (!is_non_interruptible())
			return true;

		/* ボイスがない場合はスキップにより停止する */
		if (!have_voice)
			return true;

		/* ボイスが再生完了している場合はスキップにより停止する */
		if (is_mixer_sound_finished(VOICE_STREAM))
			return true;

		/* 停止しない */
		return false;
	}

	/* コントロールキーが押下されている場合 */
	if (is_control_pressed) {
		/* オートモードの場合はコントロールキーを無視する */
		if (is_auto_mode())
			return false;

		/* 既読でない場合はスキップできないので停止しない */
		if (!is_skippable())
			return false;

		/* 割り込み不可でない場合はスキップにより停止する */
		if (!is_non_interruptible())
			return true;

		/* ボイスがない場合はスキップにより停止する */
		if (!have_voice)
			return true;

		/* ボイスが再生中の場合はスキップにより停止する */
		if (is_mixer_sound_finished(VOICE_STREAM))
			return true;

		/* 停止しない */
		return false;
	}

	/* システムGUIから戻ってコマンドが開始されている場合 */
	if (gui_sys_flag) {
		/* FYI: ボイスは再生されていない */

		/* 何もないところをクリックされた場合は停止する */
		if (is_left_clicked && !is_sysbtn_pointed())
			return true;

		/* キーが押下された場合は停止する */
		if (is_return_pressed || is_down_pressed)
			return true;

		/* 停止しない */
		return false;
	}

	/* 何もないところをクリックされた場合と、キーが押下された場合 */
	if ((is_left_clicked && !is_sysbtn_pointed()) ||
	    (is_return_pressed || is_down_pressed)) {
		/*
		 * 割り込み不可モードでない場合は停止する
		 *  - 再生中のボイスがあればcleanup()で停止される
		 */
		if (!is_non_interruptible())
			return true;

		/*
		 * 割り込み不可モードでも、ボイスがないか、再生完了後の場合は
		 * 停止する
		 */
		if (!have_voice || is_mixer_sound_finished(VOICE_STREAM))
			return true;

		/* 停止しない */
		return false;
	}

	/* 停止しない */
	return false;
}

/* 折りたたみシステムメニューを描画する */
static void render_sysbtn_extended(void)
{
#ifdef USE_EDITOR
	/* シングルステップか停止要求中の場合 */
	if (dbg_is_stop_requested())
		return;
#endif

	/* オートモードかスキップモードの場合 */
	if (is_auto_mode() || is_skip_mode())
		return;

	/* 描画する */
	render_sysbtn(is_sysbtn_pointed());
}

/*
 * 重ね塗りを行う (dimming)
 *  - 全画面スタイルで、すでに読んだ部分を暗くするための文字描画
 */
static void blit_dimming(void)
{
	struct draw_msg_context context;

	/* コンフィグでdimmingが有効 */
	assert(conf_msgbox_dim);

	/* システムGUIに移行するときにはdimmingしない */
	assert(!need_sysmenu_mode);

	/* dimming用の文字色を求める */
	body_color = make_pixel(0xff,
				(uint32_t)conf_msgbox_dim_r,
				(uint32_t)conf_msgbox_dim_g,
				(uint32_t)conf_msgbox_dim_b);
	body_outline_color = make_pixel(0xff,
					(uint32_t)conf_msgbox_dim_outline_r,
					(uint32_t)conf_msgbox_dim_outline_g,
					(uint32_t)conf_msgbox_dim_outline_b);

	/* 本文を上書き描画する */
	construct_draw_msg_context(
		&context,
		LAYER_MSG,
		msg_top,
		conf_msgbox_font,
		conf_msgbox_font_size,
		conf_msgbox_font_size,
		conf_msgbox_font_ruby,
		conf_msgbox_font_outline,
		orig_pen_x,
		orig_pen_y,
		msgbox_w,
		msgbox_h,
		conf_msgbox_margin_left,
		conf_msgbox_margin_right,
		conf_msgbox_margin_top,
		conf_msgbox_margin_bottom,
		conf_msgbox_margin_line,
		conf_msgbox_margin_char,
		body_color,
		body_outline_color,
		0,	/* bg_color */
		false,	/* fill_bg */
		true,	/* is_dimming */
		false,	/* ignore_linefeed */
		false,	/* ignore_font */
		false,	/* ignore_outline */
		true,	/* ignore_color */
		false,	/* ignore_size */
		false,	/* ignore_position */
		false,	/* ignore_ruby */
		true,	/* ignore_wait */
		NULL,	/* inline_wait_hook */
		conf_msgbox_font_tategaki);
	draw_msg_common(&context, total_chars);
}

/*
 * その他
 */

/* SEを再生する */
static void play_se(const char *file)
{
	struct wave *w;

	if (file == NULL || strcmp(file, "") == 0)
		return;

	w = create_wave_from_file(SE_DIR, file, false);
	if (w == NULL)
		return;

	set_mixer_input(SYS_STREAM, w);
}

/* スキップ可能であるか調べる */
static bool is_skippable(void)
{
	bool is_seen;

	is_seen = get_seen();
	if (is_seen) {
		return true;
	} else {
		if (conf_msgbox_skip_unseen)
			return true;
		return false;
	}
}

/* テキスト読み上げを行う */
static void speak(void)
{
	if (!conf_tts)
		return;
	if (have_voice)
		return;

	speak_text(NULL);
	speak_text(msg_top);
}

/* 口パクを実行する */
static void init_lip_sync(void)
{
	int chpos;

	if (get_command_type() != COMMAND_SERIF)
		return;
	if (gui_sys_flag || gui_gosub_flag)
		return;

	chpos = get_talking_chpos();
	if (chpos == -1)
		return;

	run_lip_anime(chpos, msg_top);
}

/* 口パクを終了する */
static void cleanup_lip_sync(void)
{
	int chpos;

	chpos = get_talking_chpos();
	if (chpos == -1)
		return;

	stop_lip_anime(chpos);
}

/*
 * 終了処理
 */

/* A wrapper of stop_command_repetition() */
static void stop(void)
{
	if (need_sysmenu_mode || need_save_mode || need_load_mode || need_history_mode || need_config_mode) {
		/* オート/スキップモードでなければ */
		if (is_in_command_repetition())
			stop_command_repetition();
		return;
	}

	if (conf_msgbox_dim && !avoid_dimming) {
		need_dimming = true;
	} else {
		/* オート/スキップモードでなければ */
		if (is_in_command_repetition())
			stop_command_repetition();
	}
}

/* 終了処理を行う */
static bool cleanup(void)
{
	int chpos;

	/* @ichooseのためにペンの位置を保存する */
	set_pen_position(pen_x, pen_y);

	/* BGVoiceの再生中でない場合 */
	if (!is_bgvoice_playing()) {
		/* ボイスを停止する */
		set_mixer_input(VOICE_STREAM, NULL);
		is_voice_stopped = true;

		/* BGVoiceが指定されていれば再生する */
		check_and_play_bgvoice();
	}

	/* クリックアニメーションを非表示にする */
	show_click(false);

	/*
	 * 次のコマンドに移動するときは、表示中のメッセージをなしとする
	 *  - システムGUIに移行する場合は、アクティブなままにしておく
	 */
	if (!need_sysmenu_mode)
		clear_message_active();

	/* 既読にする */
	set_seen();

	/* 名前と本文を解放する */
	if (!need_sysmenu_mode && !need_save_mode && !need_load_mode && !need_history_mode && !need_config_mode) {
		if (name_top != NULL) {
			free(name_top);
			name_top = NULL;
		}
		if (msg_top != NULL) {
			free(msg_top);
			msg_top = NULL;
		}
		if (voice_file != NULL) {
			free(voice_file);
			voice_file = NULL;
		}
	}

	/* ページモードの場合 */
	if (is_page_mode())
		clear_buffered_message();

	/* 次のコマンドに移動する */
	if (!need_sysmenu_mode && !need_save_mode && !need_load_mode && !need_history_mode && !need_config_mode) {
		if (!move_to_next_command())
			return false;
	}

	/* 口パクのアニメを停止する */
	chpos = get_talking_chpos();
	if (chpos != -1)
		stop_lip_anime(chpos);

	return true;
}
