#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>

#include <stdio.h>
#include <string.h>

#include "config.h"

// for some reason if another event is in the same file as the current keyrpess it will crash??? why???? so its here now
XKeyEvent keysend;

Window CreateWindow(Display *display, Window root, int screen, int background, int width, int height) {

    // window args
    XVisualInfo visualInfo;
    XMatchVisualInfo(display, screen, 32, TrueColor, &visualInfo);
    Colormap colourmap = XCreateColormap(display, root, visualInfo.visual, AllocNone);

    XSetWindowAttributes setWindowAttributes;
    setWindowAttributes.background_pixmap = None;
    setWindowAttributes.background_pixel = background;
    setWindowAttributes.win_gravity = NorthWestGravity;
    setWindowAttributes.bit_gravity = ForgetGravity;
    setWindowAttributes.border_pixel = 0;
    setWindowAttributes.save_under = 1;
    setWindowAttributes.colormap = colourmap;

    unsigned long mask = CWColormap | CWBorderPixel | CWBackPixel | CWEventMask | CWWinGravity | CWBitGravity | CWSaveUnder | CWDontPropagate;

    // TODO: must consider window manager gaps minus from root width, heigh. also border from windowAttributes.border_width
    Window window = XCreateWindow(display, root, DisplayWidth(display, screen)-width, DisplayHeight(display, screen)-height, width, height, 0, visualInfo.depth, InputOutput, visualInfo.visual, mask, &setWindowAttributes);

    // Set window name
    XStoreName(display, window, "XCastr");
    XClassHint hint = {  "xcastr", "XCastr"  };
    XSetClassHint(display, window, &hint);

    // wm specs and ewmh
    Atom property[3];
    property[1] = XInternAtom(display, "_NET_WM_WINDOW_TYPE", 0);
    property[0] = XInternAtom(display, "_NET_WM_WINDOW_TYPE_UTILITY", 0);
    // property[0] = XInternAtom(display, "_NET_WM_WINDOW_TYPE_MODAL", 0);
    XChangeProperty(display, window, property[1], XA_ATOM, 32, PropModeReplace, (unsigned char*) property, 1);
    // TODO: only uses one property, make it se both above and sticky
    property[2] = XInternAtom(display, "_NET_WM_STATE", 0);
    property[1] = XInternAtom(display, "_NET_WM_STATE_ABOVE", 0);
    property[0] = XInternAtom(display, "_NET_WM_STATE_STICKY", 0);
    XChangeProperty(display, window, property[2], XA_ATOM, 32, PropModeReplace, (unsigned char*) property, 2);

    XSetTransientForHint(display, window, window); // whats the difference between this and setting the hint manually?
    // TODO: try to raise the stacking order to always be ontop of floating windows
    // XRaiseWindow(display, window);

    return window;
}

void ShapeWindow(Display *display, Window window, XWindowAttributes windowAttr) {
    int diameter = radius * 2;

    int shapeEventBase, shapeErrorBase;
    if(!XShapeQueryExtension(display, &shapeEventBase, &shapeErrorBase)) {
        return;
        printf("Shape library not found");
    }

    // if the width or height of the window is smaller than the corners
    // TODO: if window height or width is smaller then make round = 0 (dont update shape) and change update interval
    if(windowAttr.width < diameter || windowAttr.height < diameter)
        return;

    int pixmap = XCreatePixmap(display, window, windowAttr.width, windowAttr.height, 1);

    XGCValues xgcv;
    GC gc = XCreateGC(display, pixmap, 0, &xgcv);

    // shape for non rectangular window
    XSetForeground(display, gc, 0);
    XFillRectangle(display, pixmap, gc, 0, 0, windowAttr.width, windowAttr.height);
    XSetForeground(display, gc, 1);
    XFillArc(display, pixmap, gc, 0, 0, diameter, diameter, 0, 23040); // 23040
    XFillArc(display, pixmap, gc, 0, windowAttr.height-diameter-1, diameter, diameter, 0, 23040);
    XFillArc(display, pixmap, gc, windowAttr.width-diameter-1, 0, diameter, diameter, 0, 23040);
    XFillArc(display, pixmap, gc, windowAttr.width-diameter-1, windowAttr.height-diameter-1, diameter, diameter, 0, 23040);
    XFillRectangle(display, pixmap, gc, radius, 0, windowAttr.width-diameter, windowAttr.height);
    XFillRectangle(display, pixmap, gc, 0, radius, windowAttr.width, windowAttr.height-diameter);

    XShapeCombineMask(display, window, ShapeBounding, -windowAttr.border_width, -windowAttr.border_width, pixmap, ShapeSet);
    XShapeCombineMask(display, window, ShapeClip, 0, 0, pixmap, ShapeSet);

    XFreePixmap(display, pixmap);
    XFreeGC(display, gc);
}

void HexState(Display *display, GC gc, unsigned long colour) {
    unsigned int red = (colour & 0xff0000) >> 16;
    unsigned int green = (colour & 0xff00) >> 8;
    unsigned int blue = colour & 0xff;
    XColor color;
    Colormap colormap = DefaultColormap(display, DefaultScreen(display));
    color.red = red * 257;
    color.green = green * 257;
    color.blue = blue * 257;

    XAllocColor(display, colormap, &color);
    XSetForeground(display, gc, color.pixel);
}

void ReGrab(Display *display, XKeyEvent keysend) {
    int revert;
    Window focus;
    XUngrabKeyboard(display, CurrentTime);
    XGetInputFocus(display, &focus, &revert);
    XSendEvent(display, focus, True, KeyPressMask, (XEvent *)&keysend);
    XFlush(display);

    int grab = XGrabKeyboard(display, XDefaultRootWindow(display), 0, GrabModeAsync, GrabModeAsync, CurrentTime);
    if (grab != GrabSuccess) {
        printf("Failed to grab keyboard: \"%d\"\n", grab);
        return;
    }
}

Bool WindowClosed(Display *display, Window window, XEvent event) {
    Atom Protocols = XInternAtom(display, "WM_PROTOCOLS", 0);
    Atom DeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(display, window, &DeleteWindow, 1);

    if(XCheckTypedWindowEvent(display, window, ClientMessage, &event) && event.xclient.message_type == Protocols && event.xclient.data.l[0] == DeleteWindow)
        return 1;
    return 0;
}

void WindowIntractable(Display *display, Window window) {
    // TODO: Interactable windows should be able to be managed, moved resized, etc. and only be able to pass input through also should be sticky
    // TODO: update interactable zone on update loop
    XserverRegion region = XFixesCreateRegion(display, NULL, 0);
    XFixesSetWindowShapeRegion(display, window, ShapeInput, 0, 0, region);
    XFixesDestroyRegion(display, region);
}

void TransparentWindow(Display *display, Window window) {
    transparency = transparency | transparency << 8 | transparency << 16 | transparency << 24;

    Atom windowOpacity = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", 0);
    XChangeProperty(display, window, windowOpacity, XA_CARDINAL, 32, PropModeReplace, (unsigned char*) &transparency, 1);
}
