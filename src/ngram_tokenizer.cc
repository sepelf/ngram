#include "ngram_tokenizer.h"
#include <cassert>
#include <cctype>

static uint8_t get_char_len(const uint8_t &ch) {
  if (ch >= 0xF0)
    return 4;
  else if (ch >= 0xE0)
    return 3;
  else if (ch >= 0xC0)
    return 2;
  return 1;
}

enum class CHAR_TYPE {
  SPACE,
  DIGIT,
  ALPHA,
  OTHER,
};

static CHAR_TYPE get_char_type(const uint8_t &ch) {
  if (std::isblank(ch) || std::isspace(ch) || std::iscntrl(ch))
    return CHAR_TYPE::SPACE;
  else if (std::isdigit(ch))
    return CHAR_TYPE::DIGIT;
  else if (std::isalpha(ch))
    return CHAR_TYPE::ALPHA;
  return CHAR_TYPE::OTHER;
}

NgramTokenizer::NgramTokenizer(const int &n) {
  assert(n > 0);
  this->n = n;
}

NgramTokenizer::~NgramTokenizer() {}

int NgramTokenizer::tokenize(
    const char *text, const int &text_len,
    std::function<int(const char *token, const int &token_len, const int &start,
                      const int &end)>
        token) {
  int i = 0;
  while (i < text_len) {
    CHAR_TYPE type = get_char_type(text[i]);

    if (type == CHAR_TYPE::SPACE) {
      while (++i < text_len && type == get_char_type(text[i]));
    } else if (type == CHAR_TYPE::DIGIT || type == CHAR_TYPE::ALPHA) {
      int j = i;
      while (++j < text_len && type == get_char_type(text[j]));

      int rc = token(text + i, j - i, i, j);
      if (rc) return rc;

      i = j;
    } else {
      int char_cnt = 0;
      int j = i;

      while (j < text_len && char_cnt < n && type == get_char_type(text[j])) {
        j += get_char_len(text[j]);
        char_cnt++;
      }

      if (char_cnt == n) {
        int rc = token(text + i, j - i, i, j);
        if (rc) return rc;

        i += get_char_len(text[i]);
      } else {
        i = j;
      }
    }
  }

  return 0;
}
