#include "Position.h"
#include <cmath>

Position::Position(int x, int y) : x(x), y(y) {}

int Position::getX() const {
    return x;
}

int Position::getY() const {
    return y;
}

void Position::setX(int newX) {
    x = newX;
}

void Position::setY(int newY) {
    y = newY;
}

bool Position::isOrigin() const {
    return x == 0 && y == 0;
}

double Position::distanceTo(const Position& other) const {
    int dx = x - other.x;
    int dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
}

bool Position::operator==(const Position& other) const {
    return x == other.x && y == other.y;
}

std::ostream& operator<<(std::ostream& os, const Position& pos) {
    os << "(" << pos.x << ", " << pos.y << ")";
    return os;
}
