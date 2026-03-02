#pragma once
#include "worlddata.h"
#include "board.h"
#include "character.h"
#include "charactertype.h"
#include "message.h"
class World
{
private:
    WorldData& _data;
    World();
    Board InitializeBoard();
    void PopulateBoard(Board board);
public:
    Character SpawnCharacter(Board& board, CharacterType characterType);
    static constexpr size_t BOARD_COLUMNS = 8;
    static constexpr size_t BOARD_ROWS = 8;
    World(WorldData& data):_data(data){}
    void Initialize();
    Board CreateBoard(size_t columns, size_t rows);
    Location CreateLocation(const Board& board, bool light);
    Character CreateCharacter(CharacterType characterType, Location location);
    void SetAvatar(std::optional<Character> avatar);
    std::optional<Character> GetAvatar() const;
    std::vector<Message> GetMessages() const;
    void AddMessage(const std::string_view& text, FrameBufferCellColor foreground, FrameBufferCellColor background);
    void ClearMessages();
};