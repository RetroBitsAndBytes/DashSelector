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
 * netWriter.h
 *
 */
#ifndef _NET_WRITER_H_
#define _NET_WRITER_H_

#include <xtl.h>

namespace http {
	
	enum {
		WriterOK	= 0,
		WriterError = 1
	};

	struct netWriter {
		netWriter(SOCKET s, unsigned int bs)
			: sock(s), bufSize(bs), bufPos(0), lastError(WriterOK), sendBuf((char *)malloc(bs))
		{}

		int write(const char *buf, unsigned int len) {
			int numSent = len;
			while(len > 0) {
				unsigned int as = bufSize - bufPos;
				if(as == 0) {
					int ret = flush();
					if(lastError != WriterOK)
						return 0;
					as = bufSize;
				}
				if(len > as) {
					memcpy(sendBuf+bufPos, buf, as);
					buf += as;
					len -= as;
					bufPos += as;
				} else {
					memcpy(sendBuf+bufPos, buf, len);
					bufPos += len;
					len = 0;
				}
			}
			return numSent;
		}

		int flush() {
			int ret = 0;
			if(bufPos > 0)
				ret = send(sock, sendBuf, bufPos, 0);
			if(ret == SOCKET_ERROR) {
				lastError = WriterError;
				ret = 0;
			}
			bufPos = (unsigned int)0;
			return ret;
		}

		void close() {
			flush();
			shutdown(sock, SD_BOTH);
			closesocket(sock);
		}

		SOCKET			sock;
		unsigned int	bufSize;
		unsigned int	bufPos;
		char			*sendBuf;
		int				lastError;
	};

}

#endif // _NET_WRITER_H_

