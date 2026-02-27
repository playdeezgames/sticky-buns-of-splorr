#include "roomstate.h"
constexpr int BOARD_CELL_WIDTH = 3;
constexpr int BOARD_CELL_HEIGHT = 3;
constexpr int BOARD_WIDTH = BOARD_CELL_WIDTH * World::BOARD_COLUMNS;
constexpr int BOARD_HEIGHT = BOARD_CELL_HEIGHT * World::BOARD_ROWS;
void RoomState::Draw()
{
    //draw board
    for(size_t column=0;column<BOARD_WIDTH;++column)
    {
        for(size_t row = 0; row<BOARD_HEIGHT;++row)
        {
            _frameBuffer.SetCell(
                column, 
                row, 
                0, 
                FrameBufferCellColor::BLACK, 
                (((column/BOARD_CELL_WIDTH)+(row/BOARD_CELL_HEIGHT))%2==0)
                    ?
                (FrameBufferCellColor::LIGHT_GRAY)
                    :
                (FrameBufferCellColor::DARK_GRAY));
        }
    }
    auto board = _world.GetAvatar()->GetLocation().GetBoard();
    for(size_t column=0;column<board.GetColumns();++column)
    {
        for(size_t row=0;row<board.GetRows();++row)
        {
            auto location = *board.GetLocation(column, row);
            auto character = location.GetCharacter();
            if(character.has_value())
            {
                switch(character->GetCharacterType())
                {
                    case CharacterType::KNIGHT:
                        _frameBuffer.SetCell(column * BOARD_CELL_WIDTH + 1, row * BOARD_CELL_HEIGHT + 1, 'K', FrameBufferCellColor::BLACK, std::nullopt);
                        break;
                    default:
                        //do nothing!
                        break;
                }
            }
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
            default:
                //do nothing
                break;
        }
        return true;
    }
    return false;
}
