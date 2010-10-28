/*
 * Gamepad.h
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
