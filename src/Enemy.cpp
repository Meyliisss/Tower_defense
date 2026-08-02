#include "Enemy.h"
#include <iostream>
#include <algorithm>

Enemy::Enemy(const std::string& name, const Position& startPos,
             int health, int speed, int reward)
    : name(name), pos(startPos), health(health), maxHealth(health),
      speed(speed), baseSpeed_(speed), frozenTurns_(0),
      reward(reward), logSize(0), logCapacity(4)
{
    damageLog = new int[logCapacity];
}

Enemy::Enemy(const Enemy& other)
    : name(other.name), pos(other.pos), health(other.health),
      maxHealth(other.maxHealth), speed(other.speed),
      baseSpeed_(other.baseSpeed_), frozenTurns_(other.frozenTurns_),
      reward(other.reward),
      logSize(other.logSize), logCapacity(other.logCapacity)
{
    damageLog = new int[logCapacity];
    for (int i = 0; i < logSize; ++i) {
        damageLog[i] = other.damageLog[i];
    }
}

Enemy& Enemy::operator=(const Enemy& other) {
    if (this != &other) {
        delete[] damageLog;

        name = other.name;
        pos = other.pos;
        health = other.health;
        maxHealth = other.maxHealth;
        speed = other.speed;
        baseSpeed_ = other.baseSpeed_;
        frozenTurns_ = other.frozenTurns_;
        reward = other.reward;
        logSize = other.logSize;
        logCapacity = other.logCapacity;

        damageLog = new int[logCapacity];
        for (int i = 0; i < logSize; ++i) {
            damageLog[i] = other.damageLog[i];
        }
    }
    return *this;
}

Enemy::~Enemy() {
    delete[] damageLog;
}

const std::string& Enemy::getName() const {
    return name;
}

const Position& Enemy::getPosition() const {
    return pos;
}

int Enemy::getHealth() const {
    return health;
}

int Enemy::getMaxHealth() const {
    return maxHealth;
}

int Enemy::getSpeed() const {
    return speed;
}

int Enemy::getReward() const {
    return reward;
}

bool Enemy::isAlive() const {
    return health > 0;
}

void Enemy::recordHit(int dmg) {
    if (logSize >= logCapacity) {
        int newCap = logCapacity * 2;
        int* newLog = new int[newCap];
        for (int i = 0; i < logSize; ++i) {
            newLog[i] = damageLog[i];
        }
        delete[] damageLog;
        damageLog = newLog;
        logCapacity = newCap;
    }
    damageLog[logSize++] = dmg;
}

void Enemy::takeDamage(int damage) {
    if (damage <= 0) return;
    health -= damage;
    if (health < 0) health = 0;
    recordHit(damage);
}

void Enemy::freeze(int turns) {
    frozenTurns_ = turns;
    speed = std::max(1, baseSpeed_ / 2);   // halve speed, minimum 1
}

void Enemy::tickFreeze() {
    if (frozenTurns_ > 0) {
        --frozenTurns_;
        if (frozenTurns_ == 0) {
            speed = baseSpeed_;   // restore original speed
        }
    }
}

bool Enemy::isFrozen() const {
    return frozenTurns_ > 0;
}

void Enemy::moveToward(const Position& target) {
    int curX = pos.getX();
    int curY = pos.getY();
    int targetX = target.getX();
    int targetY = target.getY();

    for (int step = 0; step < getSpeed(); ++step) {
        if (curX < targetX) curX++;
        else if (curX > targetX) curX--;
        else if (curY < targetY) curY++;
        else if (curY > targetY) curY--;
    }
    pos.setX(curX);
    pos.setY(curY);
}

std::string Enemy::buildHealthBar() const {
    float barWidth = 10.0f;
    float healthRatio = (float)health / (float)getMaxHealth();
    if (healthRatio < 0.0f) healthRatio = 0.0f;
    int filled = (int)(healthRatio * barWidth);

    std::string bar = "[";
    for (int i = 0; i < (int)barWidth; ++i) {
        if (i < filled) bar += "=";
        else bar += " ";
    }
    bar += "]";
    return bar;
}

std::string Enemy::getDamageStats() const {
    std::string stats = "Hits taken: " + std::to_string(logSize) + " (Log: ";
    for (int i = 0; i < logSize; ++i) {
        stats += std::to_string(damageLog[i]);
        if (i < logSize - 1) stats += ", ";
    }
    stats += ")";
    return stats;
}

std::ostream& operator<<(std::ostream& os, const Enemy& e) {
    os << "Enemy " << e.name << " at " << e.pos
       << " HP: " << e.health << "/" << e.getMaxHealth() << " (Spd: " << e.getSpeed() << ") " << e.buildHealthBar();
    return os;
}
