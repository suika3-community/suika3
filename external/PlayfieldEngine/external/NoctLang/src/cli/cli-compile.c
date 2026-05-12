/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Noct Programming Language
 * Copyright (c) 2025, 2026, Awe Morris
 */

/*
 * CLI: Compile Mode
 */

#include "cli-main.h"

/* Forward declaration. */
static bool compile_source(const char *file_name);

/*
 * The top level function for the compile mode.
 */
int
command_compile(
	int argc,
	char *argv[])
{
	int i;

	/* For each argument file. */
	for (i = 2; i < argc; i++) {
		/* Compile a source to bytecode. */
		if (!compile_source(argv[i]))
			return 1;
	}

	return 1;
}

/* Compile a source file. */
static bool
compile_source(
	const char *file_name)
{
	char bc_fname[1024];
	char *source_data, *dot;
	size_t source_length;

	/* Load an argument source file. */
	if (!load_file_content(file_name, &source_data, &source_length))
		return false;

	/* Make an output file name. (*.nb) */
	strcpy(bc_fname, file_name);
	dot = strstr(bc_fname, ".");
	if (dot != NULL)
		strcpy(dot, ".nb");
	else
		strcat(bc_fname, ".nb");

	/* Start translation. */
	if (!noct_bcback_start(bc_fname)) {
		free(source_data);
		return false;
	}

	/* Translate. */
	if (!noct_bcback_translate(file_name, source_data)) {
		free(source_data);
		return false;
	}
	free(source_data);

	/* Finalize. */
	if (!noct_bcback_finalize())
		return false;

	return true;
}
