#include <string>

namespace MIKROTIKPLUS {

	class APISettings {

	private:
		
		std::string ip;
		std::string user;
		std::string password;
		int port;
		bool secure;

	public:

		APISettings(const std::string &ip, const std::string &user, const std::string &password, const int port, const bool secure) {

			this->ip = ip;
			this->user = user;
			this->password = password;
			this->port = port;
			this->secure = secure;

		}

		const std::string& getIP() const {

			return ip;

		}

		const std::string &getUser() const {

			return user;

		}

		const std::string &getPassword() const {

			return password;

		}

		const int getPort() const {

			return port;

		}

		const bool getSecure() const {

			return secure;

		}


	};

}