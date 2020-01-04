# MikrotikPlus
An up-to-date C++ connector which connects to the Mikrotik/RouterOS API and wraps it.

## Table of Contents
* [General info](#general-info)
* [Usage and examples](#useage-and-examples)
* [Building](#building)

## General Info

It works well under Windows, Linux and possibly Mac.
It does not use any dependencies at all.
This supports only versions 6.43 and up of RouterOS.

## Usage And Examples

This is how we establish a connection to a Mikrotik device:
```cpp
#include "mikrotikplus/connector.hpp"

int main() {
	MIKROTIKPLUS::Connector mikrotik_connection = MIKROTIKPLUS::Connector(ip, username, password, port);

}
```

On an unsuccessful login, this throws the `MIKROTIKPLUS::LoginIncorrect` exception. If the target is unavailable, it will throw `MIKROTIKPLUS::NoSocketConnection`. When the latter exception gets thrown, the connector assumes that you provided correct details and that the target is down, so it tries to reconnect to it. (I have to fix this..)
```cpp
#include "mikrotikplus/connector.hpp"
#include "mikrotikplus/exceptions.hpp"

int main() {
	try {
		MIKROTIKPLUS::Connector mikrotik_connection = MIKROTIKPLUS::Connector(ip, username, password, port);
	} catch (MIKROTIKPLUS::LoginException e) {
		std::cerr << "The login details are incorrect";
	}
}
```

We can obtain a mapped version of the sentence.
The keys and values are without the delimiters. For instance, if the API returns this:

```
=key=value
```

Then the map will be this:

```
{key, value}
```

Here is an example:

```cpp
std::unordered_map<std::string, std::string> sentence_map

MIKROTIKPLUS::Sentence read_sentence;

// Read into read_sentence

sentence_map = read_sentence.getMap();
```

Now let's write and read sentences.
You have to read the sentences in a loop or else you will receive only the first one.

```cpp
#include "mikrotikplus/connector.hpp"
#include "mikrotikplus/sentence.hpp"
#include "mikrotikplus/exceptions.hpp"

int main() {
	try {
		MIKROTIKPLUS::Connector mikrotik_connection = MIKROTIKPLUS::Connector(ip, username, password, port);
	} catch (MIKROTIKPLUS::LoginException e) {
		std::cerr << "The login details are incorrect";
	}

	std::unordered_map<std::string, std::string> sentence_map;

	MIKROTIKPLUS::Sentence sentence_to_write;
	MIKROTIKPLUS::Sentence read_sentence;

	sentence_to_write.addWord("/system/package/getall")''
	mikrotik_connection.writeSentence(sentence_to_write);

	do {

		read_sentence = mikrotik.readSentence();

		sentence_map = read_sentence.getMap();

		if (sentence_map.find("version")->second == "6.4") {

			std::cout << "You have the latest version";
		}

	} while (read_sentence.getType() == MIKROTIKPLUS::SENTENCE_TYPES::CONTINUE);

}
```

You may view an example in the form of a full working program below. It parses the snooped data and inserts it into the database.
https://github.com/zx2512/MikrotikSnooperToMySQL

## Building
To build this, you will have to download the source code and navigate to its directory. Then, you will do this:
```
mkdir build
cd build
cmake -G generator ..
```
