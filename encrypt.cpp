#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

enum args {PROGRAM, INPUT_FILE, KEY, OUTPUT_FILE};

// Encrypt plaintext using a vigenere cipher
int main(int argc, char *argv[]) {
  if (argc < 2) {
    // input file must be specified
    std::cout << "Error: no input file specified." << std::endl;
    return 1;
  } else if (argc < 3) {
    // a key string must be specified
    std::cout << "Error: no encryption key specified." << std::endl;
    return 1;
  }

  std::string line;
  std::ifstream inputFile(argv[INPUT_FILE]);
  std::ofstream outputFile;
  std::string key = argv[KEY];
  transform(key.begin(), key.end(), key.begin(), ::toupper);
  uint16_t keyIndex = 0;

  // check for an output file, but provide a fallback
  if (argc >= 4) {
    outputFile.open(argv[OUTPUT_FILE]);
  } else {
    outputFile.open("encrypted.txt");
  }

  if (inputFile) {
    // loop over lines in the input file
    while (std::getline(inputFile, line)) {
      // transform each line to uppercase
      transform(line.begin(), line.end(), line.begin(), ::toupper);

      // encrypt message using vigenere cipher
      for (std::string::iterator it = line.begin(), end = line.end(); it != end; ++it) {
        // uppercase alphas are chars [65, 90]
        if (*it <= 90 && *it >= 65) {
          *it = (*it + key.at(keyIndex) - 65);
          // wrap back around in the alphabet if necessary
          if (*it > 90) {
            *it -= 26;
          }
          std::cout << *it;
          // write to file
          outputFile << *it;

          // increment key index
          if (++keyIndex >= key.length()) {
            keyIndex = 0;
          }
        }
      }

      std::cout << std::endl;
      outputFile << std::endl;
    }
    inputFile.close();
    outputFile.close();
  } else {
    std::cout << "Error reading input file." << std::endl;
  }

  return 0;
}
