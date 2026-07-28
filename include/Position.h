#ifndef POSITION_H
#define POSITION_H

#include <ostream>

class Position {
    int x;
    int y;

    bool isOrigin() const;

public:
    Position(int x = 0, int y = 0);

    int getX() const;
    int getY() const;
    void setX(int newX);
    void setY(int newY);

    double distanceTo(const Position& other) const;
    bool operator==(const Position& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Position& pos);
};

#endif
