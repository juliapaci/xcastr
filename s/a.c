#include <X11/Xlib.h>

int main(void) {
    Display *display = XOpenDisplay(NULL);
    Window window = XCreateSimpleWindow(display, XDefaultRootWindow(display), 0, 0, 200, 200, 0, 0, 0x00FF000000);
    int grab = XGrabKeyboard(display, window, 1, GrabModeAsync, GrabModeAsync, CurrentTime);

    if (grab != GrabSuccess) {
        printf("Failed to grab keyboard: \"%d\"\n", grab);
        return -1;
    }
    XUngrabKeyboard(display, CurrentTime);

    return 1;
}
