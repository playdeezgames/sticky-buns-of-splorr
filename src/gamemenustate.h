#pragma once
#include "state.h"
#include "world.h"
#include <vector>
#include <tuple>
#include <string>
enum class GameMenuStateItem 
{
    CONTINUE,
    RESTART,
    USE_POTION
};
class GameMenuState: public State
{
private:
    World& _world;
    std::vector<std::tuple<GameMenuStateItem, std::string>> _menuItems;
    size_t _menuItemIndex;
    GameMenuState();
    bool HandleCommand(GameState& gameState);
    void InitializeMenuItems();
    void PreviousMenuItem();
    void NextMenuItem();
    void ChooseMenuItem(GameState& gameState);
    void ContinueGame(GameState& gameState);
    void RestartGame(GameState& gameState);
    void UsePotion(GameState& gameState);
public:
    GameMenuState(
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