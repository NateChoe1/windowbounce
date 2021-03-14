/*    Windowbounce, an xlib program to bounce a ball around a moving window
 *    Copyright (C) 2021  Nathaniel Choe
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <https://www.gnu.org/licenses/>.*/

#define FRAME_RATE 60

#include <X11/Xlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main() {
	Display *dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		fprintf(stderr, "Error in opening the display\n");
		exit(1);
	}
	int s = DefaultScreen(dpy);
	Window w = XCreateSimpleWindow(dpy, RootWindow(dpy, s),
	                               0, 0, 500, 500, 1,
	                               BlackPixel(dpy, s), WhitePixel(dpy, s));
	XEvent e;
	int inputMask = ExposureMask;
	XSelectInput(dpy, w, inputMask);
	XMapWindow(dpy, w);

	for (;;) {
		XNextEvent(dpy, &e);
		if (e.type == Expose)
			break;
	}
	//Wait until window creation.
	
	int waitTime = 1000000/FRAME_RATE;
	//The reason this isn't a macro is because division is really slow in computers.
	int frameCount = 0;
	int lastX = -1;
	int lastY = -1;
	int lastWidth = -1;
	int lastHeight = -1;
	int trueBallX = 300;
	int trueBallY = 300;
	int ySpeed = 0;
	int xSpeed = 0;

	GC gc = XCreateGC(dpy, w, 0, NULL);
	XSetLineAttributes(dpy, gc, 5, LineSolid, CapButt, JoinRound);
	XMoveWindow(dpy, w, 200, 200);

	for (;;) {
		XClearWindow(dpy, w);

		Window root;
		Window child;
		int x;
		int y;
		unsigned int width;
		unsigned int height;
		unsigned int border;
		unsigned int depth;

		XGetGeometry(dpy, w, &root, &x, &y, &width, &height, &border, &depth);
		XTranslateCoordinates(dpy, w, root, 0, 0, &x, &y, &child);

		XDrawRectangle(dpy, w, gc, trueBallX - x - 3, trueBallY - y - 3, 6, 6);

		if (frameCount > 1) {
			if (trueBallX < x || trueBallX > x + width) {
				xSpeed += x - lastX;
				xSpeed *= -0.9;
				if (trueBallX < x)
					trueBallX = x + 1;
				if (trueBallX > x + width)
					trueBallX = x + width - 1;
			}
			if (trueBallY < y || trueBallY > y + height) {
				ySpeed += y - lastY;
				ySpeed *= -0.9;
				if (trueBallY < y)
					trueBallY = y + 1;
				if (trueBallY > y + height)
					trueBallY = y + height - 1;
			}
		}
		trueBallX += xSpeed;
		trueBallY += ySpeed;

		frameCount++;
		XFlush(dpy);
		lastX = x;
		lastY = y;
		usleep(waitTime);
	}
}
