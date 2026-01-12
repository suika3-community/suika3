/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (c) 2001-2026 The Suika3 Authors
 */

/*
 * Animation Subsystem
 */

#ifndef SUIKA3_ANIME_H
#define SUIKA3_ANIME_H

#include <suika3/suika3.h>

/*
 * Maximum amount of registered anime files.
 */
enum reg_anime_index {
	REG_ANIME_COUNT = 16,
};

/*
 * Acceleration
 */
enum anime_accel {
	ANIME_ACCEL_UNIFORM,
	ANIME_ACCEL_ACCEL,
	ANIME_ACCEL_DEACCEL,
};

/* Initialiez the anime subsystem. */
bool init_anime(void);

/* Cleanup the anime subsystem. */
void cleanup_anime(void);

/* Load an anime file. */
bool load_anime_from_file(const char *fname, int reg_index, bool *used_layer);

/* Clear the layer anime sequences. */
void clear_layer_anime_sequence(int layer);

/* Clear all anime sequences. */
void clear_all_anime_sequence(void);

/* Begin describing an anime for a layer. */
bool new_anime_sequence(int layer);

/* Add a float property into an anime sequence. */
bool add_anime_sequence_property_f(const char *key, float val);

/* Add an integer property into an anime sequence. */
bool add_anime_sequence_property_i(const char *key, int val);

/* Start the anime for a layer. */
bool start_layer_anime(int layer);

/* Check if there are running animes. */
bool is_anime_running(void);

/* Check if there are running animes. (with mask) */
bool is_anime_running_with_layer_mask(bool *used_layers);

/* Check if the layer anime is finished. */
bool is_anime_finished_for_layer(int layer);

/* Update the anime frame. */
void update_anime_frame(void);

/* Unregister a looped anime. */
void unregister_anime(int reg_index);

/* Get a looped anime file name. */
const char *get_reg_anime_file_name(int reg_index);

/* Load an eye blinking image. */
bool load_eye_image_if_exists(int chpos, const char *fname);

/* Reload an eye blinking image. */
bool reload_eye_anime(int chpos);

/* Load a lip synchronization image. */
bool load_lip_image_if_exists(int chpos, const char *fname);

/* Run a lip synchronization anime. */
void run_lip_anime(int chpos, const char *msg);

/* Stop a lip synchronization anime. */
void stop_lip_anime(int chpos);

#endif
