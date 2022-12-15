#include <X11/Xatom.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>

#define BASIC_EVENT_MASK (StructureNotifyMask|ExposureMask|PropertyChangeMask|EnterWindowMask|LeaveWindowMask|KeyPressMask|KeyReleaseMask|KeymapStateMask)
#define NOT_PROPAGATE_MASK (KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|ButtonMotionMask)

typedef enum { false, true } bool;

Window CreateWindow(Display *display, Window root, Colormap colourmap, XWindowAttributes rootAttr, XVisualInfo vInfo, int background, int width, int height) {

    XSetWindowAttributes setWindowAttributes;
    setWindowAttributes.background_pixmap = None;
    setWindowAttributes.background_pixel = background;
    setWindowAttributes.win_gravity = NorthWestGravity;
    setWindowAttributes.bit_gravity = ForgetGravity;
    setWindowAttributes.border_pixel = 0;
    setWindowAttributes.save_under = 1;
    setWindowAttributes.event_mask = BASIC_EVENT_MASK;
    setWindowAttributes.do_not_propagate_mask = NOT_PROPAGATE_MASK;
    setWindowAttributes.colormap = colourmap;

    unsigned long mask = CWColormap | CWBorderPixel | CWBackPixel | CWEventMask | CWWinGravity | CWBitGravity | CWSaveUnder | CWDontPropagate;

    // TODO: must consider window manager gaps minus from root width, heigh. also border from windowAttributes.border_width
    Window window = XCreateWindow(display, root, rootAttr.width-width, rootAttr.height-height, width, height, 0, vInfo.depth, InputOutput, vInfo.visual, mask, &setWindowAttributes);

    // Set window name
    XStoreName(display, window, "XCastr");
    XClassHint hint = {  "xcastr", "XCastr"  };
    XSetClassHint(display, window, &hint);

    // wm specs
    Atom property[3];
    property[1] = XInternAtom(display, "_NET_WM_WINDOW_TYPE", 0);
    property[0] = XInternAtom(display, "_NET_WM_WINDOW_TYPE_UTILITY", 0);
    XChangeProperty(display, window, property[1], XA_ATOM, 32, PropModeReplace, (unsigned char*) property, 1);
    property[2] = XInternAtom(display, "_NET_WM_STATE", 0);
    property[1] = XInternAtom(display, "_NET_WM_STATE_ABOVE", 0);
    property[0] = XInternAtom(display, "_NET_WM_STATE_STICKY", 0);
    XChangeProperty(display, window, property[2], XA_ATOM, 32, PropModeAppend, (unsigned char*) property, 2);

    // TODO: try to raise the stacking order to be ontop of floating windows
    XSetTransientForHint(display, window, window);
    // XRaiseWindow(display, window);


    return window;
}

void ShapeWindow(Display *display, Window window, XWindowAttributes windowAttr, int radius, int diameter) {
    // if the width or height of the window is smaller than the corners
    // TODO: if window height or width is smaller then make round = 0 and change update interval
    if(windowAttr.width < diameter || windowAttr.height < diameter)
        return;
    // shape for non rectangular window
    int shapeEventBase, shapeErrorBase;
    if(!XShapeQueryExtension(display, &shapeEventBase, &shapeErrorBase))
        return; // printf("Shape library not found");

    int pixmap = XCreatePixmap(display, window, windowAttr.width, windowAttr.height, 1);

    XGCValues xgcv;
    GC gc = XCreateGC(display, pixmap, 0, &xgcv);

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

bool WindowClosed(Display *display, Window window) {
    Atom wm_protocols = XInternAtom(display, "WM_PROTOCOLS", 0);
    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(display, window, &wm_delete_window, 1);

    XEvent event;
    if(XCheckTypedWindowEvent(display, window, ClientMessage, &event) && event.xclient.message_type == wm_protocols && event.xclient.data.l[0] == wm_delete_window)
        return 1;
    return 0;
}

void WindowIntractable(Display *display, Window window) {
    // TODO: Interactable windows should be able to be managed, moved resized, etc. and only be able to pass input through also should be sticky
    XserverRegion region = XFixesCreateRegion(display, NULL, 0);
    XFixesSetWindowShapeRegion(display, window, ShapeInput, 0, 0, region);
    XFixesDestroyRegion(display, region);
}

void TransparentWindow(Display *display, Window window, int alpha) {
    alpha = alpha | alpha << 8 | alpha << 16 | alpha << 24;

    Atom windowOpacity = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", 0);
    XChangeProperty(display, window, windowOpacity, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &alpha, 1);
}

