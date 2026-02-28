#include "board.h"
#include "rng.h"
#include <algorithm>
#include <iostream>
#include <format>
#include "chessttype.h"

constexpr size_t INITIAL_SUPPLIES = 10;
constexpr size_t INITIAL_HEALTH = 5;
constexpr size_t FOOD_RESUPPLY = 5;
constexpr size_t INITIAL_JOOLS = 0;

bool Board::SpawnToken(const TokenType& tokenType)
{
    size_t column = RNG::FromRange(0, BOARD_WIDTH-1);
    size_t row = RNG::FromRange(0, BOARD_HEIGHT-1);
    auto& cell = GetCell(column, row);
    if(cell.GetToken())
    {
        return false;
    }
    Token token;
    token.SetTokenType(tokenType);
    cell.SetToken(token);
    return true;
}
void Board::Initialize()
{
    CleanUp();
    while(cells.size()<Board::BOARD_WIDTH)
    {
        std::vector<BoardCell> boardColumn;
        while(boardColumn.size()<Board::BOARD_HEIGHT)
        {
            boardColumn.push_back(BoardCell());
        }
        cells.push_back(boardColumn);
    }
    while(!SpawnToken(TokenType::KNIGHT));
    while(!SpawnToken(TokenType::STICKY_BUNS));
    while(!SpawnToken(TokenType::CHESST));
    moves = 0;
    supplies = maximum_supplies = INITIAL_SUPPLIES;
    jools = INITIAL_JOOLS;
    health = maximum_health = INITIAL_HEALTH;
}
void Board::CleanUp()
{
    cells.clear();
}
const BoardCell& Board::GetCell(size_t column, size_t row) const
{
    return cells[column][row];
}
BoardCell& Board::GetCell(size_t column, size_t row)
{
    return cells[column][row];
}
void Board::Move(int deltaX, int deltaY)
{
    auto columnIter = std::find_if(
        cells.begin(),
        cells.end(), 
        [](const auto& column)
        {
            return std::find_if(
                column.begin(), 
                column.end(), 
                [](const auto& cell)
                {
                    const auto& token = cell.GetToken();
                    return token && TokenType::KNIGHT == token->GetTokenType();
                }) != column.end();
        });
    if(columnIter==cells.end())
    {
        std::cout << "You cannot move a knight that does not exist!" << std::endl;
        return;
    }
    int x = columnIter - cells.begin();
    auto rowIter = std::find_if(
        (*columnIter).begin(), 
        (*columnIter).end(), 
        [](const auto& cell)
        {
            const auto& token = cell.GetToken();
            return token && TokenType::KNIGHT == token->GetTokenType();
        });
    int y = rowIter - (*columnIter).begin();
    auto token = rowIter->GetToken();
    int nextX = x + deltaX;
    int nextY = y + deltaY;
    if(nextX>=0 && nextY>=0 && nextX<BOARD_WIDTH && nextY<BOARD_HEIGHT)
    {
        auto& destinationCell = GetCell(nextX, nextY);
        const auto& destinationToken = destinationCell.GetToken();
        std::optional<TokenType> tokenType = (destinationToken)?(std::optional<TokenType>(destinationToken->GetTokenType())):(std::nullopt);
        if(destinationToken && TokenType::BLOCK == destinationToken->GetTokenType())
        {
            std::cout << "Blocked!" << std::endl;
        }
        else
        {
            if(GetHealth()>0)
            {
                if(GetSupplies()>0)
                {
                    --supplies;
                }
                else
                {
                    std::cout << "Yer out of supplies!" << std::endl;
                    --health;
                }
                RemoveBlocks();
                destinationCell.SetToken(token);
                ++moves;
                Token blockToken;
                blockToken.SetTokenType(TokenType::BLOCK);
                rowIter->SetToken(blockToken);
                if(tokenType)
                {
                    switch(*tokenType)
                    {
                        case TokenType::STICKY_BUNS:
                            ConsumeStickyBuns();
                        break;
                        case TokenType::CHESST:
                            ConsumeChesst();
                        break;
                        default:
                            std::cout << "You just absorbed unknown token" << std::endl;
                        break;
                    }
                }
            }
            else
            {
                std::cout << "Yer dead!" <<std::endl;
            }
        }
    }
    else
    {
        GetCell(x,y).SetToken(std::nullopt);
        std::cout << "You have moved the knight off of the board." << std::endl;
    }
}
void Board::ConsumeStickyBuns()
{
    supplies = std::min(supplies + FOOD_RESUPPLY, maximum_supplies);
    std::cout << std::format("Sticky Buns! +{} supplies.", FOOD_RESUPPLY) << std::endl;
    while(!SpawnToken(TokenType::STICKY_BUNS));
}
static const std::map<ChesstType, size_t> chesstGenerator = 
{
    {ChesstType::EMPTY, 4},
    {ChesstType::JOOLS, 3},
    {ChesstType::MIMIC, 1},
    {ChesstType::TRAP, 2}
};
void Board::ConsumeChesst()
{
    auto chesstType = RNG::FromGenerator(chesstGenerator);
    switch (chesstType)
    {
        case ChesstType::EMPTY:
        std::cout << "The chesst is empty, like yer soul!" << std::endl;
        break;
        case ChesstType::JOOLS:
        AddJools();
        break;
        case ChesstType::MIMIC:
        std::cout << "That's not a chesst! That's a mimic!" << std::endl;
        break;
        case ChesstType::TRAP:
        TriggerTrap();
        break;
        default:
        throw "dint work";
        break;
    }
    while(!SpawnToken(TokenType::CHESST));
}
static const std::map<size_t, size_t> joolsGenerator =
{
    {1,1},
    {2,2},
    {3,3},
    {4,2},
    {5,1}
};
void Board::AddJools()
{
    size_t addedJools = RNG::FromGenerator(joolsGenerator);
    jools += addedJools;
    std::cout << std::format("You find {} jools!", addedJools) << std::endl;
}
static const std::map<size_t, size_t> trapDamageGenerator =
{
    {0,1},
    {1,2},
    {2,1}
};
void Board::TriggerTrap()
{
    auto damage = RNG::FromGenerator(trapDamageGenerator);
    std::cout << "The chesst is trapped!" << std::endl;
    std::cout << std::format("You take {} damage!", damage) << std::endl;
    if(damage>=health)
    {
        health = 0;
    }
    else
    {
        health -= damage;
    }
}
void Board::RemoveBlocks()
{
    for(auto& column : cells)
    {
        for(auto& cell: column)
        {
            auto token = cell.GetToken();
            if(token && TokenType::BLOCK == token->GetTokenType())
            {
                cell.SetToken(std::nullopt);
            }
        }
    }
}