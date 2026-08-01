#ifndef FREEZETOWER_H
#define FREEZETOWER_H

#include "Tower.h"
#include <memory>

// ─── Freeze Tower (4th derived class — added in a separate commit) ─────────
// Medium range, low direct damage, but slows enemies for 2 turns.
// Damage formula: baseDamage_ (low, but slow effect is the real value)
class FreezeTower : public Tower {
    int slowTurns_;   // number of turns the freeze effect lasts

protected:
    void printDetails(std::ostream& os) const override;

public:
    explicit FreezeTower(const Position& pos);

    int  computeDamage() const override;
    char getSymbol()     const override;
    std::unique_ptr<Tower> clone() const override;

    // Apply freeze to an enemy (called from attack flow)
    void applyFreeze(Enemy& enemy) const;

    int getSlowTurns() const;
};

#endif
