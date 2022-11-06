#include <X11/Xlib.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    Display *display = XOpenDisplay(NULL);
    Window window = XCreateSimpleWindow(display, XDefaultRootWindow(display), 1920, 1080, 200, 200, 0, 0, 0), root = XDefaultRootWindow(display);

    // get root resolution
    XWindowAttributes attributes;
    XGetWindowAttributes(display, root, &attributes);

    // Display window
    XMapWindow(display, window);
    XSync(display, 0);

    // TODO: render text with custom font
    // TODO: dynamically change window size by text
    // TODO: fade window when silent after a while
    // TODO: detect key presses from root window

    XCloseDisplay(display);
    return 0;
}

