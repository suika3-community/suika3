/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Noct Programming Language
 * Copyright (c) 2025, 2026, Awe Morris
 */

/*
 * Ahead-of-Time Compilation Helpers
 */

struct rt_vm;
struct rt_env;
struct rt_frame;
struct rt_value;
struct rt_object_header;
struct rt_string;
struct rt_array;
struct rt_dict;
struct rt_func;
struct rt_bindglobal;

/*
 * Initialization
 */

/* Create a runtime environment. */
bool
rt_create_vm(
	struct rt_vm **vm,
	struct rt_env **default_env);

/* Destroy a runtime environment. */
bool
rt_destroy_vm(
	struct rt_vm *vm);

/* Create an environment for the current thread. */
bool
rt_create_thread_env(
	struct rt_env *prev_env,
	struct rt_env **new_env);

/*
 * Compilation
 */

/* Register functions from a souce text. */
bool
rt_register_source(
	struct rt_env *env,
	const char *file_name,
	const char *source_text);

/* Register functions from bytecode data. */
bool
rt_register_bytecode(
	struct rt_env *env,
	size_t size,
	uint8_t *data);

/* Register an FFI C function. */
bool
rt_register_cfunc(
	struct rt_env *env,
	const char *name,
	uint32_t param_count,
	const char *param_name[],
	bool (*cfunc)(struct rt_env *env),
	struct rt_func **ret_func);

/*
 * Call
 */

/* Call a function with a name. */
bool
rt_call_with_name(
	struct rt_env *env,
	const char *func_name,
	uint32_t arg_count,
	struct rt_value *arg,
	struct rt_value *ret);

/* Call a function. */
bool
rt_call(
	struct rt_env *env,
	struct rt_func *func,
	uint32_t arg_count,
	struct rt_value *arg,
	struct rt_value *ret);

/*
 * String
 */

/* Make a string value. */
bool
rt_make_string(
	struct rt_env *env,
	struct rt_value *val,
	const char *data);

/* Make a string value. (hash version) */
bool
rt_make_string_with_hash(
	struct rt_env *env,
	struct rt_value *val,
	const char *data,
	size_t len,
	uint32_t hash);

/* Cache the hash of a string. */
void
rt_cache_string_hash(
	struct rt_string *rts);

/*
 * Array and Dictionary
 */

/* Make an empty array. */
bool
rt_make_empty_array(
	struct rt_env *env,
	struct rt_value *val);

/* Get the size of an array. */
bool
rt_get_array_size(
	struct rt_env *env,
	struct rt_array *arr,
	uint32_t *size);

/* Retrieves an array element. */
bool
rt_get_array_elem(
	struct rt_env *env,
	struct rt_array *arr,
	uint32_t index,
	struct rt_value *val);

/* Stores an value to an array. */
bool
rt_set_array_elem(
	struct rt_env *env,
	struct rt_array **arr,
	uint32_t index,
	struct rt_value *val);

/* Resizes an array. */
bool
rt_resize_array(
	struct rt_env *env,
	struct rt_array **arr,
	uint32_t size);

/* Make a shallow copy of an array. */
bool
rt_make_array_copy(
	struct rt_env *env,
	struct rt_array **dst,
	struct rt_array *src);

/* Make an empty dictionary value. */
bool
rt_make_empty_dict(
	struct rt_env *env,
	struct rt_value *val);

/* Get the size of a dictionary. */
bool
rt_get_dict_size(
	struct rt_env *env,
	struct rt_dict *dict,
	uint32_t *size);

/* Checks if a key exists in a dictionary. */
bool
rt_check_dict_key(
	struct rt_env *env,
	struct rt_dict *dict,
	const char *key,
	bool *ret);

/* Get a dictionary key by index. */
bool
rt_get_dict_key_by_index(
	struct rt_env *env,
	struct rt_dict *dict,
	uint32_t index,
	struct rt_value *key);

/* Get a dictionary value by index. */
bool
rt_get_dict_value_by_index(
	struct rt_env *env,
	struct rt_dict *dict,
	uint32_t index,
	struct rt_value *val);

/* Retrieves the value by a key in a dictionary. */
bool
rt_get_dict_elem(
	struct rt_env *env,
	struct rt_dict *dict,
	const char *key,
	struct rt_value *val);

/* Retrieves the value by a key in a dictionary. (hash version) */
bool
rt_get_dict_elem_with_hash(
	struct rt_env *env,
	struct rt_dict *dict,
	const char *key,
	size_t len,
	uint32_t hash,
	struct rt_value *val);

/* Stores a key-value-pair to a dictionary. */
bool
rt_set_dict_elem(
	struct rt_env *env,
	struct rt_dict **dict,
	const char *key,
	struct rt_value *val);

/* Stores a key-value-pair to a dictionary. (hash version) */
bool
rt_set_dict_elem_with_hash(
	struct rt_env *env,
	struct rt_dict **dict,
	const char *key,
	size_t len,
	uint32_t hash,
	struct rt_value *val);

/* Remove a dictionary key. */
bool
rt_remove_dict_elem(
	struct rt_env *env,
	struct rt_dict *dict,
	const char *key);

/* Remove a dictionary key. (hash version) */
bool
rt_remove_dict_elem_with_hash(
	struct rt_env *env,
	struct rt_dict *dict,
	const char *key,
	size_t len,
	uint32_t hash);

/* Make a shallow copy of a dictionary. */
bool
rt_make_dict_copy(
	struct rt_env *env,
	struct rt_dict **dst,
	struct rt_dict *src);

/*
 * Global Variable
 */

/* Check if a global variable exists. */
bool
rt_check_global(
	struct rt_env *env,
	const char *name);

/* Get a global variable. */
bool
rt_get_global(
	struct rt_env *env,
	const char *name,
	struct rt_value *val);

/* Get a global variable. (hash version) */
bool
rt_get_global_with_hash(
	struct rt_env *env,
	const char *name,
	size_t len,
	uint32_t hash,
	struct rt_value *val);

/* Set a global variable. */
bool
rt_set_global(
	struct rt_env *env,
	const char *name,
	struct rt_value *val);

/* Set a global variable. (hash version) */
bool
rt_set_global_with_hash(
	struct rt_env *env,
	const char *name,
	size_t len,
	uint32_t hash,
	struct rt_value *val);

/*
 * FFI Pin
 */

/* Pin a C global variable. */
bool
rt_pin_global(
	struct rt_env *env,
	struct rt_value *val);

/* Pin a C global variable. */
bool
rt_unpin_global(
	struct rt_env *env,
	struct rt_value *val);

/* Pin a C local variable. */
bool
rt_pin_local(
	struct rt_env *env,
	struct rt_value *val);

/* Unpin a C local variable. */
bool
rt_unpin_local(
	struct rt_env *env,
	struct rt_value *val);

/*
 * Error Handling
 */

/* Get an error message. */
const char *
rt_get_error_message(
	struct rt_env *env);

/* Get an error file name. */
const char *
rt_get_error_file(
	struct rt_env *env);

/* Get an error line number. */
int
rt_get_error_line(
	struct rt_env *env);

/* Output an error message.*/
void
rt_error(
	struct rt_env *env,
	const char *msg,
	...);

/* Output an out-of-memory message. */
void
rt_out_of_memory(
	struct rt_env *env);

/*
 * Execution Helpers
 */

bool
rt_assign_helper(
	struct rt_env *rt,
	int dst,
	int src);

bool
rt_add_helper(
	struct rt_env *rt,
	int dst,
	int src1,
	int src2);

bool
rt_sub_helper(
	struct rt_env *rt,
	int dst,
	int src1,
	int src2);

bool
rt_mul_helper(
	struct rt_env *rt,
	int dst,
	int src1,
	int src2);

bool
rt_div_helper(
	struct rt_env *rt,
	int dst,
	int src1,
	int src2);

bool
rt_mod_helper(
	struct rt_env *rt,
	int dst,
	int src1,
	int src2);

bool
rt_and_helper(
	struct rt_env *rt,
	int dst,
	int src1,
	int src2);

bool
rt_or_helper(
	struct rt_env *rt,
	int dst,
	int src1,
	int src2);

bool
rt_xor_helper(
	struct rt_env *rt,
	int dst,
	int src1,
	int src2);

bool
rt_shl_helper(
	struct rt_env *rt,
	int dst,
	int src1,
	int src2);

bool
rt_shr_helper(
	struct rt_env *rt,
	int dst,
	int src1,
	int src2);

bool
rt_neg_helper(
	struct rt_env *rt,
	int dst,
	int src);

bool
rt_not_helper(
	struct rt_env *rt,
	int dst,
	int src);

bool
rt_lt_helper(
	struct rt_env *rt,
	int dst,
	int src1,
	int src2);

bool
rt_lte_helper(
	struct rt_env *rt,
	int dst,
	int src1,
	int src2);

bool
rt_eq_helper(
	struct rt_env *rt,
	int dst,
	int src1,
	int src2);

bool
rt_neq_helper(
	struct rt_env *rt,
	int dst,
	int src1,
	int src2);

bool
rt_gte_helper(
	struct rt_env *rt,
	int dst,
	int src1,
	int src2);

bool
rt_gt_helper(
	struct rt_env *rt,
	int dst,
	int src1,
	int src2);

bool
rt_storearray_helper(
	struct rt_env *rt,
	int arr,
	int subscr,
	int val);

bool
rt_loadarray_helper(
	struct rt_env *rt,
	int dst,
	int arr,
	int subscr);

bool
rt_len_helper(
	struct rt_env *rt,
	int dst,
	int src);

bool
rt_getdictkeybyindex_helper(
	struct rt_env *rt,
	int dst,
	int dict,
	int subscr);

bool
rt_getdictvalbyindex_helper(
	struct rt_env *rt,
	int dst,
	int dict,
	int subscr);

bool
rt_loadsymbol_helper(
	struct rt_env *rt,
	int dst,
	const char *symbol,
	uint32_t symbol_len,
	uint32_t symbol_hash);

bool
rt_storesymbol_helper(
	struct rt_env *rt,
	const char *symbol,
	uint32_t symbol_len,
	uint32_t symbol_hash,
	int src);

bool
rt_loaddot_helper(
	struct rt_env *rt,
	int dst,
	int dict,
	const char *field,
	uint32_t field_len,
	uint32_t field_hash);

bool
rt_storedot_helper(
	struct rt_env *rt,
	int dict,
	const char *field,
	uint32_t field_len,
	uint32_t field_hash,
	int src);

bool
rt_call_helper(
	struct rt_env *rt,
	int dst,
	int func,
	int arg_count,
	int *arg);

bool
rt_thiscall_helper(
	struct rt_env *rt,
	int dst,
	int obj,
	const char *name,
	uint32_t name_len,
	uint32_t name_hash,
	int arg_count,
	int *arg);

#endif
