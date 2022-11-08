#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

int main(int argc, char *argv[]) {
    Display *display = XOpenDisplay(NULL);
    Window window = XCreateSimpleWindow(display, XDefaultRootWindow(display), 1920, 1080, 200, 200, 0, 0, 0), root = XDefaultRootWindow(display);
    XWindowAttributes rootAttributes, windowAttributes;
    windowAttributes.override_redirect = 1;

    Atom property[2];

    XGetWindowAttributes(display, root, &rootAttributes); // get root attributes for resolution (to set window at corner of screen)
    XGetWindowAttributes(display, window, &windowAttributes); // get window attributes

    // Set window name
    XStoreName(display, window, "XCastr");
    XClassHint hint = {  "xcastr", "XCastr"  };
    XSetClassHint(display, window, &hint);


    // TODO: set window floating bottom righ of screen

    property[2] = XInternAtom(display, "_NET_WM_WINDOW_TYPE", 0);
    property[1] = XInternAtom(display, "_NET_WM_WINDOW_TYPE_NOTIFICATION", 0);
    property[0] = XInternAtom(display, "_NET_WM_WINDOW_TYPE_UTILITY", 0);
    XChangeProperty(display, window, property[0], XA_ATOM, 32, PropModeReplace, (unsigned char*) property, 2L);

    property[1] = XInternAtom(display, "_NET_WM_STATE", 0);
    property[0] = XInternAtom(display, "_NET_WM_STATE_ABOVE", 0);
    XChangeProperty(display, window, property[0], XA_ATOM, 32, PropModeReplace, (unsigned char*) property, 1L);

    // make window transparent
    int alpha = 200;
    alpha = alpha | alpha << 8 | alpha << 16 | alpha << 24;

    Atom windowOpacity = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", 0);
    XChangeProperty(display, window, windowOpacity, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &alpha, 1);


    // TODO: render text with custom font
    // TODO: dynamically change window size by text
    // TODO: fade window when silent after a while
    // TODO: detect key presses from root window


    // Display window
    XMapWindow(display, window);
    XSync(display, 0);

    sleep(10);

    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}

