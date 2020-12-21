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
 * DashSelector.cpp  
 *
 * A small application that allows user to select OG XBOX dashboard
 * and can apply ISO patch if needed
 *
 */
#include "stdafx.h"
#include "XKutils.h"

#include "DashSelector.h"
#include "dsIni.h"
#include "gamepad.h"
#include "wsod.h"
#include "patcher.h"

#define LAUNCH_DATA_SIZE	127

#ifdef _DEBUG
static const char* patcherPath = "T:\\isopatcher.bin";
#else
static const char* patcherPath = TITLE_DIR "\\isopatcher.bin";
#endif

static int lastError = errNoError;

// will try to fallback to dashloader cfg files if ini was not found
#ifdef _DEBUG
static const char* dashloader_Files_path = "T:\\dashloader\\";
#else
static const char* dashloader_Files_path = "C:\\dashloader\\";
#endif

static const char dashLoaderButtonMap[][32] = {
	"#IGNORE",					// ControlLeftThumbstick
	"#IGNORE",					// ControlRightThumbstick
	"#IGNORE",					// ControlBody
	"Back_Button_Dash.cfg",		// ControlBackButton
	"Start_Button_Dash.cfg",	// ControlStartButton
	"A_Button_Dash.cfg",		// ControlAButton
	"B_Button_Dash.cfg",		// ControlBButton
	"X_Button_Dash.cfg",		// ControlXButton
	"Y_Button_Dash.cfg",		// ControlYButton
	"White_Button_Dash.cfg",	// ControlWhiteButton
	"Black_Button_Dash.cfg",	// ControlBlackButton
	"#IGNORE",					// ControlCordStub
	"#IGNORE",					// ControlGaskets
	"#IGNORE",					// ControlMemCardSlot
	"#IGNORE",					// ControlLeftTrigger
	"#IGNORE",					// ControlRightTrigger
	"EMERGENCY",				// ControlDpad
	"#IGNORE",					// ControlJewel
	"Custom_Dash.cfg"			// ControlNone
};


void __cdecl main() {

//	wsod::handleError(errEmergencySelected);

	// get controller status
	static Gamepad gpad;
	lastError = gpad.init();

	int padStatus = ControlNone;
	lastError = gpad.state(&padStatus);
	
	// we could throw white screen of death on lastError != errNoError
	// but we choose to assume no button was pressed and go ahead

	XKUtils::MountDiskC();
	XKUtils::MountDiskE();
	CreateDirectory(TITLE_DIR, NULL);

	// read INI file
	tAppConfig appConfig;
	lastError = readIni(&appConfig);
	if(lastError != errNoError) {
		char tmpBuf[256] = {0};
		tAppConfigSection *cs;
		// fallback to dashloader cfg files
		// get default dashboard first
		cs = &appConfig.section[ControlNone];
		sprintf(tmpBuf, "%s%s", dashloader_Files_path, dashLoaderButtonMap[ControlNone]);
		lastError = readDashloaderCfg(tmpBuf, tmpBuf);
		// ignore the error for now
		cs->dashboard = strdup(tmpBuf);
		cs->patcher = 1;
		// try the one that user selected now
		cs = &appConfig.section[padStatus];
		sprintf(tmpBuf, "%s%s", dashloader_Files_path, dashLoaderButtonMap[padStatus]);
		lastError = readDashloaderCfg(tmpBuf, tmpBuf);
		if(lastError != errNoError) {
			cs = &appConfig.section[ControlNone];
			if(cs->dashboard == NULL) 
				wsod::handleError(lastError);
		}
		cs->dashboard = strdup(tmpBuf);
		cs->patcher = 1;
	}
	// select dashboard
	char *dashToBoot = (char *)appConfig.section[ControlNone].dashboard;
	int ledColor = appConfig.section[ControlNone].ledColor;
	int patcher = appConfig.section[ControlNone].patcher;
	tAppConfigSection *cs = &appConfig.section[padStatus];
	if(cs->dashboard != NULL) {
		dashToBoot = (char *)cs->dashboard;
		ledColor = cs->ledColor;
		patcher = cs->patcher;
	}
	// boot to WSOD if requested
	if(stricmp(dashToBoot, WSOD) == 0) {
		// failover to WSOD
		wsod::handleError(errEmergencySelected);
	}
	// check if dashboard exists
	if(util::fileExists(dashToBoot) != 1) {
		// try default if selected doesn't exist
		if(util::fileExists(appConfig.section[ControlNone].dashboard) != 1) {
			wsod::handleError(errNoDashboard);
		}
		dashToBoot = (char *)appConfig.section[ControlNone].dashboard;
		ledColor = 255;
		patcher = appConfig.section[ControlNone].patcher;
	}

	LAUNCH_DATA dt, *pdt = NULL;
	if(patcher != 0) {
		Patcher pt;
		int ps = pt.patch();
		if(ps == statusNeedPatch) {
			// check if patcher is on the disk
			if(!util::fileExists(patcherPath)) {
				// dump patcher xbe 
				FILE *f = fopen(patcherPath, "wb");
				fwrite(patcherBin, 1, patcherBinLen, f);
				fclose(f);
			}
			memset(&dt.Data, 0, LAUNCH_DATA_SIZE);
			util::winPathToXPath(dashToBoot, (char*)&dt.Data);
			pdt = &dt;
			dashToBoot = (char*) patcherPath;
		} else if(ps == statusPatched) {
			// unmount VCdRom if needed
			XKUtils::UnMountDevice("VD:");
		}
	}
	if(ledColor >= 0)
		XKUtils::SetXBOXLEDStatus(ledColor);

	XKUtils::LaunchXBE(dashToBoot, pdt);
	
	// All Done!
}

int readDashloaderCfg(char *fname, char* target) {
	FILE* file;
	unsigned int length;
	file = fopen(fname, "r+t");
	if(file == NULL)
		return errIniRead;
	length = 0;
	while(1)
	{
		target[length] = fgetc(file);
		if( target[length] == EOF || target[length] == '\n' || target[length] == '\r' )
		break;
		length++;
	}
	fclose(file);
	if(length <= 0)
		return errIniParse;
	target[length] = '\0';
	return 0;
}


