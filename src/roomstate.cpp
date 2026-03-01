#include "roomstate.h"
#include <format>
constexpr int BOARD_CELL_WIDTH = 3;
constexpr int BOARD_CELL_HEIGHT = 3;
static unsigned char GetBoardCellCharacter(const Location& location)
{
    auto character = location.GetCharacter();
    if(character)
    {
        switch(character->GetCharacterType())
        {
            case CharacterType::BUTTHOLE:
                return '*';
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
    if(character)
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
bool RoomState::IsValidMoveDestination(Location location)
{
    for(auto knightMoveType : AllKnightMoveTypes)
    {
        auto neighbor = location.GetNeighbor(knightMoveType);
        if(neighbor)
        {
            auto character = neighbor->GetCharacter();
            if(character)
            {
                if(character->GetCharacterType() == CharacterType::KNIGHT)
                {
                    return true;
                }
            }
        }
    }
    return false;
}
void RoomState::DrawBoard()
{
    auto avatar = *_world.GetAvatar();
    auto board = avatar.GetBoard();
    for(size_t column=0;column<board.GetColumns();++column)
    {
        for(size_t row=0;row<board.GetRows();++row)
        {
            auto location = *board.GetLocation(column, row);
            bool isValidMove = false;
            if(
                !location.GetCharacter() || 
                location.GetCharacter()->GetCharacterType() != CharacterType::BLOCK)
            {
                isValidMove = IsValidMoveDestination(location);
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
}
void RoomState::DrawCursorFrame()
{
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH, y * BOARD_CELL_HEIGHT, 0xc9, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH + 1, y * BOARD_CELL_HEIGHT, 0xcd, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH + 1, y * BOARD_CELL_HEIGHT + 2, 0xcd, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH, y * BOARD_CELL_HEIGHT + 1, 0xba, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH + 2, y * BOARD_CELL_HEIGHT + 1, 0xba, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH + 2, y * BOARD_CELL_HEIGHT, 0xbb, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH + 2, y * BOARD_CELL_HEIGHT + 2, 0xbc, std::nullopt, std::nullopt);
    _frameBuffer.SetCell(x * BOARD_CELL_WIDTH, y * BOARD_CELL_HEIGHT + 2, 0xc8, std::nullopt, std::nullopt);
}
void RoomState::DrawStats()
{
    auto avatar = *_world.GetAvatar();
    auto board = avatar.GetBoard();
    size_t text_column = board.GetColumns() * BOARD_CELL_WIDTH;
    _frameBuffer.Fill(
        text_column,
        size_t{0},
        _frameBuffer.GetColumns()-text_column,
        _frameBuffer.GetRows()-1,
        0,
        std::nullopt,FrameBufferCellColor::BLACK);
    size_t text_row = 0;
    _frameBuffer.WriteText(
        text_column, 
        text_row++, 
        std::format(
            "Buns: {}/{}", 
            *avatar.GetStatistic(StatisticType::SUPPLIES),
            avatar.GetStatisticMaximum(StatisticType::SUPPLIES)), 
        FrameBufferCellColor::MAGENTA, 
        std::nullopt);
    _frameBuffer.WriteText(
        text_column, 
        text_row++, 
        std::format(
            "Position: {}{}", 
            (char)('A'+x),
            (char)('1'+y)), 
        FrameBufferCellColor::CYAN, 
        std::nullopt);
    auto location= *board.GetLocation(x,y);
    if(IsValidMoveDestination(location))
    {
        _frameBuffer.WriteText(
            text_column, 
            text_row++, 
            std::format(
                "Valid Move"), 
            FrameBufferCellColor::GREEN, 
            std::nullopt);
    }
    auto character = location.GetCharacter();
    if(character)
    {
        switch(character->GetCharacterType())
        {
            case CharacterType::BLOCK:
                _frameBuffer.WriteText(
                    text_column, 
                    text_row++, 
                    std::format(
                        "Blocked!"), 
                    FrameBufferCellColor::RED, 
                    std::nullopt);
                break;
            case CharacterType::BUTTHOLE:
                _frameBuffer.WriteText(
                    text_column, 
                    text_row++, 
                    std::format(
                        "Butthole"), 
                    FrameBufferCellColor::BROWN, 
                    std::nullopt);
                _frameBuffer.WriteText(
                    text_column, 
                    text_row++, 
                    std::format(
                        "(check it!)"), 
                    FrameBufferCellColor::BROWN, 
                    std::nullopt);
                break;
            case CharacterType::KNIGHT:
                _frameBuffer.WriteText(
                    text_column, 
                    text_row++, 
                    std::format(
                        "Knight(you)"), 
                    FrameBufferCellColor::DARK_GRAY, 
                    std::nullopt);
                break;
            case CharacterType::STICKY_BUNS:
                _frameBuffer.WriteText(
                    text_column, 
                    text_row++, 
                    std::format(
                        "+5 Buns"), 
                    FrameBufferCellColor::WHITE, 
                    std::nullopt);
                break;
            default:
                //do nothing
                break;
        }
    }
}
void RoomState::DrawStatusBar()
{

}
void RoomState::Draw()
{
    DrawBoard();
    DrawCursorFrame();
    DrawStats();
}

std::optional<GameState> RoomState::Update()
{
    while(HandleCommand()){/* do nothing */}
    return GameState::ROOM;
}

bool RoomState::HandleCommand()
{
    auto command = _commandBuffer.Read();
    if(command)
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
        if(
            destination && 
            !(destination->GetCharacter() && destination->GetCharacter()->GetCharacterType() == CharacterType::BLOCK) && 
            destination->GetIndex() == cursorLocation.GetIndex())
        {
            Move(location, cursorLocation);
        }
    }
}
void RoomState::ConsumeStickyBuns(Character& character, Character& otherCharacter)
{
    constexpr int SUPPLIES_INCREASE = 5;
    character.SetStatistic(StatisticType::SUPPLIES, *character.GetStatistic(StatisticType::SUPPLIES) + SUPPLIES_INCREASE);
    auto board = character.GetBoard();
    _world.SpawnCharacter(board, CharacterType::STICKY_BUNS);
    otherCharacter.Recycle();
}
void RoomState::CheckButthole(Character& character, Character& otherCharacter)
{
    auto board = character.GetBoard();
    _world.SpawnCharacter(board, CharacterType::BUTTHOLE);
    otherCharacter.Recycle();
}
void RoomState::Move(Location location, Location cursorLocation)
{
    RemoveBlocks();
    auto character = *location.GetCharacter();
    character.SetStatistic(StatisticType::SUPPLIES, *character.GetStatistic(StatisticType::SUPPLIES) - 1);
    _world.CreateCharacter(CharacterType::BLOCK, location);
    auto otherCharacter = cursorLocation.GetCharacter();
    character.SetLocation(cursorLocation);
    cursorLocation.SetCharacter(character);
    if(otherCharacter)
    {
        switch(otherCharacter->GetCharacterType())
        {
            case CharacterType::STICKY_BUNS:
                ConsumeStickyBuns(character, *otherCharacter);
                break;
            case CharacterType::BUTTHOLE:
                CheckButthole(character, *otherCharacter);
                break;
            default:
                //do nothing!
                break;
        }
    }
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
            if(character && character->GetCharacterType() == CharacterType::BLOCK)
            {
                location.SetCharacter(std::nullopt);
                character->Recycle();
            }
        }
    }
}
