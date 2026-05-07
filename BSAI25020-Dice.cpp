#include "BSAI25020-Dice.h"
#include <cstdlib>
#include <ctime>
using namespace std;
Dice::Dice() : lastRoll(0), consecutiveSixes(0) {
    srand((unsigned int)time(nullptr));
}
int Dice::roll() {
    lastRoll = (rand() % 6) + 1;
    if (lastRoll == 6) consecutiveSixes++;
    else consecutiveSixes = 0;
    return lastRoll;
}
int Dice::getLastRoll() const { return lastRoll; }
int Dice::getConsecutiveSixes() const { return consecutiveSixes; }
void Dice::setConsecutiveSixes(int n) { consecutiveSixes = n < 0 ? 0 : n; }
void Dice::reset() { lastRoll = 0; consecutiveSixes = 0; }
