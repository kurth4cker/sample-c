// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2025 kurth4cker <kurth4cker@gmail.com>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>

int
main(void)
{
	Display *display = XOpenDisplay(NULL);
	if (display == NULL) {
		fprintf(stderr, "could not open display\n");
		exit(EXIT_FAILURE);
	}

	Window window = XCreateSimpleWindow(
			display,
			XDefaultRootWindow(display),
			0, 0,
			800, 600,
			0,
			0,
			0x808080
	);

	XMapWindow(display, window);
	XSync(display, False);

	sleep(3);

	XCloseDisplay(display);
}
