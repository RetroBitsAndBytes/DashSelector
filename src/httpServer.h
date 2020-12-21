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
 * httpServer.h
 *
 * This is a (very) simple/naive implementation of http server.
 * Do not expect it to be full implementation or even RFC compliant in any way.
 * It is neither elegant nor fast but it does the job and I don't think Google will
 * host their services on OG XBOXes anytime soon.
 * This particular implementation is single threaded and blocking so probably not suuitable 
 * for any _real_ implementations but does the job of uploading a single file (although multipart 
 * http responses are also implemented).
 * Better/multithreaded implenetation in "under construction" but do not hold your breath as it may never 
 * be available...
 *
 */
#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include <string>
#include <vector>
#include <map>

#include "httpConn.h"
#include "httpRequest.h"
#include "httpResponse.h"

namespace http {

	// httpHandler is what httpServer users/callers use to handle request
	// these handlers can be added using server's AddHandler function
	typedef void (httpHandler)(Request*, Response*);

	// helper to set handler
	Status httpServerHandler(Server *s, Request *req, Response *resp);

	// http server
	struct Server {
		// port is TCP port the server will listen 
		// conn is the underlying xconn. The server will create xconn if conn = NULL
		// not providing xconn is OK in most cases
		Server(int port, xconn *conn) {
			this->port = port;
			this->conn = conn;
			if(conn == NULL) {
				conn = &xconn();
			}
			memset(tLocalAddress, 0, 16);
		}

		// port the server will listen on
		int port;
		// xconn
		xconn *conn;

		// local address (when available)
		char	tLocalAddress[16];

		// handlers stored as map<uri, map<method, handlerFunc*>>
		std::map< std::string, std::map< int, httpHandler* > > handlers;	
		// handlers can be added with AddHandler
		void AddHandler(int method, std::string uri, httpHandler* h) {
			std::map< int, httpHandler* > uh;
			uh[method] = h;
			handlers[uri] = uh;
		}

		// listen and serve starts http server 
		// returns last error on error
		int ListenAndServe() {
			if(!conn->Listen(port))
				return conn->lastError;
			sprintf(tLocalAddress, "%d.%d.%d.%d", 
				conn->localAddress.ina.S_un.S_un_b.s_b1, 
				conn->localAddress.ina.S_un.S_un_b.s_b2, 
				conn->localAddress.ina.S_un.S_un_b.s_b3, 
				conn->localAddress.ina.S_un.S_un_b.s_b4);
			while(1) {
				if(!conn->Serve())
					return conn->lastError;

				bufReader *reader = conn->reader;
				if(reader == NULL)
					return conn->lastError;
				netWriter *writer = conn->writer;
				if(reader == NULL)
					return conn->lastError;

				Request req = Request(reader);
				Response resp = Response(writer, &req);

				int ret = req.Read();
				if(ret == Http200)	// handle request
					ret = httpServerHandler(this, &req, &resp);
				resp.Write(ret);

				req.Close();
				resp.Close();
			}
			// not expecting to ever reach this place...
			return conn->lastError;
		}

		Status handleHttp(Request *req, Response *resp) {
			if(handlers.find(req->uri) == handlers.end())
				// not found
				return Http404;
			std::map< int, httpHandler *> uh = handlers[req->uri];
			if(uh.find(req->method) == uh.end())
				return Http404;
			httpHandler *h = uh[req->method];
			h(req, resp);

			return Http200;
		}

	};

	// helper to set handler
	Status httpServerHandler(Server *s, Request *req, Response *resp) {
		if(s == NULL)
			return Http500;
		return s->handleHttp(req, resp);
	};

};
#endif // _HTTP_SERVER_H_