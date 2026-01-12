/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * JNI code for engine-android
 */

#ifndef PLATFORM_NDKMAIN_H
#define PLATFORM_NDKMAIN_H

#include "stratohal/c89compat.h"
#include <jni.h>

struct rfile {
	jbyteArray array;
	char *buf;
	uint64_t size;
	uint64_t pos;
};

struct wfile {
	jobject os;
};

extern JNIEnv *jni_env;
extern jobject main_activity;

void post_delayed_remove_rfile_ref(struct rfile *rf);

#endif
