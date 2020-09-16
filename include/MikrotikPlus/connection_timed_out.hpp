#pragma once

#include "exception_base.hpp"

namespace MikrotikPlus {

	class ConnectionTimedOut : public ExceptionBase {

	public:

		ConnectionTimedOut(const std::string &message): ExceptionBase(message) {}

	};

}
