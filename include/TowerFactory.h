#ifndef TOWERFACTORY_H
#define TOWERFACTORY_H

#include <memory>
#include <string>
#include <vector>
#include "Tower.h"
#include "Position.h"

// ─── Data loaded from tower_config.txt ────────────────────────────────────
struct TowerConfig {
    std::string type;
    int         damage;
    double      range;
    int         cost;
    std::string description;
};

// ─── TowerFactory ─────────────────────────────────────────────────────────
// Reads tower definitions from a config file and creates concrete Tower
// instances — removes all hardcoded tower stats from application code.
class TowerFactory {
public:
    // Load the menu of available towers from a config file
    static std::vector<TowerConfig> loadConfigs(const std::string& filename);

    // Instantiate the concrete tower matching 'type' at 'pos'
    static std::unique_ptr<Tower> create(const std::string& type, const Position& pos);
};

#endif
