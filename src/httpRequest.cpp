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
 */
#include "stdafx.h"

#include <stdio.h>
#include "httpRequest.h"

namespace http {

	int pfParseHeader(char *line, Header *h) {
		char *v = strchr(line, ':') + 1;
		if(!v)
			return Http431;
		int len = int(v-line-1);
		if(len >= MAX_HEADER_LEN-1)
			return Http431;
		memcpy(h->Name, line, len);
		h->Name[len] = '\0';
		int i = 0;
		do {
			while(*v == ' ')
				++v;
		}while(h->Value[i++] = *v++);

		return Http200;
	};


}