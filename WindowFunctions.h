#ifndef __WINDOWFUNCTIONS_H_
#define __WINDOWFUNCTIONS_H_

#include <X11/Xlib.h>
#include <X11/extensions/record.h>

#include "config.h"

int queue[QUEUE_LENGTH], current = 0; // TODO: FIX this. needs to be globally declared in bopth window functions c and main.

Window CreateWindow(Display *display, Window root, int screen, int background, int width, int height);
int ShapeWindow(Display *display, Window window, XWindowAttributes windowAttr);
void HexState(Display *display, GC gc, unsigned long colour);
void callback(XPointer data, XRecordInterceptData *hook);
Bool WindowClosed(Display *display, Window window);
void TransparentWindow(Display *display, Window window);

#endif // __WINDOWFUNCTIONS_H_
