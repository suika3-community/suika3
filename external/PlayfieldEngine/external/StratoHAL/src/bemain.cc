/* -*- tab-width: 8; indent-tabs-mode: t; -*- */

/*
 * Copyright (c) 2025, Awe Morris. All rights reserved.
 */

/*
 * BeOS / Haiku Main
 */

/* HAL */
extern "C" {
#include "stratohal/platform.h"		/* Public Interface */
#include "stdfile.h"			/* Standard C File Implementation */
};

/* BeOS */
#include <Application.h>
#include <Window.h>
#include <View.h>
#include <Rect.h>
#include <Message.h>
#include <MessageRunner.h>
#include <Bitmap.h>
#include <SoundPlayer.h>

/* POSIX */
#include <sys/types.h>
#include <sys/stat.h>	/* stat(), mkdir() */
#include <sys/time.h>	/* gettimeofday() */
#include <unistd.h>	/* usleep(), access() */

/* Standard C */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

const uint32 kMsgUpdate = 'updt';

BBitmap* bitmap;
struct image* image;
char *window_title;
int window_width;
int window_height;

BSoundPlayer *sound_player[SOUND_TRACKS];
struct wave *wave[SOUND_TRACKS];
bool is_finished[SOUND_TRACKS];

extern "C" void fill_buffer(void *cookie, void *buffer, size_t size, const media_raw_audio_format& format);

class NoctView : public BView
{
	public:
	NoctView(BRect frame) :
		BView(frame, "noctview", B_FOLLOW_ALL, B_WILL_DRAW)
	{
		SetViewColor(255, 255, 255);
	}

	void Draw(BRect updateRect) override
	{
		clear_image(image, 0);

		on_event_frame();

		DrawBitmap(bitmap, BPoint(0, 0));
	}

	void MouseMoved(BPoint where, uint32 transit, const BMessage* dragMessage) override
	{
		on_event_mouse_move((int)where.x, (int)where.y);
	}

	void MouseDown(BPoint where) override
	{
		on_event_mouse_move((int)where.x, (int)where.y);

		uint32 buttons = 0;
		if (Window()->CurrentMessage() &&
		    Window()->CurrentMessage()->FindInt32("buttons", (int32*)&buttons) == B_OK) {
			if (buttons & B_PRIMARY_MOUSE_BUTTON)
				on_event_mouse_press(MOUSE_LEFT, (int)where.x, (int)where.y);
			if (buttons & B_SECONDARY_MOUSE_BUTTON)
				on_event_mouse_press(MOUSE_RIGHT, (int)where.x, (int)where.y);
		}
	}

	void MouseUp(BPoint where) override
	{
		on_event_mouse_move((int)where.x, (int)where.y);

		uint32 buttons = 0;
		if (Window()->CurrentMessage() &&
		    Window()->CurrentMessage()->FindInt32("buttons", (int32*)&buttons) == B_OK) {
			if (buttons & B_PRIMARY_MOUSE_BUTTON)
				on_event_mouse_release(MOUSE_LEFT, (int)where.x, (int)where.y);
			if (buttons & B_SECONDARY_MOUSE_BUTTON)
				on_event_mouse_release(MOUSE_RIGHT, (int)where.x, (int)where.y);
		}
	}

	void KeyDown(const char* bytes, int32 numBytes) override
	{
		// TODO
	}

	void KeyUp(const char* bytes, int32 numBytes) override
	{
		// TODO
	}

	void MessageReceived(BMessage* message) override
	{
		if (message->what == kMsgUpdate) {
			Invalidate();
		} else {
			BView::MessageReceived(message);
		}
	}
};
		
class NoctWindow : public BWindow
{
	BMessageRunner *runner;

public:
	NoctWindow(const char *title, int width, int height) :
		BWindow(BRect(100, 100, width + 100, height + 100),
				title,
				B_TITLED_WINDOW,
				0)
	{
		bitmap = new BBitmap(BRect(0, 0, width - 1, height - 1), B_RGBA32);
		create_image_with_pixels(width, height, (pixel_t*)bitmap->Bits(), &image);

		NoctView* view = new NoctView(Bounds());
		AddChild(view);

		runner = new BMessageRunner(view, new BMessage(kMsgUpdate), 33000);
	}

	~NoctWindow()
	{
		delete runner;
	}

	bool QuitRequested() override
	{
			be_app->PostMessage(B_QUIT_REQUESTED);
			return true;
	}
};

class NoctApplication : public BApplication
{
public:
	NoctApplication() : BApplication("application/x-vnd.example.noctvm")
	{
	}

	void ReadyToRun() override {
		if (!init_file())
			exit(1);
		if (!on_event_boot(&window_title, &window_width, &window_height))
			exit(1);

		NoctWindow *window = new NoctWindow(window_title, window_width, window_height);
		window->Show();

#if 0
		media_raw_audio_format format = {
			44100.0, 2, media_raw_audio_format::B_AUDIO_SHORT, B_MEDIA_LITTLE_ENDIAN, 4
		};
		for (int i = 0; i < SOUND_TRACKS; i++) {
			sound_player[i] = new BSoundPlayer(&format, "SoundPlayer", fill_buffer, NULL, (void*)(intptr_t)i);
			sound_player[i]->Start();
		}
#endif

		if (!on_event_start())
			exit(1);
	}
};

int main(int argc, char *argv[])
{
	NoctApplication app;
	app.Run();
	return 0;
}

extern "C" {

void notify_image_update(struct image *img)
{
}

void notify_image_free(struct image *img)
{
}

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
	int alpha)			/* The alpha value (0 to 255) */
{
	if (dst_width == -1)
		dst_width = src_image->width;
	if (dst_height == -1)
		dst_height = src_image->height;
	if (src_width == -1)
		src_width = src_image->width;
	if (src_height == -1)
		src_height = src_image->height;

	draw_image_alpha(image,
			 dst_left,
			 dst_top,
			 src_image,
			 src_width,
			 src_height,
			 src_left,
			 src_top,
			 alpha);
}

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
	int alpha)			/* The alpha value (0 to 255) */
{
}

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
	int alpha)			/* The alpha value (0 to 255) */
{
}

void
render_image_rule(
	struct image *src_img,		/* [IN] The source image */
	struct image *rule_img,		/* [IN] The rule image */
	int threshold)			/* The threshold (0 to 255) */
{
}

void render_image_melt(
	struct image *src_img,		/* [IN] The source image */
	struct image *rule_img,		/* [IN] The rule image */
	int progress)			/* The progress (0 to 255) */
{
}

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
	int alpha)			/* The alpha value (0 to 255) */
{
}

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
	int alpha)			/* The alpha value (0 to 255) */
{
}

void reset_lap_timer(uint64_t *origin)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);

	*origin = (uint64_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

uint64_t get_lap_timer_millisec(uint64_t *origin)
{
	struct timeval tv;
	uint64_t end;
	
	gettimeofday(&tv, NULL);

	end = (uint64_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);

	return (uint64_t)(end - *origin);
}

bool play_video(const char *fname,	/* file name */
		bool is_skippable)	/* allow skip for a unseen video */
{
	return true;
}

void stop_video(void)
{
}

bool is_video_playing(void)
{
	return false;
}

bool is_full_screen_supported(void){
	return false;
}

bool is_full_screen_mode(void)
{
	return false;
}

void enter_full_screen_mode(void)
{
}

void leave_full_screen_mode(void)
{
}

bool make_save_directory(void)
{
	return true;
}

char *make_real_path(const char *fname)
{
	return strdup(fname);
}

bool log_info(const char *s, ...)
{
	return true;
}

bool log_warn(const char *s, ...)
{
	return true;
}

bool log_error(const char *s, ...)
{
	return true;
}

bool log_out_of_memory(void)
{
	return true;
}

const char *get_system_language(void)
{
	return "en";
}

void set_continuous_swipe_enabled(bool is_enabled)
{
}

/*
 * Sound
 */

void fill_buffer(void *cookie, void *buffer, size_t size, const media_raw_audio_format& format)
{
	int n = (intptr_t)cookie;

	if (wave[n] == NULL)
		return;

	uint32_t *buf = (uint32_t*)buffer;
    memset(buffer, 0, size);
	int rsize = get_wave_samples(wave[n], buf, size / 4);
	if (is_wave_eos(wave[n])) {
		wave[n] = NULL;
		is_finished[n] = true;
	}
}

bool play_sound(int n, struct wave *w)
{
	wave[n] = w;
	return true;
}

bool stop_sound(int n)
{
	wave[n] = NULL;
	return true;
}

bool set_sound_volume(int n, float vol)
{
	return true;
}

bool is_sound_finished(int n)
{
	return is_finished[n];
}

}; /* extern "C" */
