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

#ifndef NETMAUMAU_COMMON_SIGNALBLOCKER_H
#define NETMAUMAU_COMMON_SIGNALBLOCKER_H

#include <csignal>

#include "linkercontrol.h"

#define BLOCK_ALL_SIGNALS NetMauMau::Common::SignalBlocker __signal__blocker; \
	_UNUSED(__signal__blocker)

#define BLOCK_SIGNALS(n, p) NetMauMau::Common::SignalBlocker __signal__blocker((n), (p)); \
	_UNUSED(__signal__blocker)

namespace NetMauMau {

namespace Common {

class _EXPORT SignalBlocker {
	DISALLOW_COPY_AND_ASSIGN(SignalBlocker)
public:
	SignalBlocker();
	SignalBlocker(std::size_t numsv, int *ignsv);
	~SignalBlocker();

private:
	// cppcheck-suppress functionStatic
	bool init(std::size_t numsv, int *ignsv);

private:
#if _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _POSIX_SOURCE
	sigset_t m_sigSet;
	sigset_t m_oldSet;
#else
	int m_sigSet;
	int m_oldSet;
#endif

	bool m_ok;
};

}

}

#endif /* NETMAUMAU_COMMON_SIGNALBLOCKER_H */

// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
