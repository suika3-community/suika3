StratoHAL
=========

`StratoHAL` is a highly portable hardware abstraction layer (HAL) for
game applications. With 25+ years of its history as in-house software,
it has been published as free/libre software to serve as a core
building block of the Suika3 game scripting runtime. Today, it
continues to evolve as an independent library, bridging classic and
modern platforms.

`StratoHAL` provides:

- Built-in game loop and callback mechanism.
- API for 2D graphics, audio, and input.
- GPU acceleration as well as software rendering.

## Platforms

`StratoHAL` supports most major platforms that SDL3 covers. In
addition, it also supports software rendering for small embedded
devices and older computers before the GPU era.

- Windows
- macOS
- Linux
- *BSD (FreeBSD, NetBSD, and OpenBSD)
- iOS
- Android
- HarmonyOS NEXT (OpenHarmony)
- WebAssembly
- ChromeOS
- Solaris 10 / 11
- Haiku OS
- Unity (as a native plugin)
- Xbox (via Unity Plugin, DevKit required)
- PlayStation 4 / PlayStation 5 (via Unity Plugin, DevKit required)
- Switch / Switch 2 (via Unity Plugin, DevKit required)

## Backends

| Platform                    | Graphics                                                                | Audio                    | Input                                                                    |
|-----------------------------|-------------------------------------------------------------------------|--------------------------|--------------------------------------------------------------------------|
| Windows                     | DirectX 12/11/9 and GDI (fallback)                                      | DirectSound 5            | DirectInput 8 (for PS5 controllers) and GameInput (for Xbox controllers) |
| Microsoft GDK (PC and Xbox) | DirectX 12                                                              | XAudio2                  | XInput                                                                   |
| macOS                       | Metal                                                                   | AudioUnit                | GameController                                                           |
| Linux                       | (Wayland, X11, KMS, fbdev) x (OpenGL ES2, OpenGL 3, software rendering) | ALSA                     | Wayland, X11, and evdev                                                  |
| FreeBSD                     | (Wayland, X11) x (OpenGL ES2, OpenGL 3, software rendering)             | /dev/dsp                 | Wayland or X11                                                           |
| NetBSD                      | X11 x (OpenGL 3, software rendering)                                    | /dev/audio               | X11                                                                      |
| OpenBSD                     | X11 x (OpenGL 3, software rendering)                                    | /dev/audio               | X11                                                                      |
| Solaris 10                  | X11 x software rendering                                                | /dev/audio               | X11                                                                      |
| Solaris 11                  | X11 x software rendering                                                | /dev/dsp (OSS)           | X11                                                                      |
| Haiku OS                    | Haiku native + software rendering                                       | Haiku native             | Haiku native                                                             |

## About the Name

"Strato" comes from a Latin word "stratum", i.e., a layer, for hardware abstraction.

## Usage

`StratoHAL` runs the built-in main loop. It calls back the user
application via the callback functions. User application must
implement the following.

```
#include <stratohal/stratohal.h>

static bool need_exit;

/*
 * Called back when the app boots to determine the window title, width, and height.
 */
bool hal_callback_on_event_boot(char **title, int *width, int *height)
{
        *title = strdup("Hello");
	*width = 640;
	*height = 480;
}

/*
 * Called back when the game start.
 */
bool hal_callback_on_event_start(void)
{
        return true;
}

/*
 * Called back when the game stops.
 */
void hal_callback_on_event_stop(void)
{
}

/*
 * Called back every frame.
 */
bool hal_callback_on_event_frame(void)
{
        if (need_exit)
            return false;

        return true;
}

/*
 * Called back when an input happened.
 */
void hal_callback_on_event_mouse_press(int button, int x, int y)
{
        if (button == HAL_MOUSE_LEFT)
                need_exit = true;
}

void hal_callback_on_event_mouse_release(int button, int x, int y)
{
}

void hal_callback_on_event_key_press(int key)
{
        if (key == HAL_KEY_RETURN ||
            key == HAL_KEY_GAMEPAD_A)
                need_exit = true;
}

void hal_callback_on_event_key_release(int key)
{
}

void hal_callback_on_event_mouse_move(int x, int y)
{
}

void hal_callback_on_event_analog_input(int input, int val)
{
        float v = 0.0f;

        if (input == HAL_ANALOG_X1)
                val = v;

        // Do something.
}

void hal_callback_on_event_touch_cancel(void)
{
}

void hal_callback_on_event_swipe_down(void)
{
}

void hal_callback_on_event_swipe_up(void)
{
}
```

Graphic and audio APIs are self-described in
[include/stratohal/stratohal.h](include/stratohal/stratohal.h).

Examples are found in [Playfield Engine](https://github.com/awemorris/PlayfieldEngine).
