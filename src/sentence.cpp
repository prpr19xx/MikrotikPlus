#include "MikrotikPlus/sentence.hpp"

namespace MikrotikPlus {

	std::unordered_map<std::string, std::string> Sentence::getMap() {

		std::unordered_map<std::string, std::string> sentence_map;

		int delimiter_count;

		for (auto it = this->words.begin(); it != this->words.end(); ++it) {

			delimiter_count = std::count((*it).begin(), (*it).end(), this->delimiter);

			// If the first character of the word is not a delimiter, then ignore it as it doesn't need to be parsed
			if (((*it)[0] == this->delimiter) && (delimiter_count > 1)) {

				sentence_map.insert(tokenize(*it));

			} else if (((*it)[0] == this->delimiter) && (delimiter_count == 1)) {

				sentence_map.insert({ (*it), "" });

			}

		}

		return sentence_map;

	}

	std::pair<std::string, std::string> Sentence::tokenize(const std::string &target) {

		size_t pos = 0;

		std::pair<std::string, std::string> pair;

		while ((pos = target.find(this->delimiter, 1)) != std::string::npos) {

			return { target.substr(1, (pos - 1)), target.substr((pos + 1), target.size()) };

		}

	}

	std::ostream &operator<<(std::ostream &out, const Sentence &sentence) {

		for (auto it = sentence.words.begin(); it != sentence.words.end(); ++it) {
			out << *it << "\n";
		}

		return out;

	};

}