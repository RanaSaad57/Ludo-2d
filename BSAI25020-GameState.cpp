#include "BSAI25020-GameState.h"
using namespace std;
GameState::GameState() : currentPlayerIndex(0), consecutiveSixes(0), moveNumber(0) {
    for (int p = 0; p < MAX_PLAYERS; p++)
        for (int i = 0; i < PIECES_PER_PLAYER; i++) {
            piecePositions[p][i] = -1;
            pieceStates[p][i] = PS_BASE;
        }
}
GameState::GameState(const GameState& o) {
    currentPlayerIndex = o.currentPlayerIndex;
    consecutiveSixes = o.consecutiveSixes;
    moveNumber = o.moveNumber;
    for (int p = 0; p < MAX_PLAYERS; p++)
        for (int i = 0; i < PIECES_PER_PLAYER; i++) {
            piecePositions[p][i] = o.piecePositions[p][i];
            pieceStates[p][i] = o.pieceStates[p][i];
        }
}
GameState& GameState::operator=(const GameState& o) {
    if (this == &o) return *this;
    currentPlayerIndex = o.currentPlayerIndex;
    consecutiveSixes = o.consecutiveSixes;
    moveNumber = o.moveNumber;
    for (int p = 0; p < MAX_PLAYERS; p++)
        for (int i = 0; i < PIECES_PER_PLAYER; i++) {
            piecePositions[p][i] = o.piecePositions[p][i];
            pieceStates[p][i] = o.pieceStates[p][i];
        }
    return *this;
}
