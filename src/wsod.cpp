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
 * wsod.cpp
 *
 * for the courious ones: White Screen Of Death
 * handles "error" ie. the situation where DashSelector can't find a dashboard to boot
 *
 */
#include "stdafx.h"

#include <stdio.h> 
#include "httpRequest.h"
#include "httpServer.h"
#include "wsod.h"


namespace wsod {

	// connStatus can be used to get status of connection
	// connStatus is protected with critical section csConnStatus
	CRITICAL_SECTION csConnStatus;
	typedef struct  {
		int					status;
		http::xconn			*conn;
		char				sStatus[64];
	} connStatus;
	
	// xconn will be initialized in separate thread 
	DWORD tStartNetConn(LPVOID lpParam) {
		connStatus *ns = (connStatus *)lpParam;
		http::xconn *conn = ns->conn;
		while(1) {
			EnterCriticalSection(&csConnStatus);
			conn->NetworkReady();
			ns->status = conn->netStatus;
			if(ns->status & http::HaveAddress) {
				unsigned char *buf = (unsigned char *)&conn->localAddress;
				sprintf(ns->sStatus, "http://%d.%d.%d.%d", buf[0], buf[1], buf[2], buf[3]);
			}
			LeaveCriticalSection(&csConnStatus);
			if(ns->status & http::HaveAddress) 
				break;
			Sleep(50);
		}
		return 0;
	}

	void handleIndex(http::Request *req, http::Response *resp) {
		resp->body = html::Index;
	};

	void rebootCallback(http::Response *) {
		XKUtils::LaunchXBE(WSOD_BOOT_LOCATION, NULL);
	};

	
	void handleUpload(http::Request *req, http::Response *resp) {
		XKUtils::MountDiskE();
		FILE *f = fopen(WSOD_BOOT_LOCATION, "wb");
		size_t total = 0;

		//http::RequestBody* body = req->Body();
		//if(!body) {
		//	;
		//}
		//size_t ret = 0;
		//do {
		//	ret = body->Read(readbuf, HTTP_READ_BUFFER_SIZE);
		//	if(ret > 0) {
		//		total += ret;
		//		fwrite(readbuf, 1, ret, f);
		//	}
		//} while(ret > 0);
		//fclose(f);
		//delete body;
		
		// an example of handling multipart request
		int mp = req->IsMultipart();

		http::RequestPart *part = req->NextPart();

		if(!part)
			return;
		int ret;
		do {
			ret = part->Read();
			if(ret == http::xMpEOF) 
				break;
			int cont = 0;
			do {
				ret = part->Body(readbuf, HTTP_READ_BUFFER_SIZE, &cont);
				total += ret;
				fwrite(readbuf, 1, ret, f);
			} while(cont);
		}while(ret != http::xMpEOF);

		fclose(f);

		delete part;

		resp->body = html::Rebooting;
		resp->callback = &rebootCallback;
	};

	void handleError(int lastErr) {
		XKUtils::SetXBOXLEDStatus(XKUtils::LED_FAST_ORANGE);

		WSoD ws(620, 460, 310, 10);
		ws.InitialiseD3D();

		InitializeCriticalSection(&csConnStatus);

		ws.WriteLine(0, "The XBOX could not load the Dashboard");
		ws.WriteLine(1, errorString[lastErr]);	// error
		ws.WriteLine(2, "");
		ws.WriteLine(3, "");
		ws.WriteLine(4, "Please navigate to the following URL");
		ws.WriteLine(5, "and follow the instructions");
		ws.WriteLine(6, "");
		ws.WriteLine(7, "...waiting for network...");	// URL
		
		// create xconn as we need status 
		http::xconn conn = http::xconn();

		// create server and handlers
		http::Server s(80, &conn);
		s.AddHandler(http::MethodGet, "/", &handleIndex);
		s.AddHandler(http::MethodPost, "/upload", &handleUpload);

		connStatus cStatus;
		cStatus.conn = &conn;

		HANDLE thrd = CreateThread( 
            NULL,
            0,
            (LPTHREAD_START_ROUTINE) tStartNetConn,
            &cStatus,
			0,
            NULL);  

		// check network
		int status = http::NotInitialised;

		ws.Draw();
		while(1) {
			// get connection status
			if(TryEnterCriticalSection(&csConnStatus)) {
				if(cStatus.status & http::HaveAddress) {
					status = cStatus.status;
					ws.WriteLine(7, cStatus.sStatus);
					LeaveCriticalSection(&csConnStatus);
					// render
					ws.Draw();
					break;
				}
			}
		}
		s.ListenAndServe();
	}
};

void WSoD::InitialiseD3D() {
	g_pD3D = Direct3DCreate8(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = 640;
	d3dpp.BackBufferHeight = 480;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
 
    g_pD3D->CreateDevice(0, D3DDEVTYPE_HAL, NULL,
    	D3DCREATE_HARDWARE_VERTEXPROCESSING,
     	&d3dpp, &g_pD3DDevice);
 
    g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pD3DDevice->GetBackBuffer(-1,D3DBACKBUFFER_TYPE_MONO,&g_pBackBuffer);

	XFONT_OpenDefaultFont(&fontPtr);
	fontPtr->SetTextColor(D3DCOLOR_XRGB(0,0,0));
	fontPtr->SetTextAlignment(XFONT_CENTER);
}

void WSoD::Draw() {
	g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	for (int i = 0; i < maxLines; i++) {
		if(buffer[i] != NULL)
			 fontPtr-> TextOut(g_pBackBuffer, buffer[i], -1, xOffset, yOffset + fontHeight + ((i+1)*fontHeight) );
	}
}

void WSoD::WriteLine(int lineNo, LPCSTR str) {
	if(lineNo > maxLines)
		return;
	buffer[lineNo] = new WCHAR[strlen(str)+1];
	ZeroMemory((LPVOID)buffer[lineNo],(int) strlen(str)+1 * sizeof(WCHAR));
    MultiByteToWideChar(CP_ACP,0,str,(int)strlen(str),buffer[lineNo],(int) strlen(str) * sizeof(WCHAR));
}

void WSoD::CleanUp() {
	fontPtr->Release();
	g_pD3DDevice->Release();
	g_pD3D->Release();
}

extern int defaultHttpHandler(http::Server*, http::Request*, http::Response*); 
extern int httpHandler(http::Server*, http::Request*, http::Response*);

