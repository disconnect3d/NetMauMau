/*
 * Copyright 2015 by Heiko Schäfer <heiko@rangun.de>
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

#ifndef NETMAUMAU_COMMON_MAGIC_H
#define NETMAUMAU_COMMON_MAGIC_H

#if defined(HAVE_CONFIG_H) || defined(IN_IDE_PARSER)
#include "config.h"
#endif

#if defined(HAVE_MAGIC_H) && defined(HAVE_LIBMAGIC)
#include <magic.h>

#ifndef MAGIC_MIME_TYPE
#define MAGIC_MIME_TYPE MAGIC_MIME
#endif
#endif

#include <cstdlib>

#include "linkercontrol.h"

namespace NetMauMau {

namespace Common {

class MimeMagic {
	DISALLOW_COPY_AND_ASSIGN(MimeMagic)
public:
	~MimeMagic() _WCONST;

	static MimeMagic &getInstance();

	// cppcheck-suppress functionStatic
	bool checkMime(const unsigned char *data, std::size_t dataLen, const char *mime) const _WCONST;

private:
	explicit MimeMagic() _WCONST;

private:
#if defined(HAVE_MAGIC_H) && defined(HAVE_LIBMAGIC)
	magic_t m_magic;
#endif
};

}

}

#endif /* NETMAUMAU_COMMON_MAGIC_H */

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
