/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Noct Programming Language
 * Copyright (c) 2025, 2026, Awe Morris
 */

/*
 * Interpreter
 */

#ifndef NOCT_INTERPRETER_H
#define NOCT_INTERPRETER_H

#include <noct/c89compat.h>

/* Visit bytecode. */
bool
rt_visit_bytecode(struct rt_env *rt, struct rt_func *func);

#endif

