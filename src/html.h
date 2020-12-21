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
 * html.h
 *
 */
#ifndef _HTML_H_
#define _HTML_H_

namespace html {

	const static char *NotFound = 
		"<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">"
		"<html>"
		"<head>"
		"<title>404 Not Found</title>"
		"</head>"
		"<body>"
		"<h2>Not Found</h2>"
		"<p>The requested URL was not found on this server.</p>"
		"</body>"
		"</html>";

	const static char *RequestError = 
		"<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">"
		"<html>"
		"<head>"
		"<title>Invalid Request</title>"
		"</head>"
		"<body>"
		"<p>The server experienced an error processing the request</p>"
		"</body>"
		"</html>";

	const static char *Index = 
		"<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">"
		"<html>"
		"<head>"
		"<title>XBOX Dashboard not found</title>"
		"</head>"
		"<body>"
		"<form enctype='multipart/form-data' action='/upload' method='POST' >"
		"<p>Your XBOX expereinced a problem finding a dashboard.</p>"
		"Choose XBE file to boot from: <br><input type='file' name='file' />"
		"<p><input type='submit' value='Upload & Boot' /></p>"
		"</form>"
		"</body>"
		"</html>";

	const static char *Rebooting = 
		"<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">"
		"<html>"
		"<head>"
		"<title>XBOX rebooting</title>"
		"</head>"
		"<body>"
		"<h2>XBOX Rebooting...</h2>"
		"</body>"
		"</html>";

	const static char *InvalidXBE = 
		"<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">"
		"<html>"
		"<head>"
		"<title>Invalid XBE file</title>"
		"</head>"
		"<body>"
		"<h2>Invalid XBE file/h2>"
		"<p>Looks like the uploaded file is not a valid XBOX executable</p>"
		"</body>"
		"</html>";
}

#endif // _HTML_H_