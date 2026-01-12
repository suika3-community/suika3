/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Playfield Engine
 * Copyright (c) 2025, 2026, Awe Morris. All rights reserved.
 */

/*
 * NoctLang Virtrual Machine
 */

#ifndef PLAYFIELD_VM_H
#define PLAYFIELD_VM_H

#include "stratohal/platform.h"

bool create_vm(char **title, int *width, int *height, bool *fullscreen);
void destroy_vm(void);
bool call_vm_function(const char *func_name);
bool call_vm_tag_function(bool *tag_end);
bool set_vm_int(const char *prop_name, int val);
bool get_vm_int(const char *prop_name, int *val);
size_t get_heap_usage(void);
void fast_gc(void);
void full_gc(void);
void *get_vm_env(void);

#endif
