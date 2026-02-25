#pragma once
#include "board.h"
#include <string>
class Thing
{
private:
    Board board;
    void DrawBoard() const;
    bool ProcessCommand(const std::string& command);
    void ShowStats() const;
    void Inspect(int column, int row) const;
public:
    void Initialize();
    void Run();
    void CleanUp();
};