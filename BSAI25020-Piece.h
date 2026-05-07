#pragma once
#include "BSAI25020-Enums.h"

class Piece {
private:
    int pieceNo;
    Colour colour;
    int position;
    PieceState state;
    int startPos;
    int homeColEntry;
    int homeColBase;
    int baseCell;

public:
    Piece();
    Piece(int no, Colour c);
    int getPosition() const;
    int getPieceN() const;
    Colour getColour() const;
    PieceState getState() const;
    int getStartPos() const;
    int getHomeColEntry() const;
    int getHomeColBase() const;
    int getBaseCell() const;
    void setState(PieceState s);
    void setPosition(int pos);
    bool isFinished() const;
    void reset();
};
