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

namespace MikrotikPlus {

	class Connector {

	private:

		APISettings api_settings;

		int sock_fd;

		// Receives and returns data from the socket
		std::string receiveData(const int length, const int flags);

		// Encode the the word length and write it to the sock
		void encodeLength(const int message_length);

		// Decode the word length and return it
		int decodeLength();

	public:

		Connector(const std::string &ip_address, const std::string &username,
			const std::string &password, const int port);

		virtual ~Connector();

		// Attemps to login into the API
		// true is returned on successful login
		bool login();

		// Writes a sentence (multiple words) to the socket
		void write(const Sentence &sentence);

		// Writes a word to the socket
		void write(const std::string &word);

		// Reads a sentence (muktiple words) from the socket
		Sentence readSentence();

		// Reads a word from the socket
		std::string readWord();

		// Attemps to initiate connection to the API
		void connectAPI();

		void closeSocket() const;

		// Returns false if sock_fd is -1, true otherwise
		const bool isSockDescriptorValid() const {
			return this->sock_fd == -1 ? false : true;
		}

		const APISettings &getAPISettings() const {
			return this->api_settings;
		}

	};

}