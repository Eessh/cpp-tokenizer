#include "cpp_tokenizer.hpp"

int main()
{
  std::string s = "#include <iostream>\n#include <vector>\n#include "
                  "<string>\nusing namespace std;\n\nint main() {\n\tstd::cout "
                  "<< \"Hola\" << std::endl;\n\treturn 0;\n}";
  CppTokenizer::Tokenizer tokenizer;
  CppTokenizer::log_tokens(tokenizer.tokenize(s));
  return 0;
}