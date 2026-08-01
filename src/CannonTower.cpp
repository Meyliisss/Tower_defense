#include "CannonTower.h"
#include <ostream>

// Calls base constructor with Cannon-specific stats
CannonTower::CannonTower(const Position& pos)
    : Tower("Cannon_Tower", pos, /*baseDamage=*/35, /*range=*/5.0, /*cost=*/120),
      explosiveBonus_(10) {}

int CannonTower::computeDamage() const {
    // Cannon ball deals heavy base damage + a fixed explosive splash bonus
    return baseDamage_ + explosiveBonus_;
}

char CannonTower::getSymbol() const { return 'K'; }

std::unique_ptr<Tower> CannonTower::clone() const {
    return std::make_unique<CannonTower>(*this);
}

int CannonTower::getExplosiveBonus() const { return explosiveBonus_; }

void CannonTower::printDetails(std::ostream& os) const {
    Tower::printDetails(os);
    os << " [explosive bonus: +" << explosiveBonus_ << "]";
}
