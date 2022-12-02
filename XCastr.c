#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "WindowFunctions.c"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

int main(int argc, char *argv[]) {

    // user variables
    bool interactable = true; // no input pass through window
    int background = 0x00FF000000; // background colour of window
    int width = 200, height = 200; // height and width of window
    int alpha = 200; // go for 0 - 255
    unsigned int update = 10000; // basically how laggy the window is (lower is more cpu intensive)
    int angle = 0, angle2 = 90; // shape angles

    Display *display = XOpenDisplay(NULL);
    Window root = XDefaultRootWindow(display), window;
    XWindowAttributes rootAttributes, windowAttributes;

    // window args
    XVisualInfo visualInfo;
    XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &visualInfo);
    Colormap colourmap = XCreateColormap(display, root, visualInfo.visual, AllocNone);

    // try wm specs (_NET_WM_STRUT, _NET_WORKAREA) maybe works
    // get root attributes for resolution (to set window at corner of screen)
    XGetWindowAttributes(display, root, &rootAttributes);

    // create window
    window = CreateWindow(display, root, colourmap, rootAttributes, visualInfo, background, width, height);

    // window attribuets for size (for shape)
    XGetWindowAttributes(display, window, &windowAttributes);
    angle2 *= 64;
    ShapeWindow(display, window, windowAttributes, angle, angle2);

    if(!interactable)
        WindowIntractable(display, window);

    // make window transparent
    TransparentWindow(display, window, alpha);

    // TODO: find a better way to update the window other than while loop or is that he best?
    // TODO: sticky window always no matter override_redirect
    // TODO: not Interactable should be able to move and resizew the window
    // TODO: round window (https://www.x.org/docs/Xext/shapelib.pdf)
    // TODO: something like an ini file to keep window position and user variables
    // TODO: render text with default font but optionally custom
    // TODO: remove decorations? (done with override redirect but isn tractable)
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
    while(!WindowClosed(display, window)) {
        // usleep is depricated use nanosleep?
        usleep(update);
        XGetWindowAttributes(display, window, &windowAttributes);
        ShapeWindow(display, window, windowAttributes, angle, angle2);
    }

    XUnmapWindow(display, window);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    exit(0);

    return 0;
}

