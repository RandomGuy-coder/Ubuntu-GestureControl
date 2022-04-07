//
// Created by terrancewong on 21/03/2022.
//

#ifndef UBUNTU_GESTURECONTROL_MOUSE_CONTROL_H
#define UBUNTU_GESTURECONTROL_MOUSE_CONTROL_H

#include <X11/Xlib.h>  // X11
#include <stdio.h>     // printf, fprintf and fflush
#include <string.h>    // memset

class mouse_control {
public:
    mouse_control(void);
    void click(Display *display, int button, XEvent event);
    void release(Display *display, int button, XEvent event);
    void coords(Display *display, int *x, int *y);
    void move (Display *display, int x, int y);
    void move_to (Display *display, int x, int y);
};


#endif //UBUNTU_GESTURECONTROL_MOUSE_CONTROL_H