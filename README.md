# Simple ngram tokenizer for Sqlite3 fts5

## What is a token
1. consecutive digit charactors
2. consecutive alpha charactors
3. numbers of consecutive Unicode charactors(whose encoding is in two byte or more)

## How to use
Compile it with cmake, then load the extension in sqlite3.

```
# example is in Linux

sqlite> .load ./src/libngram.so

sqlite> create virtual table movie using fts5(name, year, date, content, url, tokenize='ngram n 2');
```

