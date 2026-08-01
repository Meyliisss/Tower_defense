#include "FreezeTower.h"
#include <ostream>

// Calls base constructor with Freeze-specific stats
FreezeTower::FreezeTower(const Position& pos)
    : Tower("Freeze_Tower", pos, /*baseDamage=*/8, /*range=*/6.0, /*cost=*/100),
      slowTurns_(2) {}

int FreezeTower::computeDamage() const {
    // Freeze towers deal low direct damage — their real power is slowing enemies
    return baseDamage_;
}

char FreezeTower::getSymbol() const { return 'Z'; }

std::unique_ptr<Tower> FreezeTower::clone() const {
    return std::make_unique<FreezeTower>(*this);
}

void FreezeTower::applyFreeze(Enemy& enemy) const {
    enemy.freeze(slowTurns_);
}

int FreezeTower::getSlowTurns() const { return slowTurns_; }

void FreezeTower::printDetails(std::ostream& os) const {
    Tower::printDetails(os);
    os << " [freeze: " << slowTurns_ << " turns slow]";
}
