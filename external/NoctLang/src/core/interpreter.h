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

#define ex_make_string_with_hash noct_ex_make_string_with_hash
#define ex_make_empty_array noct_ex_make_empty_array
#define ex_make_empty_dict noct_ex_make_empty_dict
#define ex_add_helper noct_ex_add_helper
#define ex_sub_helper noct_ex_sub_helper
#define ex_mul_helper noct_ex_mul_helper
#define ex_div_helper noct_ex_div_helper
#define ex_mod_helper noct_ex_mod_helper
#define ex_and_helper noct_ex_and_helper
#define ex_or_helper noct_ex_or_helper
#define ex_xor_helper noct_ex_xor_helper
#define ex_shl_helper noct_ex_shl_helper
#define ex_shr_helper noct_ex_shr_helper
#define ex_neg_helper noct_ex_neg_helper
#define ex_not_helper noct_ex_not_helper
#define ex_lt_helper noct_ex_lt_helper
#define ex_lte_helper noct_ex_lte_helper
#define ex_eq_helper noct_ex_eq_helper
#define ex_neq_helper noct_ex_neq_helper
#define ex_gte_helper noct_ex_gte_helper
#define ex_gt_helper noct_ex_gt_helper
#define ex_storearray_helper noct_ex_storearray_helper
#define ex_loadarray_helper noct_ex_loadarray_helper
#define ex_len_helper noct_ex_len_helper
#define ex_getdictkeybyindex_helper noct_ex_getdictkeybyindex_helper
#define ex_getdictvalbyindex_helper noct_ex_getdictvalbyindex_helper
#define ex_loadsymbol_helper noct_ex_loadsymbol_helper
#define ex_storesymbol_helper noct_ex_storesymbol_helper
#define ex_loaddot_helper noct_ex_loaddot_helper
#define ex_storedot_helper noct_ex_storedot_helper
#define ex_call_helper noct_ex_call_helper
#define ex_thiscall_helper noct_ex_thiscall_helper

/* Visit bytecode. */
bool
rt_visit_bytecode(struct rt_env *rt, struct rt_func *func);

#endif

