#include "BSAI25020-Helpers.h"
#include "BSAI25020-Piece.h"
#include "BSAI25020-Board.h"
#include "BSAI25020-Cell.h"
#include "BSAI25020-Player.h"
#include <fstream>
#include <sstream>
using namespace std;
Helpers::Helpers() {}
bool Helpers::canOpen(int roll) { return roll == 6; }
bool Helpers::isValidMove(const Piece* t, int roll, const Board* b) {
    if (!t || !b) return false;
    PieceState st = t->getState();
    if (st == PS_BASE) return canOpen(roll);
    if (st == PS_FINISHED) return false;
    int dest = b->getDestination(t->getPosition(), roll, t);
    if (dest == -1) return false;
    const Cell* dc = b->getCell(dest);
    if (!dc || !dc->canEnter(t->getColour())) return false;
    return true;
}
bool Helpers::hasAnyValidMove(const Player* p, int roll, const Board* b) {
    if (!p || !b) return false;
    for (int i = 0; i < MAX_PIECES; i++) {
        const Piece* pc = p->getPiece(i);
        if (pc && isValidMove(pc, roll, b)) return true;
    }
    return false;
}
void Helpers::save(const History& hs, const GameState& currentState, int playerCount,
                   const Player players[], const string& fn) const {
    ofstream out(fn);
    if (!out.is_open()) return;
    int total = hs.getUndoIndex() + 1;
    if (total < 0) total = 0;
    out << "LUDO_SAVE_V1\n";
    out << "PLAYERS " << playerCount << "\n";
    for (int i = 0; i < playerCount; i++)
        out << "PLAYER " << i << " " << (int)players[i].getColour() << " " << players[i].getName() << "\n";
    out << "CURRENT_STATE " << currentState.currentPlayerIndex << " "
        << currentState.consecutiveSixes << " " << currentState.moveNumber << "\n";
    out << "CURRENT_PIECE_STATES";
    for (int p = 0; p < MAX_PLAYERS; p++)
        for (int j = 0; j < PIECES_PER_PLAYER; j++)
            out << " " << (int)currentState.pieceStates[p][j];
    out << "\n";
    out << "CURRENT_PIECE_POS";
    for (int p = 0; p < MAX_PLAYERS; p++)
        for (int j = 0; j < PIECES_PER_PLAYER; j++)
            out << " " << currentState.piecePositions[p][j];
    out << "\n";
    out << "HISTORY_COUNT " << total << "\n";
    for (int i = 0; i < total; i++) {
        const MoveRecord* rec = hs.getRecord(i);
        const GameState* st = hs.getState(i);
        if (!rec || !st) continue;
        out << "MOVE " << rec->pieceNo << " " << rec->playerId << " "
            << rec->source << " " << rec->dest << " " << rec->diceValue << " "
            << (rec->wasCapture ? 1 : 0) << " "
            << rec->capturedPieceNo << " " << rec->capturedPlayerId << "\n";
        out << "STATE " << st->currentPlayerIndex << " "
            << st->consecutiveSixes << " " << st->moveNumber << "\n";
        out << "PIECE_STATES";
        for (int p = 0; p < MAX_PLAYERS; p++)
            for (int j = 0; j < PIECES_PER_PLAYER; j++)
                out << " " << (int)st->pieceStates[p][j];
        out << "\n";
        out << "PIECE_POS";
        for (int p = 0; p < MAX_PLAYERS; p++)
            for (int j = 0; j < PIECES_PER_PLAYER; j++)
                out << " " << st->piecePositions[p][j];
        out << "\n";
    }
    out << "END_SAVE\n";
}
bool Helpers::load(const string& fn, History& hs, GameState& initialState,
                   int& playerCount, string playerNames[], Colour playerColours[]) const {
    ifstream in(fn);
    if (!in.is_open()) return false;
    string line;
    if (!getline(in, line) || line != "LUDO_SAVE_V1") return false;
    hs.clear();
    playerCount = 0;
    while (getline(in, line)) {
        if (line == "END_SAVE") break;
        istringstream ss(line);
        string token;
        ss >> token;
        if (token == "PLAYERS") {
            ss >> playerCount;
            if (playerCount < 2) playerCount = 2;
            if (playerCount > MAX_PLAYERS) playerCount = MAX_PLAYERS;
        } else if (token == "PLAYER") {
            int idx, col;
            string name;
            ss >> idx >> col >> name;
            if (idx >= 0 && idx < MAX_PLAYERS) {
                playerNames[idx] = name;
                playerColours[idx] = (Colour)col;
            }
        } else if (token == "CURRENT_STATE") {
            ss >> initialState.currentPlayerIndex >> initialState.consecutiveSixes >> initialState.moveNumber;
        } else if (token == "CURRENT_PIECE_STATES") {
            for (int p = 0; p < MAX_PLAYERS; p++)
                for (int j = 0; j < PIECES_PER_PLAYER; j++) {
                    int v;
                    ss >> v;
                    initialState.pieceStates[p][j] = (PieceState)v;
                }
        } else if (token == "CURRENT_PIECE_POS") {
            for (int p = 0; p < MAX_PLAYERS; p++)
                for (int j = 0; j < PIECES_PER_PLAYER; j++)
                    ss >> initialState.piecePositions[p][j];
        } else if (token == "MOVE") {
            MoveRecord rec;
            int cap;
            ss >> rec.pieceNo >> rec.playerId >> rec.source >> rec.dest
               >> rec.diceValue >> cap >> rec.capturedPieceNo >> rec.capturedPlayerId;
            rec.wasCapture = (cap == 1);
            GameState st;
            getline(in, line); { istringstream s2(line); string t; s2 >> t >> st.currentPlayerIndex >> st.consecutiveSixes >> st.moveNumber; }
            getline(in, line); { istringstream s3(line); string t; s3 >> t; for (int p = 0; p < MAX_PLAYERS; p++) for (int j = 0; j < PIECES_PER_PLAYER; j++) { int v; s3 >> v; st.pieceStates[p][j] = (PieceState)v; } }
            getline(in, line); { istringstream s4(line); string t; s4 >> t; for (int p = 0; p < MAX_PLAYERS; p++) for (int j = 0; j < PIECES_PER_PLAYER; j++) s4 >> st.piecePositions[p][j]; }
            hs.push(rec, st);
        }
    }
    if (playerCount == 0) return false;
    return true;
}
