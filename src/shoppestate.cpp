#include "shoppestate.h"
#include <ranges>
#include <format>
bool ShoppeState::HandleCommand(GameState& gameState)
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
int ShoppeState::SupplyIncreasePrice() const
{
    auto avatar = *_world.GetAvatar();
    return avatar.GetStatisticMaximum(StatisticType::SUPPLIES) * avatar.GetStatistic(StatisticType::XP_LEVEL).value();
}
int ShoppeState::PotionPrice() const
{
    constexpr int BASE_PRICE = 10;
    auto avatar = *_world.GetAvatar();
    return BASE_PRICE * avatar.GetStatistic(StatisticType::XP_LEVEL).value();
}
int ShoppeState::FloggerPrice() const
{
    constexpr int BASE_PRICE = 25;
    auto avatar = *_world.GetAvatar();
    return BASE_PRICE * avatar.GetStatistic(StatisticType::XP_LEVEL).value();
}
int ShoppeState::ArmorPrice() const
{
    auto avatar = *_world.GetAvatar();
    return (1+avatar.GetStatistic(StatisticType::ARMOUR).value_or(0)) * avatar.GetStatistic(StatisticType::XP_LEVEL).value();
}
int ShoppeState::SprayPrice() const
{
    constexpr int BASE_PRICE = 1;
    auto avatar = *_world.GetAvatar();
    return BASE_PRICE * avatar.GetStatistic(StatisticType::XP_LEVEL).value();
}
void ShoppeState::InitializeMenuItems()
{
    if(!_menuItems.empty())
    {
        return;
    }
    _menuItems.emplace_back(ShoppeMenuStateItem::LEAVE, "Leave");
    auto avatar = *_world.GetAvatar();
    auto jools = avatar.GetStatistic(StatisticType::JOOLS).value_or(0);
    auto price = SupplyIncreasePrice();
    if(jools >= price)
    {
        _menuItems.emplace_back(ShoppeMenuStateItem::INCREASE_MAXIMUM_SUPPLIES, std::format("+1 Maximum Buns({})",price));
    }
    _menuItemIndex = 0;
}
void ShoppeState::PreviousMenuItem()
{
    _menuItemIndex = (_menuItemIndex + _menuItems.size() - 1) % _menuItems.size();
}
void ShoppeState::NextMenuItem()
{
    _menuItemIndex = (_menuItemIndex + 1) % _menuItems.size();
}
void ShoppeState::ChooseMenuItem(GameState& gameState)
{
    auto menuItem = std::get<0>(_menuItems[_menuItemIndex]);
    switch(menuItem)
    {
        case ShoppeMenuStateItem::LEAVE:
            Leave(gameState);
            break;
        case ShoppeMenuStateItem::INCREASE_MAXIMUM_SUPPLIES:
            IncreaseMaximumSupplies();
            break;
        case ShoppeMenuStateItem::BUY_ANTITRAP_SPRAY:
            BuyAntitrapSpray(gameState);
            break;
        case ShoppeMenuStateItem::BUY_ARMOUR:
            BuyArmor(gameState);
            break;
        case ShoppeMenuStateItem::BUY_FLOGGER:
            BuyFlogger(gameState);
            break;
        case ShoppeMenuStateItem::BUY_POTION:
            BuyPotion(gameState);
            break;
        default:
            break;
    }
}
void ShoppeState::Leave(GameState& gameState)
{
    gameState = GameState::ROOM;
}
void ShoppeState::IncreaseMaximumSupplies()
{
    _world.GetAvatar()->ChangeStatistic(StatisticType::JOOLS, -SupplyIncreasePrice());
    //TODO: actually increase max supplies
    _menuItems.clear();
}
void ShoppeState::BuyPotion(GameState& gameState)
{

}
void ShoppeState::BuyFlogger(GameState& gameState)
{

}
void ShoppeState::BuyArmor(GameState& gameState)
{

}
void ShoppeState::BuyAntitrapSpray(GameState& gameState)
{

}
void ShoppeState::Draw()
{
    _frameBuffer.Fill(
        size_t{0}, 
        size_t{0}, 
        _frameBuffer.GetColumns(), 
        _frameBuffer.GetRows(), 
        0, 
        FrameBufferCellColor::BLACK, 
        FrameBufferCellColor::BLACK);
    _frameBuffer.WriteText(0,0,"Shoppe:",FrameBufferCellColor::WHITE,std::nullopt);
    _frameBuffer.WriteText(0,1,std::format("Jools: {}",_world.GetAvatar()->GetStatistic(StatisticType::JOOLS).value_or(0)),FrameBufferCellColor::LIGHT_GREEN,std::nullopt);
    size_t row = size_t{3};
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
std::optional<GameState> ShoppeState::Update()
{
    GameState gameState = GameState::SHOPPE;
    InitializeMenuItems();
    while(HandleCommand(gameState) && gameState == GameState::SHOPPE){/* do nothing */}
    return gameState;
}
