#include "MageTower.h"
#include <ostream>

// Calls base constructor with Mage-specific stats
MageTower::MageTower(const Position& pos)
    : Tower("Mage_Tower", pos, /*baseDamage=*/50, /*range=*/10.0, /*cost=*/150),
      killBonusPerKill_(3) {}

int MageTower::computeDamage() const {
    // Mage gains experience: each kill permanently boosts future damage
    return baseDamage_ + totalKills_ * killBonusPerKill_;
}

char MageTower::getSymbol() const { return 'M'; }

std::unique_ptr<Tower> MageTower::clone() const {
    return std::make_unique<MageTower>(*this);
}

int MageTower::getKillBonus() const {
    return totalKills_ * killBonusPerKill_;
}

void MageTower::printDetails(std::ostream& os) const {
    Tower::printDetails(os);
    os << " [kill bonus: +" << getKillBonus() << "]";
}
