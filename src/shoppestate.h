#pragma once
#include "state.h"
#include "world.h"
#include <vector>
#include <tuple>
#include <string>
enum class ShoppeMenuStateItem 
{
    LEAVE,
    BUY_POTION,
    INCREASE_MAXIMUM_SUPPLIES,
    BUY_FLOGGER,
    BUY_ARMOUR,
    BUY_ANTITRAP_SPRAY
};
class ShoppeState: public State
{
private:
    World& _world;
    std::vector<std::tuple<ShoppeMenuStateItem, std::string>> _menuItems;
    size_t _menuItemIndex;
    ShoppeState();
    bool HandleCommand(GameState& gameState);
    void InitializeMenuItems();
    void PreviousMenuItem();
    void NextMenuItem();
    void ChooseMenuItem(GameState& gameState);
    void Leave(GameState& gameState);
    void IncreaseMaximumSupplies();
    void BuyPotion(GameState& gameState);
    void BuyFlogger(GameState& gameState);
    void BuyArmor(GameState& gameState);
    void BuyAntitrapSpray(GameState& gameState);
    int SupplyIncreasePrice() const;
    int PotionPrice() const;
    int FloggerPrice() const;
    int ArmorPrice() const;
    int SprayPrice() const;
public:
    ShoppeState(
        FrameBuffer& frameBuffer, 
        CommandBuffer& commandBuffer,
        World& world)
        : State(frameBuffer, commandBuffer)
        , _world(world)
        , _menuItems()
        , _menuItemIndex(0)
        {

        }
    void Draw() override;
    std::optional<GameState> Update() override;
};