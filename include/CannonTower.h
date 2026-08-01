#ifndef CANNONTOWER_H
#define CANNONTOWER_H

#include "Tower.h"
#include <memory>

// ─── Cannon Tower ──────────────────────────────────────────────────────────
// Medium range, high damage + fixed explosive splash bonus.
// Damage formula: baseDamage_ + explosiveBonus_
class CannonTower : public Tower {
    int explosiveBonus_;   // extra damage from cannon ball explosion

protected:
    void printDetails(std::ostream& os) const override;

public:
    explicit CannonTower(const Position& pos);

    int  computeDamage()    const override;
    char getSymbol()        const override;
    std::unique_ptr<Tower> clone() const override;

    int getExplosiveBonus() const;
};

#endif
