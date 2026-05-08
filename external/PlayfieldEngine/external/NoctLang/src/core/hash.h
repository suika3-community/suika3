/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Noct Programming Language
 * Copyright (c) 2025, 2026, Awe Morris
 */

/*
 * Hash
 */

#ifndef NOCT_HASH_H
#define NOCT_HASH_H

#include <noct/c89compat.h>

uint32_t string_hash(const char *s);
void string_hash_and_len(const char *s, uint32_t *hash, uint32_t *len);

#endif
