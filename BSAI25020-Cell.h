#pragma once
#include <vector>
#include "BSAI25020-Enums.h"
using namespace std;

class Piece;
class Cell {
private:
    int cellId;
    bool isSafe;
    bool isHomeColumn;
    Colour homeColourOwner;
    bool isCentre;
    bool isBase;
    vector<Piece*> pieces;
public:
    Cell();
    Cell(int id, bool safe, bool homeCol, Colour owner, bool centre, bool base);
    int getCellId() const;
    bool isSafeSquare()const;
    bool canEnter(Colour c) const;
    void addPiece(Piece* p);
    void removePiece(Piece* p);
    const vector<Piece*>& getPieces() const;
    vector<Piece*>& getPieces();
    bool getIsHomeColumn() const;
    bool getIsCentre() const;
    bool getIsBase() const;
    Colour getHomeColourOwner() const;
    bool isEmpty() const;
    int getPieceCount() const;
};
