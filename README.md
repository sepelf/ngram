# Simple ngram tokenizer for Sqlite3 fts5

## What is a token
1. consecutive digit charactors
2. consecutive alpha charactors
3. numbers of consecutive Unicode charactors(whose encoding is in two byte or more)

## How to use
Compile it with cmake, then load the extension in sqlite3.

```bash
mkdir build
cd build
cmake -DSQLite3_INCLUDE_DIR=/path/to/sqlite3/include -DWITH_TEST=ON ..
make
```

```
sqlite> .load src/libngram.dylib
sqlite> create virtual table movie using fts5(name, year, date, content, url, tokenize='ngram');
sqlite> insert into movie values ('你好，李焕英', 2020, datetime(), '这是一部好看的电影', 'http://www.movie.com/');
sqlite> select * from movie where content match '电影 一 部';
你好，李焕英|2020|2025-01-06 10:04:06|这是一部好看的电影|http://www.movie.com/
sqlite>
```

