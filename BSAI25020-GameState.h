#pragma once
#include "BSAI25020-Enums.h"
using namespace std;

static const int MAX_PLAYERS = 4;
static const int PIECES_PER_PLAYER = 4;

struct GameState {
    int piecePositions[MAX_PLAYERS][PIECES_PER_PLAYER];
    PieceState pieceStates[MAX_PLAYERS][PIECES_PER_PLAYER];
    int currentPlayerIndex;
    int consecutiveSixes;
    int moveNumber;
    GameState();
    GameState(const GameState& o);
    GameState& operator=(const GameState& o);
};
