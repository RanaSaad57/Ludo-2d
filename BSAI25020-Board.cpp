#include "BSAI25020-Board.h"
#include "BSAI25020-Piece.h"
using namespace std;
Board::Board() : cellCount(TOTAL_CELLS) {
    cells.resize(TOTAL_CELLS); initBoard();
}
void Board::initBoard() {
    for (int i = 0; i < TRACK_SIZE; i++)
        cells[i] = Cell(i, false, false, LDO_NONE, false, false);
    markSafeSquares();
    markHomeColumns();
    markBaseCells();
    cells[CENTRE_ID] = Cell(CENTRE_ID, true, false, LDO_NONE, true, false);
}
void Board::markSafeSquares() {
    for (int i = 0; i < 8; i++) {
        int id = safePos[i];
        cells[id] = Cell(id, true, false, LDO_NONE, false, false);
    }
}
void Board::markHomeColumns() {
    Colour cols[4] = { LDO_RED, LDO_GREEN, LDO_YELLOW, LDO_BLUE };
    for (int c = 0; c < MAX_COLOURS; c++) {
        for (int j = 0; j < HOME_COL_LEN; j++) {
            int id = homeColBase[c] + j;
            cells[id] = Cell(id, true, true, cols[c], false, false);
        }
    }
}
void Board::markBaseCells() {
    Colour cols[4] = { LDO_RED, LDO_GREEN, LDO_YELLOW, LDO_BLUE };
    for (int c = 0; c < MAX_COLOURS; c++) {
        for (int j = 0; j < MAX_PIECES; j++) {
            int id = baseStart[c] + j;
            cells[id] = Cell(id, true, false, cols[c], false, true);
        }
    }
}
Cell* Board::getCell(int id) {
    if (id < 0 || id >= TOTAL_CELLS) return nullptr; return &cells[id];
}
const Cell* Board::getCell(int id) const {
    if (id < 0 || id >= TOTAL_CELLS) return nullptr; return &cells[id];
}
bool Board::isSafeSquare(int pos) const {
    const Cell* c = getCell(pos);
    return c != nullptr && c->isSafeSquare();
}
int Board::getDestination(int curId, int roll, const Piece* piece) const {
    if (!piece || curId < 0) return -1;
    int hcBase = piece->getHomeColBase();
    int hcEntry = piece->getHomeColEntry();

    if (curId >= hcBase && curId < hcBase + HOME_COL_LEN) {
        int posInHC = curId - hcBase;
        int target = posInHC + roll;
        if (target == HOME_COL_LEN) return CENTRE_ID;
        else if (target < HOME_COL_LEN) return hcBase + target;
        else return -1;
    }
    if (curId == CENTRE_ID) return -1;
    int distToEntry = (hcEntry - curId + TRACK_SIZE) % TRACK_SIZE + 1;
    if (roll < distToEntry)
        return (curId + roll) % TRACK_SIZE;
    else if (roll == distToEntry)
        return hcBase;
    else {
        int over = roll - distToEntry;
        if (over == HOME_COL_LEN) return CENTRE_ID;
        else if (over < HOME_COL_LEN) return hcBase + over;
        else return -1;
    }
}
int Board::getCellCount() const { return cellCount; }
int Board::getCentreId() const { return CENTRE_ID; }
