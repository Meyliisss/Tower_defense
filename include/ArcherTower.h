#ifndef ARCHERTOWER_H
#define ARCHERTOWER_H

#include "Tower.h"
#include <memory>

// ─── Archer Tower ──────────────────────────────────────────────────────────
// Long range, rapid fire, reliable base damage.
// Damage formula: baseDamage_ (consistent per-shot)
class ArcherTower : public Tower {
protected:
    void printDetails(std::ostream& os) const override;

public:
    explicit ArcherTower(const Position& pos);

    int  computeDamage() const override;
    char getSymbol()     const override;
    std::unique_ptr<Tower> clone() const override;
};

#endif
