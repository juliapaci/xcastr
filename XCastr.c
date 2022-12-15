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
    unsigned int update = 10000; // the update interval of the window (micro seconds) basically how laggy the window is (lower is more cpu intensive) 1 seconds = 1000000 micro seconds (should you input as seconds and then multiply by 1000000?)
    bool round = true; // round window
    int radius = 50; // radius and radius of the rounded corners

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

    // if(round) {
    //     // window attribuets for size (for shape)
    //     XGetWindowAttributes(display, window, &windowAttributes);
    //     ShapeWindow(display, window, windowAttributes, radius, diameter);
    // }

    if(!interactable)
        WindowIntractable(display, window);

    // make window transparent
    TransparentWindow(display, window, alpha);

    // TODO: find a better way to update the window other than while loop or is that he best?
    // TODO: not Interactable should be able to move and resizew the window
    // TODO: round window (https://www.x.org/docs/Xext/shapelib.pdf)
    // TODO: something like an ini file to keep window position and user variables
    // TODO: render text with default font but optionally custom
    // TODO: remove decorations? (done with override redirect but isn tractable)
    // TODO: Check if focus window asks for password and if it does then replace the characters with "*"
    // TODO: dynamically change window size by text, add maximum width variable
    // TODO: fade window when silent after a while (from 255/200 alpha to 0)
    // TODO: detect key presses from root window
    // TODO: if the window isnt round then the update interval can be alot larger (how to do this in a clean way)

    // Display window
    XMapWindow(display, window);
    XSync(display, 0);
    // XFlush(display);

    // keep window open until manually closed
    // TODO: Why does it take longer to remove window depending on how big it is? (lower sleep less time but it shouldnt be effected by size?)
    int diameter = radius * 2;
    while(!WindowClosed(display, window)) {
        // usleep is depricated use nanosleep?
        usleep(update);
        if(round) {
            XGetWindowAttributes(display, window, &windowAttributes);
            ShapeWindow(display, window, windowAttributes, radius, diameter);
        }
    }

    XUnmapWindow(display, window);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    exit(0);

    return 0;
}

