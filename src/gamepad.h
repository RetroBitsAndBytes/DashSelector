/*
 *  Copyright (C) 2020 _AA_
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with the source code; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 *
 * gamepad.h
 *
 */
#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#include <xtl.h>
#include "XKUtils.h"
#include "XBInput.h"

#include "util.h"

enum ControlState { 
	ControlLeftThumbstick,
	ControlRightThumbstick,
	ControlBody,
	ControlBackButton,
	ControlStartButton,
	ControlAButton,
	ControlBButton,
	ControlXButton,
	ControlYButton,
	ControlWhiteButton,
	ControlBlackButton,
	ControlCordStub,
	ControlGaskets,
	ControlMemCardSlot,
	ControlLeftTrigger,
	ControlRightTrigger,
	ControlDpad,
	ControlJewel,
	ControlNone,
	NumControls
};

const SHORT XINPUT_DEADZONE = (SHORT)( 0.24f * FLOAT(0x7FFF) );

struct Gamepad {
	Gamepad() {
	}

	XDEVICE_PREALLOC_TYPE* inputDeviceTypes;
    DWORD                  numInputDeviceTypes;
    XBGAMEPAD              defaultPad;
	XBGAMEPAD*             gamePad;

	int init() {
		numInputDeviceTypes = 0;
		inputDeviceTypes = NULL;

		XInitDevices( numInputDeviceTypes, inputDeviceTypes );
		if(XBInput_CreateGamepads(&gamePad) != S_OK) {
			return errGamepad;
		}
		return errNoError;
	}

	int state(int *s) {
		*s = ControlNone;
        ZeroMemory( &defaultPad, sizeof(defaultPad) );
		BOOL noPad = TRUE;
        INT nThumbLX = 0;
        INT nThumbLY = 0;
        INT nThumbRX = 0;
        INT nThumbRY = 0;
		int rep = 8;
		while(1) {
			XBInput_GetInput( gamePad );
			for( DWORD i=0; i<4; i++ ) {
				if( gamePad[i].hDevice ) {
					noPad = FALSE;
					if( gamePad[i].sThumbLX > XINPUT_DEADZONE ||
						gamePad[i].sThumbLX < -XINPUT_DEADZONE )
						nThumbLX += gamePad[i].sThumbLX;
					if( gamePad[i].sThumbLY > XINPUT_DEADZONE ||
						gamePad[i].sThumbLY < -XINPUT_DEADZONE )
						nThumbLY += gamePad[i].sThumbLY;
					if( gamePad[i].sThumbRX > XINPUT_DEADZONE ||
						gamePad[i].sThumbRX < -XINPUT_DEADZONE )
						nThumbRX += gamePad[i].sThumbRX;
					if( gamePad[i].sThumbRY > XINPUT_DEADZONE ||
						gamePad[i].sThumbRY < -XINPUT_DEADZONE )
						nThumbRY += gamePad[i].sThumbRY;

					defaultPad.fX1 += gamePad[i].fX1;
					defaultPad.fY1 += gamePad[i].fY1;
					defaultPad.fX2 += gamePad[i].fX2;
					defaultPad.fY2 += gamePad[i].fY2;
					defaultPad.wButtons        |= gamePad[i].wButtons;
					defaultPad.wPressedButtons |= gamePad[i].wPressedButtons;
					defaultPad.wLastButtons    |= gamePad[i].wLastButtons;

					if( g_Gamepads[i].Event )
						defaultPad.Event = gamePad[i].Event;

					for( DWORD b=0; b<8; b++ ) {
						defaultPad.bAnalogButtons[b]        |= gamePad[i].bAnalogButtons[b];
						defaultPad.bPressedAnalogButtons[b] |= gamePad[i].bPressedAnalogButtons[b];
						defaultPad.bLastAnalogButtons[b]    |= gamePad[i].bLastAnalogButtons[b];
					}
				}
			}
			defaultPad.sThumbLX = (SHORT)nThumbLX;
			defaultPad.sThumbLY = (SHORT)nThumbLY;
			defaultPad.sThumbRX = (SHORT)nThumbRX;
			defaultPad.sThumbRY = (SHORT)nThumbRY;
			
			// ugly but effective ;)
			if((defaultPad.wButtons & XINPUT_GAMEPAD_DPAD_UP) ||
				(defaultPad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ||
				(defaultPad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) || 
				(defaultPad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)) {
				*s = ControlDpad;
				return errNoError;
			}
			if(defaultPad.bAnalogButtons[XINPUT_GAMEPAD_A]) {
				*s = ControlAButton;
				return errNoError;
			}
			if(defaultPad.bAnalogButtons[XINPUT_GAMEPAD_B]) {
				*s = ControlBButton;
				return errNoError;
			}
			if(defaultPad.bAnalogButtons[XINPUT_GAMEPAD_X]) {
				*s = ControlXButton;
				return errNoError;
			}
			if(defaultPad.bAnalogButtons[XINPUT_GAMEPAD_Y]) {
				*s = ControlYButton;
				return errNoError;
			}
			if(defaultPad.bAnalogButtons[XINPUT_GAMEPAD_BLACK]) {
				*s = ControlBlackButton;
				return errNoError;
			}
			if(defaultPad.bAnalogButtons[XINPUT_GAMEPAD_WHITE]) {
				*s = ControlWhiteButton;
				return errNoError;
			}
			if(defaultPad.wButtons & XINPUT_GAMEPAD_BACK) {
				*s = ControlBackButton;
				return errNoError;
			}
			if(defaultPad.wButtons & XINPUT_GAMEPAD_START) {
				*s = ControlStartButton;
				return errNoError;
			}
			if(defaultPad.bAnalogButtons[XINPUT_GAMEPAD_LEFT_TRIGGER]) {
				*s = ControlLeftTrigger;
				return errNoError;
			}
			if(defaultPad.bAnalogButtons[XINPUT_GAMEPAD_RIGHT_TRIGGER]) {
				*s = ControlRightTrigger;
				return errNoError;
			}
			rep--;
			if(rep < 0) {
				return errNoError;
			}
			Sleep(50);
		}

		return errNoError;
	}
};

#endif // _GAMEPAD_H_