#include "BSAI25020-Game.h"
using namespace std;

Game::Game() : playerCount(0), currentPlayer(0), replayMode(false), appstate(AS_MENU) {}
void Game::startGame(int count) {
    if (count < 2) count = 2;
    if (count > MAX_P) count = MAX_P;
    playerCount = count;
    Colour cols[4] = { LDO_RED, LDO_GREEN, LDO_YELLOW, LDO_BLUE };
    string names[4] = { "Red", "Green", "Yellow", "Blue" };
    for (int i = 0; i < playerCount; i++)
        players[i] = Player(names[i], cols[i]);
    history.clear();
    Gamestates.clear();
    currentPlayer = 0;
    dice.reset();
    appstate = AS_PLAYING;
    for (int id = 0; id < board.getCellCount(); id++) {
        Cell* cell = board.getCell(id);
        if (cell) cell->getPieces().clear();
    }
    for (int p = 0; p < playerCount; p++) {
        for (int j = 0; j < MAX_PIECES; j++) {
            Piece* piece = players[p].getPiece(j);
            piece->reset();
            Cell* cell = board.getCell(piece->getPosition());
            if (cell) cell->addPiece(piece);
        }
    }
    initialState = captureState();
}
void Game::startGame(int count, const string customNames[]) {
    if (count < 2) count = 2;
    if (count > MAX_P) count = MAX_P;
    playerCount = count;
    Colour cols[4] = { LDO_RED, LDO_GREEN, LDO_YELLOW, LDO_BLUE };
    string fallback[4] = { "Red", "Green", "Yellow", "Blue" };
    for (int i = 0; i < playerCount; i++) {
        string n = customNames[i].empty() ? fallback[i] : customNames[i];
        players[i] = Player(n, cols[i]);
    }
    history.clear();
    Gamestates.clear();
    currentPlayer = 0;
    dice.reset();
    appstate = AS_PLAYING;
    for (int id = 0; id < board.getCellCount(); id++) {
        Cell* cell = board.getCell(id);
        if (cell) cell->getPieces().clear();
    }
    for (int p = 0; p < playerCount; p++) {
        for (int j = 0; j < MAX_PIECES; j++) {
            Piece* piece = players[p].getPiece(j);
            piece->reset();
            Cell* cell = board.getCell(piece->getPosition());
            if (cell) cell->addPiece(piece);
        }
    }
    initialState = captureState();
}
void Game::advancePlayer() {
    do {
        currentPlayer = (currentPlayer + 1) % playerCount;
    } while (!isPlayerActive(currentPlayer));
}
bool Game::isPlayerActive(int idx) const {
    if (idx < 0 || idx >= playerCount) return false;
    return !players[idx].allFinished();
}
int Game::rollDice() { return dice.roll(); }
void Game::resetDice() { dice.reset(); }
void Game::forfeitTurn() {
    int mover = currentPlayer;
    while (true) {
        int idx = history.getUndoIndex();
        MoveRecord* rec = history.getRecord(idx);
        if (!rec || rec->playerId != mover || rec->diceValue != 6) break;
        history.undo();
    }
    int idx = history.getUndoIndex();
    const GameState* prev = (idx >= 0) ? history.getState(idx) : &initialState;
    if (prev) applyState(*prev);
    history.remove();
    if ((int)Gamestates.size() > history.getTotalMoves()) Gamestates.resize(history.getTotalMoves());
    currentPlayer = mover;
    dice.reset();
    advancePlayer();
}
void Game::movePiece(int moverIdx, int rollVal, int pieceIdx) {
    if (moverIdx < 0 || moverIdx >= playerCount) return;
    Player* mover = &players[moverIdx];
    Piece* chosen = nullptr;
    int chosenIdx = -1;
    if (pieceIdx >= 0 && pieceIdx < MAX_PIECES) {
        Piece* p = mover->getPiece(pieceIdx);
        if (p && Helpers::isValidMove(p, rollVal, &board)) {
            chosen = p;
            chosenIdx = pieceIdx;
        }
    } else {
        for (int i = 0; i < MAX_PIECES; i++) {
            Piece* p = mover->getPiece(i);
            if (p && Helpers::isValidMove(p, rollVal, &board)) {
                chosen = p; chosenIdx = i; break;
            }
        }
    }

    if (!chosen) { advancePlayer(); return; }

    int src = chosen->getPosition();
    int dest = -1;

    Cell* srcCell = board.getCell(src);
    if (srcCell) srcCell->removePiece(chosen);

    if (chosen->getState() == PS_BASE) {
        dest = chosen->getStartPos();
        chosen->setPosition(dest);
        chosen->setState(PS_ACTIVE);
    } else {
        dest = board.getDestination(src, rollVal, chosen);
        chosen->setPosition(dest);
        if (dest == board.getCentreId()) {
            chosen->setState(PS_FINISHED);
        } else {
            const Cell* dc = board.getCell(dest);
            chosen->setState((dc && dc->getIsHomeColumn()) ? PS_HOMECOL : PS_ACTIVE);
        }
    }

    Cell* destCell = board.getCell(dest);
    if (destCell) destCell->addPiece(chosen);

    int capPNo = -1, capPId = -1;
    bool captured = false;
    if (!board.isSafeSquare(dest) && chosen->getState() == PS_ACTIVE)
        captured = checkCapture_return(dest, moverIdx, capPNo, capPId);

    bool won = checkWin();
    if (!won && !(rollVal == 6 || captured)) advancePlayer();

    MoveRecord rec(chosenIdx, moverIdx, src, dest, rollVal, captured, capPNo, capPId);
    GameState gs = captureState();
    gs.moveNumber = history.getTotalMoves() + 1;
    history.push(rec, gs);
    Gamestates.push_back(gs);
}

bool Game::checkCapture_return(int pos, int moverIdx, int& capPNo, int& capPId) {
    Cell* cell = board.getCell(pos);
    if (!cell || board.isSafeSquare(pos)) return false;
    vector<Piece*>& ps = cell->getPieces();
    for (int i = (int)ps.size() - 1; i >= 0; i--) {
        Piece* p = ps[i];
        if (!p || p->getColour() == players[moverIdx].getColour()) continue;
        capPNo = p->getPieceN();
        for (int pi = 0; pi < playerCount; pi++) {
            if (players[pi].getColour() == p->getColour()) { capPId = pi; break; }
        }
        ps.erase(ps.begin() + i);
        p->reset();
        Cell* bc = board.getCell(p->getPosition());
        if (bc) bc->addPiece(p);
        return true;
    }
    return false;
}

bool Game::checkWin() {
    for (int i = 0; i < playerCount; i++) {
        if (players[i].allFinished()) { appstate = AS_GAMEOVER; return true; }
    }
    return false;
}

void Game::undoMove() {
    MoveRecord* rec = history.undo();
    if (!rec) return;
    int idx = history.getUndoIndex();
    const GameState* prev = (idx >= 0) ? history.getState(idx) : &initialState;
    if (prev) applyState(*prev);
}

void Game::redoMove() {
    MoveRecord* rec = history.redo();
    if (!rec) return;
    const GameState* next = history.getState(history.getUndoIndex());
    if (next) applyState(*next);
}

void Game::startReplay() {
    replayMode = true;
    applyState(initialState);
}

void Game::stepReplayForward() {
    if (!replayMode) return;
    MoveRecord* rec = history.redo();
    if (!rec) { replayMode = false; return; }
    const GameState* st = history.getState(history.getUndoIndex());
    if (st) applyState(*st);
}

void Game::stepReplayBackward() {
    if (!replayMode) return;
    MoveRecord* rec = history.undo();
    if (!rec) return;
    int idx = history.getUndoIndex();
    const GameState* st = (idx >= 0) ? history.getState(idx) : &initialState;
    if (st) applyState(*st);
}

void Game::saveGame(const string& fn) { helpers.save(history, captureState(), playerCount, players, fn); }

void Game::loadGame(const string& fn) {
    int pc = 0;
    string names[MAX_P];
    Colour cols[MAX_P];
    GameState loaded;
    if (!helpers.load(fn, history, loaded, pc, names, cols)) return;
    playerCount = pc;
    for (int i = 0; i < playerCount; i++) players[i] = Player(names[i], cols[i]);
    initialState = loaded;
    applyState(loaded);
    appstate = AS_PLAYING;
}

GameState Game::captureState() const {
    GameState gs;
    gs.currentPlayerIndex = currentPlayer;
    gs.consecutiveSixes = dice.getConsecutiveSixes();
    gs.moveNumber = history.getTotalMoves();
    for (int p = 0; p < playerCount; p++)
        for (int j = 0; j < MAX_PIECES; j++) {
            const Piece* pc = players[p].getPiece(j);
            if (pc) { gs.piecePositions[p][j] = pc->getPosition(); gs.pieceStates[p][j] = pc->getState(); }
        }
    return gs;
}

void Game::applyState(const GameState& gs) {
    currentPlayer = gs.currentPlayerIndex;
    dice.setConsecutiveSixes(gs.consecutiveSixes);
    for (int id = 0; id < board.getCellCount(); id++) {
        Cell* c = board.getCell(id);
        if (c) c->getPieces().clear();
    }
    for (int p = 0; p < playerCount; p++)
        for (int j = 0; j < MAX_PIECES; j++) {
            Piece* pc = players[p].getPiece(j);
            if (!pc) continue;
            pc->setPosition(gs.piecePositions[p][j]);
            pc->setState(gs.pieceStates[p][j]);
            Cell* c = board.getCell(gs.piecePositions[p][j]);
            if (c) c->addPiece(pc);
        }
}

void Game::changeAppState(AppState ap) { appstate = ap; }
AppState Game::getAppState() const { return appstate; }
int Game::getCurrentPlayer() const { return currentPlayer; }
Player* Game::getPlayer(int i) { if (i < 0 || i >= playerCount) return nullptr; return &players[i]; }
const Player* Game::getPlayer(int i) const { if (i < 0 || i >= playerCount) return nullptr; return &players[i]; }
int Game::getPlayerCount() const { return playerCount; }
const Board* Game::getBoard() const { return &board; }
const Dice* Game::getDice() const { return &dice; }
const History* Game::getHistory() const { return &history; }
bool Game::isReplayMode() const { return replayMode; }
