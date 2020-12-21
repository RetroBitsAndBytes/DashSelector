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
 * dsIni.h
 *
 */
#ifndef _DSINI_H_
#define _DSINI_H_

#include "util.h"
#include "ini.h"
#include "gamepad.h"

// dashselector.ini is either c:\dashselector.ini or e:\dashselector.ini (read in this order)
#if _DEBUG
# define INI_LOC_E "T:\\dashselector.ini"
# define INI_LOC_C "T:\\dashselector.ini"
#else
# define INI_LOC_E "E:\\dashselector.ini"
# define INI_LOC_C "C:\\dashselector.ini"
#endif

#define WSOD				"EMERGENCY"

#define MAX_SECTOIN_NAME_LEN	10

typedef struct _tAppConfigSection {
	const char	*dashboard;		// full path to dashboard to boot
	int			ledColor;		// change LED to ledColor
	int			patcher;		// patch kernel (if possible)
} tAppConfigSection;

typedef struct _tAppConfig {
	tAppConfigSection	section[NumControls];	// 8 buttons + default.
} tAppConfig;

// read ini file
int readIni(tAppConfig *cfg);

// ini handler (see ini.h)
static int iniHandler(void* user, const char* section, const char* name, const char* value);

#endif // _DSINI_H_