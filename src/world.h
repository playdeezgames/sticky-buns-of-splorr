#pragma once
#include "worlddata.h"
#include "board.h"
#include "character.h"
#include "charactertype.h"
class World
{
private:
    WorldData& _data;
    World();
    Board InitializeBoard();
    void PopulateBoard(Board board);
public:
    static constexpr size_t BOARD_COLUMNS = 8;
    static constexpr size_t BOARD_ROWS = 8;
    World(WorldData& data):_data(data){}
    void Initialize();
    Board CreateBoard(size_t columns, size_t rows);
    Location CreateLocation(const Board& board, bool light);
    Character CreateCharacter(CharacterType characterType, Location location);
    void SetAvatar(std::optional<Character> avatar);
    std::optional<Character> GetAvatar() const;
};