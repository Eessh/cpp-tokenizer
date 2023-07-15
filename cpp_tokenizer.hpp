#pragma once

#include <string>
#include <vector>

namespace CppTokenizer
{

typedef enum class TokenType
{
  WHITESPACE,
  NEWLINE,
  TAB,
  SEMICOLON,
  COMMA,
  ESCAPE_BACKSLASH,
  BRACKET_OPEN,
  BRACKET_CLOSE,
  SQUARE_BRACKET_OPEN,
  SQUARE_BRACKET_CLOSE,
  CURLY_BRACE_OPEN,
  CURLY_BRACE_CLOSE,
  CHARACTER,
  STRING,
  COMMENT,
  MULTILINE_COMMENT,
  OPERATOR,
  KEYWORD,
  PREPROCESSOR_DRIECTIVE,
  IDENTIFIER,

  UNKNOWN
} TokenType;

class Token
{
public:
  Token() noexcept;
  Token(const TokenType& token_type) noexcept;

  TokenType type;
  uint32_t start_offset, end_offset;
  std::string value;
};

class Tokenizer
{
public:
  Tokenizer() noexcept;

  [[nodiscard]] const std::vector<Token>&
  tokenize(const std::string& str) noexcept;

private:
  Token _current_token;
  std::vector<Token> _tokens;
  bool _inside_string, _inside_char, _inside_comment, _inside_multiline_comment;
  uint32_t _position;
};

void log_tokens(const std::vector<Token>& tokens) noexcept;

}; // namespace CppTokenizer