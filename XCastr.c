#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "WindowFunctions.c"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

int main(int argc, char *argv[]) {

    // user variables
    bool interactable = true; // Input pass through window
    int background = 0x00FF000000; // background colour of window
    int width = 200, height = 200; // height and width of window
    int alpha = 200; // go for 0 - 255

    Display *display = XOpenDisplay(NULL);
    Window root = XDefaultRootWindow(display), window;
    XWindowAttributes rootAttributes;

    // window args
    XVisualInfo visualInfo;
    XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &visualInfo);
    Colormap colourmap = XCreateColormap(display, root, visualInfo.visual, AllocNone);

    // try wm specs (_NET_WM_STRUT, _NET_WORKAREA) maybe works
    // get root attributes for resolution (to set window at corner of screen)
    XGetWindowAttributes(display, root, &rootAttributes);

    // TODO: look into XDisplayWidth instead of root attributes for display
    // create window
    window = CreateWindow(display, root, colourmap, rootAttributes, visualInfo, background, width, height);

    if(!interactable)
        WindowIntractable(display, window);

    // TODO: try to raise the stacking order to be ontop of floating windows
    XSetTransientForHint(display, window, window);
    // XRaiseWindow(display, window);

    // make window transparent
    TransparentWindow(display, window, alpha);

    // TODO: update shape as resized
    // TODO: sticky window
    // TODO: not Interactable should be able to move and resizew the window
    // TODO: round window (https://www.x.org/docs/Xext/shapelib.pdf)
    // TODO: something like an ini file to keep window position and user variables
    // TODO: render text with default font but optionally custom
    // TODO: remove decorations? (done with override redirect but istn tractable)
    // TODO: Check if focus window asks for password and if it does then replace the characters with "*"
    // TODO: dynamically change window size by text
    // TODO: fade window when silent after a while
    // TODO: detect key presses from root window

    // Display window
    XMapWindow(display, window);
    XSync(display, 0);
    // XFlush(display);

    // keep window open until manually closed
    // TODO: Why does it take longer to remove window depending on how big it is? (lower sleep less time but it shouldnt eb effected by size?)
    while(!WindowClosed(display, window))
        // usleep is depricated use nanosleep?
        usleep(100000);

    XUnmapWindow(display, window);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    exit(0);

    return 0;
}

