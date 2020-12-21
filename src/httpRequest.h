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
 * httpRequest.h
 * 
 * Basic handling of http request.
 * Needs a lot of work to be.. complete but it is sufficient to handle/do what it's currently required to do...
 *
 */
#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include "xbNet.h"
#include "bufReader.h"

namespace http {

	int pfParseHeader(char *line, Header *h);

	struct RequestBody {
		RequestBody(bufReader *rdr, size_t bs) 
			: reader(rdr), bodySize(bs)
		{}

		bufReader	*reader;
		size_t		bodySize;

		int init() {
			return(reader->getBody(bodySize));
		}

		// reads body into buf.
		// call until 0 is returned
		size_t Read(char *buf, size_t bufSize) {
			return reader->readBody(buf, bufSize);
		}
	};

	struct RequestPart {
		RequestPart(bufReader *rdr)
			: reader(rdr), curHeader(0)
		{}

		bufReader	*reader;
		Header		headers[MAX_MULTIPART_HEADERS];
		int			curHeader;
		char		line[MAX_LINE_LEN];

		int parseHeader(char *line) {
			int ret = pfParseHeader(line, &headers[curHeader]);
			if(ret == Http200)
				curHeader++;
			return ret;
		}

		int Read() {
			int ret = reader->readLine(line, MAX_LINE_LEN);
			if(!strncmp(line, "--", MAX_LINE_LEN)) {
				return xMpEOF;
			}
			do {
				ret = reader->readLine(line, MAX_LINE_LEN);
				if(ret != Http200)
					return ret;
				if(line[0] == '\0') 
					return Http200;
				ret = parseHeader(line);
				if(ret != Http200)
					return ret;
			} while(1);

			return Http200;
		}

		int Body(char *buf, size_t bufSize, int *cont) {
			return reader->readMpBody(buf, bufSize, cont);
		}
	};

	struct Request {
		Request(bufReader *reader)
			: reader(reader), lastError(Http200), curHeader(0)
		{}

		int		method;
		char	uri[MAX_URI_LEN];
		char	version[MAX_HEADER_LEN];
		Header	headers[MAX_HEADERS];
		int		lastError;

		// reads http request headers, doesn't read the body
		int Read() {
			// initialize reader
			int sts = reader->init();
			if(sts != Http200)
				return sts;
			// read Request Line
			sts = reader->readLine(line, MAX_LINE_LEN);
			if(sts != Http200)
				return sts;
			sts = parseRequestLine(line);
			if(sts != Http200)
				return sts;
			// read headers
			while(line[0] != '\0') {
				sts = reader->readLine(line, MAX_LINE_LEN);
				if(sts != Http200)
					return sts;
				if(line[0] == '\0') 
					return Http200;
				sts = parseHeader(line);
				if(sts != Http200)
					return sts;
			}
			return Http200;
		}

		// caller owns body and must free/delete it
		RequestBody *Body() {
			size_t bs = bodySize();
			if(bs <= 0) {
				lastError = Http400;
				return NULL;
			}
			RequestBody *body = new RequestBody(reader, bs);
			int ret = body->init();
			if(ret != Http200) {
				lastError = ret;
				return NULL;
			}
			return body;
		}

		// private stuff
		bufReader	*reader;
		int			curHeader;
		char		line[MAX_LINE_LEN];

		int parseRequestLine(char *line) {
			method = validMethod(line);
			if(method == MethodInvalid)
				return Http400;

			char *u = strchr(line, ' ');
			if(!u)
				return Http400;
			char *v = strchr(u+1, ' ');
			if(!v)
				return Http400;

			int len = int(v-u-1);
			if(len > MAX_URI_LEN)
				return Http400;
			memcpy(uri, u+1, len);
			uri[len] = '\0';
			return Http200;
		}

		int parseHeader(char *line) {
			int ret = pfParseHeader(line, &headers[curHeader]);
			if(ret == Http200)
				curHeader++;
			return ret;
		}

		// returns true if the request is multipart
		int IsMultipart() {
			char *boundary = boundaryString();
			if(strlen(boundary) <= 0)
				return 0;
			int ret = bodySize();
			if(ret <= 0) 
				return Http400;
			ret = reader->getMpBody(boundary, ret);
			if(ret != Http200)
				return ret;
			
			return 1;
		}

		// caller must delete part once done
		RequestPart* NextPart() {
			return new RequestPart(reader) ;
		}

		void Close() {
			reader->close();
			delete reader;
		}

		size_t	bodySize() {
			for(int i=0; i<=curHeader; i++) {
				if(stricmp(headers[i].Name, "Content-Length"))
					continue;
				return atoi(headers[i].Value);
			}
			return 0;
		}

		// TODO: this needs rewrite to extracet actual content type and 
		// correctly extract boundary
		char *boundaryString() {
			for(int i=0; i<=curHeader; i++) {
				if(stricmp(headers[i].Name, "Content-Type"))
					continue;
				char *res = strstr(headers[i].Value, "boundary=");
				if(res != NULL) {
					return (res+9);
				}
			}
			return NULL;			
		}

	};

}

#endif // _HTTP_REQUEST_H_