/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (C) 2001-2026 The Suika3 Authors
 */

/*
 * Seen Flag
 */

#include <suika3/suika3.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>

/* Maximum seen flags per tag file. */
#define SEEN_COUNT	(8192)

/*
 * Seen flag.
 *  - Each flag indicates a text or a choose.
 *  - For a text, a flag must be 0 (unseen) or 1 (seen).
 *  - For a choose, the bit N of a flag indicates whether the option N was chosen (1) or not (0).
 */
static uint8_t seen_flag[SEEN_COUNT / 8];

/* Is initialized. */
static bool is_initialized;

/* Forward declarations. */
static const char *hash(const char *file);
static char hex(int c);

/*
 * Initialize the seen subsystem.
 */
bool
s3i_init_seen(void)
{
	is_initialized = false;
	memset(seen_flag, 0, sizeof(seen_flag));

	return true;
}

/*
 * Cleanup the seen subsystem.
 */
void
s3i_cleanup_seen(void)
{
	if (is_initialized) {
		is_initialized = false;
		memset(seen_flag, 0, sizeof(seen_flag));
	}
}

/*
 * Load the seen file for the current tag file.
 */
bool
s3_load_seen(void)
{
	char key[128];
	size_t file_size;

	/* Clear all flags. */
	memset(seen_flag, 0, sizeof(seen_flag));

	/* Get the save data key. */
	snprintf(key, sizeof(key), "s-%s", hash(s3_get_tag_file()));

	/* Read the save data. */
	if (!s3_read_save_data(key, &seen_flag, sizeof(seen_flag), &file_size))
		return false;

	return true;
}

/*
 * Save the seen file for the current tag file.
 */
bool
s3_save_seen(void)
{
	char key[128];
	const char *fname;
	size_t write_size;

	/* Get the save data key. */
	snprintf(key, sizeof(key), "s-%s", hash(s3_get_tag_file()));

	/* Write the save data. */
	if (!s3_write_save_data(key, &seen_flag, sizeof(seen_flag)))
		return false;
	if (write_size != sizeof(seen_flag))
		return false;

	return true;
}

/*
 * Get the seen flags for the current tag.
 */
int
s3_get_seen_flags(void)
{
	int index;

	index = s3_get_tag_index();
	if (index < SEEN_COUNT)
		return seen_flag[index];
	return false;
}

/*
 * Set the seen flags for the current tag.
 */
void
s3_set_seen_flags(int flag)
{
	int index;

	index = s3_get_tag_index();
	if (index < SEEN_COUNT)
		seen_flag[index] = flag;
}

/* Get a hash string from a tag file name. */
static const char *
hash(const char *file)
{
	static char h[129];
	int len, i;

	len = (int)strlen(file);
	if (len > 128)
		len = 128;

	memset(h, 0, sizeof(h));

	for (i = 0; i < len; i++) {
		h[i * 2] = hex(file[i] >> 4);
		h[i * 2 + 1] = hex(file[i] & 0x0f);
	}

	return h;
}

/* Get a hex code. */
static char hex(int c)
{
	assert(c >= 0 && c <= 15);

	if (c >= 0 && c <= 9)
		return (char)('0' + (char)c);
	else
		return (char)('a' + (char)(c - 10));
}
