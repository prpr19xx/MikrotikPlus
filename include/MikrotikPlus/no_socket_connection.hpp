#pragma once

#include "exception_base.hpp"

namespace MIKROTIKPLUS {

	class NoSocketConnection: public ExceptionBase {

	public:

		NoSocketConnection(const std::string &message): ExceptionBase(message) { }

	};

}
