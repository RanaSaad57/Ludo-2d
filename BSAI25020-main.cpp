#include "raylib.h"
#include "BSAI25020-Game.h"
#include "BSAI25020-Displaying.h"
#include "BSAI25020-Helpers.h"
#include "BSAI25020-Player.h"
#include <string>
using namespace std;

void RunGame() {
    const int scrW = 1180;
    const int scrH = 900;
    const int cell = 54;

    InitWindow(scrW, scrH, "Ludo - BSAI25020");
    SetTargetFPS(60);

    Game game;
    Displaying disp(scrW, scrH, cell);

    int selPlayer = -1;
    int selPiece = -1;
    int lastRoll = 0;
    bool rolled = false;
    bool needPick = false;
    bool enteringNames = false;
    bool diceAnimating = false;
    int setupCount = 0;
    int setupIndex = 0;
    string setupNames[4];
    string nameBuffer;
    double diceAnimEnd = 0.0;
    double diceAnimNext = 0.0;

    while (!WindowShouldClose()) {
        AppState st = game.getAppState();
        int curIdx = game.getCurrentPlayer();

        if (st == AS_MENU) {
            if (!enteringNames) {
                if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_THREE)) {
                    setupCount = IsKeyPressed(KEY_ONE) ? 2 : (IsKeyPressed(KEY_TWO) ? 3 : 4);
                    setupIndex = 0;
                    nameBuffer.clear();
                    for (int i = 0; i < 4; i++) setupNames[i].clear();
                    enteringNames = true;
                }
                if (IsKeyPressed(KEY_FOUR)) {
                    game.loadGame("save.txt");
                    lastRoll = 0;
                    rolled = false;
                    needPick = false;
                    selPiece = -1;
                    selPlayer = -1;
                }
            } else {
                int ch = GetCharPressed();
                while (ch > 0) {
                    if (ch >= 32 && ch <= 126 && nameBuffer.size() < 18) nameBuffer.push_back((char)ch);
                    ch = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE) && !nameBuffer.empty()) nameBuffer.pop_back();
                if (IsKeyPressed(KEY_ENTER)) {
                    setupNames[setupIndex] = nameBuffer;
                    setupIndex++;
                    nameBuffer.clear();
                    if (setupIndex >= setupCount) {
                        game.startGame(setupCount, setupNames);
                        enteringNames = false;
                        lastRoll = 0;
                        rolled = false;
                        needPick = false;
                        selPiece = -1;
                        selPlayer = -1;
                    }
                }
                if (IsKeyPressed(KEY_ESCAPE)) {
                    enteringNames = false;
                    nameBuffer.clear();
                }
            }
        }
        else if (st == AS_PLAYING) {
            if (IsKeyPressed(KEY_U)) { game.undoMove(); rolled = false; needPick = false; }
            if (IsKeyPressed(KEY_R)) { game.redoMove(); rolled = false; needPick = false; }
            if (IsKeyPressed(KEY_S)) game.saveGame("save.txt");
            if (IsKeyPressed(KEY_L)) { game.loadGame("save.txt"); rolled = false; needPick = false; }
            if (IsKeyPressed(KEY_ESCAPE)) game.changeAppState(AS_MENU);

            if (diceAnimating) {
                double now = GetTime();
                if (now >= diceAnimNext) {
                    lastRoll = GetRandomValue(1, 6);
                    diceAnimNext = now + 0.07;
                }
                if (now >= diceAnimEnd) {
                    diceAnimating = false;
                    lastRoll = game.rollDice();
                    if (game.getDice()->getConsecutiveSixes() >= 3) {
                        game.forfeitTurn();
                        rolled = false;
                        needPick = false;
                        selPiece = -1;
                        selPlayer = -1;
                    } else {
                        rolled = true;
                        needPick = Helpers::hasAnyValidMove(game.getPlayer(curIdx), lastRoll, game.getBoard());
                        if (!needPick) {
                            game.movePiece(curIdx, lastRoll);
                            rolled = false;
                            selPiece = -1;
                            selPlayer = -1;
                        } else {
                            selPlayer = curIdx;
                            selPiece = -1;
                            for (int i = 0; i < MAX_PIECES; i++) {
                                if (Helpers::isValidMove(game.getPlayer(curIdx)->getPiece(i), lastRoll, game.getBoard())) {
                                    selPiece = i;
                                    break;
                                }
                            }
                        }
                    }
                }
            }

            if (!rolled && !diceAnimating && IsKeyPressed(KEY_SPACE)) {
                diceAnimating = true;
                diceAnimEnd = GetTime() + 0.65;
                diceAnimNext = 0.0;
                lastRoll = GetRandomValue(1, 6);
                needPick = false;
                selPiece = -1;
                selPlayer = -1;
            }

            if (needPick && rolled) {
                if (IsKeyPressed(KEY_TAB)) {
                    int nxt = (selPiece + 1) % MAX_PIECES;
                    for (int t = 0; t < MAX_PIECES; t++) {
                        if (Helpers::isValidMove(game.getPlayer(curIdx)->getPiece(nxt), lastRoll, game.getBoard())) break;
                        nxt = (nxt + 1) % MAX_PIECES;
                    }
                    selPiece = nxt;
                }

                if (IsKeyPressed(KEY_ENTER) && selPiece >= 0) {
                    if (Helpers::isValidMove(game.getPlayer(curIdx)->getPiece(selPiece), lastRoll, game.getBoard())) {
                        game.movePiece(curIdx, lastRoll, selPiece);
                        rolled = false; needPick = false; selPiece = -1; selPlayer = -1;
                    }
                }

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Vector2 mouse = GetMousePosition();
                    for (int i = 0; i < MAX_PIECES; i++) {
                        const Piece* pc = game.getPlayer(curIdx)->getPiece(i);
                        if (!pc || !Helpers::isValidMove(pc, lastRoll, game.getBoard())) continue;
                        Vector2 pp = disp.getPieceScreenPos(pc->getPosition());
                        if (disp.isClickOnPiece(mouse, pp)) {
                            if (selPiece == i) {
                                game.movePiece(curIdx, lastRoll, selPiece);
                                rolled = false; needPick = false; selPiece = -1; selPlayer = -1;
                            } else {
                                selPiece = i;
                                selPlayer = curIdx;
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (st == AS_GAMEOVER) {
            if (IsKeyPressed(KEY_R)) { game.startReplay(); rolled = false; }
            if (IsKeyPressed(KEY_N)) game.changeAppState(AS_MENU);
            if (IsKeyPressed(KEY_ESCAPE)) break;
        }
        else if (st == AS_PAUSE) {
            if (IsKeyPressed(KEY_SPACE)) game.changeAppState(AS_PLAYING);
        }

        if (game.isReplayMode()) {
            if (IsKeyPressed(KEY_RIGHT)) game.stepReplayForward();
            if (IsKeyPressed(KEY_LEFT)) game.stepReplayBackward();
            if (IsKeyPressed(KEY_ESCAPE)) game.changeAppState(AS_PLAYING);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (st == AS_MENU) {
            disp.drawMenu();
            if (enteringNames) disp.drawNameEntry(setupIndex, setupCount, nameBuffer);
        }
        else if (st == AS_PLAYING || game.isReplayMode()) {
            disp.drawBoard();
            disp.drawPieces(&game, selPiece, selPlayer, rolled ? lastRoll : 0);
            disp.drawDice(lastRoll, lastRoll > 0);
            disp.drawUI(&game);
        }
        else if (st == AS_GAMEOVER) {
            disp.drawBoard();
            disp.drawPieces(&game, -1, -1);
            disp.drawGameOver(&game);
        }

        EndDrawing();
    }

    CloseWindow();
}

int main() {
    RunGame();
    return 0;
}
