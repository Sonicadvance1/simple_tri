#include <stdio.h>
#include <epoxy/gl.h>
#include <waffle-1/waffle.h>

#include "Context.h"

namespace Context
{
	waffle_display* dpy;
	waffle_window* win;
	waffle_context* ctx;

void MsgCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void*) {
	printf("KHR: %s\n", msg);
}

	void Create()
	{
		int32_t init_attribs[] = {
			WAFFLE_PLATFORM, WAFFLE_PLATFORM_X11_EGL,
			WAFFLE_NONE,
		};

		int32_t config_attribs[] = {
			WAFFLE_CONTEXT_API, WAFFLE_CONTEXT_OPENGL,
			WAFFLE_RED_SIZE, 8,
			WAFFLE_GREEN_SIZE, 8,
			WAFFLE_BLUE_SIZE, 8,
			WAFFLE_ALPHA_SIZE, 8,
			WAFFLE_DOUBLE_BUFFERED, 1,
			WAFFLE_CONTEXT_PROFILE, WAFFLE_CONTEXT_CORE_PROFILE,
			WAFFLE_CONTEXT_MAJOR_VERSION, 3,
			WAFFLE_CONTEXT_MINOR_VERSION, 3,
			WAFFLE_NONE,
		};

		// Init library
		if (!waffle_init(init_attribs))
			printf("Couldn't initialize waffle!\n");

		// Open display
		dpy = waffle_display_connect(nullptr);

		if (!waffle_display_supports_context_api(dpy, WAFFLE_CONTEXT_OPENGL))
			printf("Display doesn't support GL!\n");

		// Get the config we want
		waffle_config* cfg = waffle_config_choose(dpy, config_attribs);
		if (!cfg)
			printf("Couldn't get waffle config!\n");

		// Create our window
		win = waffle_window_create(cfg, 640, 480);

		if (!win)
			printf("Couldn't create waffle window!\n");

		waffle_window_show(win);

		// Create OpenGL context
		ctx = waffle_context_create(cfg, nullptr);

		if (!ctx)
			printf("Couldn't create waffle context!\n");

		waffle_config_destroy(cfg);

		// Make Current
		waffle_make_current(dpy, win, ctx);

		printf("Test: %p\n", waffle_get_proc_address("glGetString"));

		glDebugMessageCallback(MsgCallback, nullptr);
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}

	void Shutdown()
	{
		waffle_context_destroy(ctx);
		waffle_window_destroy(win);

		waffle_display_disconnect(dpy);
	}

	void Swap()
	{
		waffle_window_swap_buffers(win);
	}
}
