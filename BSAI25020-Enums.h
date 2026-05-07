#pragma once

enum Colour {
    LDO_RED = 0,
    LDO_GREEN = 1,
    LDO_YELLOW = 2,
    LDO_BLUE = 3,
    LDO_NONE = 4
};
enum PieceState {
    PS_BASE = 0,
    PS_ACTIVE = 1,
    PS_HOMECOL = 2,
    PS_FINISHED = 3
};

enum AppState {
    AS_MENU = 0,
    AS_PLAYING = 1,
    AS_PAUSE = 2,
    AS_GAMEOVER = 3
};
static const int MAX_PIECES = 4;
static const int MAX_COLOURS = 4;
static const int TRACK_SIZE = 52;
static const int HOME_COL_LEN = 6;
static const int CENTRE_ID = 76;
static const int TOTAL_CELLS = 94;
