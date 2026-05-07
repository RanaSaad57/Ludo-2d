#pragma once
#include <string>
#include "BSAI25020-Enums.h"
#include "BSAI25020-History.h"
#include "BSAI25020-GameState.h"
using namespace std;

class Piece;
class Board;
class Player;
class Helpers {
public:
    Helpers();
    static bool canOpen(int roll);
    static bool isValidMove(const Piece* t, int roll, const Board* b);
    static bool hasAnyValidMove(const Player* p, int roll, const Board* b);
    void save(const History& hs, const GameState& currentState, int playerCount,
              const Player players[], const string& filename) const;
    bool load(const string& filename, History& hs, GameState& initialState,
              int& playerCount, string playerNames[], Colour playerColours[]) const;
};
