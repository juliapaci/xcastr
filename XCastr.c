#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "WindowFunctions.c"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>


int main(int argc, char *argv[]) {

    // user variables
    bool interactable = false;
    int background = 0x00FF000000;
    int alpha = 200;


    Display *display = XOpenDisplay(NULL);
    Window root = XDefaultRootWindow(display), window;

    XVisualInfo visualInfo;
    XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &visualInfo);
    Colormap colourmap = XCreateColormap(display, root, visualInfo.visual, AllocNone);

    XWindowAttributes rootAttributes, windowAttributes;

    Atom property[2];




    XGetWindowAttributes(display, root, &rootAttributes); // get root attributes for resolution (to set window at corner of screen)

    // create window
    window = CreateWindow(display, root, colourmap, rootAttributes, visualInfo, background);

    XGetWindowAttributes(display, window, &windowAttributes); // get window attributes

    if(!interactable)
        WindowInteractable(display, window);



    // Set window name
    XStoreName(display, window, "XCastr");
    XClassHint hint = {  "xcastr", "XCastr"  };
    XSetClassHint(display, window, &hint);



    // Set window position to bottom right of screen
    // XTranslateCoordinates(display, window, root, 0, 0, (int *) rootAttributes.width, (unsigned int *) rootAttributes.height, window);

    property[1] = XInternAtom(display, "_NET_WM_WINDOW_TYPE", 0);
    property[0] = XInternAtom(display, "_NET_WM_WINDOW_TYPE_UTILITY", 0);
    XChangeProperty(display, window, property[0], XA_ATOM, 32, PropModeReplace, (unsigned char*) property, 2L);

    property[1] = XInternAtom(display, "_NET_WM_STATE", 0);
    property[0] = XInternAtom(display, "_NET_WM_STATE_ABOVE", 0);
    XChangeProperty(display, window, property[0], XA_ATOM, 32, PropModeReplace, (unsigned char*) property, 1L);



    // make window transparent
    TransparentWindow(display, window, alpha);

    // TODO: render text with custom font
    // TODO: remove decorations
    // TODO: Check if focus window asks for password and if it does then replace the characters with "*"
    // TODO: dynamically change window size by text
    // TODO: fade window when silent after a while
    // TODO: detect key presses from root window


    // Display window
    XMapWindow(display, window);
    XSync(display, 0);
    // XFlush(display);

    sleep(10);

    RemoveWindow(display, window);

    return 0;
}

