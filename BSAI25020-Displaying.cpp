#include "BSAI25020-Displaying.h"
#include "BSAI25020-Board.h"
#include "BSAI25020-Player.h"
#include "BSAI25020-Game.h"
#include "BSAI25020-Piece.h"
#include "BSAI25020-Cell.h"
#include <cstdio>
using namespace std;

static const int TRACK[52][2] = {
    {1,6},{2,6},{3,6},{4,6},{5,6},{6,5},{6,4},{6,3},{6,2},{6,1},{6,0},{7,0},{8,0},
    {8,1},{8,2},{8,3},{8,4},{8,5},{9,6},{10,6},{11,6},{12,6},{13,6},{14,6},{14,7},{14,8},
    {13,8},{12,8},{11,8},{10,8},{9,8},{8,9},{8,10},{8,11},{8,12},{8,13},{8,14},{7,14},{6,14},
    {6,13},{6,12},{6,11},{6,10},{6,9},{5,8},{4,8},{3,8},{2,8},{1,8},{0,8},{0,7},{0,6}
};
static const float BASE_TUNE_X[4] = {0.0f, 0.0f, 0.0f, 0.0f};
static const float BASE_TUNE_Y[4] = {0.0f, 0.0f, 0.0f, 0.0f};
static const float BASE_DOT_CENTERS[4][4][2] = {
    {{1.977f, 1.964f}, {3.914f, 1.964f}, {2.037f, 3.926f}, {3.974f, 3.927f}},
    {{10.959f, 1.903f}, {12.896f, 1.903f}, {11.018f, 3.868f}, {12.956f, 3.868f}},
    {{10.918f, 10.908f}, {12.856f, 10.908f}, {10.977f, 12.873f}, {12.916f, 12.873f}},
    {{2.015f, 10.929f}, {3.953f, 10.929f}, {2.077f, 12.892f}, {4.014f, 12.891f}}
};
Displaying::Displaying()
    : SW(1180), SH(900), CS(54), OX(30), OY(45), BS(810),
    lC_Red({220,50,50,255}), lC_Green({50,180,50,255}),
    lC_Yellow({230,200,30,255}), lC_Blue({50,100,220,255}) {
    boardTex = LoadTexture("assets/Ludo_Board_main.png");
    pawnTex[0] = LoadTexture("assets/red.png");
    pawnTex[1] = LoadTexture("assets/green.png");
    pawnTex[2] = LoadTexture("assets/yellow.png");
    pawnTex[3] = LoadTexture("assets/blue.png");
}
Displaying::Displaying(int w, int h, int cs)
    : SW(w), SH(h), CS(cs),
    lC_Red({220,50,50,255}), lC_Green({50,180,50,255}),
    lC_Yellow({230,200,30,255}), lC_Blue({50,100,220,255}) {
    BS = CS * 15;
    OX = 30;
    OY = (h - BS) / 2;
    if (OY < 24) OY = 24;
    boardTex = LoadTexture("assets/Ludo_Board_main.png");
    pawnTex[0] = LoadTexture("assets/red.png");
    pawnTex[1] = LoadTexture("assets/green.png");
    pawnTex[2] = LoadTexture("assets/yellow.png");
    pawnTex[3] = LoadTexture("assets/blue.png");
}
Displaying::~Displaying() {
    UnloadTexture(boardTex);
    for (int i = 0; i < 4; i++) UnloadTexture(pawnTex[i]);
}
Color Displaying::toRayColor(Colour c) const {
    if (c == LDO_RED) return lC_Red;
    if (c == LDO_GREEN) return lC_Green;
    if (c == LDO_YELLOW) return lC_Yellow;
    if (c == LDO_BLUE) return lC_Blue;
    return LIGHTGRAY;
}
Texture2D Displaying::getPawnTex(Colour c) const {
    if (c == LDO_RED) return pawnTex[0];
    if (c == LDO_GREEN) return pawnTex[1];
    if (c == LDO_YELLOW) return pawnTex[2];
    if (c == LDO_BLUE) return pawnTex[3];
    return pawnTex[0];
}
Vector2 Displaying::boardCell(float col, float row) const {
    return {(float)(OX + col * CS), (float)(OY + row * CS)};
}
Vector2 Displaying::trackCellToScreen(int tp) const {
    if (tp < 0 || tp >= 52) return {(float)OX, (float)OY};
    return boardCell((float)TRACK[tp][0], (float)TRACK[tp][1]);
}
Vector2 Displaying::homeColToScreen(Colour c, int posInHC) const {
    if (c == LDO_RED) return boardCell((float)(1 + posInHC), 7.0f);
    if (c == LDO_GREEN) return boardCell(7.0f, (float)(1 + posInHC));
    if (c == LDO_YELLOW) return boardCell((float)(13 - posInHC), 7.0f);
    return boardCell(7.0f, (float)(13 - posInHC));
}
Vector2 Displaying::baseCellToScreen(Colour c, int pieceIdx) const {
    int ci = (int)c;
    if (ci < 0 || ci >= 4 || pieceIdx < 0 || pieceIdx >= 4) return boardCell(0, 0);
    float col = BASE_DOT_CENTERS[ci][pieceIdx][0] - 0.5f + BASE_TUNE_X[ci];
    float row = BASE_DOT_CENTERS[ci][pieceIdx][1] - 0.5f + BASE_TUNE_Y[ci];
    return boardCell(col, row);
}
Vector2 Displaying::cellIdToScreen(int id) const {
    if (id >= 0 && id < TRACK_SIZE) return trackCellToScreen(id);
    if (id >= 52 && id < 58) return homeColToScreen(LDO_RED, id - 52);
    if (id >= 58 && id < 64) return homeColToScreen(LDO_GREEN, id - 58);
    if (id >= 64 && id < 70) return homeColToScreen(LDO_YELLOW, id - 64);
    if (id >= 70 && id < 76) return homeColToScreen(LDO_BLUE, id - 70);
    if (id == CENTRE_ID) return boardCell(7, 7);
    if (id >= 78 && id < 94) {
        int rel = id - 78;
        Colour c = LDO_BLUE;
        int pidx = rel - 12;
        if (rel < 4) { c = LDO_RED; pidx = rel; }
        else if (rel < 8) { c = LDO_GREEN; pidx = rel - 4; }
        else if (rel < 12) { c = LDO_YELLOW; pidx = rel - 8; }
        return baseCellToScreen(c, pidx);
    }
    return {(float)OX, (float)OY};
}
Vector2 Displaying::getPieceScreenPos(int cellId) const {
    return cellIdToScreen(cellId);
}
bool Displaying::isClickOnPiece(Vector2 mouse, Vector2 pos) const {
    float r = CS * 0.42f;
    float dx = mouse.x - pos.x - CS / 2.0f;
    float dy = mouse.y - pos.y - CS / 2.0f;
    return (dx * dx + dy * dy) <= r * r;
}
void Displaying::drawPieceSprite(Vector2 pos, Colour c, bool selected) const {
    Texture2D tex = getPawnTex(c);
    float size = (float)CS * 0.50f;
    float cx = pos.x + CS / 2.0f - size / 2.0f;
    float cy = pos.y + CS / 2.0f - size / 2.0f;
    if (selected) {
        DrawCircleLines((int)(pos.x + CS / 2.0f), (int)(pos.y + CS / 2.0f), size * 0.62f, GOLD);
        DrawCircleLines((int)(pos.x + CS / 2.0f), (int)(pos.y + CS / 2.0f), size * 0.74f, GOLD);
    }
    Rectangle src = {0, 0, (float)tex.width, (float)tex.height};
    Rectangle dst = {cx, cy, size, size};
    DrawTexturePro(tex, src, dst, {0, 0}, 0.0f, WHITE);
}
void Displaying::drawBoard() const {
    Rectangle src = {0, 0, (float)boardTex.width, (float)boardTex.height};
    Rectangle dst = {(float)OX, (float)OY, (float)BS, (float)BS};
    DrawTexturePro(boardTex, src, dst, {0, 0}, 0.0f, WHITE);
}
void Displaying::drawDestinationHighlight(const Game* g, int selPiece, int selPlayer, int roll) const {
    if (!g || roll <= 0 || selPlayer < 0 || selPiece < 0) return;
    const Player* pl = g->getPlayer(selPlayer);
    if (!pl) return;
    const Piece* pc = pl->getPiece(selPiece);
    const Board* b = g->getBoard();
    if (!pc || !b) return;
    int dest = -1;
    if (pc->getState() == PS_BASE) {
        if (roll != 6) return;
        dest = pc->getStartPos();
    } else {
        dest = b->getDestination(pc->getPosition(), roll, pc);
    }
    if (dest < 0) return;
    const Cell* dc = b->getCell(dest);
    if (!dc || !dc->canEnter(pc->getColour())) return;
    bool capture = false;
    if (!b->isSafeSquare(dest)) {
        const vector<Piece*>& pieces = dc->getPieces();
        for (int i = 0; i < (int)pieces.size(); i++) {
            if (pieces[i] && pieces[i]->getColour() != pc->getColour()) {
                capture = true;
                break;
            }
        }
    }
    Vector2 pos = cellIdToScreen(dest);
    Color fill = capture ? Fade(RED, 0.42f) : Fade(GREEN, 0.42f);
    Color line = capture ? RED : GREEN;
    DrawRectangle((int)pos.x + 4, (int)pos.y + 4, CS - 8, CS - 8, fill);
    DrawRectangleLinesEx({pos.x + 4, pos.y + 4, (float)CS - 8, (float)CS - 8}, 3.0f, line);
}
void Displaying::drawPieces(const Game* g, int selPiece, int selPlayer, int roll) const {
    drawDestinationHighlight(g, selPiece, selPlayer, roll);
    for (int pi = 0; pi < g->getPlayerCount(); pi++) {
        const Player* pl = g->getPlayer(pi);
        for (int j = 0; j < MAX_PIECES; j++) {
            const Piece* pc = pl->getPiece(j);
            if (!pc) continue;
            Vector2 pos = cellIdToScreen(pc->getPosition());
            int sameCount = 0;
            int sameIndex = 0;
            for (int op = 0; op < g->getPlayerCount(); op++) {
                const Player* other = g->getPlayer(op);
                for (int oj = 0; oj < MAX_PIECES; oj++) {
                    const Piece* opc = other->getPiece(oj);
                    if (!opc || opc->getPosition() != pc->getPosition()) continue;
                    if (op < pi || (op == pi && oj < j)) sameIndex++;
                    sameCount++;
                }
            }
            if (sameCount > 1) {
                float ox[4] = {-0.16f, 0.16f, -0.16f, 0.16f};
                float oy[4] = {-0.16f, -0.16f, 0.16f, 0.16f};
                int si = sameIndex % 4;
                pos.x += ox[si] * CS;
                pos.y += oy[si] * CS;
            }
            drawPieceSprite(pos, pl->getColour(), pi == selPlayer && j == selPiece);
        }
    }
}
void Displaying::drawDice(int roll, bool rolled) const {
    int x = OX + BS + 80;
    int y = SH / 2 - 58;
    DrawRectangleRounded({(float)x, (float)y, 112, 112}, 0.18f, 8, WHITE);
    DrawRectangleRoundedLines({(float)x, (float)y, 112, 112}, 0.18f, 8, DARKGRAY);
    if (!rolled) {
        DrawText("?", x + 40, y + 27, 56, DARKGRAY);
        return;
    }
    float cx = (float)x + 56.0f;
    float cy = (float)y + 56.0f;
    float off = 30.0f;
    float dr = 8.5f;
    Color dc = BLACK;
    if (roll == 1)
        DrawCircle((int)cx, (int)cy, dr, dc);
    else if (roll == 2) {
        DrawCircle((int)(cx - off), (int)(cy - off), dr, dc);
        DrawCircle((int)(cx + off), (int)(cy + off), dr, dc);
    } else if (roll == 3) {
        DrawCircle((int)(cx - off), (int)(cy - off), dr, dc);
        DrawCircle((int)cx, (int)cy, dr, dc);
        DrawCircle((int)(cx + off), (int)(cy + off), dr, dc);
    } else if (roll == 4) {
        DrawCircle((int)(cx - off), (int)(cy - off), dr, dc);
        DrawCircle((int)(cx + off), (int)(cy - off), dr, dc);
        DrawCircle((int)(cx - off), (int)(cy + off), dr, dc);
        DrawCircle((int)(cx + off), (int)(cy + off), dr, dc);
    } else if (roll == 5) {
        DrawCircle((int)(cx - off), (int)(cy - off), dr, dc);
        DrawCircle((int)(cx + off), (int)(cy - off), dr, dc);
        DrawCircle((int)cx, (int)cy, dr, dc);
        DrawCircle((int)(cx - off), (int)(cy + off), dr, dc);
        DrawCircle((int)(cx + off), (int)(cy + off), dr, dc);
    } else {
        DrawCircle((int)(cx - off), (int)(cy - off), dr, dc);
        DrawCircle((int)(cx + off), (int)(cy - off), dr, dc);
        DrawCircle((int)(cx - off), (int)cy, dr, dc);
        DrawCircle((int)(cx + off), (int)cy, dr, dc);
        DrawCircle((int)(cx - off), (int)(cy + off), dr, dc);
        DrawCircle((int)(cx + off), (int)(cy + off), dr, dc);
    }
}

void Displaying::drawUI(const Game* g) const {
    int px = OX + BS + 48;
    int py = OY;
    for (int i = 0; i < g->getPlayerCount(); i++) {
        const Player* pl = g->getPlayer(i);
        Color rc = toRayColor(pl->getColour());
        bool cur = (i == g->getCurrentPlayer());
        DrawRectangle(px, py, 210, 38, cur ? rc : Fade(rc, 0.35f));
        DrawRectangleLines(px, py, 210, 38, cur ? GOLD : DARKGRAY);
        DrawText(pl->getName().c_str(), px + 48, py + 10, 18, cur ? WHITE : DARKGRAY);
        if (cur) DrawText(">", px - 18, py + 10, 18, GOLD);
        Texture2D pt = getPawnTex(pl->getColour());
        float psz = 26.0f;
        Rectangle src = {0, 0, (float)pt.width, (float)pt.height};
        Rectangle dst = {(float)(px + 10), (float)(py + 6), psz, psz};
        DrawTexturePro(pt, src, dst, {0, 0}, 0.0f, WHITE);
        py += 48;
    }
    char buf[48];
    snprintf(buf, sizeof(buf), "Moves: %d", g->getHistory()->getTotalMoves());
    DrawText(buf, px, SH - 178, 15, DARKGRAY);
    if (g->isReplayMode()) {
        DrawText("REPLAY MODE", px, SH - 148, 16, ORANGE);
        DrawText("[LEFT] Back", px, SH - 122, 14, DARKGRAY);
        DrawText("[RIGHT] Fwd", px, SH - 100, 14, DARKGRAY);
        DrawText("[ESC] Exit", px, SH - 78, 14, DARKGRAY);
    } else {
        DrawText("[SPACE] Roll", px, SH - 148, 14, DARKGRAY);
        DrawText("[TAB/CLICK] Select", px, SH - 126, 14, DARKGRAY);
        DrawText("[ENTER] Move", px, SH - 104, 14, DARKGRAY);
        DrawText("[U] Undo  [R] Redo", px, SH - 82, 14, DARKGRAY);
        DrawText("[S] Save  [L] Load", px, SH - 60, 14, DARKGRAY);
        DrawText("[ESC] Menu", px, SH - 38, 14, DARKGRAY);
    }
}
void Displaying::drawMenu() const {
    DrawRectangle(0, 0, SW, SH, {30, 30, 60, 255});
    if (boardTex.id > 0) {
        float bsz = (float)(SH) * 0.58f;
        Rectangle src = {0, 0, (float)boardTex.width, (float)boardTex.height};
        Rectangle dst = {(SW - bsz) / 2.0f, (SH - bsz) / 2.0f - 70.0f, bsz, bsz};
        DrawTexturePro(boardTex, src, dst, {0, 0}, 0.0f, Fade(WHITE, 0.16f));
    }
    DrawText("LUDO", SW / 2 - 90, SH / 2 - 230, 76, GOLD);
    Colour pcs[4] = {LDO_RED, LDO_GREEN, LDO_YELLOW, LDO_BLUE};
    float iconSz = 40.0f;
    float startX = SW / 2 - (4 * iconSz + 3 * 12) / 2.0f;
    for (int i = 0; i < 4; i++) {
        Texture2D pt = getPawnTex(pcs[i]);
        Rectangle src = {0, 0, (float)pt.width, (float)pt.height};
        Rectangle dst = {startX + i * (iconSz + 12), (float)(SH / 2 - 168), iconSz, iconSz};
        DrawTexturePro(pt, src, dst, {0, 0}, 0.0f, WHITE);
    }
    int bx = SW / 2 - 140, bw = 280, bh = 48;
    DrawRectangle(bx, SH / 2 - 64, bw, bh, {60, 60, 100, 255});
    DrawRectangle(bx, SH / 2, bw, bh, {60, 60, 100, 255});
    DrawRectangle(bx, SH / 2 + 64, bw, bh, {60, 60, 100, 255});
    DrawRectangle(bx, SH / 2 + 128, bw, bh, {60, 60, 100, 255});
    DrawText("1 - New Game (2P)", bx + 16, SH / 2 - 48, 20, WHITE);
    DrawText("2 - New Game (3P)", bx + 16, SH / 2 + 16, 20, WHITE);
    DrawText("3 - New Game (4P)", bx + 16, SH / 2 + 80, 20, WHITE);
    DrawText("4 - Load Game", bx + 16, SH / 2 + 144, 20, WHITE);
}
void Displaying::drawNameEntry(int playerIndex, int playerCount, const string& text) const {
    DrawRectangle(0, 0, SW, SH, Fade(BLACK, 0.45f));
    int boxW = 520;
    int boxH = 210;
    int x = SW / 2 - boxW / 2;
    int y = SH / 2 - boxH / 2;
    DrawRectangleRounded({(float)x, (float)y, (float)boxW, (float)boxH}, 0.08f, 10, {245, 239, 222, 255});
    DrawRectangleRoundedLines({(float)x, (float)y, (float)boxW, (float)boxH}, 0.08f, 10, DARKGRAY);
    char title[80];
    snprintf(title, sizeof(title), "Player %d of %d", playerIndex + 1, playerCount);
    DrawText(title, x + 28, y + 26, 26, DARKGRAY);
    DrawText("Enter name", x + 28, y + 70, 18, DARKGRAY);
    DrawRectangle(x + 28, y + 100, boxW - 56, 44, WHITE);
    DrawRectangleLines(x + 28, y + 100, boxW - 56, 44, GRAY);
    DrawText(text.c_str(), x + 40, y + 112, 22, DARKGRAY);
    DrawText("[ENTER] Next   [BACKSPACE] Delete", x + 28, y + 166, 16, DARKGRAY);
}
void Displaying::drawGameOver(const Game* g) const {
    DrawRectangle(0, 0, SW, SH, Fade(BLACK, 0.72f));
    DrawText("GAME OVER", SW / 2 - 140, SH / 2 - 130, 64, GOLD);
    for (int i = 0; i < g->getPlayerCount(); i++) {
        const Player* pl = g->getPlayer(i);
        if (pl->allFinished()) {
            string msg = "Winner: " + pl->getName();
            DrawText(msg.c_str(), SW / 2 - 110, SH / 2 - 40, 38, toRayColor(pl->getColour()));
            Texture2D pt = getPawnTex(pl->getColour());
            float psz = 74.0f;
            Rectangle src = {0, 0, (float)pt.width, (float)pt.height};
            Rectangle dst = {SW / 2 - psz / 2.0f, (float)(SH / 2 + 14), psz, psz};
            DrawTexturePro(pt, src, dst, {0, 0}, 0.0f, WHITE);
            break;
        }
    }
    DrawText("[R] Replay  [N] New Game  [ESC] Quit", SW / 2 - 220, SH / 2 + 112, 24, WHITE);
}
int Displaying::getCS() const { return CS; }
