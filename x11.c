#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdint.h>

int main(void)
{
    Display *display = XOpenDisplay(NULL);
    if (display == NULL)
    {
	printf("Error on display open\n");
	return(1);
    }
    
    Window window = XCreateSimpleWindow(display, 
					XDefaultRootWindow(display),
					0, 0, 
					800, 600, 
					0, 
					0, 
					0);

    XWindowAttributes wind_attr = {0};
    XGetWindowAttributes(display, window, &wind_attr);

    // format == ZPixmap. so we have color, else XYPixmap. not explained in docs btw.
    XImage *image = XCreateImage(display, 
				 wind_attr.visual, wind_attr.depth,
				 ZPixmap,
				 0,
				 (char *) grid_px, // TODO: map to game code
				 GRID_WIDTH_PX, GRID_HEIGHT_PX,
				 32,
				 GRID_WIDTH * sizeof(RGBA32));

    GC gc = XCreateGC(display, window, 0, NULL);
    
    // event-msg we wanna receive from window maager layer. kinda? docs suck. 
    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wm_delete_window, 1);

    XSelectInput(display, window, KeyPressMask);

    XMapWindow(display, window);

    XSync(display, False);

    int quit = 0;
    while(!quit)
    {
	XEvent event = {0};
	XNextEvent(display, &event);
	
	switch (event.type)
	{
	    case KeyPress: {
		printf("x_window key pressed master weasel\n");
	    } break;

	    case ClientMessage: {
		if ((Atom) event.xclient.data.l[0] == wm_delete_window) // thnx S.O
		{
		    printf("WM_DELETE go bbbrrr");
		    quit = 1;
		}
	    } break;
	}
    }

    XCloseDisplay(display);
    
    return 0;    
}
