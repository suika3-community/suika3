/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Playfield Engine
 * Copyright (c) 2025, 2026, Awe Morris. All rights reserved.
 */

/*
 * Dummy Initialization Hook
 */

#include <playfield/c89compat.h>

/*
 * Special initialization function for derived engines.
 */
bool
pf_init_hook(
	int width,
	int height)
{
	/* Stub. */
	return true;
}

#if defined(PF_TARGET_PC98)
/*
 * OpenWatcom cannot search main() in libraries.
 * We define main() here, and call main2() in libstrato.
 */
int main(int argc, char argv[])
{
	int main2(int argc, char *argv[]);
	return main2(argc, argv);
}
#endif
