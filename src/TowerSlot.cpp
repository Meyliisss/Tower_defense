#include "TowerSlot.h"
#include "MageTower.h"
#include "CannonTower.h"
#include "ArcherTower.h"
#include "FreezeTower.h"
#include <utility>

// ─── Construction ──────────────────────────────────────────────────────────

TowerSlot::TowerSlot(std::unique_ptr<Tower> tower)
    : tower_(std::move(tower)) {}

// Deep copy: clone() calls the virtual constructor of the concrete type
TowerSlot::TowerSlot(const TowerSlot& other)
    : tower_(other.tower_ ? other.tower_->clone() : nullptr) {}

// Copy-and-swap: parameter is either copy-constructed or move-constructed,
// then we swap our internals with it — gives us both copy= and move= in one.
TowerSlot& TowerSlot::operator=(TowerSlot other) {
    swap(other);
    return *this;
}

void TowerSlot::swap(TowerSlot& other) noexcept {
    std::swap(tower_, other.tower_);
}

// ─── Accessors ────────────────────────────────────────────────────────────

Tower&       TowerSlot::get()       { return *tower_; }
const Tower& TowerSlot::get() const { return *tower_; }

// ─── Delegated actions (virtual dispatch through base pointer) ─────────────

bool TowerSlot::attack(Enemy& enemy) {
    return tower_->attack(enemy);   // calls Tower::attack() → computeDamage() virtual
}

bool TowerSlot::canAttack(const Enemy& enemy) const {
    return tower_->canAttack(enemy);
}

void TowerSlot::displayStats() const {
    tower_->displayStats();         // calls NVI displayStats() → virtual printDetails()
}

// ─── Type-specific info via dynamic_cast ──────────────────────────────────

std::string TowerSlot::getSpecialInfo() const {
    if (const auto* mage = dynamic_cast<MageTower*>(tower_.get())) {
        return "Mage special: kill scaling +" + std::to_string(mage->getKillBonus()) + " dmg";
    }
    if (const auto* cannon = dynamic_cast<CannonTower*>(tower_.get())) {
        return "Cannon special: +" + std::to_string(cannon->getExplosiveBonus()) + " explosive";
    }
    if (dynamic_cast<ArcherTower*>(tower_.get())) {
        return "Archer special: rapid fire, no miss";
    }
    if (const auto* freeze = dynamic_cast<FreezeTower*>(tower_.get())) {
        return "Freeze special: slows enemies for " + std::to_string(freeze->getSlowTurns()) + " turns";
    }
    return "Unknown tower type";
}

// ─── Display ──────────────────────────────────────────────────────────────

std::ostream& operator<<(std::ostream& os, const TowerSlot& ts) {
    os << ts.get();   // calls Tower::operator<< → NVI printDetails()
    return os;
}
