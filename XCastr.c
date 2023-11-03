#include <stdio.h>
#include <string.h>

#include "config.h"
#include "WindowFunctions.h"

int main(void) {
    XInitThreads();

    Display *display = XOpenDisplay(NULL);
    int screen = DefaultScreen(display);
    Window root = XDefaultRootWindow(display), window;

    if(width == -1)
        width = DisplayWidth(display, screen) / 9;
    if(height == -1)
        height = DisplayHeight(display, screen) / 13;


    window = CreateWindow(display, root, screen, background, width, height);

    XWindowAttributes windowAttributes;
    if(radius != 0) {
        // window attribuets for size (for shape)
        XGetWindowAttributes(display, window, &windowAttributes);
        ShapeWindow(display, window, windowAttributes);
    }

    if(transparency < 255)
        TransparentWindow(display, window);

    // TODO: not Interactable should be able to move and resizew the window
    // TODO: *Maybe* something like an ini file to keep window position or telast an option to save settings
    // TODO: Check if focus window asks for password and if it does then replace the characters with '*'
    // TODO: dynamically change window size by text, add maximum width variable
    // TODO: fade window when silent after a while (from 255/200 alpha to 0)
    // TODO: why does a non runded window use so much cpu
    // TODO: dont rely on rarely usqed extensions
    // TODO: capital and other for modifier keys

    GC gc = XCreateGC(display, window, 0, 0);
    XSetBackground(display, gc, WhitePixel(display, 0));
    HexState(display, gc, colour);

    XFontStruct *loadedFont = XLoadQueryFont(display, font);
    if(loadedFont == NULL) {
        printf("font: \"%s\" does not exist\n", font);
        return 1;
    }
    XSetFont(display, gc, loadedFont->fid);

    Display *recordDisplay = XOpenDisplay(NULL);
    XRecordRange *range = XRecordAllocRange();
    range->device_events.first = KeyPress;
    range->device_events.last = KeyRelease;
    XRecordClientSpec client = XRecordAllClients;
    XRecordContext context = XRecordCreateContext(recordDisplay, 0, &client, 1, &range, 1);
    XFree(range);
    if(!context)
        printf("Unable to create context\n");

    XRecordEnableContextAsync(recordDisplay, context, callback, NULL);

    XMapRaised(display, window);
    XSync(display, 0);

    int offset = paddingX + space, currentWidth;
    for(int i = 0; i < QUEUE_LENGTH; i++)
        queue[i] = -1;

    while(!WindowClosed(display, window)) {
        XRecordProcessReplies(recordDisplay);
        XGetWindowAttributes(display, window, &windowAttributes);
        if(radius == 0 || ShapeWindow(display, window, windowAttributes)) // TODO: only shape window on widht/height difference
            break;
        if(queue[current] == -1)
            continue;

        char *text_item = text[queue[current]];
        currentWidth = XTextWidth(loadedFont, text_item, strlen(text_item));

        if(offset + currentWidth + space + paddingX > windowAttributes.width - paddingX) { // if text x pos surpasses the window border
            offset = paddingX+ space;
            XClearArea(display, window, 0, 0, windowAttributes.width, windowAttributes.height, 0);
            for(int i = 0; i < current; i++) {
                char *iterText = text[queue[i]];
                int newWidth = XTextWidth(loadedFont, iterText, strlen(iterText));
                XDrawString(display, window, gc, offset += newWidth + space, paddingY, iterText, strlen(iterText));
            }
        } else
            XDrawString(display, window, gc, offset, paddingY, text_item, strlen(text_item));

        offset += currentWidth + space;
        current++;
    }

    XRecordDisableContext(display, context);
    XRecordFreeContext(display, context);
    XUnloadFont(display, loadedFont->fid);
    XUnmapWindow(display, window);
    XDestroyWindow(display, window);
    XFlush(display);
    XCloseDisplay(display);

    return 0;
}
