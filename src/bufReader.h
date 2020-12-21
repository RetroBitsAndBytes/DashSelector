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
 * bufReader.h
 *
 * Provides basic buffered reader.
 *
 *
 */
#ifndef _BUF_READER_H_
#define _BUF_READER_H_

#include "http.h"

#ifdef _MSC_VER 
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

namespace http {

	struct bufReader {
		bufReader(SOCKET s, size_t bs)
			: sock(s), bufSize(bs), buf((char *)malloc(bs+MAX_HEADER_LEN)),
			boundarySize(MAX_HEADER_LEN), readerPos(0), bytesRead(0),
			totalSize(bs+MAX_HEADER_LEN), n(0), boundary((char *)malloc(MAX_HEADER_LEN))
		{}

		SOCKET sock;

		// buffer
		char *	buf;
		// size of the buffer to allocate
		size_t	bufSize;
		// additional buffer for boundary string
		size_t	boundarySize;
		char    *boundary;
		// total size = bufSize + boundarySize
		size_t	totalSize;
		// current position of next character to be sent to caller
		size_t	readerPos;
		// number of bytes in buffer
		size_t	n;
		// number of bytes read
		// this only applies to body part of the request
		size_t bytesRead;
		size_t bodySize;

		int init() {
			n = 0;
			return fill(0);
		}

		int fill(size_t pos) {
			size_t r = recv(sock, buf+pos, totalSize-pos, 0);
			if(r < 0) {
				return Http500;
			}
			bytesRead += r;
			n += r;
			return Http200;
		}

		// refills buffer 
		int shiftAndRefill() {
			// shift contents if needed
			if(readerPos < n) {
				memmove(buf, buf + readerPos, n);
			}
			n -= readerPos;
			readerPos = 0;
			// read if needed
			if(bytesRead < bodySize ) {
				return(fill(n));
			}
			return Http200;
		}

		int readLine(char *line, size_t maxLen) {
			size_t i = 0;
			while(buf[readerPos] != '\r' && readerPos < n) {
				line[i++] = buf[readerPos++];
				if(readerPos == bufSize)
					return Http431;
				if(i > maxLen-1 || readerPos > n)
					return Http431;
			}
			line[i] = '\0';
			// skip /r/n
			readerPos += 2;
			return Http200;
		}

		// must be called before readBody
		int getBody(size_t bs) {
			bytesRead = n - readerPos;
			bodySize = bs;
			// shift to the beginning of the buffer
			return shiftAndRefill();
		}

		int readBody(char *bbuf, size_t blen) {
			size_t i = 0, br = 0;
			while(readerPos < n && readerPos < bufSize && i < blen) {
				bbuf[i++] = buf[readerPos++];
				br++;
			}
			if(br < blen) {
				shiftAndRefill();
			}
			return br;
		}

		// Multipart request
		// call getMpBody()
		// readLine. If line is empty - continue, if line == "--" - end of multipart request
		// readMpBody until continueBody == 0

		// goes to the beginning of first part (ie. skipping 1st boundary)
		int getMpBody(char *bndry, size_t bs) {
			strcpy(boundary, "--");
			strncpy(boundary+2, bndry, MAX_HEADER_LEN-2);
			boundarySize = strlen(boundary);
			bytesRead = n - readerPos;
			readerPos += boundarySize;  // +2 // /r/n
			bodySize = bs;
			// shift to the beginning of the buffer
			shiftAndRefill();        
			return Http200;
		}

		int readMpBody(char *bbuf, size_t blen, int *continueBody) {
			size_t i = 0, br = 0;
			*continueBody = 1;
			while(readerPos < n && readerPos < bufSize && i < blen) {
				if(buf[readerPos] == '\r') {    // potential boundary
					if(!strncmp(buf+readerPos+2, boundary, boundarySize)) {
						// skip 
						readerPos += boundarySize + 2; 
						bbuf[i] = '\0';
						*continueBody = 0;
						break;
					}
				}
				bbuf[i++] = buf[readerPos++];
				br++;
			}
			if(br < blen) {
				shiftAndRefill();
			}
			return br;
		}

		void close() {
			free(buf);
			free(boundary);
		}
	};
}

#endif	// __BUF_READER_H_
