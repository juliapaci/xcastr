#include <X11/Xatom.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>

#define BASIC_EVENT_MASK (StructureNotifyMask|ExposureMask|PropertyChangeMask|EnterWindowMask|LeaveWindowMask|KeyPressMask|KeyReleaseMask|KeymapStateMask)
#define NOT_PROPAGATE_MASK (KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|ButtonMotionMask)

typedef enum { false, true } bool;

Window CreateWindow(Display *display, Window root, Colormap colourmap, XWindowAttributes rootAttr, XVisualInfo vInfo, int background, int width, int height) {

    XSetWindowAttributes setWindowAttributes;
    setWindowAttributes.override_redirect = 1;
    setWindowAttributes.background_pixmap = None;
    setWindowAttributes.background_pixel = background;
    setWindowAttributes.win_gravity = NorthWestGravity;
    setWindowAttributes.bit_gravity = ForgetGravity;
    setWindowAttributes.border_pixel = 0;
    setWindowAttributes.save_under = 1;
    setWindowAttributes.event_mask = BASIC_EVENT_MASK;
    setWindowAttributes.do_not_propagate_mask = NOT_PROPAGATE_MASK;
    setWindowAttributes.colormap = colourmap;


    unsigned long mask = CWColormap | CWBorderPixel | CWBackPixel | CWEventMask | CWWinGravity | CWBitGravity | CWSaveUnder | CWDontPropagate | CWOverrideRedirect;

    // TODO: must consider window manager gaps minus from root width, height
    return XCreateWindow(display, root, rootAttr.width-width, rootAttr.height-height, width, height, 0, vInfo.depth, InputOutput, vInfo.visual, mask, &setWindowAttributes);

}

void RemoveWindow(Display *display, Window window) {
    XUnmapWindow(display, window);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
}

void WindowInteractable(Display *display, Window window) {
    XserverRegion region = XFixesCreateRegion(display, NULL, 0);
    XFixesSetWindowShapeRegion (display, window, ShapeInput, 0, 0, region);
    XFixesDestroyRegion (display, region);
}

void TransparentWindow(Display *display, Window window, int alpha) {
    alpha = alpha | alpha << 8 | alpha << 16 | alpha << 24;

    Atom windowOpacity = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", 0);
    XChangeProperty(display, window, windowOpacity, XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &alpha, 1);
}

