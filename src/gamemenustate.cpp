#include "gamemenustate.h"
#include <ranges>
#include <format>
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
    size_t row = size_t{2};
    for(size_t index: std::views::iota(size_t{0}, _menuItems.size()))
    {
        FrameBufferCellColor foreground = (index==_menuItemIndex)?(FrameBufferCellColor::BLACK):(FrameBufferCellColor::WHITE);
        FrameBufferCellColor background = (index==_menuItemIndex)?(FrameBufferCellColor::WHITE):(FrameBufferCellColor::BLACK);
        const auto& menuItem = _menuItems[index];
        _frameBuffer.WriteText(
            0,
            row++,
            std::get<1>(menuItem),
            foreground,
            background);
    }
}
void GameMenuState::InitializeMenuItems()
{
    if(!_menuItems.empty())
    {
        return;
    }
    _menuItems.emplace_back(GameMenuStateItem::CONTINUE, "Continue");
    _menuItems.emplace_back(GameMenuStateItem::RESTART, "Restart");
    auto avatar = *_world.GetAvatar();
    if(*avatar.GetStatistic(StatisticType::HEALTH) > 0)
    {
        if(*avatar.GetStatistic(StatisticType::POTIONS) > 0)
        {
            _menuItems.emplace_back(GameMenuStateItem::USE_POTION, "Use Potion");
        }
    }
    _menuItemIndex = 0;
}
std::optional<GameState> GameMenuState::Update()
{
    GameState gameState = GameState::GAME_MENU;
    InitializeMenuItems();
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
            case CommandType::UP:
                PreviousMenuItem();
                break;
            case CommandType::DOWN:
                NextMenuItem();
                break;
            case CommandType::GREEN:
                ChooseMenuItem(gameState);
                break;
            default:
                break;
        }
        return true;
    }
    return false;
}
void GameMenuState::PreviousMenuItem()
{
    _menuItemIndex = (_menuItemIndex + _menuItems.size() - 1) % _menuItems.size();
}
void GameMenuState::NextMenuItem()
{
    _menuItemIndex = (_menuItemIndex + 1) % _menuItems.size();
}
void GameMenuState::ChooseMenuItem(GameState& gameState)
{
    auto menuItem = std::get<0>(_menuItems[_menuItemIndex]);
    switch(menuItem)
    {
        case GameMenuStateItem::CONTINUE:
            ContinueGame(gameState);
            break;
        case GameMenuStateItem::RESTART:
            RestartGame(gameState);
            break;
        case GameMenuStateItem::USE_POTION:
            UsePotion(gameState);
            break;
        default:
            break;
    }
}
void GameMenuState::UsePotion(GameState& gameState)
{
    auto avatar = *_world.GetAvatar();
    auto potionBenefit = avatar.GetStatisticMaximum(StatisticType::HEALTH) - *avatar.GetStatistic(StatisticType::HEALTH);
    avatar.ChangeStatistic(StatisticType::HEALTH, potionBenefit);
    avatar.ChangeStatistic(StatisticType::POTIONS, -1);
    _world.AddMessage(std::format("+{} Health", potionBenefit), FrameBufferCellColor::LIGHT_RED, FrameBufferCellColor::BLACK);
    _world.AddMessage("-1 Potion", FrameBufferCellColor::RED, FrameBufferCellColor::BLACK);
    _menuItems.clear();
    gameState = GameState::ROOM;
}
void GameMenuState::ContinueGame(GameState& gameState)
{
    _menuItems.clear();
    gameState = GameState::ROOM;
}
void GameMenuState::RestartGame(GameState& gameState)
{
    _world.Initialize();
    ContinueGame(gameState);
}
