#ifndef TOWER_H
#define TOWER_H

#include <ostream>
#include <string>
#include <memory>
#include "Position.h"
#include "Enemy.h"

// ─── Abstract base class for all tower types ────────────────────────────────
// Derived classes must override computeDamage(), getSymbol(), and clone().
// Display is handled via the Non-Virtual Interface pattern:
//   operator<< and displayStats() call the virtual printDetails() hook.
class Tower {
protected:
    std::string name_;
    Position    pos_;
    int         baseDamage_;
    double      range_;
    int         cost_;
    int         totalKills_;

    static int  towerCount_;   // tracks all living Tower instances

    bool isInRange(const Position& p) const;

    // NVI hook — derived classes override this for custom display details
    virtual void printDetails(std::ostream& os) const;

public:
    Tower(const std::string& name, const Position& pos,
          int baseDamage, double range, int cost);
    virtual ~Tower();

    // ── Pure virtual interface ─────────────────────────────────────────────
    // Each tower type computes its own damage formula (theme-specific)
    virtual int  computeDamage() const = 0;
    // Returns the single character displayed on the grid (A, K, M, …)
    virtual char getSymbol()     const = 0;
    // Virtual constructor — enables deep-copying through a base pointer
    virtual std::unique_ptr<Tower> clone() const = 0;

    // ── Accessors ─────────────────────────────────────────────────────────
    const std::string& getName()       const;
    const Position&    getPosition()   const;
    int                getBaseDamage() const;
    double             getRange()      const;
    int                getCost()       const;
    int                getTotalKills() const;
    static int         getTowerCount();   // total living tower objects

    // ── Game actions ──────────────────────────────────────────────────────
    bool canAttack(const Enemy& enemy) const;
    bool attack(Enemy& enemy);      // calls computeDamage() via virtual dispatch

    // NVI: non-virtual display calls virtual printDetails()
    void displayStats() const;

    friend std::ostream& operator<<(std::ostream& os, const Tower& t);
};

#endif
