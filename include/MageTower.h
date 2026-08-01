#ifndef MAGETOWER_H
#define MAGETOWER_H

#include "Tower.h"
#include <memory>

// ─── Mage Tower ────────────────────────────────────────────────────────────
// Longest range, massive base damage that scales with experience (kills).
// Damage formula: baseDamage_ + totalKills_ * killBonusPerKill_
class MageTower : public Tower {
    int killBonusPerKill_;   // extra damage gained per enemy killed

protected:
    void printDetails(std::ostream& os) const override;

public:
    explicit MageTower(const Position& pos);

    int  computeDamage() const override;
    char getSymbol()     const override;
    std::unique_ptr<Tower> clone() const override;

    // Returns the current accumulated kill bonus damage
    int getKillBonus() const;
};

#endif
