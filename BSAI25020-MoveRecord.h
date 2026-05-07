#pragma once
#include "BSAI25020-Enums.h"
using namespace std;

struct MoveRecord {
    int pieceNo;
    int playerId;
    int source;
    int dest;
    int capturedPieceNo;
    int capturedPlayerId;
    int diceValue;
    bool wasCapture;
    MoveRecord();
    MoveRecord(int pNo, int pId, int src, int dst, int diceVal,
               bool capture = false, int capPNo = -1, int capPId = -1);
};
