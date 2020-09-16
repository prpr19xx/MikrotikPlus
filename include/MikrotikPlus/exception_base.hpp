#pragma once

#include <exception>
#include <string>

namespace MikrotikPlus {

	class ExceptionBase : public std::exception {
	private:

		std::string message;

	public:

		ExceptionBase(const std::string &message): message(message) {}

		virtual const char *what() const noexcept override {
			return this->message.c_str();
		}

	};

}
