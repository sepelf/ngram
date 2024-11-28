
add_executable(test_tokenize test_tokenzie.cc)
target_link_libraries(test_tokenize ngram_tokenizer)

add_test(NAME test_tokenize COMMAND test_tokenize)
