#pragma once

#include "exception_base.hpp"

namespace MIKROTIKPLUS {

	class LoginIncorrect : public ExceptionBase {

	public:

		LoginIncorrect(const std::string& message) : ExceptionBase(message) { }

	};

}
