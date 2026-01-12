/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Suika3
 * Copyright (c) 2001-2026 The Suika3 Authors
 */

/*
 * Tag File
 */

#include <suika3/suika3.h>
#include "tag.h"

#include <playfield/playfield.h>

/*
 * Initialize the tag subsystem.
 */
bool
init_tag(void)
{
	if (pf_move_to_tag_file(S3_FIRST_TAG_FILE))
		return false;

	return true;
}

/*
 * Cleanup the tag subsystem.
 */
void
cleanup_tag(void)
{
}

/*
 * Move to a tag file.
 */
bool
s3_move_to_tag_file(const char *file)
{
	if (pf_move_to_tag_file(S3_FIRST_TAG_FILE))
		return false;

	return true;
}

/*
 * Move to a next tag.
 */
bool
s3_move_to_next_tag(void)
{
	if (pf_move_to_next_tag())
		return false;

	return true;
}

/*
 * Move to a label.
 */
bool
s3_move_to_label(
	const char *label)
{
	/* TODO */
	return false;
}

/*
 * Get the current tag file name.
 */
const char *
s3_get_tag_file(void)
{
	return pf_get_tag_file();
}

/*
 * Get the command index of the current tag.
 */
int
s3_get_tag_index(void)
{
	return pf_get_tag_index();
}

/*
 * Get the line number of the current tag.
 */
int
s3_get_tag_line(void)
{
	return pf_get_tag_line();
}

/*
 * Tag Argument Access
 */

/*
 * Check if a tag argument exists.
 */
bool
s3_check_tag_arg(
	const char *name)
{
	int i, count;
	const char *prop_name;

	count = pf_get_tag_property_count();

	for (i = 0; i < count; i++) {
		prop_name = pf_get_tag_property_name(i);
		if (strcmp(name, prop_name) == 0)
			return true;
	}

	return false;
}

/*
 * Get an integer tag argument.
 */
int
s3_get_tag_arg_int(
	const char *name)
{
	int i, count;
	const char *prop_name;
	const char *prop_value;
	int val;

	count = pf_get_tag_property_count();

	for (i = 0; i < count; i++) {
		prop_name = pf_get_tag_property_name(i);
		if (strcmp(name, prop_name) == 0)
			break;
	}
	if (i == count) {
		/* Not found, return the default value. */
		return 0;
	}

	prop_value = pf_get_tag_property_value(i);
	val = atoi(prop_value);

	return val;
}

/*
 * Get a float tag argument.
 */
float
s3_get_tag_arg_float(
	const char *name)
{
	int i, count;
	const char *prop_name;
	const char *prop_value;
	float val;

	count = pf_get_tag_property_count();

	for (i = 0; i < count; i++) {
		prop_name = pf_get_tag_property_name(i);
		if (strcmp(name, prop_name) == 0)
			break;
	}
	if (i == count) {
		/* Not found, return the default value. */
		return 0;
	}

	prop_value = pf_get_tag_property_value(i);
	val = (float)atof(prop_value);

	return val;
}

/*
 * Get a string tag argument.
 */
const char *
s3_get_tag_arg_string(
	const char *name)
{
	int i, count;
	const char *prop_name;
	const char *prop_value;

	count = pf_get_tag_property_count();

	for (i = 0; i < count; i++) {
		prop_name = pf_get_tag_property_name(i);
		if (strcmp(name, prop_name) == 0)
			break;
	}
	if (i == count) {
		/* Not found, return the default value. */
		return 0;
	}

	prop_value = pf_get_tag_property_value(i);

	return prop_value;
}
