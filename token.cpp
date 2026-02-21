#include "token.h"
TokenType Token::GetTokenType() const
{
    return tokenType;
}
void Token::SetTokenType(TokenType tokenType)
{
    this->tokenType = tokenType;
}