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

		APISettings(const std::string& ip, const std::string& user, const std::string& password,
			const int port, const bool secure) : ip(ip), user(user), password(password),
			port(port), secure(secure) { }

		const std::string& getIP() const {
			return this->ip;
		}

		const std::string& getUser() const {
			return this->user;
		}

		const std::string& getPassword() const {
			return this->password;
		}

		const int getPort() const {
			return this->port;
		}

		const bool getSecure() const {
			return this->secure;
		}

	};

}