#include "gamemenustate.h"
void GameMenuState::Draw()
{
    _frameBuffer.Fill(
        size_t{0}, 
        size_t{0}, 
        _frameBuffer.GetColumns(), 
        _frameBuffer.GetRows(), 
        0, 
        FrameBufferCellColor::BLACK, 
        FrameBufferCellColor::BLACK);
    _frameBuffer.WriteText(0,0,"Game Menu:",FrameBufferCellColor::WHITE,std::nullopt);
}
std::optional<GameState> GameMenuState::Update()
{
    GameState gameState = GameState::GAME_MENU;
    while(HandleCommand(gameState) && gameState == GameState::GAME_MENU){/* do nothing */}
    return gameState;
}
bool GameMenuState::HandleCommand(GameState& gameState)
{
    auto command = _commandBuffer.Read();
    if(command)
    {
        switch(*command)
        {
            case CommandType::YELLOW:
                gameState = GameState::ROOM;
                break;
            default:
                break;
        }
        return true;
    }
    return false;
}
