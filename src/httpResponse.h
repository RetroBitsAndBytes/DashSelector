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
 * httpResponse.h
 *
 */
#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_

#include "httpRequest.h"
#include "html.h"

namespace http {

	struct Response;

	// responseCallback will be be called right after the resoonse is sent to the client
	typedef void (*responseCallback)(Response *);

	struct Response {
		Response(netWriter *w, Request *r)
			: writer(w), req(r), numHeaders(0), body(NULL), callback(NULL), status(NULL)
		{}

		// write http response
		int Write(int s) {
			status = tStatus(s);
			if(s != Http200)
				body = tResponseBody(s);
			writeResponse();					

			return 1;
		}

		void Close() {
			writer->close();
			if(callback != NULL) 
				callback(this);
			delete writer;
		}

		void addHeader(Header *header) {
			// TODO:
			// I should probably report overflow...
			if(numHeaders<MAX_HEADERS)
				headers[numHeaders++] = header;
		}

		int writeResponse() {
			int ret;
			// write status
			ret = writer->write(status, strlen(status));
			if(ret == SOCKET_ERROR)
				return ret;
			
			// TODO:
			//	callers should really handle content-type
			//	Connection: close should also be added as a header...
			const char *qq = "Content-Type: text/html; charset=utf-8\r\nConnection: close\r\n";
			ret = writer->write(qq, strlen(qq));
			if(ret == SOCKET_ERROR)
				return ret;

			for(int i=0; i<numHeaders; i++) {
				ret = writer->write(headers[i]->Name, strlen(headers[i]->Name));
				if(ret == SOCKET_ERROR)
					return ret;
				ret = writer->write(": ", 2);
				if(ret == SOCKET_ERROR)
					return ret;
				ret = writer->write(headers[i]->Value, strlen(headers[i]->Value));
				if(ret == SOCKET_ERROR)
					return ret;
			}
			// write body
			if(body != NULL) {
				const char *cls = "Content-Length: ";
				ret = writer->write(cls, strlen(cls));
				if(ret == SOCKET_ERROR)
					return ret;
				char cl[16];
				itoa(strlen(body), cl, 10);
				ret = writer->write((const char *)cl, strlen(cl));
				if(ret == SOCKET_ERROR)
					return ret;
				ret = writer->write("\r\n\r\n", 4);
				if(ret == SOCKET_ERROR)
					return ret;
				ret = writer->write(body, strlen(body));
				if(ret == SOCKET_ERROR)
					return ret;
			}
			return 1;
		}

		netWriter *writer;
		// optional callback
		responseCallback	callback;
		// body needs to be set by handler
		const char			*body;

		Request		*req;
		const char	*status;

		Header		*headers[MAX_HEADERS];
		int			numHeaders;
	};
}

#endif // _HTTP_RESPONSE_H_