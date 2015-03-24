/*
 * Copyright 2014-2015 by Heiko Schäfer <heiko@rangun.de>
 *
 * This file is part of NetMauMau.
 *
 * NetMauMau is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * NetMauMau is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with NetMauMau.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string>

#include <cstring>
#include <cerrno>

#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#endif

#include "errorstring.h"

#ifdef _WIN32
namespace {
TCHAR buffer[1024];
}
#endif

const char *NetMauMau::Common::errorString() {
#ifndef _WIN32
	return errorString(errno);
#else
	return errorString(WSAGetLastError());
#endif
}

const char *NetMauMau::Common::errorString(int errnum) {
#ifndef _WIN32
	return std::strerror(errnum);
#else

	if(!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL,
					  errnum, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, sizeof(buffer),
					  NULL)) {
		return std::strerror(errnum);
	}

	return buffer;

#endif
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
