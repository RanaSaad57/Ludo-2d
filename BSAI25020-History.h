#pragma once
#include <vector>
#include "BSAI25020-MoveRecord.h"
#include "BSAI25020-GameState.h"
using namespace std;

class History {
private:
    vector<MoveRecord> records;
    vector<GameState> states;
    int recordCount;
    int undoIndex;

public:
    History();
    void push(const MoveRecord& m, const GameState& s);
    MoveRecord* undo();
    MoveRecord* redo();
    void remove();
    MoveRecord* getRecord(int i);
    const MoveRecord* getRecord(int i) const;
    const GameState* getState(int i) const;
    int getTotalMoves() const;
    int getUndoIndex() const;
    void clear();
};
