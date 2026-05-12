/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Noct Programming Language
 * Copyright (c) 2025, 2026, Awe Morris
 */

/*
 * CLI: C Translation Mode
 */

#include "cli-main.h"

/* Bytecode File Header */
#define BYTECODE_HEADER		"Noct Bytecode"

/* i18n.c */
#if defined(NOCT_USE_TRANSLATION)
void noct_init_locale(void);
#endif

/* Forward declaration. */
static bool do_transpile_c(const char *out_file, int in_file_count, const char *in_file[]);
static bool add_file_hook_c(const char *fname);

/*
 * C Translation
 */

/*
 * The top level function for the C translation mode.
 */
int
command_transpile_c(
	int argc,
	char *argv[])
{
	if (argc < 4) {
		show_usage();
		return 1;
	}

	if (!do_transpile_c(argv[2], argc - 3, (const char **)&argv[3]))
		return 1;

	return 0;
}

/* Do C translation. */
static bool
do_transpile_c(
	const char *out_file,
	int in_file_count,
	const char *in_file[])
{
	int i;

	/* Initialize the backend. */
	if (!noct_cback_start(out_file))
		return false;

	/* For each input file or directory. */
	for (i = 0; i < in_file_count; i++) {
		/* Recursively add files. */
		if (!add_file(in_file[i], add_file_hook_c))
			return false;
	}

	/* Put a epilogue code. */
	if (!noct_cback_finalize())
		return false;

	return true;
}

/* "On file add" callback for the recursive file search. */
static bool
add_file_hook_c(
	const char *fname)
{
	char *data;
	size_t len;

	/* Load a file. */
	if (!load_file_content(fname, &data, &len))
		return false;

	/* Translate. */
	if (!noct_cback_translate(fname, data)) {
		free(data);
		return false;
	}

	free(data);
	return true;
}
