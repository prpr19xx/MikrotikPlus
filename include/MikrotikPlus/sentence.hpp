#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include "enums.hpp"

namespace MIKROTIKPLUS {

	class Sentence {

	private:

		SENTENCE_TYPES type;

		char delimiter;

		std::vector<std::string> words;

		std::pair<std::string, std::string> tokenize(const std::string &target);

	public:

		Sentence() {

			this->delimiter = '=';

		}

		const std::string& operator[](size_t index) const {
			return words[index];
		}

		const std::string& at(size_t index) const {
			return words.at(index);
		}

		std::unordered_map<std::string, std::string> getMap();

		SENTENCE_TYPES getType() const {
			return type;
		}

		const size_t getLength() const {
			return words.size();
		}

		void setType(SENTENCE_TYPES type) {
			this->type = type;
		}

		void addWord(const std::string &word_to_add) {
			words.push_back(word_to_add);
		}

		friend std::ostream &operator<<(std::ostream &out, const Sentence &sentence);

	};

}