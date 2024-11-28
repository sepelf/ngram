
add_library(ngram_tokenizer STATIC ngram_tokenizer.cc)
target_compile_options(ngram_tokenizer PRIVATE -fPIC)
target_include_directories(ngram_tokenizer INTERFACE .)

add_executable(tokenize_tool tokenize_tool.cc)
target_link_libraries(tokenize_tool ngram_tokenizer)

# use sqlite3_bind_pointer to get fts5 extension to work,
# which is available in 3.20.0 and later version
find_package(SQLite3 3.20.0 REQUIRED)

add_library(ngram SHARED ngram.cc)
add_library(Ngram::ngram ALIAS ngram)
target_link_libraries(ngram ngram_tokenizer SQLite::SQLite3)

