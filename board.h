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
    void RemoveBlocks();
    bool SpawnToken(const TokenType& tokenType);
    void ConsumeStickyBuns();
    void ConsumeChesst();
public:
    static constexpr size_t BOARD_WIDTH = 8;
    static constexpr size_t BOARD_HEIGHT = 8;
    void Initialize();
    void CleanUp();
    BoardCell& GetCell(size_t column, size_t row);
    const BoardCell& GetCell(size_t column, size_t row) const;
    void Move(int deltaX, int deltaY);
    size_t GetMoves() const;
    size_t GetSupplies() const;
    size_t GetMaximumSupplies() const;
};