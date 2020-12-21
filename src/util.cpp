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
 * util.cpp  
 *
 */
#include "stdafx.h"

#include "XKUtils.h"
#include "util.h"

namespace util {

	int fileExists(const char *name) {
		if(name == NULL)
			return 0;
		struct stat   buffer;   
		return (stat (name, &buffer) == 0);
	}

	void winPathToXPath(char *winPath, char *xPath) {
		char sDrive=tolower(winPath[0]);
		switch (sDrive)
		{
			case 'c':
				strcpy(xPath,DeviceC);
			break;
			case 'd':
				strcpy(xPath,CdRom);
			break;
			case 'e':
				strcpy(xPath,DeviceE);
			break;
			case 'f':
				strcpy(xPath,DeviceF);
			break;
			case 'g':
				strcpy(xPath,DeviceG);
			break;
			case 'h':
				strcpy(xPath,DeviceH);
			break;
			case 'x':
				strcpy(xPath,DeviceX);
			break;
			case 'y':
				strcpy(xPath,DeviceY);
			break;
			case 'z':
				strcpy(xPath,DeviceZ);
			break;
			case 's':
				strcpy(xPath,DeviceS);
			break;
			default:
				return;
			break;
		}
		lstrcat(xPath,&winPath[2]);
	}

}
