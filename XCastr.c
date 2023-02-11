#include "WindowFunctions.c"

int main(int argc, char *argv[]) {
    Display *display = XOpenDisplay(NULL);
    int screen = DefaultScreen(display);
    Window root = XDefaultRootWindow(display), window;

    if(width == -1)
        width = DisplayWidth(display, screen) / 9;
    if(height == -1)
        height = DisplayHeight(display, screen) / 13;


    window = CreateWindow(display, root, screen, background, width, height);

    XWindowAttributes windowAttributes;
    if(radius != -1) {
        // window attribuets for size (for shape)
        XGetWindowAttributes(display, window, &windowAttributes);
        ShapeWindow(display, window, windowAttributes);
    }

    if(!interactable)
        WindowIntractable(display, window);
    if(transparency != 255)
        TransparentWindow(display, window);

    // TODO: not Interactable should be able to move and resizew the window
    // TODO: *Maybe* something like an ini file to keep window position or telast an option to save settings
    // TODO: Check if focus window asks for password and if it does then replace the characters with '*'
    // TODO: dynamically change window size by text, add maximum width variable
    // TODO: fade window when silent after a while (from 255/200 alpha to 0)
    // TODO: detect key presses from root window
    // TODO: why does a non runded window use so much cpu
    // TODO: only include headers that are corelate with user config (e.g. no need for lxfixes if window isnt intractable) orfind a way to do it without extension
    // TODO: is XLookupString better than the text array?
    // TODO: dont rely on rarely usqed extensions
    // TODO: maybe have to handle KeyRelease event
    // TODO: capital and other for modifier keys
    // TODO: only use xtest for programs that dont like XSendEvent or no?
    // TODO: Mask support (control, meta, alt, etc.) for XSendEvent
    // TODO: implement key down functionality

    GC gc = XCreateGC(display, window, 0, 0);
    XSetBackground(display, gc, WhitePixel(display, 0));
    HexState(display, gc, colour);

    XFontStruct *loadedFont = XLoadQueryFont(display, font);
    if(loadedFont == NULL) {
        printf("font: \"%p\" does not exist\n", font);
        return -1;
    }
    XSetFont(display, gc, loadedFont->fid);

    XMapRaised(display, window);
    XSync(display, 0);

    int grab = XGrabKeyboard(display, root, 0, GrabModeAsync, GrabModeAsync, CurrentTime);
    if (grab != GrabSuccess) {
        printf("Failed to grab keyboard: \"%d\"\n", grab);
        return -1;
    }

    XEvent keypress;
    keysend.type = KeyPress;
    keysend.display = display;
    keysend.subwindow = None;
    keysend.time = CurrentTime;
    keysend.x = keysend.y = keysend.x_root = keysend.y_root = 1;
    keysend.same_screen = 1;

    int offset = paddingX, currentWidth;
    int key;

    while(!WindowClosed(display, window, keypress)) {
        XGetWindowAttributes(display, window, &windowAttributes);

        if(radius != -1) // TODO: only shape window on widht/height difference
            ShapeWindow(display, window, windowAttributes);

        if(XPending(display)) {
            XNextEvent(display, &keypress);
            if(keypress.type == KeyPress) {
                keysend.keycode = keypress.xkey.keycode;
                ReGrab(display, keysend);

                // printf("Keycode: %d\n", keypress.xkey.keycode);
                key = keypress.xkey.keycode - 8;
                currentWidth = XTextWidth(loadedFont, text[key], strlen(text[key]));
                if(keypress.xkey.keycode == 9 || offset + currentWidth + space + paddingX > windowAttributes.width - paddingX) {
                    XClearArea(display, window, 0, 0, windowAttributes.width, windowAttributes.height, 0);
                    offset = paddingX;
                }
                if(keypress.xkey.keycode == 24)
                    XUngrabKeyboard(display, CurrentTime);

                XDrawString(display, window, gc, offset, paddingY, text[key], strlen(text[key]));

                offset += currentWidth + space;
                // XCopyArea(display, window, window, gc, );

            }
        }
    }

    XUngrabKeyboard(display, CurrentTime);
    XUnmapWindow(display, window);
    XUnloadFont(display, loadedFont->fid);
    XDestroyWindow(display, window);
    XFlush(display);
    XCloseDisplay(display);

    return 0;
}
