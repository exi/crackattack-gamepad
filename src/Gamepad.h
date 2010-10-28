
#ifndef GAMEPAD_H
#define GAMEPAD_H

#include<linux/joystick.h>

using namespace std;

class Gamepad {
    public:
        Gamepad (void) {};
        Gamepad (char* dev);
        int modifyState ( int state );

    private:
        int fd;
        bool usable;
        int modifyAxisState(int state, js_event js);
        int modifyButtonState(int state, js_event js);

};

#endif
