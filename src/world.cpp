#include "world.h"
#include "rng.h"
#include "knightwalker.h"
Board World::InitializeBoard()
{
    auto board = CreateBoard(BOARD_COLUMNS, BOARD_ROWS);
    for(size_t column = 0; column < board.GetColumns(); ++column)
    {
        for(size_t row = 0; row < board.GetRows(); ++row)
        {
            board.SetLocation(column, row, CreateLocation(board, (column+row)%2==1));
        }
    }
    for(int column = 0; column < (int)board.GetColumns(); ++column)
    {
        for(int row = 0; row < (int)board.GetRows(); ++row)
        {
            auto location = *board.GetLocation(column, row);
            for(auto knightMoveType : AllKnightMoveTypes)
            {
                auto destinationXY = KnightWalker::Walk(column, row, knightMoveType);
                int destinationX = std::get<0>(destinationXY);
                int destinationY = std::get<1>(destinationXY);
                if(destinationX >= 0 && destinationY >= 0 && destinationX < (int)board.GetColumns() && destinationY < (int)board.GetRows())
                {
                    auto destination = *board.GetLocation(destinationX, destinationY);
                    location.SetNeighbor(knightMoveType, destination);
                }
            }
        }
    }
    return board;
}
void World::PopulateBoard(Board board)
{
    size_t column;
    size_t row;
    do
    {
        column = RNG::FromRange(0, BOARD_COLUMNS - 1);
        row = RNG::FromRange(0, BOARD_ROWS - 1);
    } while (board.GetLocation(column, row)->GetCharacter().has_value());
    SetAvatar(CreateCharacter(CharacterType::KNIGHT, *board.GetLocation(column, row)));
}
void World::Initialize()
{
    _data.Clear();
    auto board = InitializeBoard();
    PopulateBoard(board);
}
Board World::CreateBoard(size_t columns, size_t rows)
{
    size_t boardIndex = _data.CreateBoard(columns, rows);
    return Board(_data, boardIndex);
}
Location World::CreateLocation(const Board& board, bool light)
{
    size_t locationIndex = _data.CreateLocation(board.GetIndex(), light);
    return Location(_data, locationIndex);
}
Character World::CreateCharacter(CharacterType characterType, Location location)
{
    size_t characterIndex = _data.CreateCharacter(characterType, location.GetIndex());
    auto result = Character(_data, characterIndex);
    location.SetCharacter(result);
    return result;
}
void World::SetAvatar(std::optional<Character> avatar)
{
    if(avatar.has_value())
    {
        _data.SetAvatarIndex(avatar->GetIndex());
    }
    else
    {
        _data.SetAvatarIndex(std::nullopt);
    }
}
std::optional<Character> World::GetAvatar() const
{
    auto avatarIndex = _data.GetAvatarIndex();
    if(avatarIndex.has_value())
    {
        return Character(_data, *avatarIndex);
    }
    return std::nullopt;
}
