#include "BSAI25020-Piece.h"
using namespace std;
Piece::Piece() : pieceNo(0), colour(LDO_NONE), position(-1), state(PS_BASE),
    startPos(-1), homeColEntry(-1), homeColBase(-1), baseCell(-1) {}
Piece::Piece(int no, Colour c) : pieceNo(no), colour(c), state(PS_BASE) {
    if (c == LDO_RED) { startPos = 0; homeColEntry = 50; homeColBase = 52; baseCell = 78 + no; }
    else if (c == LDO_GREEN) { startPos = 13; homeColEntry = 11; homeColBase = 58; baseCell = 82 + no; }
    else if (c == LDO_YELLOW) { startPos = 26; homeColEntry = 24; homeColBase = 64; baseCell = 86 + no; }
    else if (c == LDO_BLUE) { startPos = 39; homeColEntry = 37; homeColBase = 70; baseCell = 90 + no; }
    else { startPos = -1; homeColEntry = -1; homeColBase = -1; baseCell = -1; }
    position = baseCell;
}
int Piece::getPosition() const { return position; }
int Piece::getPieceN() const { return pieceNo; }
Colour Piece::getColour() const { return colour; }
PieceState Piece::getState() const { return state; }
int Piece::getStartPos() const { return startPos; }
int Piece::getHomeColEntry() const { return homeColEntry; }
int Piece::getHomeColBase() const { return homeColBase; }
int Piece::getBaseCell() const { return baseCell; }
void Piece::setState(PieceState s) { state = s; }
void Piece::setPosition(int pos) { position = pos; }
bool Piece::isFinished() const { return state == PS_FINISHED; }
void Piece::reset() { position = baseCell; state = PS_BASE; }
