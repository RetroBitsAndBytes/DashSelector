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
 * httpConn.h
 *
 * Provides basic network initialization and handles underlying connection for httpServer
 *
 *
 */
#ifndef _HTTP_CONN_H_
#define _HTTP_CONN_H_

#include <xtl.h>
#include <stdio.h>

#include "XKUtils.h"
#include "xbNet.h"
#include "xbutil.h"

#include "http.h"
#include "bufReader.h"
#include "netWriter.h"

namespace http {
	struct Server;

	// xconn is httpServer's underlying connection
	struct xconn {
		xconn()
			: listenSock(INVALID_SOCKET), netStatus(NotInitialised), reader(NULL), writer(NULL)  //, handlerFunc(&defaultHttpHandler)
		{}

		// xconn listen socket
		SOCKET listenSock;
			
		// reader and writer
		bufReader *reader;
		netWriter *writer;

		// local address
		XNADDR	localAddress;

		// current network status and last error
		int		netStatus;
		int		lastError;

		// initializes the network
		// repeat calling function if return is 0 and lastError == AddressError
		// returns 1 when network and IP address are ready 
		int NetworkReady() {
			if( !(netStatus & Started) ) {
				HRESULT hr = XBNet_Init( XNET_STARTUP_BYPASS_SECURITY );
				if( FAILED(hr) ) {
					lastError = XBInitError;
					return 0;
				}
				WSADATA wsaData;
				if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
					lastError = WSAInitError;
					return 0;
				}    
				netStatus |= Started;
			}
			if( !(netStatus & HaveAddress) ) {
				DWORD dwStatus = XNetGetTitleXnAddr( &localAddress );
				if(dwStatus & XNET_GET_XNADDR_NONE) {
					lastError = DHCPError;
					return 0;
				}
				if( dwStatus != XNET_GET_XNADDR_PENDING ) {
					netStatus |= HaveAddress;
					return 1;
				}
				else {
					lastError = AddressError;
					return 0;
				}
			}
			return 0;
		}

		// Listens for connections
		// returns 1 on success
		int Listen(unsigned int port) {
			if( !(netStatus & HaveAddress) ) 
				return 0;
			listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if(listenSock == INVALID_SOCKET) {
				lastError = SocketError;
				return 0;
			}

			SOCKADDR_IN sa;
			sa.sin_family = AF_INET;
			sa.sin_addr.s_addr = INADDR_ANY;
			sa.sin_port = htons( port );
			
			if(bind(listenSock, (sockaddr*)( &sa ), sizeof( struct sockaddr )) != 0) {
				Stop();
				lastError = BindError;
				return 0;
			}
			if(listen(listenSock, 1) != 0) {
				Stop();
				lastError = ListenError;
				return 0;
			}
			netStatus |= Listening;
			return 1;
		}

		// Accepts connection and returns netReader
		// caller must close clientSock and free reader and writer!
		int Serve() {
			if(!(netStatus & Listening)) {
				return 0;
			}
			SOCKET clientSock;
			clientSock = accept(listenSock, NULL, NULL);
			if(clientSock == INVALID_SOCKET) {
				lastError = ClientSockError;
				return 0;
			}
			// create netReader for the session
			reader = new bufReader(clientSock, HTTP_BUFFER_SIZE);
			writer = new netWriter(clientSock, HTTP_BUFFER_SIZE);
			return 1;
		}
	
		void Stop() {
			netStatus &= ~Listening;
			if(listenSock != INVALID_SOCKET) {
				shutdown(listenSock, SD_BOTH);
				closesocket(listenSock);
			}
		}

	};
};

#endif //_HTTP_CONN_H_
