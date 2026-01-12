/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * Android App Assets and Save Data Access
 */

/* Base */
#include "stratohal/platform.h"

/* HAL */
#include "ndkmain.h"
#include "ndkfile.h"

/* C */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Path Size */
#define PATH_SIZE	(1024)

/*
 * Forward Declaration
 */
static void ungetc_rfile(struct rfile *rf, char c);

/*
 * Initialization
 */

/*
 * Initialize ndkfile.
 */
bool init_file(void)
{
	return true;
}

/*
 * Cleanup ndkfile.
 */
void cleanup_file(void)
{
}

/*
 * Read
 */

/*
 * Check if a file exists.
 */
bool check_file_exist(const char *file)
{
	char path[PATH_SIZE];
	struct rfile *rf;
	jclass cls;
	jmethodID mid;
	jboolean ret;

	/* Make a path. */
	snprintf(path, sizeof(path), "%s", file);

	/* Get a file content. */
	cls = (*jni_env)->FindClass(jni_env, "io/noctvm/playfield/engineandroid/MainActivity");
	mid = (*jni_env)->GetMethodID(jni_env, cls, "bridgeCheckFileExists", "(Ljava/lang/String;)Z");
	ret = (*jni_env)->CallBooleanMethod(jni_env, main_activity, mid, (*jni_env)->NewStringUTF(jni_env, path));
	if (ret) {
		/* File exists. */
		return true;
	}

	/* File does not exist. */
	return false;
}

/*
 * Open a file input stream.
 */
bool open_rfile(const char *file, struct rfile **rf)
{
	char path[PATH_SIZE];
	jclass cls;
	jmethodID mid;
	jobject ret;

	/* Make a path. */
	snprintf(path, sizeof(path), "%s", file);

	/* Get a file content. */
	cls = (*jni_env)->FindClass(jni_env, "io/noctvm/playfield/engineandroid/MainActivity");
	mid = (*jni_env)->GetMethodID(jni_env, cls, "bridgeGetFileContent", "(Ljava/lang/String;)[B");
	ret = (*jni_env)->CallObjectMethod(jni_env, main_activity, mid, (*jni_env)->NewStringUTF(jni_env, path));
	if (ret == NULL) {
		log_error("Failed to open file \"%s\".", file);
		return NULL;
	}

	/* Alloc a buffer for rfile. */
	*rf = malloc(sizeof(struct rfile));
	if (*rf == NULL) {
		log_out_of_memory();
		return NULL;
	}

	/* Set rfile members. */
	(*rf)->array = (jbyteArray)(*jni_env)->NewGlobalRef(jni_env, ret);
	(*rf)->buf = (char *)(*jni_env)->GetByteArrayElements(jni_env, (*rf)->array, JNI_FALSE);
	(*rf)->size = (uint64_t)(*jni_env)->GetArrayLength(jni_env, (*rf)->array);
	(*rf)->pos = 0;

	return rf;
}

/*
 * Get a file size.
 */
bool get_rfile_size(struct rfile *rf, size_t *size)
{
	*size = (size_t)rf->size;
	return true;
}

/*
 * Enable de-obfuscation on a read file stream.
 */
void decode_rfile(struct rfile *f)
{
	UNUSED_PARAMETER(f);
}

/*
 * Read from a file input stream.
 */
bool read_rfile(struct rfile *rf, void *buf, size_t size, size_t *ret)
{
	if (rf->pos + size > rf->size)
		size = (size_t)(rf->size - rf->pos);
	if (size == 0)
		return false;
	memcpy(buf, rf->buf + rf->pos, size);
	rf->pos += size;
	*ret = size;
	return true;
}

/* ungetc() */
static void ungetc_rfile(struct rfile *rf, char c)
{
	/* HINT: ignore c */
	assert(rf->pos != 0);
	rf->pos--;
}

/*
 * Read a line from an input file stream.
 */
bool gets_rfile(struct rfile *rf, char *buf, size_t size)
{
	char *ptr;
	size_t len, ret;
	char c;

	ptr = (char *)buf;

	for (len = 0; len < size - 1; len++) {
		if (!read_rfile(rf, &c, 1, &ret)) {
			*ptr = '\0';
			return len == 0 ? false : true;
		}
		if (c == '\n' || c == '\0') {
			*ptr = '\0';
			return true;
		}
		if (c == '\r') {
			if (!read_rfile(rf, &c, 1, &ret)) {
				*ptr = '\0';
				return true;
			}
			if (c == '\n') {
				*ptr = '\0';
				return true;
			}
			ungetc_rfile(rf, c);
			*ptr = '\0';
			return true;
		}
		*ptr++ = c;
	}
	*ptr = '\0';
	return true;
}

/*
 * Rewind a file input stream.
 */
void rewind_rfile(struct rfile *rf)
{
	rf->pos = 0;
}

/*
 * Close a file input stream.
 */
void close_rfile(struct rfile *rf)
{
#ifndef TARGET_ANDROID
	(*jni_env)->ReleaseByteArrayElements(jni_env, rf->array, (jbyte *)rf->buf, JNI_ABORT);
	(*jni_env)->DeleteGlobalRef(jni_env, rf[i]->array);
	free(delayed_rfile_free_slot);
#else
	/*
	 * Defer a removal of reference deletion when called from
	 * OpenSL callback as jni_env == NULL.
	 */
	if (jni_env != NULL) {
		/* In the main thread. */
		(*jni_env)->ReleaseByteArrayElements(jni_env, rf->array, (jbyte *)rf->buf, JNI_ABORT);
		(*jni_env)->DeleteGlobalRef(jni_env, rf->array);
		free(rf);
	} else {
		/* In the sound thread. */
		post_delayed_remove_rfile_ref(rf);
	}
#endif
}

/*
 * Write
 */

/*
 * Open a file output stream.
 */
bool open_wfile(const char *file, struct wfile **wf)
{
	/* Alloc a buffer for wfile */
	*wf = malloc(sizeof(struct wfile));
	if (*wf == NULL) {
		log_out_of_memory();
		return NULL;
	}

	/* Open a file. */
	jclass cls = (*jni_env)->FindClass(jni_env, "io/noctvm/playfield/engineandroid/MainActivity");
	jmethodID mid = (*jni_env)->GetMethodID(jni_env, cls, "bridgeOpenSaveFile", "(Ljava/lang/String;)Ljava/io/OutputStream;");
	jobject ret = (*jni_env)->CallObjectMethod(jni_env, main_activity, mid, (*jni_env)->NewStringUTF(jni_env, file));
	if (ret == NULL) {
		log_error("Failed to open file \"%s\".", file);
		free(wf);
		return NULL;
	}
	(*wf)->os = (*jni_env)->NewGlobalRef(jni_env, ret);

	return wf;
}

/*
 * Write to a file output stream.
 */
bool write_wfile(struct wfile *wf, const void *buf, size_t size, size_t *ret)
{
	jclass cls = (*jni_env)->FindClass(jni_env, "io/noctvm/playfield/engineandroid/MainActivity");
	jmethodID mid = (*jni_env)->GetMethodID(jni_env, cls, "bridgeWriteSaveFile", "(Ljava/io/OutputStream;I)Z");

	size_t i;
	for (i = 0; i < size; i++) {
		int c = (unsigned char)*((char *)buf + i);
		jboolean ret = (*jni_env)->CallBooleanMethod(jni_env, main_activity, mid, wf->os, c);
		if (ret != JNI_TRUE)
			return false;
	}

	/* Release, otherwise `local reference table` will overflow. */
	(*jni_env)->DeleteLocalRef(jni_env, cls);

	*ret = i;
	return true;
}

/*
 * Close a file output stream.
 */
void close_wfile(struct wfile *wf)
{
	jclass cls = (*jni_env)->FindClass(jni_env, "io/noctvm/playfield/engineandroid/MainActivity");
	jmethodID mid = (*jni_env)->GetMethodID(jni_env, cls, "bridgeCloseSaveFile", "(Ljava/io/OutputStream;)V");
	(*jni_env)->CallVoidMethod(jni_env, main_activity, mid, wf->os);
	(*jni_env)->DeleteGlobalRef(jni_env, wf->os);
	free(wf);
}

/*
 * Remove a file.
 */
void remove_file(const char *file)
{
	jclass cls = (*jni_env)->FindClass(jni_env, "io/noctvm/playfield/engineandroid/MainActivity");
	jmethodID mid = (*jni_env)->GetMethodID(jni_env, cls, "bridgeRemoveSaveFile", "(Ljava/lang/String;)V;");
	(*jni_env)->CallObjectMethod(jni_env, main_activity, mid, (*jni_env)->NewStringUTF(jni_env, file));
}
