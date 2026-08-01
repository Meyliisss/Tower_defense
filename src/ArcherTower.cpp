#include "ArcherTower.h"
#include <ostream>

// Calls base constructor with Archer-specific stats
ArcherTower::ArcherTower(const Position& pos)
    : Tower("Archer_Tower", pos, /*baseDamage=*/15, /*range=*/8.0, /*cost=*/75) {}

int ArcherTower::computeDamage() const {
    // Archers fire rapidly and consistently — straightforward base damage
    return baseDamage_;
}

char ArcherTower::getSymbol() const { return 'A'; }

std::unique_ptr<Tower> ArcherTower::clone() const {
    return std::make_unique<ArcherTower>(*this);
}

void ArcherTower::printDetails(std::ostream& os) const {
    Tower::printDetails(os);
    os << " [rapid fire, consistent dmg]";
}
