/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (c) 2001-2026, The authors. All rights reserved.
 */

/*
 * Command
 */

#ifndef SUIKA_COMMAND_H
#define SUIKA_COMMAND_H

#include <suika.h>

/*
 * Command Implementation
 */

bool message_command(bool *cont);
bool anime_command(void);
bool bg_command(void);
bool ch_command(void);
bool chch_command(void);
bool chapter_command(void);
bool choose_command(void);
bool click_command(void);
bool config_command(void);
bool gosub_command(void);
bool goto_command(bool *cont);
bool if_command(void);
bool return_command(bool *cont);
bool story_command(void);
bool set_command(void);
bool shake_command(void);
bool skip_command(void);
bool sound_command(void);
bool text_command(void);
bool time_command(void);
bool layer_command(void);
bool menu_command(void);
bool move_command(void);
bool music_command(void);
bool plugin_command(void);
bool video_command(void);
bool volume_command(void);
bool ciel_command(bool *cont);
bool addmsg_command(void);

#endif
