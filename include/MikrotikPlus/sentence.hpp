#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include "enums.hpp"

namespace MIKROTIKPLUS {

	class Sentence {

	private:

		SENTENCE_TYPES type;

		char delimiter;

		std::vector<std::string> words;

		std::pair<std::string, std::string> tokenize(const std::string& target);

	public:

		Sentence() : delimiter('='), type(SENTENCE_TYPES::NONE) { }

		const std::string& operator[](size_t index) const {
			return this->words[index];
		}

		const std::string& at(size_t index) const {
			return this->words.at(index);
		}

		std::unordered_map<std::string, std::string> getMap();

		SENTENCE_TYPES getType() const {
			return this->type;
		}

		const size_t getLength() const {
			return this->words.size();
		}

		void setType(const SENTENCE_TYPES type) {
			this->type = type;
		}

		void addWord(const std::string& word_to_add) {
			this->words.push_back(word_to_add);
		}

		void clear() {
			this->words.clear();
		}

		friend std::ostream& operator<<(std::ostream& out, const Sentence& sentence);

	};

}