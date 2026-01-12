/* -*- coding: utf-8; tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * Main Header of Game Porting Layer
 */
#ifndef STRATOHAL_PLATFORM_H
#define STRATOHAL_PLATFORM_H

#include <stratohal/c89compat.h>

/*************************
 * Files and Directories *
 *************************/

/* Package file name */
#define PACKAGE_FILE		"assets.arc"

/* Save directory name */
#define SAVE_DIR		"save"

/*************
 * File Read *
 *************/

struct rfile;

/* Initialize file read. */
bool init_rfile(void);

/* Check whether a file exists. */
bool check_file_exist(const char *file);

/* Open a file stream. */
bool open_rfile(const char *file, struct rfile **f);

/* Enable de-obfuscation on a read file stream. */
void decode_rfile(struct rfile *f);

/* Get a file size. */
bool get_rfile_size(struct rfile *rf, size_t *ret);

/* Read from a file stream. */
bool read_rfile(struct rfile *rf, void *buf, size_t size, size_t *ret);

/* Read a u64 from a file stream. */
bool get_rfile_u64(struct rfile *rf, uint64_t *data);

/* Read a u32 from a file stream. */
bool get_rfile_u32(struct rfile *rf, uint32_t *data);

/* Read a u16 from a file stream. */
bool get_rfile_u16(struct rfile *rf, uint16_t *data);

/* Read a u8 from a file stream. */
bool get_rfile_u8(struct rfile *rf, uint8_t *data);

/* Read a string from a file stream. */
bool get_rfile_string(struct rfile *rf, char *buf, size_t size);

/* Close a file stream. */
void close_rfile(struct rfile *rf);

/* Rewind a file stream. */
void rewind_rfile(struct rfile *rf);

/**************
 * File Write *
 **************/

struct wfile;

/* Open a write file stream. */
bool open_wfile(const char *file, struct wfile **wf);

/* Write to a file stream. */
bool write_wfile(struct wfile *wf, const void *buf, size_t size, size_t *ret);

/* Close a write file stream. */
void close_wfile(struct wfile *wf);

/* Remove a file. */
void remove_file(const char *file);

/*********
 * Image *
 *********/

/* Pixel value type. */
typedef uint32_t pixel_t;

/* Image Structure */
struct image {
	/* Size. */
	int width;
	int height;

	/* Pixels. */
	pixel_t *pixels;
	bool no_free;

	/* Texture pointer. */
	void *texture;

	/* Texture ID. */
	int id;

	/* Context ID. (for graphics reinit) */
	int context;

	/* Upload flag. */
	bool need_upload;
};

#if !defined(USE_QT) && \
    ( \
        defined(TARGET_WINDOWS) || \
        defined(TARGET_MACOS) || \
        defined(TARGET_IOS) || \
        (defined(TARGET_POSIX) && defined(USE_X11_SOFTRENDER)) || \
	defined(TARGET_UNITY) \
    )
#define ORDER_BGRA	/* Use RGBA on Direct3D and Metal */
#else
#define ORDER_RGBA	/* Use BGRA on OpenGL */
#define ORDER_OPENGL
#endif

/* Compose a pixel value. */
static INLINE pixel_t make_pixel(uint32_t a, uint32_t r, uint32_t g, uint32_t b)
{
#ifdef ORDER_RGBA
	return (((pixel_t)a) << 24) | (((pixel_t)b) << 16) | (((pixel_t)g) << 8) | ((pixel_t)r);
#else
	return (((pixel_t)a) << 24) | (((pixel_t)r) << 16) | (((pixel_t)g) << 8) | ((pixel_t)b);
#endif
}

/* Get an alpha component value from a pixel value. */
static INLINE uint32_t get_pixel_a(pixel_t p)
{
	return (p >> 24) & 0xff;
}

/* Get a red component value from a pixel value. */
static INLINE uint32_t get_pixel_r(pixel_t p)
{
#ifdef ORDER_RGBA
	return (p >> 16) & 0xff;
#else
	return p & 0xff;
#endif
}

/* Get a green component value from a pixel value. */
static INLINE uint32_t get_pixel_g(pixel_t p)
{
#ifdef ORDER_RGBA
	return (p >> 8) & 0xff;
#else
	return (p >> 8) & 0xff;
#endif
}

/* Get a blue component value from a pixel value. */
static INLINE uint32_t get_pixel_b(pixel_t p)
{
#ifdef ORDER_RGBA
	return p & 0xff;
#else
	return (p >> 16) & 0xff;
#endif
}

#undef ORDER_RGBA
#undef ORDER_BGRA

/* Create an image. */
bool create_image(int w, int h, struct image **img);

/* Creata an image with a color string. (#rrggbb) */
struct image *create_image_from_color_string(int w, int h, const char *color);

/* Create an image with a pixel buffer. */
bool create_image_with_pixels(int w, int h, pixel_t *pixels, struct image **img);

/* Create an image with a PNG file. */
bool create_image_with_png(const uint8_t *data, size_t size, struct image **img);

/* Create an image with a JPEG file. */
bool create_image_with_jpeg(const uint8_t *data, size_t size, struct image **img);

/* Create an image with a WebP file. */
bool create_image_with_webp(const uint8_t *data, size_t size, struct image **img);

/* Destroy an image. */
void destroy_image(struct image *img);

/* Get an image width. */
int get_image_width(struct image *img);

/* Get an image height. */
int get_image_height(struct image *img);

/* Get image pixels. */
pixel_t *get_image_pixels(struct image *img);

/* Clear an image with a uniform color. */
void clear_image(struct image *img, pixel_t color);

/* Clear an image rectangle with a uniform color. */
void clear_image_rect(struct image *img, int x, int y, int w, int h, pixel_t color);

/* Clear alpha channel with 255. */
void fill_image_alpha(struct image *img);

/* Draw an image on an image. (copy) */
void draw_image_copy(struct image *dst_image, int dst_left, int dst_top,
		     struct image *src_image, int width, int height,
		     int src_left, int src_top);

/* Draw an image on an image. (alpha-blending, dst_alpha=255) */
void draw_image_alpha(struct image *dst_image, int dst_left, int dst_top,
		      struct image *src_image, int width, int height,
		      int src_left, int src_top, int alpha);

/* Draw an image on an image. (add-blending) */
void draw_image_add(struct image *dst_image, int dst_left, int dst_top,
		    struct image *src_image, int width, int height,
		    int src_left, int src_top, int alpha);

/* Draw an image on an image. (add-blending) */
void draw_image_sub(struct image *dst_image, int dst_left, int dst_top,
		    struct image *src_image, int width, int height,
		    int src_left, int src_top, int alpha);

/* Draw a glyph image on an image. (alphablending, special alpha value) */
void draw_image_glyph(struct image *dst_image, int dst_left, int dst_top,
		      struct image *src_image, int width, int height,
		      int src_left, int src_top, int alpha);

/* Draw an emoji image on an image. (alphablending, special alpha value) */
void draw_image_emoji(struct image *dst_image, int dst_left, int dst_top,
		      struct image *src_image, int width, int height,
		      int src_left, int src_top, int alpha);

/* Draw an image on an image. (50% dimming) */
void draw_image_dim(struct image *dst_image, int dst_left, int dst_top,
		    struct image *src_image, int width, int height,
		    int src_left, int src_top, int alpha);

/* Draw an image with 1-bit rule image. */
void draw_image_rule(struct image *dst_image,
		     struct image *src_image,
		     struct image *rule_image,
		     int threshold);

/* Draw an image with 8-bit rule image. */
void draw_image_melt(struct image *dst_image,
		     struct image *src_image,
		     struct image *rule_image,
		     int threshold);

/* Draw an image with scaling. */
void draw_image_scale(struct image *dst_image,
		      int virtual_dst_width,
		      int virtual_dst_height,
		      int virtual_dst_left,
		      int virtual_dst_top,
		      struct image *src_image);

/* Clip a rectangle by a source size. */
bool clip_by_source(int src_cx, int src_cy, int *cx, int *cy,
		    int *dst_x, int *dst_y, int *src_x, int *src_y);

/* Clip a rectangle by a destination size. */
bool clip_by_dest(int dst_cx, int dst_cy, int *cx, int *cy,
		  int *dst_x, int *dst_y, int *src_x, int *src_y);

/*********
 * Glyph *
 *********/

/* Unicode Codepoints */
#define CHAR_TOUTEN		(0x3001)
#define CHAR_KUTEN		(0x3002)
#define CHAR_YENSIGN		(0x00a5)

/* Maximum Font Index */
#define GLYPH_DATA_COUNT	(4)

/* Initialize the font renderer. */
bool init_glyph(void);

/* Cleanup the font renderer. */
void cleanup_glyph(void);

/* Load a glyph data. (TTF) */
bool load_glyph_data(int slot, const uint8_t *data, size_t len);

/* Destroy a glyph data. */
void destroy_glyph_data(int index);

/* Get a top character of a utf-8 string as utf-32. */
int utf8_to_utf32(const char *mbs, uint32_t *wc);

/* Get a character count of a utf-8 string. */
int count_utf8_chars(const char *mbs);

/* Get a width for a character. */
int get_glyph_width(int slot, int size, uint32_t codepoint);

/* Get a height for a character. */
int get_glyph_height(int slot, int size, uint32_t codepoint);

/* Get a width and a height for a character. */
void get_glyph_width_and_height(int slot, int size, uint32_t codepoint, int *width, int *height);

/* Get a width for a string. */
int get_string_width(int font_type, int font_size, const char *mbs);

/* Get a height for a string. */
int get_string_height(int font_type, int font_size, const char *mbs);

/* Get a width and a height for a string. */
void get_string_width_and_height(int slot, int size, const char *mbs, int *width, int *height);

/* Draw a character. */
bool draw_glyph(struct image *img,
		int font_index,
		int font_size,
		int base_font_size,
		int outline_size,
		int x,
		int y,
		pixel_t color,
		pixel_t outline_color,
		uint32_t codepoint,
		int *ret_w,
		int *ret_h,
		bool is_dim);

/*********
 * Sound *
 *********/

/* Sound Tracks */
#define SOUND_TRACKS	(4)

/* PCM Stream */
struct wave;

/* Create a wave stream from a file. */
struct wave *create_wave_from_file(const char *file, bool loop);

/* Destroy a wave stream. */
void destroy_wave(struct wave *w);

/* Set a repeat count of a wave stream. */
void set_wave_repeat_times(struct wave *w, int n);

/* Get whether a wave stream is reached end-of-stream or not. */
bool is_wave_eos(struct wave *w);

/* Get PCM samples from a wave stream. */
int get_wave_samples(struct wave *w, uint32_t *buf, int samples);

/************************
 * Texture Manipulation *
 ************************/

/*
 * Notifies an image update.
 *  - This function tells a HAL that an image needs to be uploaded to GPU.
 */
void notify_image_update(struct image *img);

/*
 * Notifies an image free.
 *  - This function tells a HAL that an image is no longer used.
 *  - This function must be called from destroy_image() only.
 */
void notify_image_free(struct image *img);

/*
 * Returns if RGBA values have to be reversed to BGRA.
 */
#if defined(TARGET_ANDROID) || \
    defined(TARGET_WASM) || \
    (defined(TARGET_LINUX) && (defined(USE_X11_OPENGL) || defined(USE_QT)))
#define IS_OPENGL_BYTE_ORDER()	true
#else
#define IS_OPENGL_BYTE_ORDER()	false
#endif

/*************
 * Rendering *
 *************/

/*
 * Renders an image to the screen with the "normal" shader pipeline.
 *  - The "normal" shader pipeline renders pixels with alpha blending.
 */
void
render_image_normal(
	int dst_left,			/* The X coordinate of the screen */
	int dst_top,			/* The Y coordinate of the screen */
	int dst_width,			/* The width of the destination rectangle */
	int dst_height,			/* The width of the destination rectangle */
	struct image *src_image,	/* [IN] The image to be rendered */
	int src_left,			/* The X coordinate of a source image */
	int src_top,			/* The Y coordinate of a source image */
	int src_width,			/* The width of the source rectangle */
	int src_height,			/* The height of the source rectangle */
	int alpha);			/* The alpha value (0 to 255) */

/*
 * Renders an image to the screen with the "add" shader pipeline.
 *  - The "add" shader pipeline renders pixels with add blending.
 */
void
render_image_add(
	int dst_left,			/* The X coordinate of the screen */
	int dst_top,			/* The Y coordinate of the screen */
	int dst_width,			/* The width of the destination rectangle */
	int dst_height,			/* The width of the destination rectangle */
	struct image *src_image,	/* [IN] The image to be rendered */
	int src_left,			/* The X coordinate of a source image */
	int src_top,			/* The Y coordinate of a source image */
	int src_width,			/* The width of the source rectangle */
	int src_height,			/* The height of the source rectangle */
	int alpha);			/* The alpha value (0 to 255) */

/*
 * Renders an image to the screen with the "sub" shader pipeline.
 *  - The "sub" shader pipeline renders pixels with subtract blending.
 */
void
render_image_sub(
	int dst_left,			/* The X coordinate of the screen */
	int dst_top,			/* The Y coordinate of the screen */
	int dst_width,			/* The width of the destination rectangle */
	int dst_height,			/* The width of the destination rectangle */
	struct image *src_image,	/* [IN] The image to be rendered */
	int src_left,			/* The X coordinate of a source image */
	int src_top,			/* The Y coordinate of a source image */
	int src_width,			/* The width of the source rectangle */
	int src_height,			/* The height of the source rectangle */
	int alpha);			/* The alpha value (0 to 255) */

/*
 * Renders an image to the screen with the "dim" shader pipeline.
 *  - The "dim" shader pipeline renders pixels at 50% value for each RGB component.
 */
void
render_image_dim(
	int dst_left,			/* The X coordinate of the screen */
	int dst_top,			/* The Y coordinate of the screen */
	int dst_width,			/* The width of the destination rectangle */
	int dst_height,			/* The width of the destination rectangle */
	struct image *src_image,	/* [IN] The image to be rendered */
	int src_left,			/* The X coordinate of a source image */
	int src_top,			/* The Y coordinate of a source image */
	int src_width,			/* The width of the source rectangle */
	int src_height,			/* The height of the source rectangle */
	int alpha);			/* The alpha value (0 to 255) */

/*
 * Renders an image to the screen with the "rule" shader pipeline.
 *  - The "rule" shader pipeline is a variation of "universal transition" with a threshold value.
 *  - A rule image must have the same size as the screen.
 */
void
render_image_rule(
	struct image *src_img,		/* [IN] The source image */
	struct image *rule_img,		/* [IN] The rule image */
	int threshold);			/* The threshold (0 to 255) */

/*
 * Renders an image to the screen with the "melt" shader pipeline.
 *  - The "melt" shader pipeline is a variation of "universal transition" with a progress value
 *  - A rule image must have the same size as the screen
 */
void render_image_melt(
	struct image *src_img,		/* [IN] The source image */
	struct image *rule_img,		/* [IN] The rule image */
	int progress);			/* The progress (0 to 255) */

/*
 * Renders an image to the screen as a triangle strip with the "normal" shader pipeline.
 */
void
render_image_3d_normal(
	float x1,			/* x1 */
	float y1,			/* y1 */
	float x2,			/* x2 */
	float y2,			/* y2 */
	float x3,			/* x3 */
	float y3,			/* y3 */
	float x4,			/* x4 */
	float y4,			/* y4 */
	struct image *src_image,	/* [IN] The source image */
	int src_left,			/* The X coordinate of a source image */
	int src_top,			/* The Y coordinate of a source image */
	int src_width,			/* The width of the source rectangle */
	int src_height,			/* The height of the source rectangle */
	int alpha);			/* The alpha value (0 to 255) */

/*
 * Renders an image to the screen as a triangle strip with the "add" shader pipeline.
 */
void
render_image_3d_add(
	float x1,			/* x1 */
	float y1,			/* y1 */
	float x2,			/* x2 */
	float y2,			/* y2 */
	float x3,			/* x3 */
	float y3,			/* y3 */
	float x4,			/* x4 */
	float y4,			/* y4 */
	struct image *src_image,	/* [IN] The source image */
	int src_left,			/* The X coordinate of a source image */
	int src_top,			/* The Y coordinate of a source image */
	int src_width,			/* The width of the source rectangle */
	int src_height,			/* The height of the source rectangle */
	int alpha);			/* The alpha value (0 to 255) */

/*
 * Renders an image to the screen as a triangle strip with the "sub" shader pipeline.
 */
void
render_image_3d_sub(
	float x1,			/* x1 */
	float y1,			/* y1 */
	float x2,			/* x2 */
	float y2,			/* y2 */
	float x3,			/* x3 */
	float y3,			/* y3 */
	float x4,			/* x4 */
	float y4,			/* y4 */
	struct image *src_image,	/* [IN] The source image */
	int src_left,			/* The X coordinate of a source image */
	int src_top,			/* The Y coordinate of a source image */
	int src_width,			/* The width of the source rectangle */
	int src_height,			/* The height of the source rectangle */
	int alpha);			/* The alpha value (0 to 255) */

/*
 * Renders an image to the screen as a triangle strip with the "dim" shader pipeline.
 */
void
render_image_3d_dim(
	float x1,			/* x1 */
	float y1,			/* y1 */
	float x2,			/* x2 */
	float y2,			/* y2 */
	float x3,			/* x3 */
	float y3,			/* y3 */
	float x4,			/* x4 */
	float y4,			/* y4 */
	struct image *src_image,	/* [IN] The source image */
	int src_left,			/* The X coordinate of a source image */
	int src_top,			/* The Y coordinate of a source image */
	int src_width,			/* The width of the source rectangle */
	int src_height,			/* The height of the source rectangle */
	int alpha);			/* The alpha value (0 to 255) */

/*************
 * Lap Timer *
 *************/

/*
 * Resets a lap timer and initializes it with a current time.
 */
void reset_lap_timer(uint64_t *origin);

/*
 * Gets a lap time in milliseconds.
 */
uint64_t get_lap_timer_millisec(uint64_t *origin);

/******************
 * Sound Playback *
 ******************/

/*
 * Note: we have the following sound streams:
 *  - SOUND_BGM:   background music
 *  - SOUND_SE:    sound effect
 *  - SOUND_VOICE: character voice
 *  - SOUND_SYS1:  system sound
 *  - SOUND_SYS2:  system sound
 */

/*
 * Starts playing a sound file on a track.
 */
bool
play_sound(int stream,		/* A sound stream index */
	   struct wave *w);	/* [IN] A sound object, ownership will be delegated to the callee */

/*
 * Stops playing a sound track.
 */
bool stop_sound(int stream);

/*
 * Sets sound volume.
 */
bool set_sound_volume(int stream, float vol);

/*
 * Returns whether a sound playback for a stream is already finished.
 */
bool is_sound_finished(int stream);

/******************
 * Video Playback *
 ******************/

/*
 * Starts playing a video file.
 */
bool play_video(const char *fname,	/* file name */
		bool is_skippable);	/* allow skip for a unseen video */

/*
 * Stops playing music stream.
 */
void stop_video(void);

/*
 * Returns whether a video playcack is running.
 */
bool is_video_playing(void);

/***********************
 * Window Manipulation *
 ***********************/

/*
 * Returns whether the current HAL supports the "full screen mode".
 *  - The "full screen mode" includes the dock-in of some game consoles.
 *  - A HAL can implement the "full screen mode" but it is optional.
 */
bool is_full_screen_supported(void);

/*
 * Returns whether the current HAL is in the "full screen mode".
 */
bool is_full_screen_mode(void);

/*
 * Enters the full screen mode.
 *  - A HAL can ignore this call.
 */
void enter_full_screen_mode(void);

/*
 * Leaves the full screen mode.
 *  - A HAL can ignore this call.
 */
void leave_full_screen_mode(void);

/*********************
 * Path Manipulation *
 *********************/

/*
 * Creates a save directory if it does not exist.
 */
bool make_save_directory(void);

/*
 * Creates a real path string from a file name.
 *  - Return value must be freed by callers with free().
 */
char *make_real_path(const char *fname);

/***********
 * Logging *
 ***********/

/*
 * Note that sound threads cannot use logging
 */

/*
 * Puts a "info" level log with printf formats.
 */
bool log_info(const char *s, ...);

/*
 * Puts a "warn" level log with printf formats.
 */
bool log_warn(const char *s, ...);

/*
 * Puts an "error" level log with printf formats.
 */
bool log_error(const char *s, ...);

/*
 * Log out-of-memory.
 */
bool log_out_of_memory(void);

/**********
 * Locale *
 **********/

/*
 * Gets the system language.
 *  - Return value can be:
 *    - "ja": Japanese
 *    - "en": English
 *    - "zh": Simplified Chinese
 *    - "tw": Traditional Chinese
 *    - "fr": French
 *    - "it": Italian
 *    - "es": Spanish (Castellano)
 *    - "de": Deutsch
 *    - "el": Greek
 *    - "ru": Russian
 *    - "other": Other (must fallback to English)
 */
const char *get_system_language(void);

/****************
 * Touch Screen *
 ****************/

/*
 * Enable/disable message skip by touch move.
 */
void set_continuous_swipe_enabled(bool is_enabled);

/*************
 * Callbacks *
 *************/

/*
 * Callbacks are defined outside the HAL.
 */

/* Keycode. */
enum key_code {
	KEY_ESCAPE,
	KEY_RETURN,
	KEY_SPACE,
	KEY_TAB,
	KEY_BACKSPACE,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_SHIFT,
	KEY_CONTROL,
	KEY_ALT,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_0,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_GAMEPAD_UP,
	KEY_GAMEPAD_DOWN,
	KEY_GAMEPAD_LEFT,
	KEY_GAMEPAD_RIGHT,
	KEY_GAMEPAD_A,
	KEY_GAMEPAD_B,
	KEY_GAMEPAD_X,
	KEY_GAMEPAD_Y,
	KEY_GAMEPAD_L,
	KEY_GAMEPAD_R,
	KEY_MAX,
};

/* Mouse button. */
enum mouse_button {
	MOUSE_LEFT,
	MOUSE_RIGHT,
};

/* Analog input. */
enum analog_input {
	ANALOG_X1,
	ANALOG_Y1,
	ANALOG_X2,
	ANALOG_Y2,
	ANALOG_L,
	ANALOG_R,
};

/* Callbacks. */
bool on_event_boot(char **title, int *width, int *height);
bool on_event_start(void);
void on_event_stop(void);
bool on_event_frame(void);
void on_event_key_press(int key);
void on_event_key_release(int key);
void on_event_mouse_press(int button, int x, int y);
void on_event_mouse_release(int button, int x, int y);
void on_event_mouse_move(int x, int y);
void on_event_analog_input(int input, int val);
void on_event_touch_cancel(void);
void on_event_swipe_down(void);
void on_event_swipe_up(void);

/***************************
 * Forein Language Support *
 ***************************/

#if !defined(USE_CSHARP) && !defined(USE_SWIFT)
#define UNSAFEPTR(t) t
#endif

/*
 * Swift Support (Swift Function Pointers)
 */
#if defined(USE_SWIFT)
#define UNSAFEPTR(t)  t	/* For Swift */
extern void (*wrap_log_info)(UNSAFEPTR(const char *) s);
extern void (*wrap_log_warn)(UNSAFEPTR(const char *) s);
extern void (*wrap_log_error)(UNSAFEPTR(const char *) s);
extern void (*wrap_make_sav_dir)(void);
extern void (*wrap_make_valid_path)(UNSAFEPTR(const char *) dir, UNSAFEPTR(const char *) fname, UNSAFEPTR(const char *) dst, int len);
extern void (*wrap_notify_image_update)(int id, int width, int height, UNSAFEPTR(const uint32_t *) pixels);
extern void (*wrap_notify_image_free)(int id);
extern void (*wrap_render_image_normal)(int dst_left, int dst_top, int dst_width, int dst_height, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha);
extern void (*wrap_render_image_add)(int dst_left, int dst_top, int dst_width, int dst_height, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha);
extern void (*wrap_render_image_sub)(int dst_left, int dst_top, int dst_width, int dst_height, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha);
extern void (*wrap_render_image_dim)(int dst_left, int dst_top, int dst_width, int dst_height, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha);
extern void (*wrap_render_image_rule)(int src_img, int rule_img, int threshold);
extern void (*wrap_render_image_melt)(int src_img, int rule_img, int progress);
extern void (*wrap_render_image_3d_normal)(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha);
extern void (*wrap_render_image_3d_add)(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha);
extern void (*wrap_render_image_3d_sub)(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha);
extern void (*wrap_render_image_3d_dim)(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha);
extern void (*wrap_reset_lap_timer)(UNSAFEPTR(uint64_t *) origin);
extern uint64_t (*wrap_get_lap_timer_millisec)(UNSAFEPTR(uint64_t *) origin);
extern void (*wrap_play_sound)(int stream, UNSAFEPTR(void *) wave);
extern void (*wrap_stop_sound)(int stream);
extern void (*wrap_set_sound_volume)(int stream, float vol);
extern int (*wrap_is_sound_finished)(int stream);
extern bool (*wrap_play_video)(UNSAFEPTR(const char *) fname, bool is_skippable);
extern void (*wrap_stop_video)(void);
extern bool (*wrap_is_video_playing)(void);
extern bool (*wrap_is_full_screen_supported)(void);
extern bool (*wrap_is_full_screen_mode)(void);
extern void (*wrap_enter_full_screen_mode)(void);
extern void (*wrap_leave_full_screen_mode)(void);
extern void (*wrap_get_system_language)(UNSAFEPTR(char *) dst, int len);
extern void (*wrap_set_continuous_swipe_enabled)(bool is_enabled);
extern void (*wrap_free_shared)(UNSAFEPTR(void *) p);
extern bool (*wrap_check_file_exist)(UNSAFEPTR(const char *) file_name);
extern UNSAFEPTR(void *) (*wrap_get_file_contents)(UNSAFEPTR(const char *) file_name, UNSAFEPTR(int *) len);
extern void (*wrap_open_save_file)(UNSAFEPTR(const char *) file_name);
extern void (*wrap_write_save_file)(int b);
extern void (*wrap_close_save_file)(void);
extern void (*wrap_set_continuous_swipe_enabled)(bool is_enabled);
extern bool (*wrap_is_continue_pushed)(void);
extern bool (*wrap_is_next_pushed)(void);
extern bool (*wrap_is_stop_pushed)(void);
extern bool (*wrap_is_script_opened)(void);
extern UNSAFEPTR(const char *) (*wrap_get_opened_script)(void);
extern bool (*wrap_is_exec_line_changed)(void);
extern bool (*wrap_get_changed_exec_line)(void);
extern void (*wrap_on_change_running_state)(int running, int request_stop);
extern void (*wrap_on_load_script)(void);
extern void (*wrap_on_change_position)(void);
extern void (*wrap_on_update_variable)(void);
#endif /* defined(USE_SWIFT) */

/*
 * C# Support (init_hal_func_table() is called from C#)
 */
#if defined(USE_CSHARP)

#define UNSAFEPTR(t)  intptr_t	/* For C# */

#ifdef NO_CDECL
#define __cdecl
#endif

void init_hal_func_table(
	void __cdecl (*p_log_info)(UNSAFEPTR(const char *) s),
	void __cdecl (*p_log_warn)(UNSAFEPTR(const char *) s),
	void __cdecl (*p_log_error)(UNSAFEPTR(const char *) s),
	void __cdecl (*p_log_out_of_memory)(UNSAFEPTR(const char *) s),
	void __cdecl (*p_make_save_directory)(void),
	void __cdecl (*p_make_real_path)(UNSAFEPTR(const char *) fname, UNSAFEPTR(const char *) dst, int len),
	void __cdecl (*p_notify_image_update)(int id, int width, int height, UNSAFEPTR(const uint32_t *) pixels),
	void __cdecl (*p_notify_image_free)(int id),
	void __cdecl (*p_render_image_normal)(int dst_left, int dst_top, int dst_width, int dst_height, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha),
	void __cdecl (*p_render_image_add)(int dst_left, int dst_top, int dst_width, int dst_height, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha),
	void __cdecl (*p_render_image_sub)(int dst_left, int dst_top, int dst_width, int dst_height, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha),
	void __cdecl (*p_render_image_dim)(int dst_left, int dst_top, int dst_width, int dst_height, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha),
	void __cdecl (*p_render_image_rule)(int src_img, int rule_img, int threshold),
	void __cdecl (*p_render_image_melt)(int src_img, int rule_img, int progress),
	void __cdecl (*p_render_image_3d_normal)(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha),
	void __cdecl (*p_render_image_3d_add)(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha),
	void __cdecl (*p_render_image_3d_sub)(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha),
	void __cdecl (*p_render_image_3d_dim)(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, int src_img, int src_left, int src_top, int src_width, int src_height, int alpha),
	void __cdecl (*p_reset_lap_timer)(UNSAFEPTR(uint64_t *) origin),
	uint64_t __cdecl (*p_get_lap_timer_millisec)(UNSAFEPTR(uint64_t *) origin),
	void __cdecl (*p_play_sound)(int stream, UNSAFEPTR(void *) wave),
	void __cdecl (*p_stop_sound)(int stream),
	void __cdecl (*p_set_sound_volume)(int stream, float vol),
	bool __cdecl (*p_is_sound_finished)(int stream),
	bool __cdecl (*p_play_video)(UNSAFEPTR(const char *) fname, bool is_skippable),
	void __cdecl (*p_stop_video)(void),
	bool __cdecl (*p_is_video_playing)(void),
	bool __cdecl (*p_is_full_screen_supported)(void),
	bool __cdecl (*p_is_full_screen_mode)(void),
	void __cdecl (*p_enter_full_screen_mode)(void),
	void __cdecl (*p_leave_full_screen_mode)(void),
	void __cdecl (*p_get_system_language)(UNSAFEPTR(char *) dst, int len),
	void __cdecl (*p_set_continuous_swipe_enabled)(bool is_enabled),
	void __cdecl (*p_free_shared)(UNSAFEPTR(void *) p),
	bool __cdecl (*p_check_file_exist)(UNSAFEPTR(const char *) file_name),
	UNSAFEPTR(void *) __cdecl (*p_get_file_contents)(UNSAFEPTR(const char *) file_name, UNSAFEPTR(int *) len),
	void __cdecl (*p_open_save_file)(UNSAFEPTR(const char *) file_name),
	void __cdecl (*p_write_save_file)(int b),
	void __cdecl (*p_close_save_file)(void)
);

#ifdef NO_CDECL
#undef __cdecl
#endif

#endif /* deinfed(USE_CSHARP) */

/***********
 * Helpers *
 ***********/

#if defined(TARGET_WINDOWS)
const wchar_t *win32_utf8_to_utf16(const char *s);
const char *win32_utf16_to_utf8(const wchar_t *s);
#endif

#endif
