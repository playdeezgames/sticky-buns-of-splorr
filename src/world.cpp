#include "world.h"
#include "rng.h"
void World::Initialize()
{
    _data.Clear();
    auto board = CreateBoard(BOARD_COLUMNS, BOARD_ROWS);
    for(size_t column = 0; column < BOARD_COLUMNS; ++column)
    {
        for(size_t row = 0; row < BOARD_ROWS; ++row)
        {
            board.SetLocation(column, row, CreateLocation(board));
        }
    }
    size_t column;
    size_t row;
    do
    {
        column = RNG::FromRange(0, BOARD_COLUMNS - 1);
        row = RNG::FromRange(0, BOARD_ROWS - 1);
    } while (board.GetLocation(column, row)->GetCharacter().has_value());
    SetAvatar(CreateCharacter(CharacterType::KNIGHT, *board.GetLocation(column, row)));
}
Board World::CreateBoard(size_t columns, size_t rows)
{
    size_t boardIndex = _data.CreateBoard(columns, rows);
    return Board(_data, boardIndex);
}
Location World::CreateLocation(const Board& board)
{
    size_t locationIndex = _data.CreateLocation(board.GetIndex());
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
