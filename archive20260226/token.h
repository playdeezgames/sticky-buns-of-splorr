#pragma once
#include "tokentype.h"
class Token
{
private:
    TokenType tokenType;
public:
    void SetTokenType(TokenType tokenType);
    TokenType GetTokenType() const;
};