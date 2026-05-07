#pragma once
#include <string>
#include <vector>
#include "BSAI25020-Enums.h"
#include "BSAI25020-Board.h"
#include "BSAI25020-Dice.h"
#include "BSAI25020-History.h"
#include "BSAI25020-GameState.h"
#include "BSAI25020-Helpers.h"
#include "BSAI25020-Player.h"
using namespace std;

class Game {
private:
    static const int MAX_P = 4;
    Player players[MAX_P];
    int playerCount;
    Board board;
    Dice dice;
    History history;
    GameState initialState;
    int currentPlayer;
    bool replayMode;
    AppState appstate;
    vector<GameState> Gamestates;
    Helpers helpers;
    void applyState(const GameState& gs);
    GameState captureState() const;
    void advancePlayer();
    bool isPlayerActive(int idx) const;
    bool checkCapture_return(int pos, int moverIdx, int& capPNo, int& capPId);

public:
    Game();
    void startGame(int count);
    void startGame(int count, const string names[]);
    void movePiece(int moverIdx, int rollVal, int pieceIdx = -1);
    int rollDice();
    void resetDice();
    void forfeitTurn();
    bool checkWin();
    void undoMove();
    void redoMove();
    void startReplay();
    void stepReplayForward();
    void stepReplayBackward();
    void saveGame(const string& filename);
    void loadGame(const string& filename);
    void changeAppState(AppState ap);
    AppState getAppState() const;
    int getCurrentPlayer() const;
    Player* getPlayer(int idx);
    const Player* getPlayer(int idx) const;
    int getPlayerCount() const;
    const Board* getBoard() const;
    const Dice* getDice() const;
    const History* getHistory() const;
    bool isReplayMode() const;
};
