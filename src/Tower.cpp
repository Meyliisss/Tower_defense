#include "Tower.h"
#include <cmath>
#include <iostream>

// ── Static member definition ──────────────────────────────────────────────────
int Tower::towerCount_ = 0;

// ── Constructor / Destructor ──────────────────────────────────────────────────

Tower::Tower(const std::string& name, const Position& pos,
             int baseDamage, double range, int cost)
    : name_(name), pos_(pos),
      baseDamage_(baseDamage), range_(range), cost_(cost),
      totalKills_(0)
{
    ++towerCount_;
}

Tower::~Tower() {
    --towerCount_;
}

// ── Accessors ─────────────────────────────────────────────────────────────────

const std::string& Tower::getName() const       { return name_; }
const Position&    Tower::getPosition() const   { return pos_; }
// cppcheck-suppress unusedFunction -- public API, may be used by external callers
int                Tower::getBaseDamage() const { return baseDamage_; }
// cppcheck-suppress unusedFunction -- public API, may be used by external callers
double             Tower::getRange() const      { return range_; }
int                Tower::getCost() const       { return cost_; }
// cppcheck-suppress unusedFunction -- public API, may be used by external callers
int                Tower::getTotalKills() const { return totalKills_; }
int                Tower::getTowerCount()       { return towerCount_; }

// ── Helpers ───────────────────────────────────────────────────────────────────

bool Tower::isInRange(const Position& p) const {
    int dx = pos_.getX() - p.getX();
    int dy = pos_.getY() - p.getY();
    double dist = std::sqrt(static_cast<double>(dx * dx + dy * dy));
    return dist <= range_;
}

// ── Game actions ──────────────────────────────────────────────────────────────

bool Tower::canAttack(const Enemy& enemy) const {
    return enemy.isAlive() && isInRange(enemy.getPosition());
}

bool Tower::attack(Enemy& enemy) {
    if (!canAttack(enemy)) return false;
    int dmg = computeDamage();   // virtual dispatch to concrete type
    enemy.takeDamage(dmg);
    if (!enemy.isAlive()) {
        ++totalKills_;
    }
    return true;
}

// ── NVI display ───────────────────────────────────────────────────────────────

void Tower::printDetails(std::ostream& os) const {
    os << name_
       << " | pos=" << pos_
       << " dmg=" << baseDamage_
       << " range=" << range_
       << " cost=" << cost_
       << " kills=" << totalKills_;
}

void Tower::displayStats() const {
    printDetails(std::cout);   // NVI: calls virtual override if derived class overrides it
    std::cout << '\n';
}

std::ostream& operator<<(std::ostream& os, const Tower& t) {
    t.printDetails(os);        // NVI hook
    return os;
}
