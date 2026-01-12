/* -*- tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * Dummy Sound
 */

#if !defined(__linux__) && !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__) && !defined(__sun)

#include "platform.h"

/*
 * Initialize sound.
 */
bool init_sound(void)
{
	return true;
}

/*
 * Cleanup sound.
 */
void cleanup_sound(void)
{
}

/*
 * Start sound playback on a stream.
 */
bool play_sound(int n, struct wave *w)
{
	UNUSED_PARAMETER(n);
	UNUSED_PARAMETER(w);
	return true;
}

/*
 * Stop sound playback on a stream.
 */
bool stop_sound(int n)
{
	UNUSED_PARAMETER(n);
	return true;
}

/*
 * Set a sound volume for a stream.
 */
bool set_sound_volume(int n, float vol)
{
	UNUSED_PARAMETER(n);
	UNUSED_PARAMETER(vol);
	return true;
}

/*
 * Check if a sound stream is finished.
 */
bool is_sound_finished(int n)
{
	UNUSED_PARAMETER(n);
	return true;
}

#endif
