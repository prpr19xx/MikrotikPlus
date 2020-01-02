#pragma once
#pragma warning(disable:4996)

#include <iostream>
#include <memory>
#include <array>
#include <bitset>
#include <cstddef>

#if defined _WIN32

#include <Ws2tcpip.h>
#include <windows.h>

#elif defined __unix__ || defined __APPLE__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#endif

#include <cstring>
#include <vector>
#include <string>
#include "exceptions.hpp"
#include "enums.hpp"
#include "sentence.hpp"
#include "api_settings.hpp"

namespace MIKROTIKPLUS {

	class Connector {

	private:
		
		APISettings api_settings;

		int sock_fd;

		void connectAPI();
		void disconnectAPI() const;

		int sendSocket(const char *data, const size_t length, const int flags) const;
		std::string receiveSocket(const size_t length, const int flags);
		void closeSocket() const;

		void writeLength(const int message_length);
		int readLength();
		void writeWord(const std::string &str_word);
		std::string readWord();

	public:

		Connector(const std::string &ip_address, const std::string &username,
					const std::string &password, const int port);
		~Connector();

		bool login();

		void writeSentence(const Sentence &write_sentence);
		Sentence readSentence();

		void connectAndLogin();

	};

}