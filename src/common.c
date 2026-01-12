/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Playfield Engine
 * Copyright (c) 2025, 2026, Awe Morris. All rights reserved.
 */

/*
 * Common Routines
 */

#include "engine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*
 * Load a file content.
 */
bool load_file(const char *file, char **buf, size_t *size)
{
	struct rfile *f;
	size_t file_size, read_size;

	assert(buf != NULL);

	if (!open_rfile(file, &f)) {
		log_error(PPS_TR("Cannot open file \"%s\"."), file);
		return false;
	}

	if (!get_rfile_size(f, &file_size)) {
		log_error(PPS_TR("Cannot get the size of file \"%s\"."), file);
		return false;
	}

	if (buf != NULL) {
		*buf = malloc(file_size + 1);
		if (*buf == NULL) {
			log_out_of_memory();
			return false;
		}

		if (!read_rfile(f, *buf, file_size, &read_size)) {
			log_error(PPS_TR("Cannot read file \"%s\"."), file);
			free(*buf);
			return false;
		}

		(*buf)[file_size] = '\0';
	}

	close_rfile(f);

	if (size != NULL)
		*size = file_size;

	return true;
}
