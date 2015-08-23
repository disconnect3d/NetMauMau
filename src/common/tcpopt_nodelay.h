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

#ifndef NETMAUMAU_COMMON_TCPOPT_NODELAY_H
#define NETMAUMAU_COMMON_TCPOPT_NODELAY_H

#include "tcpopt_base.h"

#define TCPOPT_NODELAY(fd) const volatile NetMauMau::Common::TCPOptNodelay __tcp__opt__nodelay__(fd); \
	_UNUSED(__tcp__opt__nodelay__)

namespace NetMauMau {

namespace Common {

class _EXPORT TCPOptNodelay : public TCPOptBase {
	DISALLOW_COPY_AND_ASSIGN(TCPOptNodelay)
public:
	explicit TCPOptNodelay(SOCKET fd) throw();
	virtual ~TCPOptNodelay() throw();
};

}

}

#endif /* NETMAUMAU_COMMON_TCPOPT_NODELAY_H */

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
