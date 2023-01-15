#include <X11/Xatom.h>

#include <string.h>

#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>


#include <stdio.h>

typedef enum { false, true } bool;

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

    // wm specs
    Atom property[3];
    property[1] = XInternAtom(display, "_NET_WM_WINDOW_TYPE", 0);
    property[0] = XInternAtom(display, "_NET_WM_WINDOW_TYPE_UTILITY", 0);
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

void ShapeWindow(Display *display, Window window, XWindowAttributes windowAttr, int radius, int diameter) {

    int shapeEventBase, shapeErrorBase;
    if(!XShapeQueryExtension(display, &shapeEventBase, &shapeErrorBase))
        return; // printf("Shape library not found");

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

const char *text[249] = {"reserved", "esc", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "minus", "equal", "backspace", "tab", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "leftbrace", "rightbrace", "enter", "leftctrl", "a", "s", "d", "f", "g", "h", "j", "k", "l", "semicolon", "apostrophe", "grave", "leftshift", "backslash", "z", "x", "c", "v", "b", "n", "m", "comma", "dot", "slash", "rightshift", "kpasterisk", "leftalt", "space", "capslock", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "numlock", "scrolllock", "kp7", "kp8", "kp9", "kpminus", "kp4", "kp5", "kp6", "kpplus", "kp1", "kp2", "kp3", "kp0", "kpdot", "zenkakuhankaku", "102nd", "f11", "f12", "ro", "katakana", "hiragana", "henkan", "katakanahiragana", "muhenkan", "kpjpcomma", "kpenter", "rightctrl", "kpslash", "sysrq", "rightalt", "linefeed", "home", "up", "pageup", "left", "right", "end", "down", "pagedown", "insert", "delete", "macro", "mute", "volumedown", "volumeup", "power", "kpequal", "kpplusminus", "pause", "scale", "kpcomma", "hangeul", "hanguel", "hanja", "yen", "leftmeta", "rightmeta", "compose", "stop", "again", "props", "undo", "front", "copy", "open", "paste", "find", "cut", "help", "menu", "calc", "setup", "sleep", "wakeup", "file", "sendfile", "deletefile", "xfer", "prog1", "prog2", "www", "msdos", "coffee", "screenlock", "rotate_display", "direction", "cyclewindows", "mail", "bookmarks", "computer", "back", "forward", "closecd", "ejectcd", "ejectclosecd", "nextsong", "playpause", "previoussong", "stopcd", "record", "rewind", "phone", "iso", "config", "homepage", "refresh", "exit", "move", "edit", "scrollup", "scrolldown", "kpleftparen", "kprightparen", "new", "redo", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "playcd", "pausecd", "prog3", "prog4", "all_applications", "dashboard", "suspend", "close", "play", "fastforward", "bassboost", "print", "hp", "camera", "sound", "question", "email", "chat", "search", "connect", "finance", "sport", "shop", "alterase", "cancel", "brightnessdown", "brightnessup", "media", "switchvideomode", "kbdillumtoggle", "kbdillumdown", "kbdillumup", "send", "reply", "forwardmail", "save", "documents", "battery", "bluetooth", "wlan", "uwb", "unknown", "video_next", "video_prev", "brightness_cycle", "brightness_auto", "brightness_zero", "display_off", "wwan", "wimax", "rfkill", "micmute"}; // /usr/include/linux/input-event-codes.h

void WindowText(Display *display, Window window, char *style, unsigned long colour, int keycode, GC gc) {
    XFontStruct *font = XLoadQueryFont(display, style);
    if(font == NULL) {
        printf("font: \"%p\" does not exist", style);
        return;
    }
    XSetFont(display, gc, font->fid);

    unsigned int red = (colour & 0xff0000) >> 16;
    unsigned int green = (colour & 0xff00) >> 8;
    unsigned int blue = colour & 0xff;

    XColor color;
    Colormap colormap = DefaultColormap(display, DefaultScreen(display));

    color.red = red * 257; //convert to 16-bit
    color.green = green * 257; //convert to 16-bit
    color.blue = blue * 257; //convert to 16-bit
    XAllocColor(display, colormap, &color);
    XSetForeground(display, gc, color.pixel);

    // int fontHeight = font->ascent + font->descent;
    // XTextItem key[1];
    // key[0].chars = "aa";
    // key[0].nchars = 2;
    // key[0].delta = 0;
    // key[0].font = font->fid;
    // int fontWidth = XkeyWidth(font, key, 1);
    // XDrawText(display, window, gc, 0, fontHeight, key, 1);

    // printf("Key: %d, Text: %s\n", keycode, text[keycode - 9]);
    XDrawString(display, window, gc, 20, 20, text[keycode], strlen(text[keycode]));
    // XUnloadFont(display, font->fid);
}

bool WindowClosed(Display *display, Window window) {
    Atom Protocols = XInternAtom(display, "WM_PROTOCOLS", 0);
    Atom DeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", 0);
    XSetWMProtocols(display, window, &DeleteWindow, 1);

    XEvent event;
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

void TransparentWindow(Display *display, Window window, int alpha) {
    alpha = alpha | alpha << 8 | alpha << 16 | alpha << 24;

    Atom windowOpacity = XInternAtom(display, "_NET_WM_WINDOW_OPACITY", 0);
    XChangeProperty(display, window, windowOpacity, XA_CARDINAL, 32, PropModeReplace, (unsigned char*) &alpha, 1);
}

