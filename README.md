# MikrotikPlus
An up-to-date C++ API wrapper for Mikrotik/RouterOS

# Table of Contents
* [General info](#general-info)
* [TODO](#todo)
* [Usage and examples](#usage-and-examples)
* [Building](#building)

# General Info

It has been tested under Windows, Linux and should work on Mac.
It does not use dependencies.

# TODO

* TLS support

# Usage And Examples

___
## Establishing a connection, logging in

This example attempts to establish connection and then login to the API

```cpp
#include "MikrotikPlus/connector.hpp"

int main() {

	MikrotikPlus::Connector mikrotik("192.168.1.1", "admin", "default_pass", 8728); // Constructors an API Wrapper object

    mikrotik.connectAPI();

    if (mikrotik.isSockDescriptorValid()) {

        if (mikrotik.login()) {

            std::cout << "Login successful";

        }

    } else {

        std::cout << "Failed to connect to " + mikrotik->getAPISettings().getIP() + ":" + std::to_string(mikrotik->getAPISettings().getPort());

    }

}
```

Constructors do what they are supposed to do; they merely initialize the variables and perform actions necessary for the program to run, nothing else.
You are responsible for ensuring that the connection and login was successful.

___
## Reading and writing sentences

The RouterOS API communicates through sentences, which are simply a collection of words.
They are often in the `key, value` form. If the API sends:
```
=rx-bits-per-second=8531
```
You obtain
```
{rx-bits-per-second, 8531}
```

### Sentence type

The `Sentence` type is located in the `MikrotikPlus/sentence.hpp` header.

It defines a function which tokenizes the words and returns a map of type:
```cpp
std::unordered_map<std::string, std::string>
```

```cpp
#include "MikrotikPlus/connector.hpp"
#include "MikrotikPlus/sentence.hpp"

MikrotikPlus::Sentence write_sentence;
MikrotikPlus::Sentence read_sentence;

std::unordered_map<std::string, std::string> sentence_map;

write_sentence.addWord("/system/package/getall")
write_sentence.addWord("some additional argument")
mikrotik_connection.write(write_sentence);

do {

	read_sentence = mikrotik.readSentence();

	sentence_map = read_sentence.getMap();

    auto it = sentence_map.find("version");

	if (it != sentence_map.end()) {

        if (*it == "6.7") {

    		std::cout << "You have the latest version";

            break;

        }

	}

} while (read_sentence.getType() == MikrotikPlus::SENTENCE_TYPES::CONTINUE);

```

You can also write or read a single word of the type `std::string`

```cpp
std::string word = mikrotik.readWord();
mikrotik.write("/some/command");
mikrotik.write(""); // You have to terminate the sentence if you are done with constructing it
```

`Sentence` defines a `clear` member function which clears the underlying vector of strings (array of words) it holds.

It also defines a `reset` member function which resets the sentence type. Sentence types are as following:
```
NONE,
DONE,
TRAP,
FATAL,
CONTINUE,
```

It overloads the `<<` operator, which prints out words each on its line, when invoked.

___
## Exceptions

Exceptions are located in the `MikrotikPlus/exceptions.hpp` header

Sometimes a connection error might occur when connected to the RouterOS API, i.e the connection got timed out or simply lost. In such a case, the `ConnectionTimedOut` exception gets thrown. We have to attempt to read a sentence from the API in order to know that.

```cpp

block1:

try {

    do {

	    read_sentence = mikrotik.readSentence();

    	// Read some data on an infinite loop
        // Process it

    } while (read_sentence.getType() == MikrotikPlus::SENTENCE_TYPES::CONTINUE);

} catch (MikrotikPlus::ConnectionTimedOut &e) {

    // Reconnect and login

    goto block1;

}
```

## Building

To build this, you will have to download the source code and navigate to its directory. Then, you will do this:
```
mkdir build
cd build
cmake ..
(sudo) make install
```
