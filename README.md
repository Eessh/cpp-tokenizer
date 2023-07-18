# Cpp Tokenizer
Super simple tokenizer for C++ language. Geared towards syntax highlighting.

### Usage
```cpp
#include "cpp_tokenizer.hpp"

int main() {
  // string to tokenize
  std::string string_to_tokenize = "#include <iostream>\n";

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
    type: PREPROCESSOR_DIRECTIVE,
    start_offset: 0,
    end_offset: 7,
    value: "#include"
  },
  {
    type: WHITESPACE,
    start_offset: 8,
    end_offset: 8,
    value: " "
  },
  {
    type: HEADER,
    start_offset: 9,
    end_offset: 18,
    value: "<iostream>"
  }
]
*/
```

### Integrating
Simply put `cpp_tokenizer.hpp` in your project include path and `cpp_tokenizer.cpp` in project source files.

### Test
```powershell
g++ -O3 test.cpp cpp_tokenizer.cpp -o tokenizer.exe
.\tokenizer.exe test.cpp
```
