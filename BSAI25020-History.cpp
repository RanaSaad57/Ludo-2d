#include "BSAI25020-History.h"
using namespace std;
History::History() : recordCount(0), undoIndex(-1) {}
void History::push(const MoveRecord& m, const GameState& s) {
    remove();
    records.push_back(m);
    states.push_back(s);
    recordCount = (int)records.size();
    undoIndex = recordCount - 1;
}
MoveRecord* History::undo() {
    if (undoIndex < 0) return nullptr;
    MoveRecord* rec = &records[undoIndex];
    undoIndex--;
    return rec;
}
MoveRecord* History::redo() {
    if (undoIndex + 1 >= recordCount) return nullptr;
    undoIndex++;
    return &records[undoIndex];
}
void History::remove() {
    int newSize = undoIndex + 1;
    if (newSize < (int)records.size()) {
        records.resize(newSize);
        states.resize(newSize);
        recordCount = newSize;
    }
}
MoveRecord* History::getRecord(int i) {
    if (i < 0 || i >= recordCount) return nullptr;
    return &records[i];
}
const MoveRecord* History::getRecord(int i) const {
    if (i < 0 || i >= recordCount) return nullptr;
    return &records[i];
}
const GameState* History::getState(int i) const {
    if (i < 0 || i >= (int)states.size()) return nullptr;
    return &states[i];
}
int History::getTotalMoves() const { return recordCount; }
int History::getUndoIndex() const { return undoIndex; }
void History::clear() {
    records.clear();
    states.clear();
    recordCount = 0;
    undoIndex = -1;
}
