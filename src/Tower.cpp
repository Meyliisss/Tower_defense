#include "Tower.h"
#include <iostream>

// ─── Static member definition ──────────────────────────────────────────────
int Tower::towerCount_ = 0;

// ─── Constructor / Destructor ──────────────────────────────────────────────

Tower::Tower(const std::string& name, const Position& pos,
             int baseDamage, double range, int cost)
    : name_(name), pos_(pos), baseDamage_(baseDamage), range_(range),
      cost_(cost), totalKills_(0)
{
    ++towerCount_;
}

Tower::~Tower() {
    --towerCount_;
}

// ─── Static accessor ──────────────────────────────────────────────────────

int Tower::getTowerCount() {
    return towerCount_;
}

// ─── Accessors ────────────────────────────────────────────────────────────

const std::string& Tower::getName()       const { return name_;        }
const Position&    Tower::getPosition()   const { return pos_;         }
int                Tower::getBaseDamage() const { return baseDamage_;  }
double             Tower::getRange()      const { return range_;       }
int                Tower::getCost()       const { return cost_;        }
int                Tower::getTotalKills() const { return totalKills_;  }

// ─── Game logic ───────────────────────────────────────────────────────────

bool Tower::isInRange(const Position& p) const {
    return pos_.distanceTo(p) <= range_;
}

bool Tower::canAttack(const Enemy& enemy) const {
    return enemy.isAlive() && isInRange(enemy.getPosition());
}

bool Tower::attack(Enemy& enemy) {
    if (!canAttack(enemy)) return false;
    enemy.takeDamage(computeDamage());   // virtual dispatch — derived damage formula
    if (!enemy.isAlive()) {
        ++totalKills_;
        return true;
    }
    return false;
}

// ─── Display (NVI pattern) ────────────────────────────────────────────────

void Tower::printDetails(std::ostream& os) const {
    // Base implementation — derived classes call Tower::printDetails() then add extras
    os << name_ << " (" << getSymbol() << ") at " << pos_
       << " [Dmg: " << computeDamage()
       << ", Range: " << getRange()
       << ", Cost: " << cost_
       << ", Kills: " << getTotalKills() << "]";
}

void Tower::displayStats() const {
    // Non-virtual: delegates to virtual printDetails() hook
    printDetails(std::cout);
    std::cout << "\n";
}

std::ostream& operator<<(std::ostream& os, const Tower& t) {
    t.printDetails(os);   // NVI: calls derived override if available
    return os;
}
