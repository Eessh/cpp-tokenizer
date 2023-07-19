# Cpp Tokenizer
Super simple tokenizer for C++ language. Geared towards syntax highlighting.

### Usage
```cpp
#include "cpp_tokenizer.hpp"

int main() {
  // string to tokenize
  std::string string_to_tokenize = "int foo = 0;";

  // creating tokenizer
  CppTokenizer::Tokenizer tokenizer;

  // parsing to tokens
  std::vector<CppTokenizer::Token> tokens = tokenizer.tokenize(string_to_tokenize);

  // logging tokens
  CppTokenizer::log_tokens(tokens);

  return 0;
}

/*
Output:
Tokens: [
  {
    type: KEYWORD,
    start_offset: 0,
    end_offset: 2,
    value: "int"
  },
  {
    type: WHITESPACE,
    start_offset: 3,
    end_offset: 3,
    value: " "
  },
  {
    type: IDENTIFIER,
    start_offset: 4,
    end_offset: 6,
    value: "foo"
  },
  {
    type: WHITESPACE,
    start_offset: 7,
    end_offset: 7,
    value: " "
  },
  {
    type: OPERATOR,
    start_offset: 8,
    end_offset: 8,
    value: "="
  },
  {
    type: WHITESPACE,
    start_offset: 9,
    end_offset: 9,
    value: " "
  },
  {
    type: NUMBER,
    start_offset: 10,
    end_offset: 10,
    value: "0"
  },
  {
    type: SEMICOLON,
    start_offset: 11,
    end_offset: 11,
    value: ";"
  }
]
*/
```

### Integrating
Simply put `cpp_tokenizer.hpp` in your project include path and `cpp_tokenizer.cpp` in project source files.

### Test
```powershell
g++ -O3 test.cpp cpp_tokenizer.cpp -o tokenize.exe
tokenize.exe sample.cpp
```
