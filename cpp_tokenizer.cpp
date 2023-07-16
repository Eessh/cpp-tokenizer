#include "cpp_tokenizer.hpp"

// static std::vector<std::string> seperators = {
//   " ", "\n", ".", "!", "\t", ";", ":", "\\", "/", "+", "-",  "*",  "&",
//   "%", "<",  ">", "=", "(",  ")", "{", "}",  "[", "]", "\"", "\'", ","};

// static std::vector<char> char_seps = {
//   ' ', '\n', '.', '!', '\t', ';', ':', '\\', '/', '+', '-', '*', '&',
//   '%', '<', '>', '=', '(', ')', '{', '}', '[', ']', '"', '\'', ','
// };

static std::string seperators = " \n.!\t;:\\/+-*&%<>=(){}[]\"',|~^";

static std::vector<std::string> operators = {
  "::", "->", "<=", ">=", "+=", "-=", "/=", "*=", "^=", "&&", "==",
  "&=", "||", "%=", ">>", "<<", "~",  "+",  "-",  "*",  "/",  "=",
  "<",  ">",  "!",  "?",  ":",  "^",  "&",  "|",  "%"};

static std::vector<std::string> keywords = {"alignas",
                                            "alignof",
                                            "and",
                                            "and_eq",
                                            "asm",
                                            "atomic_cancel",
                                            "atomic_commit",
                                            "atomic_noexcept",
                                            "auto",
                                            "bitand",
                                            "bitor",
                                            "bool",
                                            "break",
                                            "case",
                                            "catch",
                                            "char",
                                            "char8_t",
                                            "char16_t",
                                            "char32_t",
                                            "class",
                                            "compl",
                                            "concept",
                                            "const",
                                            "consteval",
                                            "constexpr",
                                            "constinit",
                                            "const_cast",
                                            "continue",
                                            "co_await",
                                            "co_return",
                                            "co_yeild",
                                            "decltype",
                                            "default",
                                            "delete",
                                            "do",
                                            "double",
                                            "dynalic_cast",
                                            "else",
                                            "enum",
                                            "explicit",
                                            "export",
                                            "extern",
                                            "false",
                                            "float",
                                            "for",
                                            "friend",
                                            "goto",
                                            "if",
                                            "inline",
                                            "int",
                                            "long",
                                            "mutable",
                                            "namespace",
                                            "new",
                                            "noexcept",
                                            "not",
                                            "not_eq",
                                            "nullptr",
                                            "operator",
                                            "or",
                                            "or_eq",
                                            "private",
                                            "protected",
                                            "public",
                                            "reflexpr",
                                            "register",
                                            "reinterpret_cast",
                                            "requires",
                                            "return",
                                            "short",
                                            "signed",
                                            "sizeof",
                                            "static",
                                            "static_assert",
                                            "static_cast",
                                            "struct",
                                            "switch",
                                            "synchronized",
                                            "template",
                                            "this",
                                            "thread_local",
                                            "throw",
                                            "true",
                                            "try",
                                            "typedef",
                                            "typeid",
                                            "typename",
                                            "union",
                                            "unsigned",
                                            "using",
                                            "virtual",
                                            "void",
                                            "volatile",
                                            "wchar_t",
                                            "while",
                                            "xor",
                                            "xor_eq"};

static std::vector<std::string> preprocessor_directives = {"#ifndef",
                                                           "#ifdef",
                                                           "#if",
                                                           "#else",
                                                           "#elif",
                                                           "#elifdef",
                                                           "#elifndef",
                                                           "#endif",
                                                           "#define",
                                                           "#undef",
                                                           "#include",
                                                           "#error",
                                                           "#warning",
                                                           "#pragma",
                                                           "#line"};

std::string token_type_to_string(const CppTokenizer::TokenType& type)
{
  switch(type)
  {
  case CppTokenizer::TokenType::WHITESPACE:
    return "WHITESPACE";
  case CppTokenizer::TokenType::NEWLINE:
    return "NEWLINe";
  case CppTokenizer::TokenType::TAB:
    return "TAB";
  case CppTokenizer::TokenType::SEMICOLON:
    return "SEMICOLON";
  case CppTokenizer::TokenType::COMMA:
    return "COMMA";
  case CppTokenizer::TokenType::ESCAPE_BACKSLASH:
    return "ESCAPE_BACKSLASH";
  case CppTokenizer::TokenType::BRACKET_OPEN:
    return "BRACKET_OPEN";
  case CppTokenizer::TokenType::BRACKET_CLOSE:
    return "BRACKET_CLOSE";
  case CppTokenizer::TokenType::SQUARE_BRACKET_OPEN:
    return "SQUARE_BRACKET_OPEN";
  case CppTokenizer::TokenType::SQUARE_BRACKET_CLOSE:
    return "SQUARE_BRACKET_CLOSE";
  case CppTokenizer::TokenType::CURLY_BRACE_OPEN:
    return "CURLY_BRACE_OPEN";
  case CppTokenizer::TokenType::CURLY_BRACE_CLOSE:
    return "CURLY_BRACE_CLOSE";
  case CppTokenizer::TokenType::CHARACTER:
    return "CHARACTER";
  case CppTokenizer::TokenType::STRING:
    return "STRING";
  case CppTokenizer::TokenType::COMMENT:
    return "COMMENT";
  case CppTokenizer::TokenType::MULTILINE_COMMENT:
    return "MULTILINE_COMMENT";
  case CppTokenizer::TokenType::OPERATOR:
    return "OPERATOR";
  case CppTokenizer::TokenType::KEYWORD:
    return "KEYWORD";
  case CppTokenizer::TokenType::PREPROCESSOR_DRIECTIVE:
    return "PREPROCESSOR_DIRECTIVE";
  case CppTokenizer::TokenType::IDENTIFIER:
    return "IDENTIFIER";
  default:
    break;
  }

  return "UNKNOWN";
}

namespace CppTokenizer
{

Token::Token() noexcept
  : type(TokenType::UNKNOWN), start_offset(0), end_offset(0), value("")
{}

Token::Token(const TokenType& token_type) noexcept
  : type(token_type), start_offset(0), end_offset(0), value("")
{}

bool Token::operator!=(const Token& other) const noexcept
{
  return type != other.type || start_offset != other.start_offset ||
         end_offset != other.end_offset || value != other.value;
}

Tokenizer::Tokenizer() noexcept
  : _current_token(Token())
  , _tokens({})
  , _inside_char(false)
  , _inside_string(false)
  , _inside_comment(false)
  , _inside_multiline_comment(false)
  , _position(0)
{}

const std::vector<Token>& Tokenizer::tokenize(const std::string& str) noexcept
{
  while(_position < str.size())
  {
    char character = str[_position];
    printf("character: %c\n", character);

    /// Preprocessor Directive
    if(character == '#')
    {
      Token token(TokenType::PREPROCESSOR_DRIECTIVE);
      token.start_offset = _position;
      token.value.push_back(character);
      // move forward until space is occurred
      while(_position < str.size() && str[_position] != ' ')
      {
        if(str[_position] == '\n')
        {
          // no whitespace encountered
          token.end_offset = _position - 1;
          token.value.pop_back();
          _tokens.emplace_back(token);
          break;
        }
        _position++;
        token.value.push_back(str[_position]);
      }
      if(str[_position] == ' ')
      {
        token.end_offset = _position - 1;
        token.value.pop_back();
        _tokens.emplace_back(token);
        Token whitespace_token(TokenType::WHITESPACE);
        whitespace_token.start_offset = _position;
        whitespace_token.end_offset = _position;
        _position++;
        whitespace_token.value = " ";
        _tokens.emplace_back(whitespace_token);
        continue;
      }
      _position++;
      continue;
    }

    /// Seperator
    if(seperators.find(character) != std::string::npos)
    {
      if(character == '\'')
      {
        if(_inside_char)
        {
          _inside_char = false;
          _current_token.value.push_back(character);
          _current_token.end_offset = _position;
          _tokens.emplace_back(_current_token);
          _position++;
          continue;
        }
        _inside_char = true;
        _current_token = Token(TokenType::CHARACTER);
        _current_token.value.push_back(character);
        _current_token.start_offset = _position;
        _position++;
        continue;
      }
      else if(character == '"')
      {
        if(_inside_string)
        {
          _inside_string = false;
          _current_token.value.push_back(character);
          _current_token.end_offset = _position;
          _tokens.emplace_back(_current_token);
          _position++;
          continue;
        }
        _inside_string = true;
        _current_token = Token(TokenType::STRING);
        _current_token.value.push_back(character);
        _current_token.start_offset = _position;
        _position++;
        continue;
      }
      else if(character == '/')
      {
        if(_inside_multiline_comment)
        {
          if(_current_token.value.back() == '*')
          {
            _inside_multiline_comment = false;
            _current_token.end_offset = _position;
            _current_token.value.push_back(character);
            _tokens.emplace_back(_current_token);
            _position++;
            goto while_loop_continue;
          }
          _current_token.value.push_back(character);
          _position++;
          goto while_loop_continue;
        }
        if(_inside_comment)
        {
          _current_token.value.push_back(character);
          _position++;
          goto while_loop_continue;
        }
        if(_position + 1 < str.size())
        {
          if(str[_position + 1] == '/')
          {
            // comment
            _current_token = Token(TokenType::COMMENT);
            _inside_comment = true;
            _current_token.value = "//";
            _current_token.start_offset = _position;
            _position += 2;
            goto while_loop_continue;
          }
          if(str[_position + 1] == '*')
          {
            // multiline comment
            _current_token = Token(TokenType::MULTILINE_COMMENT);
            _inside_multiline_comment = true;
            _current_token.value = "/*";
            _current_token.start_offset = _position;
            _position += 2;
            goto while_loop_continue;
          }
        }
      }
      else if(character == '\n')
      {
        if(_inside_comment)
        {
          _inside_comment = false;
          _current_token.end_offset = _position - 1;
          _tokens.emplace_back(_current_token);
          _position++;
          goto while_loop_continue;
        }
        if(_inside_multiline_comment || _inside_string)
        {
          _current_token.value.push_back(character);
          _position++;
          goto while_loop_continue;
        }
        if(_current_token.type == TokenType::IDENTIFIER &&
           _tokens.back() != _current_token)
        {
          _current_token.end_offset = _position - 1;
          _tokens.emplace_back(_current_token);
          _position++;
          goto while_loop_continue;
        }
        // ignore
        _position++;
        goto while_loop_continue;
      }
      else if(character == ' ')
      {
        if(_inside_comment || _inside_multiline_comment || _inside_string)
        {
          _current_token.value.push_back(character);
          _position++;
          goto while_loop_continue;
        }
        _current_token = Token(TokenType::WHITESPACE);
        _current_token.start_offset = _position;
        _current_token.end_offset = _position;
        _current_token.value.push_back(character);
        _tokens.emplace_back(_current_token);
        _position++;
        goto while_loop_continue;
      }
      else if(character == '.')
      {
        if(_inside_comment || _inside_multiline_comment || _inside_char ||
           _inside_string)
        {
          _current_token.value.push_back(character);
          _position++;
          goto while_loop_continue;
        }
        _current_token = Token(TokenType::OPERATOR);
        _current_token.start_offset = _position;
        _current_token.end_offset = _position;
        _current_token.value.push_back(character);
        _tokens.emplace_back(_current_token);
        _position++;
        goto while_loop_continue;
      }
      else if(character == ',')
      {
        if(_inside_comment || _inside_multiline_comment || _inside_char ||
           _inside_string)
        {
          _current_token.value.push_back(character);
          _position++;
          goto while_loop_continue;
        }
        _current_token = Token(TokenType::COMMA);
        _current_token.start_offset = _position;
        _current_token.end_offset = _position;
        _current_token.value.push_back(character);
        _tokens.emplace_back(_current_token);
        _position++;
        goto while_loop_continue;
      }
      else if(character == ';')
      {
        if(_inside_comment || _inside_multiline_comment || _inside_char ||
           _inside_string)
        {
          _current_token.value.push_back(character);
          _position++;
          goto while_loop_continue;
        }
        _current_token = Token(TokenType::SEMICOLON);
        _current_token.start_offset = _position;
        _current_token.end_offset = _position;
        _current_token.value.push_back(character);
        _tokens.emplace_back(_current_token);
        _position++;
        goto while_loop_continue;
      }
      else if(character == '(')
      {
        /// TODO: Identifying function, if token before this is literal
        if(_inside_comment || _inside_multiline_comment || _inside_char ||
           _inside_string)
        {
          _current_token.value.push_back(character);
          _position++;
          goto while_loop_continue;
        }
        _current_token = Token(TokenType::BRACKET_OPEN);
        _current_token.start_offset = _position;
        _current_token.end_offset = _position;
        _current_token.value.push_back(character);
        _tokens.emplace_back(_current_token);
        _position++;
        goto while_loop_continue;
      }
      else if(character == ')')
      {
        if(_inside_comment || _inside_multiline_comment || _inside_char ||
           _inside_string)
        {
          _current_token.value.push_back(character);
          _position++;
          goto while_loop_continue;
        }
        _current_token = Token(TokenType::BRACKET_CLOSE);
        _current_token.start_offset = _position;
        _current_token.end_offset = _position;
        _current_token.value.push_back(character);
        _tokens.emplace_back(_current_token);
        _position++;
        goto while_loop_continue;
      }
      else if(character == '[')
      {}
      else if(character == ']')
      {}
      else if(character == '{')
      {}
      else if(character == '}')
      {}
    }

    /// continuing if inside comment
    if(_inside_comment)
    {
      while(_position < str.size() && str[_position] != '\n')
      {
        _current_token.value.push_back(str[_position]);
        _position++;
      }
      goto while_loop_continue;
    }

    /// continuing if inside multiline comment
    if(_inside_multiline_comment)
    {
      while(_position < str.size() && str[_position] != '*')
      {
        _current_token.value.push_back(str[_position]);
        _position++;
      }
      goto while_loop_continue;
    }

    /// Operator
    for(const std::string& op : operators)
    {
      if(str.compare(_position, op.size(), op) == 0)
      {
        // found operator match
        _current_token = Token(TokenType::OPERATOR);
        _current_token.start_offset = _position;
        _current_token.end_offset = _position + op.size() - 1;
        _current_token.value = op;
        _tokens.emplace_back(_current_token);
        _position += op.size();
        printf("op: %s\n", op.c_str());
        goto while_loop_continue;
      }
    }
    printf("hehe\n");

    /// Keyword
    for(const std::string keyword : keywords)
    {
      if(str.compare(_position, keyword.size(), keyword) == 0)
      {
        // found keyword match
        _current_token = Token(TokenType::KEYWORD);
        _current_token.start_offset = _position;
        _current_token.end_offset = _position + keyword.size() - 1;
        _current_token.value = keyword;
        _tokens.emplace_back(_current_token);
        _position += keyword.size();
        goto while_loop_continue;
      }
    }

    /// Alphabets
    if(_inside_comment || _inside_multiline_comment || _inside_string ||
       _inside_char)
    {
      _current_token.value.push_back(character);
      _position++;
      goto while_loop_continue;
    }
    _current_token = Token(TokenType::IDENTIFIER);
    _current_token.start_offset = _position;
    _current_token.value = character;
    _position++;
    // moving forward until a separator is encountered
    while(_position < str.size() &&
          seperators.find(str[_position]) == std::string::npos)
    {
      _current_token.value.push_back(str[_position]);
      _position++;
    }
    _current_token.end_offset = _position - 1;
    _tokens.emplace_back(_current_token);

  while_loop_continue:
    continue;
  }

  return _tokens;
}

void log_tokens(const std::vector<Token>& tokens) noexcept
{
  printf("Tokens: [");
  for(const Token& token : tokens)
  {
    printf("\n  {\n    type: %s,\n    start_offset: %ld,\n    end_offset: "
           "%ld,\n    value: \"%s\"\n  },",
           token_type_to_string(token.type).c_str(),
           token.start_offset,
           token.end_offset,
           token.value.c_str());
  }
  printf("\n]\n");
}

}; // namespace CppTokenizer
