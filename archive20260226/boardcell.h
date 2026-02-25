#pragma once
#include <optional>
#include "token.h"
class BoardCell
{
private:
    std::optional<Token> token;
public:
    const std::optional<Token>& GetToken() const;
    void SetToken(const std::optional<Token>& token);
};