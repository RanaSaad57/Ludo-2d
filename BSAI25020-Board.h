#pragma once
#include <vector>
#include "BSAI25020-Cell.h"
#include "BSAI25020-Enums.h"
using namespace std;

class Piece;
class Board {
private:
    vector<Cell> cells;
    int cellCount;
    const int safePos[8] = {0, 8, 13, 21, 26, 34, 39, 47};
    const int homeColBase[4] = {52, 58, 64, 70};
    const int baseStart[4] = {78, 82, 86, 90};
    void initBoard();
    void markSafeSquares();
    void markHomeColumns();
    void markBaseCells();

public:
    Board();
    Cell* getCell(int id);
    const Cell* getCell(int id) const;
    bool isSafeSquare(int pos) const;
    int getDestination(int currentCellId, int roll, const Piece* piece) const;
    int getCellCount() const;
    int getCentreId() const;
};
