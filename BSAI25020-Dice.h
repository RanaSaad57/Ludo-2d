#pragma once
using namespace std;

class Dice {
private:
    int lastRoll, consecutiveSixes;
public:
    Dice();
    int roll();
    int getLastRoll() const;
    int getConsecutiveSixes() const;
    void setConsecutiveSixes(int n);
    void reset();
};
