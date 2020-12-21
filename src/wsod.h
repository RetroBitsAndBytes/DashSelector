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
 * wsod.h  
 * for the courious ones: White Screen Of Death
 *
 */
#ifndef _WSOD_H_
#define _WSOD_H_

#include <xtl.h>
#include <xfont.h>

#include "XKUtils.h"
#include "util.h"

#define TITLE_ID "000AA000"
//#define TITLE_NAME "DASHSEL"
#define TITLE_DIR "E:\\TDATA\\" TITLE_ID
#define WSOD_BOOT_LOCATION TITLE_DIR "\\wsodboot.xbe"


#define HTTP_READ_BUFFER_SIZE	16*1024
static char readbuf[HTTP_READ_BUFFER_SIZE+1];


namespace http {
	struct xconn;
}

namespace wsod {
	void handleError(int err);
};

DWORD	tStartNetConn(LPVOID lpParam);

struct WSoD {
	WSoD(long width, long height, long xOffset, long yOffset)
		: width(width), height(height), xOffset(xOffset), yOffset(yOffset),
		g_pD3D(NULL), g_pD3DDevice(NULL), status(NULL), fontPtr(NULL)
	{
		fontWidth = 12;
		fontHeight = 22;
		maxLines = ((height-fontHeight) / fontHeight)-1;
		buffer = new LPWSTR[maxLines];
		for(int i=0; i<maxLines; i++)
			buffer[i] = NULL;
		//ZeroMemory (buffer, maxLines * sizeof(LPCWSTR));
	};

public:
	int		status;		// network status 

	void	InitialiseD3D();
	void	CleanUp();
	void	Draw();
	void	WriteLine(int, LPCSTR);
	void	RedrawStatus(const char*, int);

private:
	long	width, height, xOffset, yOffset;
	long	fontHeight, fontWidth;
	int		maxLines;
	LPWSTR	*buffer;
	XFONT	*fontPtr;

	LPDIRECT3D8			g_pD3D;           
	LPDIRECT3DDEVICE8	g_pD3DDevice; 
	LPDIRECT3DSURFACE8  g_pBackBuffer;
};
#endif // _WSOD_H_
