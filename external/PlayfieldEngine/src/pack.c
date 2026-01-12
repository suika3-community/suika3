/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Playfield Engine
 * Copyright (c) 2025, 2026, Awe Morris. All rights reserved.
 */

int command_archive(int argc, char *argv[]);

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
#ifdef _WIN32
	if (argc == 1)
	{
		MessageBoxW(NULL,
			    L"Please specify files and folders in relative paths to make an archive file.",
			    L"Playfield Archiver",
			    MB_OK | MB_ICONINFORMATION);
		return 1;
	}
	else
	{
		MessageBoxW(NULL,
			    L"Playfield Archiver will make an archive file. This will take a while.",
			    L"Playfield Archiver",
			    MB_OK | MB_ICONINFORMATION);
	}
#endif

	int ret = command_archive(argc, argv);

#ifdef _WIN32
	if (ret == 0)
	{
		MessageBoxW(NULL,
			    L"Done making the assets.arc file.",
			    L"Playfield Archiver",
			    MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		MessageBoxW(NULL,
			    L"Error(s) on making the assets.arc file. See the console.",
			    L"Playfield Archiver",
			    MB_OK | MB_ICONINFORMATION);
	}
#endif

	return ret;
}
