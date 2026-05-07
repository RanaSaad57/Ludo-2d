#pragma once
#include "raylib.h"
#include "BSAI25020-Enums.h"
#include <string>
using namespace std;

class Board;
class Player;
class Game;
class Piece;
class Displaying {
private:
    int SW, SH, CS, OX, OY, BS;
    Texture2D boardTex, pawnTex[4];
    Color lC_Red, lC_Green, lC_Yellow, lC_Blue;
    Color toRayColor(Colour c) const;
    Vector2 trackCellToScreen(int tp) const;
    Vector2 homeColToScreen(Colour c, int posInHC) const;
    Vector2 baseCellToScreen(Colour c, int pieceIdx) const;
    Vector2 cellIdToScreen(int id) const;
    Texture2D getPawnTex(Colour c) const;
    void drawPieceSprite(Vector2 pos, Colour c, bool selected) const;
    void drawDestinationHighlight(const Game* g, int selPiece, int selPlayer, int roll) const;
    Vector2 boardCell(float col, float row) const;
public:
    Displaying();
    Displaying(int w, int h, int cs);
    ~Displaying();
    void drawBoard() const;
    void drawPieces(const Game* g, int selPiece, int selPlayer, int roll = 0) const;
    void drawDice(int roll, bool rolled) const;
    void drawUI(const Game* g) const;
    void drawMenu() const;
    void drawNameEntry(int playerIndex, int playerCount, const string& text) const;
    void drawGameOver(const Game* g) const;
    int getCS() const;
    Vector2 getPieceScreenPos(int cellId) const;
    bool isClickOnPiece(Vector2 mouse, Vector2 piecePos) const;
};
