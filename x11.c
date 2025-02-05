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
		printf("got the key press master weasel\n");
	    } break;

	    case ClientMessage: {
		if ((Atom) event.xclient.data.l[0] == wm_delete_window) // becoz S.O said so
		{
		    quit = 1;
		}
	    } break;
	}
    }

    XCloseDisplay(display);
    
    return 0;
    
}
