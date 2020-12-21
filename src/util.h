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
 * util.h
 *
 */
#ifndef _UTIL_H_
#define _UTIL_H_

#include <sys\stat.h>

// errors
enum {
	errNoError,				// no errors
	errIniRead,				// ini file open/read error
	errIniParse,			// ini parsing error
	errGamepad,				// gamepad problem
	errNoDashboard,			// dashboard defined in ini missing
	errEmergencySelected
};

static const char *errorString[] = {
	"",
	"[ Error opening/reading ini file ]",
	"[ Error parsing ini file ]",
	"[ Error detecting/initializing gamepad ]",
	"[ Dashboard not found ]",
	"[ Emergency Button selected ]"
};

namespace util {

	int fileExists(const char *name);
	void winPathToXPath(char *winPath, char *xPath);

}

#endif // _UTIL_H_