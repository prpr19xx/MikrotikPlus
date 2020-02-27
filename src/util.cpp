#include "MikrotikPlus/util.hpp"

namespace MIKROTIKPLUS {

	namespace UTIL {

        std::vector<char> hexToBytes(const std::string& hex) {

            std::vector<char> bytes;

            for (int i = 0; i < hex.length(); i += 2) {

                std::string byteString = hex.substr(i, 2);

                char byte = (char)strtol(byteString.c_str(), NULL, 16);

                bytes.push_back(byte);
            }

            return bytes;
        }

	}

}