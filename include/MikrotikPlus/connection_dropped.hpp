#pragma once

#include "exception_base.hpp"

namespace MIKROTIKPLUS {

	class ConnectionDropped: public ExceptionBase {

	public:

		ConnectionDropped(const std::string &message): ExceptionBase(message) { }

	};

}
