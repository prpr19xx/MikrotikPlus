#include "MikrotikPlus/connector.hpp"

namespace MIKROTIKPLUS {

	Connector::Connector(const std::string& ip_address, const std::string& username,
		const std::string& password, const int port) : api_settings(ip_address, username, password, port, true),
		sock_fd(-1337) {

#if defined _WIN32

		/* Initialize winsock */
		WORD versionWanted = MAKEWORD(2, 2);
		WSADATA wsaData;
		int wsa_return = WSAStartup(versionWanted, &wsaData);

		if (wsa_return != 0) {

			std::cerr << "Winsock failed to initialize itself" << "\n";
			std::cerr << "Winsock error code: " << wsa_return << "\n";
			std::cerr << "Terminating the program." << "\n";

			exit(-1);

		}

#endif

	}

	Connector::~Connector() {

		this->closeSocket();

	}

	void Connector::connectAndLogin() {

		connectAPI();

		if (sock_fd != -1) {

			bool loginResult = login();

			if (!loginResult) {

				this->closeSocket();

				throw LoginIncorrect("Invalid login details");

			}

		}
		else {

			throw NoSocketConnection("Failed to connect to " + this->api_settings.getIP() + ":" + std::to_string(this->api_settings.getPort()) + "\n");

		}

	}

	void Connector::connectAPI() {

		struct sockaddr_in address;
		int connect_result = 0;
		int address_size = 0;

		sock_fd = socket(AF_INET, SOCK_STREAM, 0);

		address.sin_family = AF_INET;
		address.sin_addr.s_addr = inet_addr(this->api_settings.getIP().c_str());

		address.sin_port = htons(this->api_settings.getPort());
		address_size = sizeof(address);

		connect_result = connect(sock_fd, (struct sockaddr*) & address, address_size);

		if (connect_result == -1) {

			//std::cerr << "Connection problem" << "\n";

			this->closeSocket();

			sock_fd = -1;

		}

	}

	bool Connector::login() {

		Sentence write_sentence;
		Sentence read_sentence;

		write_sentence.addWord("/login");
		write_sentence.addWord("=name=" + this->api_settings.getUser());
		write_sentence.addWord("=password=" + this->api_settings.getPassword());

		writeSentence(write_sentence);

		read_sentence = readSentence();

		if (read_sentence.getType() != SENTENCE_TYPES::DONE) {

			//std::cerr << "Error" << "\n";

			return false;

		}

		std::unordered_map<std::string, std::string> sentence_map;

		sentence_map = read_sentence.getMap();

		if (sentence_map.find("ret") != sentence_map.end()) {

			write_sentence.clear();

			write_sentence.addWord("/login");
			write_sentence.addWord("=name=" + this->api_settings.getUser());

			std::vector<char> bytes = UTIL::hexToBytes(sentence_map["ret"]);

			MD5 md5;

			char null_character[1] = { 0 };

			md5.update(null_character, 1);
			md5.update(this->api_settings.getPassword().c_str(), this->api_settings.getPassword().size());
			md5.update(bytes.data(), bytes.size());
			md5.finalize();

			write_sentence.addWord("=response=00" + md5.hexdigest());

			writeSentence(write_sentence);

			read_sentence = readSentence();

			if (read_sentence.getType() != SENTENCE_TYPES::DONE) {

				return false;

			}

		}

		return true;

	}

	void Connector::encodeLength(int message_length) {

		std::array<char, 5> encoded_length_data;

		// 1 Byte
		if (message_length < 0x80) {

			encoded_length_data[0] = message_length;

			send(this->sock_fd, encoded_length_data.data(), 1, 0);

			// 2 Bytes
		}
		else if (message_length < 0x4000) {

			encoded_length_data[0] = (message_length >> 8) | 0x80;
			encoded_length_data[1] = message_length;

			send(this->sock_fd, encoded_length_data.data(), 2, 0);

			// 3 Bytes
		}
		else if (message_length < 0x200000) {

			encoded_length_data[0] = (message_length >> 16) | 0xC0;
			encoded_length_data[1] = message_length >> 8;
			encoded_length_data[2] = message_length;

			send(this->sock_fd, encoded_length_data.data(), 3, 0);

			// 4 Bytes
		}
		else if (message_length < 0x10000000) {

			encoded_length_data[0] = (message_length >> 24) | 0xE0;
			encoded_length_data[1] = message_length >> 16;
			encoded_length_data[2] = message_length >> 8;
			encoded_length_data[3] = message_length;

			send(this->sock_fd, encoded_length_data.data(), 4, 0);

			// 5 Bytes
		}
		else {

			encoded_length_data[0] = 0xF0;
			encoded_length_data[1] = message_length >> 24;
			encoded_length_data[2] = message_length >> 16;
			encoded_length_data[3] = message_length >> 8;
			encoded_length_data[4] = message_length;

			send(this->sock_fd, encoded_length_data.data(), 5, 0);

		}

	}

	void Connector::writeWord(const std::string& word) {

		encodeLength(word.length());

		send(this->sock_fd, word.c_str(), word.length(), 0);

	}

	void Connector::writeSentence(const Sentence& write_sentence) {

		for (int i = 0; i < write_sentence.getLength(); ++i) {

			writeWord(write_sentence[i]);

		}

		// Terminate the sentence
		writeWord("");

	}

	int Connector::decodeLength() {

		std::array<char, 4> read_length;

		read_length[0] = (receiveData(1, 0)[0]);

		if ((*read_length.data() & 0xC0) == 0x80) {

			*read_length.data() &= ~0xC0;
			*read_length.data() <<= 8;
			*read_length.data() += (receiveData(1, 0)[0]);

		}
		else if ((*read_length.data() & 0xE0) == 0xC0) {

			*read_length.data() &= ~0xE0;
			*read_length.data() <<= 8;
			*read_length.data() += (receiveData(1, 0)[0]);
			*read_length.data() <<= 8;
			*read_length.data() += (receiveData(1, 0)[0]);

		}
		else if ((*read_length.data() & 0xF0) == 0xE0) {

			*read_length.data() &= ~0xF0;
			*read_length.data() <<= 8;
			*read_length.data() += (receiveData(1, 0)[0]);
			*read_length.data() <<= 8;
			*read_length.data() += (receiveData(1, 0)[0]);
			*read_length.data() <<= 8;
			*read_length.data() += (receiveData(1, 0)[0]);

		}
		else if ((*read_length.data() & 0xF8) == 0xF0) {

			*read_length.data() += (receiveData(1, 0)[0]);
			*read_length.data() <<= 8;
			*read_length.data() += (receiveData(1, 0)[0]);
			*read_length.data() <<= 8;
			*read_length.data() += (receiveData(1, 0)[0]);
			*read_length.data() <<= 8;
			*read_length.data() += (receiveData(1, 0)[0]);

		}

		return *read_length.data();

	}

	std::string Connector::readWord() {

		std::string word;

		const int message_length = decodeLength();

		word = receiveData(message_length, 0);

		return word;

	}

	Sentence Connector::readSentence() {

		Sentence sentence;

		std::string word = readWord();

		while (!word.empty()) {

			sentence.addWord(word);

			if (word.at(0) != '!') {

				// Do nothing, skip the other comparisons

			}
			else if (word.find("!done") != std::string::npos) {

				sentence.setType(SENTENCE_TYPES::DONE);

			}
			else if (word.find("!trap") != std::string::npos) {

				sentence.setType(SENTENCE_TYPES::TRAP);

			}
			else if (word.find("!fatal") != std::string::npos) {

				sentence.setType(SENTENCE_TYPES::FATAL);

			}
			else {

				sentence.setType(SENTENCE_TYPES::CONTINUE);

			}

			word = readWord();

		}

		return sentence;

	}

	std::string Connector::receiveData(const int length, const int flags) {

		int to_read = length;

		std::string buffer;

		if (length > 0) {

			buffer.resize(length);

		}

		while (to_read > 0) {

			int received = recv(this->sock_fd, buffer.data(), to_read, flags);

			// Something went wrong. The connection is probably closed, so we will try to reconnect
			// Ideally, this should never happen but sometimes it does when the application gets halted for too long
			if (received <= 0) {

				/*std::cerr << "Failed to read" << "\n";
				std::cerr << "recv returned: " << received << " " << WSAGetLastError() << "\n";*/

				this->closeSocket();

				throw ConnectionTimedOut("The connection has been timed out whilst reading data");

			}

			to_read -= received;

		}

		return buffer;

	}

	void Connector::closeSocket() const {

#if defined _WIN32

		closesocket(this->sock_fd);

#elif defined __unix__ || defined __APPLE__

		close(this->sock_fd);

#endif

	}

}