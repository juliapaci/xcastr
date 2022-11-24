#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "WindowFunctions.c"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>


int main(int argc, char *argv[]) {


    // user variables
    bool interactable = false; // move, resize, etc. the window
    int background = 0x00FF000000; // background colour of window
    int alpha = 200; // go for 0 - 255
    int width = 200, height = 200; // height and width of window


    Display *display = XOpenDisplay(NULL);
    Window root = XDefaultRootWindow(display), window;
    XWindowAttributes rootAttributes, windowAttributes;

    // window args
    XVisualInfo visualInfo;
    XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &visualInfo);
    Colormap colourmap = XCreateColormap(display, root, visualInfo.visual, AllocNone);


    //try wm specs (_NET_WM_STRUT, _NET_WORKAREA) maybe works
    // get root attributes for resolution (to set window at corner of screen)
    XGetWindowAttributes(display, root, &rootAttributes);

    // create window
    window = CreateWindow(display, root, colourmap, rootAttributes, visualInfo, background, width, height);

    XGetWindowAttributes(display, window, &windowAttributes); // get window attributes

    if(!interactable)
        WindowInteractable(display, window);

    // Set window name
    XStoreName(display, window, "XCastr");
    XClassHint hint = {  "xcastr", "XCastr"  };
    XSetClassHint(display, window, &hint);


    Atom property[2];
    property[1] = XInternAtom(display, "_NET_WM_WINDOW_TYPE", 0);
    property[0] = XInternAtom(display, "_NET_WM_WINDOW_TYPE_UTILITY", 0);
    XChangeProperty(display, window, property[0], XA_ATOM, 32, PropModeReplace, (unsigned char*) property, 2L);
    property[1] = XInternAtom(display, "_NET_WM_STATE", 0);
    property[0] = XInternAtom(display, "_NET_WM_STATE_ABOVE", 0);
    XChangeProperty(display, window, property[0], XA_ATOM, 32, PropModeReplace, (unsigned char*) property, 1L);

    // TODO: try to raise the stacking order
    XSetTransientForHint(display, window, window);
    // XRaiseWindow(display, window);

    // make window transparent
    TransparentWindow(display, window, alpha);

    // TODO: round window (https://www.x.org/docs/Xext/shapelib.pdf)
    // TODO: something like an ini file to keep window position and user variables
    // TODO: render text with custom font
    // TODO: remove decorations
    // TODO: Check if focus window asks for password and if it does then replace the characters with "*"
    // TODO: dynamically change window size by text
    // TODO: fade window when silent after a while
    // TODO: detect key presses from root window

    XMapWindow(display, window);
    // Display window
    XSync(display, 0);
    // XFlush(display);

    // keep window open until manually closed
    while(!WindowClosed(display, window))
        sleep(1);

    RemoveWindow(display, window);
    exit(0);

    return 0;
}

