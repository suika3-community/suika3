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
