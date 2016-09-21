#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <set>
#include <vector>

// dictionaries for word lookups
std::vector<std::set<std::string>> dicts(20);

// load dictionary into memory
void loadDictionary(std::string filename) {
  std::string line;
  std::ifstream inputFile(filename);
  if (inputFile) {
    while (std::getline(inputFile, line)) {
      dicts[line.length()].insert(line);
    }
    inputFile.close();
  }
}

// decrypt message using vigenere cipher
std::string decrypt(std::string str, std::string key) {
  std::string plaintext;
  uint16_t keyIndex = 0;

  for (std::string::iterator it = str.begin(), end = str.end(); it != end; ++it) {
    // uppercase alphas are chars [65, 90]
    if (*it <= 90 && *it >= 65) {
      *it = (*it - key.at(keyIndex) + 65);
      // wrap back around in the alphabet if necessary
      if (*it < 65) {
        *it += 26;
      }
       plaintext += *it;

      // increment key index
      if (++keyIndex >= key.length()) {
        keyIndex = 0;
      }
    }
  }
  return plaintext;
}

// Parameters:
// (1) string -- ciphertext
// (2) uint16_t -- length of the key
// (3) uint16_t -- length of the first word of the plaintext
// (4) string -- optional flags

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cout << "Error: bad input." << std::endl;
    return 1;
  }
  // read arguments into variables
  std::string ciphertext = argv[1];
  uint16_t keyLength = atoi(argv[2]);
  uint16_t firstWordLength = atoi(argv[3]);
  // allow option to exit program after first possible solution is found
  bool exitOnFirst = 0;
  if (argc >= 5 && std::string(argv[4]) == "-f"){
    exitOnFirst = true;
  }

  // load memory from dictionary file
  loadDictionary("dictionary.txt");

  // letter frequency order: etaoinshrdlcumwfgypbvkjxqz
  char letters[27] = "ETAOINSHRDLCUMWFGYPBVKJXQZ";
  uint32_t possibleKeys = pow(26, keyLength);
  uint64_t baseKeyIndex;
  std::string tempKey;
  std::string firstPlain;

  // loop over all possible keys
  for (uint64_t keyIndex=0; keyIndex < possibleKeys; ++keyIndex) {
    baseKeyIndex = keyIndex;
    // loop over all letters in one key
    // convert keyIndex from base 10 to base 26 and store letter in key
    for (uint16_t i=0; i < keyLength; ++i) {
      // allow for a way to exit early from the base conversion
      if (baseKeyIndex == 0 && i >= keyLength) {
        tempKey.clear();
        break;
      }

      tempKey += letters[baseKeyIndex % 26];
      baseKeyIndex /= 26;
    }
    // test key
    if (!tempKey.empty()) {
      firstPlain = decrypt(ciphertext.substr(0, firstWordLength), tempKey);

      if (dicts[firstWordLength].find(firstPlain) != dicts[firstWordLength].end()) {
        // first word was decrypted properly with this key
        // decrypt the full ciphertext
        std::cout << tempKey << ": " << decrypt(ciphertext, tempKey) << std::endl;
        if (exitOnFirst) {
          return 0;
        }
      }
    }
    // reset tempKey
    tempKey.clear();
  }

  return 0;
}
