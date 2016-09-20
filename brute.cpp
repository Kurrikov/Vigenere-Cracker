#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <set>
#include <vector>

// dictionary for word lookups
std::set<std::string> dict;
// vector of keys
std::vector<std::string> keys;
static const uint32_t VECTOR_MAX_SIZE = 150000;

// load dictionary into memory
void loadDictionary(std::string filename) {
  std::string line;
  std::ifstream inputFile(filename);
  if (inputFile) {
    while (std::getline(inputFile, line)) {
      dict.insert(line);
    }
    inputFile.close();
  }
}

// generate keys for decryption
void keygen(uint16_t keyLength) {
  // letter frequency order: etaoinshrdlcumwfgypbvkjxqz
  char letters[27] = "ETAOINSHRDLCUMWFGYPBVKJXQZ";
  uint32_t possibleKeys = pow(26, keyLength);
  uint64_t baseKeyIndex;
  std::string tempKey;
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
      // std::cout << letters[baseKeyIndex % 26];
      baseKeyIndex /= 26;
    }
    // store one key
    if (!tempKey.empty()) {
      // wait while key vector is full
      while (keys.size() >= VECTOR_MAX_SIZE) {}
      keys.push_back(tempKey);
    }
    // reset tempKey
    tempKey.clear();
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

// Your password cracker is expected to take three parameters:
// (1) a string of ciphertext;
// (2) an integer keyLength that denotes the length of the key;
// (3) an integer firstWordLength that denotes the length of the first word of the plaintext.

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cout << "Error: bad input." << std::endl;
    return 1;
  }
  // read arguments into variables
  std::string ciphertext = argv[1];
  uint16_t keyLength = atoi(argv[2]);
  uint16_t firstWordLength = atoi(argv[3]);

  // load memory from dictionary file
  loadDictionary("dictionary.txt");
  // test dictionary
  std::cout << "Size of dictionary: " << dict.size() << std::endl;
  // std::cout << "dict contains:";
  // for (const std::string& x : dict) {
  //   std::cout << x << std::endl;
  // }

  // test decrypter
  // std::cout << decrypt(ciphertext, "LEMON", 0) << std::endl;
  // test every possible key that has the length of keyLength
  // for (int i=0; i<keyLength; i++) {
  //   // loop over alphabet
  //   for (int j=0; j<26; j++) {
  //
  //   }
  // }

  // test keygen
  std::string firstPlain;
  keygen(keyLength);
  std::cout << "Size of keyspace: " << keys.size() << std::endl;
  for (int i=0; i < keys.size(); ++i) {
    if (!keys[i].empty()) {
      // check if the first word is in the dictionary when decrypted with keys[i]
      firstPlain = decrypt(ciphertext.substr(0, firstWordLength), keys[i]);
      if (dict.find(firstPlain) != dict.end()) {
        // first word was decrypted properly with this key
        // decrypt the full ciphertext
        // std::cout << firstPlain << " is in dict" << std::endl;
        std::cout << keys[i] << ": " << decrypt(ciphertext, keys[i]) << std::endl;
      }
    }
  }


  return 0;
}
