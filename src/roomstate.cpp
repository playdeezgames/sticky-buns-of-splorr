#include "roomstate.h"
constexpr int BOARD_CELL_WIDTH = 3;
constexpr int BOARD_CELL_HEIGHT = 3;
static unsigned char GetBoardCellCharacter(const Location& location)
{
    auto character = location.GetCharacter();
    if(character.has_value())
    {
        switch(character->GetCharacterType())
        {
            case CharacterType::STICKY_BUNS:
                return '@';
            case CharacterType::KNIGHT:
                return 'N';
            case CharacterType::BLOCK:
                return 'X';
            default:
                return 0;
        }
    }
    return 0;
}
static FrameBufferCellColor GetBoardCellForegroundColor(const Location& location)
{
    auto character = location.GetCharacter();
    if(character.has_value())
    {
        switch(character->GetCharacterType())
        {
            case CharacterType::STICKY_BUNS:
                return FrameBufferCellColor::WHITE;
            case CharacterType::KNIGHT:
                return FrameBufferCellColor::BLACK;
            case CharacterType::BLOCK:
                return FrameBufferCellColor::BLACK;
            default:
                return FrameBufferCellColor::BLACK;
        }
    }
    return FrameBufferCellColor::BLACK;
}
static FrameBufferCellColor GetBoardCellBackgroundColor(bool light, bool isValid)
{
    if(isValid)
    {
        if(light)
        {
            return FrameBufferCellColor::YELLOW;
        }
        else
        {
            return FrameBufferCellColor::BROWN;
        }
    }
    else
    {
        if(light)
        {
            return FrameBufferCellColor::LIGHT_GRAY;
        }
        else
        {
            return FrameBufferCellColor::DARK_GRAY;
        }
    }
}
void RoomState::Draw()
{
    auto board = _world.GetAvatar()->GetLocation().GetBoard();
    for(size_t column=0;column<board.GetColumns();++column)
    {
        for(size_t row=0;row<board.GetRows();++row)
        {
            auto location = *board.GetLocation(column, row);
            bool isValidMove = false;
            if(!location.GetCharacter().has_value())
            {
                for(auto knightMoveType : AllKnightMoveTypes)
                {
                    auto neighbor = location.GetNeighbor(knightMoveType);
                    if(neighbor.has_value())
                    {
                        auto character = neighbor->GetCharacter();
                        if(character.has_value())
                        {
                            if(character->GetCharacterType() == CharacterType::KNIGHT)
                            {
                                isValidMove = true;
                                break;
                            }
                        }
                    }
                }
            }
            _frameBuffer.Fill(
                column * BOARD_CELL_WIDTH, 
                row * BOARD_CELL_HEIGHT, 
                BOARD_CELL_WIDTH, 
                BOARD_CELL_HEIGHT, 
                0, 
                FrameBufferCellColor::BLACK, 
                GetBoardCellBackgroundColor(location.GetLight(), isValidMove));
            _frameBuffer.SetCell(column * BOARD_CELL_WIDTH + 1, row * BOARD_CELL_HEIGHT + 1, GetBoardCellCharacter(location), GetBoardCellForegroundColor(location), std::nullopt);
        }
    }
    //draw cursor
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH, y * BOARD_CELL_HEIGHT, 0xc9, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH + 1, y * BOARD_CELL_HEIGHT, 0xcd, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH + 1, y * BOARD_CELL_HEIGHT + 2, 0xcd, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH, y * BOARD_CELL_HEIGHT + 1, 0xba, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH + 2, y * BOARD_CELL_HEIGHT + 1, 0xba, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH + 2, y * BOARD_CELL_HEIGHT, 0xbb, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH + 2, y * BOARD_CELL_HEIGHT + 2, 0xbc, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH, y * BOARD_CELL_HEIGHT + 2, 0xc8, std::nullopt, std::nullopt);
}

std::optional<GameState> RoomState::Update()
{
    while(HandleCommand()){/* do nothing */}
    return GameState::ROOM;
}

bool RoomState::HandleCommand()
{
    auto command = _commandBuffer.Read();
    if(command.has_value())
    {
        switch(*command)
        {
            case CommandType::UP:
                y = (y > 0) ? (y - 1) : (y);
                break;
            case CommandType::DOWN:
                y = (y < World::BOARD_ROWS - 1) ? (y + 1) : (y);
                break;
            case CommandType::LEFT:
                x = (x > 0) ? (x - 1) : (x);
                break;
            case CommandType::RIGHT:
                x = (x < World::BOARD_COLUMNS - 1) ? (x + 1) : (x);
                break;
            case CommandType::GREEN:
                AttemptMove();
                break;
            default:
                //do nothing
                break;
        }
        return true;
    }
    return false;
}
void RoomState::AttemptMove()
{
    auto location = _world.GetAvatar()->GetLocation();
    auto cursorLocation = *location.GetBoard().GetLocation(x,y);
    for(auto knightMoveType : AllKnightMoveTypes)
    {
        auto destination = location.GetNeighbor(knightMoveType);
        if(destination.has_value() && !destination->GetCharacter().has_value() && destination->GetIndex() == cursorLocation.GetIndex())
        {
            Move(location, cursorLocation);
        }
    }
}
void RoomState::Move(Location location, Location cursorLocation)
{
    RemoveBlocks();
    auto character = *location.GetCharacter();
    _world.CreateCharacter(CharacterType::BLOCK, location);
    character.SetLocation(cursorLocation);
    cursorLocation.SetCharacter(character);
}
void RoomState::RemoveBlocks()
{
    auto board = _world.GetAvatar()->GetLocation().GetBoard();
    for(size_t column = 0; column < board.GetColumns(); ++column)
    {
        for(size_t row = 0; row < board.GetRows(); ++row)
        {
            auto location = *board.GetLocation(column, row);
            auto character = location.GetCharacter();
            if(character.has_value() && character->GetCharacterType() == CharacterType::BLOCK)
            {
                location.SetCharacter(std::nullopt);
                character->Recycle();
            }
        }
    }
}
