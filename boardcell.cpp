#include "boardcell.h"
void BoardCell::SetToken(const std::optional<Token>& token)
{
    this->token = token;
}
const std::optional<Token>& BoardCell::GetToken() const
{
    return token;
}