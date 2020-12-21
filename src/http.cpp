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
 */
#include "stdafx.h"
#include "http.h"

namespace http {

	const char* tNetworkStatus(int s){
		switch(s) {
			case NotInitialised:
				return "Network not initialised";
			case Started:
				return "Network Started";
			case HaveAddress:
				return "Have IP address";
			case Listening:
				return "Listening";
			default:
				return "Unknown";
		}
	}
	
	const char* tNetworkError(int s){
		switch(s) {
			case DHCPError:
				return "DHCP error";
			case AddressError:
				return "Address error";
			case XBInitError:
				return "XBInit error";
			case WSAInitError:
				return "WSAInit error";
			case SocketError:
				return "Socket error";
			case ListenError:
				return "Listen error";
			case BindError:
				return "Bind error";
			case ClientSockError:
				return "Client Socket error";
			default:
				return "Unknown";
		}
	}

	const char* tStatus(int s) {
		switch(s) {
			case Http200:
				return "HTTP/1.1 200 OK\r\n";
			case Http400:
				return "HTTP/1.1 400 Bad Request\r\n";
			case Http404:
				return "HTTP/1.1 404 Not Found\r\n";
			case Http405:
				return "HTTP/1.1 405 Method Not Allowed\r\n";
			case Http413:
				return "HTTP/1.1 413 Payload Too Large\r\n";
			case Http431:
				return "HTTP/1.1 431 Request Header Fields Too Large\r\n";
			case Http500:
				return "HTTP/1.1 500 Internal Server Error\r\n";
			default:
				return "Unknown";
		}
	}

	const char* tResponseBody(int s) {
		switch(s) {
			case Http400:
			case Http405:
			case Http413:
			case Http431:
			case Http500:
				return html::RequestError;
			case Http404:
				return html::NotFound;
			default:
				return html::RequestError;
		}
	}

	int validMethod(char *str) {
		if(!strnicmp("GET", str, 3))
			return MethodGet;
		if(!strnicmp("POST", str, 4))
			return MethodPost;

		return MethodInvalid;
	}
}