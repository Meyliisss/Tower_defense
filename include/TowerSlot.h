#ifndef TOWERSLOT_H
#define TOWERSLOT_H

#include <memory>
#include <ostream>
#include <string>
#include "Tower.h"
#include "Enemy.h"

// ─── TowerSlot ─────────────────────────────────────────────────────────────
// Owns a polymorphic Tower through a unique_ptr<Tower>.
// Copy semantics use clone() (virtual constructor) + copy-and-swap.
// Virtual functions are called through the base pointer held here.
class TowerSlot {
    std::unique_ptr<Tower> tower_;

public:
    explicit TowerSlot(std::unique_ptr<Tower> tower);

    // Deep copy via clone() — produces a correct polymorphic copy
    TowerSlot(const TowerSlot& other);
    TowerSlot(TowerSlot&&) noexcept = default;

    // copy-and-swap: takes by value (copy or move), then swaps
    TowerSlot& operator=(TowerSlot other);

    void swap(TowerSlot& other) noexcept;

    Tower&       get();
    const Tower& get() const;

    // ── Actions delegated through base pointer (virtual dispatch) ──────────
    bool attack(Enemy& enemy);
    bool canAttack(const Enemy& enemy) const;
    void displayStats() const;

    // Uses dynamic_cast to expose type-specific tower information
    std::string getSpecialInfo() const;

    friend std::ostream& operator<<(std::ostream& os, const TowerSlot& ts);
};

#endif
