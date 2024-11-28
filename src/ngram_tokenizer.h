#include <cstdint>
#include <functional>

class NgramTokenizer {
public:
  NgramTokenizer(const int &n = 1);
  virtual ~NgramTokenizer();

  int tokenize(const char *text, const int &text_len,
               std::function<int(const char *token, const int &token_len,
                                 const int &start, const int &end)>
                   token);

private:
  int n;
};
