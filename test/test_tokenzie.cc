#include "ngram_tokenizer.h"
#include <cassert>
#include <iostream>
#include <vector>

struct Token {
  const char *token;
  int token_len;
  int start;
  int end;
};

int main() {
  {
    std::string s = "I love you,  you?";

    NgramTokenizer tokenizer(1);

    std::vector<Token> tokens;
    int rc = tokenizer.tokenize(
        s.c_str(), s.length(),
        [&](const char *token, const int &token_len, const int &start,
            const int &end) -> int {
          tokens.emplace_back(Token{token, token_len, start, end});
          return 0;
        });

    assert(rc == 0);
    assert(tokens.size() == 6);
    int i = 0;
    assert(std::string(tokens[i].token, tokens[i].token_len) == "I");
    assert(tokens[i].start == 0);
    assert(tokens[i].end == 1);
    i++;
    assert(std::string(tokens[i].token, tokens[i].token_len) == "love");
    assert(tokens[i].start == 2);
    assert(tokens[i].end == 6);
    i++;
    assert(std::string(tokens[i].token, tokens[i].token_len) == "you");
    assert(tokens[i].start == 7);
    assert(tokens[i].end == 10);
    i++;
    assert(std::string(tokens[i].token, tokens[i].token_len) == ",");
    assert(tokens[i].start == 10);
    assert(tokens[i].end == 11);
    i++;
    assert(std::string(tokens[i].token, tokens[i].token_len) == "you");
    assert(tokens[i].start == 13);
    assert(tokens[i].end == 16);
    i++;
    assert(std::string(tokens[i].token, tokens[i].token_len) == "?");
    assert(tokens[i].start == 16);
    assert(tokens[i].end == 17);
    i++;
  }

  {
    std::string s = " 《不”  ";

    NgramTokenizer tokenizer(1);

    std::vector<Token> tokens;
    int rc = tokenizer.tokenize(
        s.c_str(), s.length(),
        [&](const char *token, const int &token_len, const int &start,
            const int &end) -> int {
          tokens.emplace_back(Token{token, token_len, start, end});
          return 0;
        });

    assert(rc == 0);
    assert(tokens.size() == 3);
    int i = 0;
    assert(std::string(tokens[i].token, tokens[i].token_len) == "《");
    assert(tokens[i].start == 1);
    assert(tokens[i].end == 4);
    i++;
    assert(std::string(tokens[i].token, tokens[i].token_len) == "不");
    assert(tokens[i].start == 4);
    assert(tokens[i].end == 7);
    i++;
    assert(std::string(tokens[i].token, tokens[i].token_len) == "”");
    assert(tokens[i].start == 7);
    assert(tokens[i].end == 10);
    i++;
  }

  {
    std::string s = " 《不”  ";

    NgramTokenizer tokenizer(2);

    std::vector<Token> tokens;
    int rc = tokenizer.tokenize(
        s.c_str(), s.length(),
        [&](const char *token, const int &token_len, const int &start,
            const int &end) -> int {
          tokens.emplace_back(Token{token, token_len, start, end});
          return 0;
        });

    assert(rc == 0);
    assert(tokens.size() == 2);
    int i = 0;
    assert(std::string(tokens[i].token, tokens[i].token_len) == "《不");
    assert(tokens[i].start == 1);
    assert(tokens[i].end == 7);
    i++;
    assert(std::string(tokens[i].token, tokens[i].token_len) == "不”");
    assert(tokens[i].start == 4);
    assert(tokens[i].end == 10);
    i++;
  }

  {
    std::string s = " 《不”  ";

    NgramTokenizer tokenizer(3);

    std::vector<Token> tokens;
    int rc = tokenizer.tokenize(
        s.c_str(), s.length(),
        [&](const char *token, const int &token_len, const int &start,
            const int &end) -> int {
          tokens.emplace_back(Token{token, token_len, start, end});
          return 0;
        });

    assert(rc == 0);
    assert(tokens.size() == 1);
    int i = 0;
    assert(std::string(tokens[i].token, tokens[i].token_len) == "《不”");
    assert(tokens[i].start == 1);
    assert(tokens[i].end == 10);
    i++;
  }
}
