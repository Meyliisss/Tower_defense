#include "TowerFactory.h"
#include "ArcherTower.h"
#include "CannonTower.h"
#include "MageTower.h"
#include "FreezeTower.h"
#include "Exceptions.h"
#include <fstream>
#include <sstream>

// ─── Load tower configurations from file ──────────────────────────────────
// File format (lines starting with # are comments):
//   type  baseDamage  range  cost  description...
std::vector<TowerConfig> TowerFactory::loadConfigs(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw TowerDefenseException("Cannot open tower config file: " + filename);
    }

    std::vector<TowerConfig> configs;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        TowerConfig cfg;
        if (iss >> cfg.type >> cfg.damage >> cfg.range >> cfg.cost) {
            std::getline(iss >> std::ws, cfg.description);
            configs.push_back(cfg);
        }
    }
    return configs;
}

// ─── Create a concrete tower by type name ─────────────────────────────────
std::unique_ptr<Tower> TowerFactory::create(const std::string& type, const Position& pos) {
    if (type == "archer") return std::make_unique<ArcherTower>(pos);
    if (type == "cannon") return std::make_unique<CannonTower>(pos);
    if (type == "mage")   return std::make_unique<MageTower>(pos);
    if (type == "freeze") return std::make_unique<FreezeTower>(pos);
    throw TowerDefenseException("Unknown tower type: '" + type + "'");
}
