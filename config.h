bool interactable = true; // no input pass through window
int background = 0x00FF000000; // background colour of window
int width = 200, height = 100; // height and width of window. Set width or height to -1 for determined windows size based on screen size.
int transparency = 200; // the transparancy of the window (aim for 0 - 255)
unsigned int update = 10000; // the update interval of the window (micro seconds) basically how laggy the window is (lower is more cpu intensive) 1 seconds = 1000000 micro seconds (should you input as seconds and then multiply by 1000000?)
int radius = 40; // radius and radius of the rounded corners. Put -1 for rectangular windows (no round corners)
char *font = "fixed";

