#ifndef __CONFIG_H_
#define __CONFIG_H_

#define DRY 0                                   // If keypresses get outputted to std out
#define QUEUE_LENGTH 200
unsigned long background = 0x00FF000000; // background colour of window (hex)
int width = 200, height = 100;           // Set width or height to -1 for determined windows size based on screen size
unsigned int transparency = 200;         // the transparancy (alpha) of the window (0 - 255)
const int radius = 40;                         // radius of the rounded corners
const char *font = "fixed";                    // XLFD string or alias of font
const unsigned long colour = 0x0FFFFFF;        // colour of text (hex)
const float space = 1;                         // the amount of space between each keypress
const int paddingY = 20, paddingX = 10;        // text wrap padding

const char *text[249] = {"reserved", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "<--", "-->|", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "enter", "leftctrl", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "\'", "`", "leftshift", "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "rightshift", "kpasterisk", "leftalt", "_", "capslock", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "numlock", "scrolllock", "kp7", "kp8", "kp9", "kpminus", "kp4", "kp5", "kp6", "kpplus", "kp1", "kp2", "kp3", "kp0", "kpdot", "zenkakuhankaku", "102nd", "F11", "F12", "ro", "katakana", "hiragana", "henkan", "katakanahiragana", "muhenkan", "kpjpcomma", "kpenter", "rightctrl", "kpslash", "sysrq", "rightalt", "linefeed", "home", "up", "pageup", "<-", "->", "end", "down", "pagedown", "insert", "delete", "macro", "mute", "volumedown", "volumeup", "power", "kpequal", "kpplusminus", "pause", "scale", "kpcomma", "hangeul", "hanguel", "hanja", "yen", "leftmeta", "rightmeta", "compose", "stop", "again", "props", "undo", "front", "copy", "open", "paste", "find", "cut", "help", "menu", "calc", "setup", "sleep", "wakeup", "file", "sendfile", "deletefile", "xfer", "prog1", "prog2", "www", "msdos", "coffee", "screenlock", "rotate_display", "direction", "cyclewindows", "mail", "bookmarks", "computer", "back", "forward", "closecd", "ejectcd", "ejectclosecd", "nextsong", "playpause", "previoussong", "stopcd", "record", "rewind", "phone", "iso", "config", "homepage", "refresh", "exit", "move", "edit", "scrollup", "scrolldown", "kpleftparen", "kprightparen", "new", "redo", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", "playcd", "pausecd", "prog3", "prog4", "all_applications", "dashboard", "suspend", "close", "play", "fastforward", "bassboost", "print", "hp", "camera", "sound", "question", "email", "chat", "search", "connect", "finance", "sport", "shop", "alterase", "cancel", "brightnessdown", "brightnessup", "media", "switchvideomode", "kbdillumtoggle", "kbdillumdown", "kbdillumup", "send", "reply", "forwardmail", "save", "documents", "battery", "bluetooth", "wlan", "uwb", "unknown", "video_next", "video_prev", "brightness_cycle", "brightness_auto", "brightness_zero", "display_off", "wwan", "wimax", "rfkill", "micmute"}; // text display in ascending order of keycodes from /usr/include/linux/input-event-codes.h

#endif // __CONFIG_H_
