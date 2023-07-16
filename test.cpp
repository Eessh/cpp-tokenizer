#include <cstdio>
#include "cpp_tokenizer.hpp"

int main(int argc, char** argv)
{
  if(argc < 2)
  {
    printf("Usage: a.exe <path_to_cpp_file>\n");
    exit(1);
  }

  std::string file_path = argv[1];

  /// reading file contents to string
  std::FILE* fp = std::fopen(file_path.c_str(), "rb");
  if(!fp)
  {
    printf("Error: Unable to open file: %s\n", file_path.c_str());
    exit(1);
  }
  std::string contents;
  std::fseek(fp, 0, SEEK_END);
  contents.resize(std::ftell(fp));
  std::rewind(fp);
  std::fread(&contents[0], 1, contents.size(), fp);
  std::fclose(fp);

  CppTokenizer::Tokenizer tokenizer;
  CppTokenizer::log_tokens(tokenizer.tokenize(contents));

  return 0;
}