#include "world.h"
#include "rng.h"
#include "knightwalker.h"
#include "statistictype.h"
#include <ranges>
Board World::InitializeBoard()
{
    auto board = CreateBoard(BOARD_COLUMNS, BOARD_ROWS);
    for(size_t column: std::views::iota(size_t{0}, board.GetColumns()))
    {
        for(size_t row: std::views::iota(size_t{0}, board.GetRows()))
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
Character World::SpawnCharacter(Board& board, CharacterType characterType)
{
    size_t column;
    size_t row;
    do
    {
        column = RNG::FromRange(0, BOARD_COLUMNS - 1);
        row = RNG::FromRange(0, BOARD_ROWS - 1);
    } while (board.GetLocation(column, row)->GetCharacter());
    return CreateCharacter(characterType, *board.GetLocation(column, row));
}
void World::PopulateBoard(Board board)
{
    constexpr int KNIGHT_INITIAL_SUPPLIES = 15;
    auto avatar = SpawnCharacter(board, CharacterType::KNIGHT);
    SetAvatar(avatar);
    avatar.SetStatisticMinimum(StatisticType::SUPPLIES, 0);
    avatar.SetStatisticMaximum(StatisticType::SUPPLIES, KNIGHT_INITIAL_SUPPLIES);
    avatar.SetStatistic(StatisticType::SUPPLIES, KNIGHT_INITIAL_SUPPLIES);
    SpawnCharacter(board, CharacterType::STICKY_BUNS);
    SpawnCharacter(board, CharacterType::BUTTHOLE);
}
void World::Initialize()
{
    _data.Clear();
    PopulateBoard(InitializeBoard());
}
Board World::CreateBoard(size_t columns, size_t rows)
{
    return Board(_data, _data.CreateBoard(columns, rows));
}
Location World::CreateLocation(const Board& board, bool light)
{
    return Location(_data, _data.CreateLocation(board.GetIndex(), light));
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
    if(avatar)
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
    if(auto avatarIndex = _data.GetAvatarIndex())
    {
        return Character(_data, *avatarIndex);
    }
    return std::nullopt;
}
