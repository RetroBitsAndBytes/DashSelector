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
 * http.h
 *
 */
#ifndef _HTTP_H_
#define _HTTP_H_

#include "html.h"

// http request size 
#define HTTP_BUFFER_SIZE	4096

// request/response limits
#define MAX_URI_LEN					256
#define MAX_HEADERS					32
#define MAX_MULTIPART_HEADERS		4
#define MAX_HEADER_LEN				64
#define MAX_HEADER_VALUE			256
#define MAX_LINE_LEN				((MAX_HEADER_VALUE) + (MAX_HEADER_LEN))

#define ACCEPTED_CHARS	 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-."

namespace http {
	// Network Status
	enum NetworkStatus {
		NotInitialised		= 0,
		Started				= 1 << 0,
		HaveAddress			= 1 << 1,
		Listening			= 1 << 2
	};
	const char* tNetworkStatus(int s);
	
	// Network init errors
	enum NetworkError {
		DHCPError			= 1 << 0,
		AddressError		= 1 << 1,
		XBInitError			= 1 << 2,
		WSAInitError		= 1 << 3,
		SocketError			= 1 << 4,
		ListenError			= 1 << 5,
		BindError			= 1 << 6,
		ClientSockError		= 1 << 7
	};
	const char* tNetworkError(int s);
	
	// http status
	enum Status {
		Http200,		// OK
		Http400,		// Bad Request
		Http404,		// Not Found
		Http405,		// Method Not Allowed
		Http413,		// Payload Too Large
		Http431,		// Request Header Field Too Large
		Http500,		// Internal Server Error
		xMpEOF			// special case for multipart request 
						// singalling no more parts

	};
	const char* tStatus(int s);

	struct Header {
		char Name[MAX_HEADER_LEN];
		char Value[MAX_HEADER_VALUE];
	};

	// not much is currently implemented (and not much more will ever be implemented)
	enum {
		MethodGet,
		MethodPost,
		MethodInvalid
	};
	int validMethod(char *);

	const char* tResponseBody(int s);

}
#endif	// _HTTP_H_