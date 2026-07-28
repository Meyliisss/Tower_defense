#ifndef TOWER_H
#define TOWER_H

#include <ostream>
#include <string>
#include "Position.h"
#include "Enemy.h"

class Tower {
    std::string name;
    Position    pos;
    int         damage;
    double      range;
    int         cost;
    int         totalKills;

    bool isInRange(const Position& p) const;

public:
    Tower(const std::string& name, const Position& pos,
          int damage, double range, int cost);

    const std::string& getName()      const;
    const Position&    getPosition()  const;
    int                getDamage()    const;
    double             getRange()     const;
    int                getCost()      const;
    int                getTotalKills()const;

    bool canAttack(const Enemy& enemy) const;
    bool attack(Enemy& enemy);
    void displayStats()                const;

    friend std::ostream& operator<<(std::ostream& os, const Tower& t);
};

#endif
