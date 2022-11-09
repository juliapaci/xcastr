#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    Display *display = XOpenDisplay(NULL);
    // int visual = DefaultVisual(display, screen);
    Window root = XDefaultRootWindow(display), window;

    XVisualInfo vinfo;
    XMatchVisualInfo(display, DefaultScreen(display), 32, TrueColor, &vinfo);
    Colormap colourmap = XCreateColormap(display, root, vinfo.visual, AllocNone);
    // XColor backgroundColour = createXColorFromRGBA(0, 0, 0, 0);


    XWindowAttributes rootAttributes, windowAttributes;

    Atom property[2];




    XGetWindowAttributes(display, root, &rootAttributes); // get root attributes for resolution (to set window at corner of screen)

    XSetWindowAttributes setWindowAttributes;
    setWindowAttributes.override_redirect = 1;
    setWindowAttributes.background_pixmap = None;
    setWindowAttributes.background_pixel = "0x00FF000000";
    setWindowAttributes.border_pixel=0;
    setWindowAttributes.win_gravity=NorthWestGravity;
    setWindowAttributes.bit_gravity=ForgetGravity;
    setWindowAttributes.save_under=1;
    setWindowAttributes.colormap = colourmap;


    unsigned long m = CWColormap | CWBorderPixel | CWBackPixel | CWEventMask | CWWinGravity|CWBitGravity | CWSaveUnder | CWDontPropagate | CWOverrideRedirect;

    window = XCreateWindow(display, root, rootAttributes.x, rootAttributes.y, 200, 200, 0, vinfo.depth, InputOutput, vinfo.visual, m, &setWindowAttributes);

    // XGetWindowAttributes(display, window, &windowAttributes); // get window attributes




    // Set window name
    XStoreName(display, window, "XCastr");
    XClassHint hint = {  "xcastr", "XCastr"  };
    XSetClassHint(display, window, &hint);



    // Set window position to bottom right of screen
    // XTranslateCoordinates(display, window, root, 0, 0, (unsigned int *) rootAttributes.width, (unsigned int *) rootAttributes.height, window);





    // TODO: set window floating
    property[1] = XInternAtom(display, "_NET_WM_WINDOW_TYPE", 0);
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
    // TODO: set window backgroudn colour
    // TODO: remove decorations
    // TODO: Check if focus window asks for password and if it does then replace the characters with "*"
    // TODO: dynamically change window size by text
    // TODO: fade window when silent after a while
    // TODO: detect key presses from root window


    // Display window
    XMapWindow(display, window);
    XSync(display, 0);
    // XFlush(display);

    XUnmapWindow(display, window);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}

