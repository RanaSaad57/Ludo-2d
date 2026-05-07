#include "BSAI25020-Player.h"
using namespace std;
Player::Player() : name(""), colour(LDO_NONE) {
    for (int i = 0; i < MAX_PIECES; i++) pieces[i] = Piece(i, LDO_NONE);
}
Player::Player(const string& n, Colour c) : name(n), colour(c) {
    for (int i = 0; i < MAX_PIECES; i++) pieces[i] = Piece(i, c);
}
string Player::getName() const { return name; }
Colour Player::getColour() const { return colour; }
Piece* Player::getPiece(int i) {
    if (i < 0 || i >= MAX_PIECES) return nullptr;
    return &pieces[i];
}
const Piece* Player::getPiece(int i) const {
    if (i < 0 || i >= MAX_PIECES) return nullptr;
    return &pieces[i];
}
bool Player::allFinished() const {
    for (int i = 0; i < MAX_PIECES; i++)
        if (!pieces[i].isFinished()) return false;
    return true;
}
int Player::getActivePieceCount() const {
    int cnt = 0;
    for (int i = 0; i < MAX_PIECES; i++) {
        PieceState s = pieces[i].getState();
        if (s == PS_ACTIVE || s == PS_HOMECOL) cnt++;
    }
    return cnt;
}
