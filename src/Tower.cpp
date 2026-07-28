#include "Tower.h"
#include <iostream>

Tower::Tower(const std::string& name, const Position& pos,
             int damage, double range, int cost)
    : name(name), pos(pos), damage(damage), range(range), cost(cost), totalKills(0) {}

const std::string& Tower::getName() const {
    return name;
}

const Position& Tower::getPosition() const {
    return pos;
}

int Tower::getDamage() const {
    return damage;
}

double Tower::getRange() const {
    return range;
}

int Tower::getCost() const {
    return cost;
}

int Tower::getTotalKills() const {
    return totalKills;
}

bool Tower::isInRange(const Position& p) const {
    return pos.distanceTo(p) <= getRange();
}

bool Tower::canAttack(const Enemy& enemy) const {
    return enemy.isAlive() && isInRange(enemy.getPosition());
}

bool Tower::attack(Enemy& enemy) {
    if (!canAttack(enemy)) {
        return false;
    }
    enemy.takeDamage(getDamage());
    if (!enemy.isAlive()) {
        totalKills++;
        return true;
    }
    return false;
}

void Tower::displayStats() const {
    std::cout << "Tower " << name << " at " << pos
              << " [Dmg: " << getDamage() << ", Range: " << getRange()
              << ", Cost: " << getCost() << ", Kills: " << getTotalKills() << "]\n";
}

std::ostream& operator<<(std::ostream& os, const Tower& t) {
    os << "Tower[" << t.name << " pos=" << t.pos
       << " dmg=" << t.getDamage() << " range=" << t.getRange() << "]";
    return os;
}
