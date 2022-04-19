#ifndef GESTRO_KEYBOARDACTION_H
#define GESTRO_KEYBOARDACTION_H

#include <X11/Xlib.h>
#include "KeyboardEvent.h"

//class to hold all available keyboard shortcuts
//Class is declared using the keycode
//as defined by the keysymdef library

class KeyboardAction
{
private:
    Display *display;
public:
    KeyboardAction(Display *d);
    void pressKey(int k);
};


#endif //GESTRO_KEYBOARDACTION_H