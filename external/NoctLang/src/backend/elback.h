/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Noct Programming Language
 * Copyright (c) 2025, 2026, Awe Morris
 */

/*
 * elback: GNU Emacs Lisp translation backend
 */

#ifndef NOCT_ELBACK_H
#define NOCT_ELBACK_H

#include <noct/c89compat.h>
#include <stdio.h>

struct hir_block;

/* Clear translator states. */
bool elback_init(const char *fname);

/* Translate HIR to elisp. */
bool elback_translate_func(struct hir_block *func);

/* Put a finalization code. */
bool elback_finalize(void);

#endif
