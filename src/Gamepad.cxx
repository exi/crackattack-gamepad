/*
 * Gamepad.cxx
 * Reno Reckling - 10/29/10
 *
 * Copyright (C) 2010  Reno Reckling
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * Reno Reckling - blog.exi.wthack.de
 * Bienleinstor Strasse 7
 * 76227 Karlsruhe
 *
 * Manages Gamepads from device paths.
 */

#include "Game.h"
#include "Gamepad.h"
#include "Controller.h"
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>

#include <linux/input.h>
#include <linux/joystick.h>

using namespace std;

Gamepad::Gamepad(char* dev) {
    cout<<"gamepad init"<<endl;
    if ((this->fd = open(dev, O_RDONLY)) < 0) {
        cout<<"error while opening the gamepad device"<<endl;
    } else {
	fcntl(this->fd, F_SETFL, O_NONBLOCK);
        this->usable = true;
        cout<<"gamepad is usable"<<endl;
    }
}

int Gamepad::modifyState ( int state )
{
    if(this->usable) {
        struct js_event js;
        bool queuedone = false;

        read(this->fd, &js, sizeof(struct js_event)) == sizeof(struct js_event);
        switch(js.type & ~JS_EVENT_INIT) {
            case JS_EVENT_AXIS: 
                state = modifyAxisState(state, js);
                break;
            case JS_EVENT_BUTTON: 
                state = modifyButtonState(state, js);
                break;
        }

    }
    return state;
}

int Gamepad::modifyAxisState(int state, js_event js) {
    if (js.number == 0) {//XAxis

        if (js.value < 0)  {//left
            state |= CC_LEFT;
        } else if (js.value > 0) { //right
            state |= CC_RIGHT;
        } else {
            state &= ~CC_LEFT;
            state &= ~CC_RIGHT;
        }
    } else if (js.number == 1) { //YAxis
        if (js.value < 0) { //up
            state |= CC_UP;
        } else if (js.value > 0) { //down
            state |= CC_DOWN;
        } else {
            state &= ~CC_UP;
            state &= ~CC_DOWN;
        }
    }

    return state;
}

int Gamepad::modifyButtonState(int state, js_event js) {
    if (js.number == 2 ) { //3 button

        if (js.value == 1)  {//down
            state |= CC_SWAP;
        } else {//up
            state &= ~CC_SWAP;
        }
    } else if (js.number == 0 ) { //1 button
        if (js.value == 1) { //down
            state |= CC_PAUSE;
        } else { //up
            state &= ~CC_PAUSE;
        }
    } else if (js.number == 5 || js.number == 7 ) { //R1/2 buttons
        if (js.value == 1) { //down
            state |= CC_ADVANCE;
        } else { //up
            state &= ~CC_ADVANCE;
        }
    } else if (js.number == 4 || js.number == 6 ) { //R1/2 buttons
        if (js.value == 1) {
            Game::concession();
        }
    }

    return state;
}
