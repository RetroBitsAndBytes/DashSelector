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
 * dsIni.cpp  
 *
 * Handles ini file
 *
 */
#include "stdafx.h"

#include "dsIni.h"
#include "gamepad.h"

// mapping button names as per gamepad.h
static const char buttonMap[][MAX_SECTOIN_NAME_LEN] = {
	"#IGNORE",	// ControlLeftThumbstick
	"#IGNORE",	// ControlRightThumbstick
	"#IGNORE",	// ControlBody
	"BACK",		// ControlBackButton
	"START",	// ControlStartButton
	"A",		// ControlAButton
	"B",		// ControlBButton
	"X",		// ControlXButton
	"Y",		// ControlYButton
	"WHITE",	// ControlWhiteButton
	"BLACK",	// ControlBlackButton
	"#IGNORE",	// ControlCordStub
	"#IGNORE",	// ControlGaskets
	"#IGNORE",	// ControlMemCardSlot
	"LT",		// ControlLeftTrigger
	"RT",		// ControlRightTrigger
	"#IGNORE",	// ControlDpad
	"#IGNORE",	// ControlJewel
	"DEFAULT"	// ControlNone
};

// read ini file
int readIni(tAppConfig *cfg) {
	char *iniFile;

	// initialize config with sane values
    for(int i=0; i<NumControls; i++) {
        cfg->section[i].dashboard = NULL;
        cfg->section[i].ledColor = -1;
        cfg->section[i].patcher = 1;
    }
	// special case for WSOD
	cfg->section[ControlDpad].dashboard = WSOD;
    cfg->section[ControlDpad].ledColor = 255;
    cfg->section[ControlDpad].patcher = 0;

	if(util::fileExists(INI_LOC_C)) { 
		iniFile = INI_LOC_C;
	} else if(util::fileExists(INI_LOC_E)) {
		iniFile = INI_LOC_E;
	} else {
		return errIniRead;
	}
	int ret = ini_parse(iniFile, iniHandler, cfg);
	if(ret < 0)
		return errIniRead;
	if(ret > 0)
		return errIniParse;

	return errNoError;
}

// ini handler (see ini.h)
// not the most efficient way of doing this but will do for now...
static int iniHandler(void* data, const char* section, const char* name, const char* value) {
	tAppConfig* conf = (tAppConfig*)data;

	for(int i=0; i<NumControls; i++) {
		const char *s = buttonMap[i];
		if(stricmp(section, s) != 0) {
			continue;
		}
		tAppConfigSection *c = &conf->section[i];
		if(stricmp(name, "dashboard") == 0) {
			c->dashboard = strdup(value);            
		} else if(stricmp(name, "led") == 0) {
			c->ledColor = atoi(value);
		} else if(stricmp(name, "patcher") == 0) {
			c->patcher = atoi(value);
		} else {
			return 0;
		}
		break;
	}
	return 1;
}

