#include "cpp_tokenizer.hpp"

int main()
{
  std::string s = "#include\n// hola";
  CppTokenizer::Tokenizer tokenizer;
  CppTokenizer::log_tokens(tokenizer.tokenize(s));
  return 0;
}