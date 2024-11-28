#include "ngram_tokenizer.h"
#include <iostream>
#include <vector>

int main(int argc, char *argv[]) {
  int n = 1;
  int i = 1;
  if (argc > 3) {
      if (std::string(argv[1]) == "-n") {
          n = std::stol(argv[2]);
          i = 3;
      }
  }
  for (; i < argc; i++) {
    NgramTokenizer tokenizer(n);
    std::string s(argv[i]);
    std::cout << "[" << s << "]" << std::endl;
    std::vector<std::string> tokens;
    int rc = tokenizer.tokenize(s.c_str(), s.length(),
                                [&](const char *token, const int &token_len,
                                    const int &start, const int &end) -> int {
                                  tokens.emplace_back(std::string(token, token_len));
                                  return 0;
                                });
    if (rc) {
        std::cerr << "token failed" << rc << std::endl;
    } else {
        if (tokens.empty()) {
            std::cout << "No token" << std::endl;
        } else {
            for (uint32_t i=0; i < tokens.size(); i++) {
                std::cout << "Token " << i+1 << " [" << tokens[i] << "]" << std::endl;
            }
        }
    }
  }
}
