#pragma once
#include "boardcell.h"
#include <vector>
#include <optional>
class Board
{
private:
    std::vector<std::vector<BoardCell>> cells;
    size_t moves;
    size_t supplies;
    size_t maximum_supplies;
    size_t jools;
    size_t health;
    size_t maximum_health;
    void RemoveBlocks();
    bool SpawnToken(const TokenType& tokenType);
    void ConsumeStickyBuns();
    void ConsumeChesst();
    void AddJools();
    void TriggerTrap();
public:
    static constexpr size_t BOARD_WIDTH = 8;
    static constexpr size_t BOARD_HEIGHT = 8;
    void Initialize();
    void CleanUp();
    BoardCell& GetCell(size_t column, size_t row);
    const BoardCell& GetCell(size_t column, size_t row) const;
    void Move(int deltaX, int deltaY);
    size_t GetMoves() const { return moves; }
    size_t GetSupplies() const { return supplies; }
    size_t GetMaximumSupplies() const { return maximum_supplies; }
    size_t GetJools() const { return jools; }
    size_t GetHealth() const { return health; }
    size_t GetMaximumHealth() const { return maximum_health; }
};