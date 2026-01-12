/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * Ogg Vorbis Input Stream
 */

#include "stratohal/platform.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Ignore conversion problem in vorbisfile.h */
#if defined(__llvm__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#include <vorbis/vorbisfile.h>
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <vorbis/vorbisfile.h>
#pragma GCC diagnostic pop
#else
#include <vorbis/vorbisfile.h>
#endif

#define SAMPLING_RATE	(44100)
#define IOSIZE		(4096)

/*
 * PCM stream with a format of 44.1kHz, 16bit, stereo
 */
struct wave {
	/* Input file. */
	char *file;
	struct rfile *rf;
	bool loop;
	uint32_t loop_start;
	uint32_t loop_length;
	bool monaural;

	/*
	 * Enabled when loop=true.
	 *  -1: infinite loop
	 *  0-: remaining loop count
	 */
	int times;

	/* Status. */
	bool eos;
	bool err;
	bool do_skip;
	long consumed_bytes;

	/* Vorbis object. */
	OggVorbis_File ovf;
};

/*
 * Forward declarations.
 */
static bool reopen(struct wave *w, bool loop);
static size_t read_func(void *ptr, size_t size, size_t nmemb, void *datasource);
static int get_wave_samples_monaural(struct wave *w, uint32_t *buf, int samples);
static int get_wave_samples_stereo(struct wave *w, uint32_t *buf, int samples);
static void skip_if_needed(struct wave *w, int sample_bytes);

/*
 * Create a PCM stream from a file.
 */
struct wave *create_wave_from_file(const char *fname, bool loop)
{
	struct wave *w;
	vorbis_info *vi;
	vorbis_comment *vc;
	int i;

	const char *LOOPSTART = "LOOPSTART=";
	const char *LOOPLENGTH = "LOOPLENGTH=";
	const size_t LOOPSTART_LEN = strlen(LOOPSTART);
	const size_t LOOPLENGTH_LEN = strlen(LOOPLENGTH);

	UNUSED_PARAMETER(OV_CALLBACKS_STREAMONLY_NOCLOSE);
	UNUSED_PARAMETER(OV_CALLBACKS_STREAMONLY);
	UNUSED_PARAMETER(OV_CALLBACKS_STREAMONLY);
	UNUSED_PARAMETER(OV_CALLBACKS_NOCLOSE);
	UNUSED_PARAMETER(OV_CALLBACKS_NOCLOSE);
	UNUSED_PARAMETER(OV_CALLBACKS_DEFAULT);

	/* Alloc a wave struct. */
	w = malloc(sizeof(struct wave));
	if (w == NULL) {
		log_out_of_memory();
		return NULL;
	}
	memset(w, 0, sizeof(struct wave));

	/* Save a file name. */
	w->file = strdup(fname);
	if (w->file == NULL) {
		log_out_of_memory();
		free(w);
		return NULL;
	}

	/* Open a file. */
	if (!reopen(w, false))
		return NULL;

	/* TODO: Check sampling rate and channel count by using ov_info(). */
	vi = ov_info(&w->ovf, -1);
	w->monaural = vi->channels == 1 ? true : false;

	/* Alloc a wave struct. */
	w->loop = loop;
	w->loop_start = 0;
	w->loop_length = 0;
	w->times = -1;
	w->eos = false;
	w->err = false;
	w->do_skip = false;
	w->consumed_bytes = 0;

	/* Get LOOPSTART and LOOPLENGTH. */
	vc = ov_comment(&w->ovf, -1);
	if (vc != NULL) {
		for (i = 0; i < vc->comments; i++) {
			if (strncmp(vc->user_comments[i], LOOPSTART, LOOPSTART_LEN) == 0) {
				w->loop = true;
				w->loop_start = (uint32_t)atol(vc->user_comments[i] + LOOPSTART_LEN);
			} else if (strncmp(vc->user_comments[i], LOOPLENGTH, LOOPLENGTH_LEN) == 0) {
				w->loop = true;
				w->loop_length = (uint32_t)atol(vc->user_comments[i] + LOOPLENGTH_LEN);
			}
		}
	}

	/* Succeeded. */
	return w;
}

/* Reopen a file. */
static bool reopen(struct wave *w, bool loop)
{
	ov_callbacks cb;
	int err;

	if (w->rf == NULL) {
		/* Open a file input stream. */
		if (!open_rfile(w->file, &w->rf)) {
			free(w->file);
			free(w);
			return false;
		}
	} else {
		rewind_rfile(w->rf);
	}

	/* Open a file by a callback. */
	memset(&cb, 0, sizeof(cb));
	cb.read_func = read_func;
	cb.close_func = NULL;
	cb.seek_func = NULL;
	cb.tell_func = NULL;
	err = ov_open_callbacks(w, &w->ovf, NULL, 0, cb);
	if (err != 0) {
		log_error("Audio file format error (%s).", w->file);
		free(w->file);
		free(w);
		return false;
	}

	w->do_skip = loop;
	w->consumed_bytes = 0;

	return true;
}

/* File input callback. */
static size_t read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	struct wave *w;	
	size_t len;

	assert(ptr != NULL);
	assert(datasource != NULL);

	w = (struct wave *)datasource;

	if (!read_rfile(w->rf, ptr, size * nmemb, &len))
		return 0;

	return len / size;
}

#if 0
/* File close callback. */
static int close_func(void *datasource)
{
	struct wave *w;

	assert(datasource != NULL);

	w = (struct wave *)datasource;

	if (w->loop && w->times != -1) {
		close_rfile(w->rf);
		w->rf = NULL;
	}

	return 0;
}
#endif

/*
 * Set a loop count for a PCM stream.
 */
void set_wave_repeat_times(struct wave *w, int n)
{
	assert(w != NULL);
	assert(n > 0);

	w->times = n;
}

/*
 * Destroy a PCM stream.
 */
void destroy_wave(struct wave *w)
{
	ov_clear(&w->ovf);
	free(w->file);
	if (w->rf != NULL)
		close_rfile(w->rf);
	free(w);
}

/*
 * Check if a PCM stream reached end-of-stream.
 */
bool is_wave_eos(struct wave *w)
{
	return w->eos;
}

/*
 * Get PCM samples from a stream.
 */
int get_wave_samples(struct wave *w, uint32_t *buf, int samples)
{
	/* If already reached end-of-stream. */
	if (w->eos)
		return 0;

	/* Monaural case. */
	if (w->monaural)
		return get_wave_samples_monaural(w, buf, samples);

	/* Stereo case. */
	return get_wave_samples_stereo(w, buf, samples);
}

/* Get samples from a monaural stream. */
static int get_wave_samples_monaural(struct wave *w, uint32_t *buf, int samples)
{
	unsigned char mbuf[IOSIZE];
	long read_bytes, ret_bytes, last_ret_bytes;
	int retain, bitstream, i;
	bool loop_end;

	/* Until finish reading or reach eos. */
	retain = 0;
	last_ret_bytes = -1;
	while (retain < samples) {
		/* Skip for LOOPSTART. */
		skip_if_needed(w, 2);

		/* Decode. */
		read_bytes = (samples - retain) * 2 > IOSIZE ? IOSIZE :
			     (samples - retain) * 2;
		loop_end = false;
		if (w->loop_length > 0 &&
		    w->consumed_bytes + read_bytes >= (long)(w->loop_start + w->loop_length) * 2) {
			read_bytes = (long)(w->loop_start + w->loop_length) * 2 - w->consumed_bytes;
			loop_end = true;
		}
		ret_bytes = ov_read(&w->ovf, (char *)mbuf, (int)read_bytes, 0, 2, 1, &bitstream);
		if (ret_bytes == 0 || (loop_end && ret_bytes == read_bytes)) {
			/* End-of-stream. */
			if ((w->loop && (w->times == -1 || w->times > 0)) || loop_end) {
				/* Reopen. */
				if (last_ret_bytes == 0)
					return 0; 	/* Error */
				ov_clear(&w->ovf);
				if (!reopen(w, true))
					return 0;	/* Error */
				last_ret_bytes = 0;
				if (w->times != -1)
					w->times--;
			} else {
				/* Return read samples. */
				w->eos = true;
				return retain;
			}
		} else {
			w->consumed_bytes += ret_bytes;
		}

		/* Convert to stereo. */
		for (i = 0; i < ret_bytes / 2; i++) {
			buf[retain + i] = (uint32_t)(mbuf[i*2] |
						     (mbuf[i*2 + 1] << 8) |
						     (mbuf[i*2] << 16) |
						     (mbuf[i*2 + 1] << 24));
		}
		retain += (int)ret_bytes / 2;
	}

	/* Fully filled. */
	return samples;
}

/* Get samples from a stereo stream. */
static int get_wave_samples_stereo(struct wave *w, uint32_t *buf, int samples)
{
	long read_bytes, ret_bytes, last_ret_bytes;
	int retain, bitstream;
	bool loop_end;

	/* Until finish reading or reach eos. */
	retain = 0;
	last_ret_bytes = -1;
	while (retain < samples) {
		/* Skip for LOOPSTART. */
		skip_if_needed(w, 4);

		/* Decode. */
		read_bytes = (samples - retain) * 4;
		loop_end = false;
		if (w->loop_length > 0 &&
		    w->consumed_bytes + read_bytes >= (long)(w->loop_start + w->loop_length) * 4) {
			read_bytes = (long)(w->loop_start + w->loop_length) * 4 - w->consumed_bytes;
			loop_end = true;
		}
		ret_bytes = ov_read(&w->ovf, (char *)(buf + retain), (int)read_bytes, 0, 2, 1, &bitstream);
		if (ret_bytes == 0 || (loop_end && ret_bytes == read_bytes)) {
			/* End-of-stream. */
			if ((w->loop && (w->times == -1 || w->times > 0)) || loop_end) {
				/* Reopen. */
				if (last_ret_bytes == 0)
					return 0; 	/* Error. */
				ov_clear(&w->ovf);
				if (!reopen(w, true))
					return 0;	/* Error. */
				last_ret_bytes = 0;
				if (w->times != -1)
					w->times--;
			} else {
				/* Return read sample. */
				w->eos = true;
				return retain;
			}
		} else {
			w->consumed_bytes += ret_bytes;
		}
		retain += (int)ret_bytes / 4;
	}

	/* Fully filled. */
	return samples;
}

/* Skip for LOOPSTART. */
static void skip_if_needed(struct wave *w, int sample_bytes)
{
	uint32_t buf[1024];
	size_t remain_samples;
	size_t get_samples;
	long ret_bytes;
	int bitstream;

	if (!w->do_skip || w->loop_start == 0)
		return;

	remain_samples = w->loop_start;
	while (remain_samples > 0) {
		get_samples = remain_samples > 1024 ? 1024 : remain_samples;

		ret_bytes = ov_read(&w->ovf,
				    (char *)buf,
				    (int)get_samples * sample_bytes,
				    0,
				    2,
				    1,
				    &bitstream);
		if (ret_bytes <= 0)
			break;

		remain_samples -= (size_t)(ret_bytes / sample_bytes);
	}

	w->do_skip = false;
	w->consumed_bytes = (long)w->loop_start * sample_bytes;
}
