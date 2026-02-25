#pragma once
#include "celltype.h"
class BoardCell
{
private:
    CellType _cellType;
    BoardCell();
public:
    BoardCell(CellType cellType):_cellType(cellType){}
    CellType GetCellType() const {return _cellType;}
    void SetCellType(CellType cellType) {_cellType = cellType;}
};