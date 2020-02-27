#pragma once
#pragma warning(disable:4996)

#include <iostream>
#include <memory>
#include <array>
#include <bitset>
#include <cstddef>

#if defined _WIN32

// Without this definition, some things might get messed up for some code/libraries

#define NOMINMAX

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

#include "MikrotikPlus/md5.hpp"

#include "MikrotikPlus/exceptions.hpp"
#include "MikrotikPlus/enums.hpp"
#include "MikrotikPlus/sentence.hpp"
#include "MikrotikPlus/api_settings.hpp"
#include "MikrotikPlus/util.hpp"

namespace MIKROTIKPLUS {

	class Connector {

	private:
		
		APISettings api_settings;

		int sock_fd;

		
		void connectAPI();

		//void disconnectAPI() const;


		int sendSocket(const char *data, const int length, const int flags) const;

		std::string receiveSocket(const int length, const int flags);


		void writeLength(const int message_length);

		int readLength();

		void writeWord(const std::string &str_word);

		std::string readWord();

	public:

		Connector(const std::string &ip_address, const std::string &username,
					const std::string &password, const int port);

		virtual ~Connector();

		bool login();

		void writeSentence(const Sentence &write_sentence);
		
		Sentence readSentence();

		void connectAndLogin();

		const APISettings& getAPISettings() const {
			return this->api_settings;
		}

		void closeSocket() const;

	};

}