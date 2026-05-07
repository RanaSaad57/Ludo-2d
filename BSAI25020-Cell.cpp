#include "BSAI25020-Cell.h"
#include "BSAI25020-Piece.h"
using namespace std;

Cell::Cell(): cellId(-1), isSafe(false), isHomeColumn(false), homeColourOwner(LDO_NONE), isCentre(false), isBase(false) {}
Cell::Cell(int id, bool safe, bool homeCol, Colour owner, bool centre, bool base)
    : cellId(id), isSafe(safe), isHomeColumn(homeCol), homeColourOwner(owner), isCentre(centre), isBase(base) {}
int Cell::getCellId() const { return cellId; }
bool Cell::isSafeSquare() const { return isSafe; }
bool Cell::getIsHomeColumn() const { return isHomeColumn; }
bool Cell::getIsCentre() const { return isCentre; }
bool Cell::getIsBase() const { return isBase; }
Colour Cell::getHomeColourOwner() const { return homeColourOwner; }
bool Cell::isEmpty() const { return pieces.empty(); }
int Cell::getPieceCount() const { return (int)pieces.size(); }
bool Cell::canEnter(Colour c) const { return (isHomeColumn) ? homeColourOwner == c : true; }
void Cell::addPiece(Piece* p) { if (p) pieces.push_back(p); }
void Cell::removePiece(Piece* p) {
    for (int i = 0; i < (int)pieces.size(); i++)
        if (pieces[i] == p) { pieces.erase(pieces.begin() + i); return; }
}
const vector<Piece*>& Cell::getPieces() const { return pieces; }
vector<Piece*>& Cell::getPieces() { return pieces; }
