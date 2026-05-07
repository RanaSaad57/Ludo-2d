#pragma once
#include <string>
#include "BSAI25020-Piece.h"
using namespace std;

class Player {
private:
    string name;
    Colour colour;
    Piece pieces[MAX_PIECES];

public:
    Player();
    Player(const string& n, Colour c);
    string getName() const;
    Colour getColour() const;
    Piece* getPiece(int i);
    const Piece* getPiece(int i) const;
    bool allFinished() const;
    int getActivePieceCount() const;
};
