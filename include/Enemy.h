#ifndef ENEMY_H
#define ENEMY_H

#include <ostream>
#include <string>
#include "Position.h"

class Enemy {
    std::string name;
    Position    pos;
    int         health;
    int         maxHealth;
    int         speed;
    int         reward;
    int*        damageLog;
    int         logSize;
    int         logCapacity;

    void        recordHit(int dmg);
    std::string buildHealthBar() const;

public:
    Enemy(const std::string& name, const Position& startPos,
          int health, int speed, int reward);
    Enemy(const Enemy& other);
    Enemy& operator=(const Enemy& other);
    ~Enemy();

    const std::string& getName()     const;
    const Position&    getPosition() const;
    int                getHealth()   const;
    int                getMaxHealth()const;
    int                getSpeed()    const;
    int                getReward()   const;

    bool        isAlive()                        const;
    void        takeDamage(int damage);
    void        moveToward(const Position& target);
    std::string getDamageStats()                 const;

    friend std::ostream& operator<<(std::ostream& os, const Enemy& e);
};

#endif
