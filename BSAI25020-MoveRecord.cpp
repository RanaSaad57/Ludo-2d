#include "BSAI25020-MoveRecord.h"
using namespace std;
MoveRecord::MoveRecord()
    : pieceNo(-1), playerId(-1), source(-1), dest(-1),
      capturedPieceNo(-1), capturedPlayerId(-1), diceValue(0), wasCapture(false) {}
MoveRecord::MoveRecord(int pNo, int pId, int src, int dst, int diceVal,
                       bool capture, int capPNo, int capPId)
    : pieceNo(pNo), playerId(pId), source(src), dest(dst),
      capturedPieceNo(capPNo), capturedPlayerId(capPId),
      diceValue(diceVal), wasCapture(capture) {}
