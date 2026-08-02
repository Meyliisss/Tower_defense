#include "Game.h"
#include "Exceptions.h"
#include "FreezeTower.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <fstream>
#include <sstream>

// ─── Static member definition ──────────────────────────────────────────────
int Game::totalWavesGlobal_ = 0;

// ─── Private: load wave config from file ──────────────────────────────────

WaveConfig Game::loadWaveConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw TowerDefenseException("Cannot open wave config file: " + filename);
    }
    WaveConfig cfg{20, 10, 15, 5};   // safe defaults
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream iss(line);
        iss >> cfg.baseHp >> cfg.hpPerWave >> cfg.baseReward >> cfg.rewardIncrement;
        break;   // only one data line expected
    }
    return cfg;
}

// ─── Constructor ──────────────────────────────────────────────────────────

Game::Game(const std::string& playerName, int startGold, int startLives,
           const std::string& waveConfigFile)
    : playerName_(playerName),
      castlePos_(Position(0, 0)),
      gold_(startGold), lives_(startLives), wave_(0), score_(0),
      waveConfig_(loadWaveConfig(waveConfigFile)) {}

// ─── Static accessor ──────────────────────────────────────────────────────

int Game::getTotalWaves() { return totalWavesGlobal_; }

// ─── Accessors ────────────────────────────────────────────────────────────

int  Game::getGold()    const { return gold_;            }
int  Game::getLives()   const { return lives_;           }
int  Game::getWave()    const { return wave_;            }
int  Game::getScore()   const { return score_;           }
bool Game::isGameOver() const { return lives_ <= 0;      }

// ─── Validation ───────────────────────────────────────────────────────────

bool Game::isValidPosition(const Position& pos) const {
    return pos.getX() >= 0 && pos.getX() < GRID_W &&
           pos.getY() >= 0 && pos.getY() < GRID_H;
}

// ─── Tower placement ──────────────────────────────────────────────────────

void Game::placeTower(std::unique_ptr<Tower> tower) {
    if (!isValidPosition(tower->getPosition())) {
        throw InvalidPositionException(tower->getPosition().getX(),
                                       tower->getPosition().getY());
    }
    if (gold_ < tower->getCost()) {
        throw InsufficientGoldException(tower->getCost(), gold_);
    }
    gold_ -= tower->getCost();
    std::cout << "[GAME] Built " << tower->getName()
              << " (" << tower->getSymbol() << ") at " << tower->getPosition()
              << " | Active towers: " << Tower::getTowerCount() << "\n";
    towers_.emplace_back(std::move(tower));
}

// ─── Enemy management ─────────────────────────────────────────────────────

void Game::addEnemy(const Enemy& enemy) {
    enemies_.push_back(enemy);
}

void Game::startNewWave(int numEnemies) {
    ++wave_;
    ++totalWavesGlobal_;
    std::cout << "\n=================== WAVE " << wave_ << " STARTING ===================\n";

    for (int i = 1; i <= numEnemies; ++i) {
        // Positions and stats come from the loaded wave config (no hardcoded values)
        int startX = std::min(GRID_W - 1, 8 + i * 2);
        int startY = std::min(GRID_H - 1, 5 + i * 2);
        int hp     = waveConfig_.baseHp + wave_ * waveConfig_.hpPerWave + i * 5;
        int speed  = 1 + (i % 2);
        int reward = waveConfig_.baseReward + i * waveConfig_.rewardIncrement;

        std::string name = "Goblin_" + std::to_string(wave_) + "_" + std::to_string(i);
        addEnemy(Enemy(name, Position(startX, startY), hp, speed, reward));
    }
}

// ─── Combat: find nearest enemy a tower can hit ───────────────────────────

int Game::findNearestEnemyIndex(const TowerSlot& ts) const {
    int    nearestIdx = -1;
    double minDist    = std::numeric_limits<double>::max();

    for (size_t i = 0; i < enemies_.size(); ++i) {
        if (!enemies_[i].isAlive()) continue;
        if (ts.canAttack(enemies_[i])) {
            double dist = ts.get().getPosition().distanceTo(enemies_[i].getPosition());
            if (dist < minDist) {
                minDist    = dist;
                nearestIdx = static_cast<int>(i);
            }
        }
    }
    return nearestIdx;
}

// ─── Simulate one turn ────────────────────────────────────────────────────

int Game::simulateTurn() {
    if (isGameOver()) {
        throw GameOverException();
    }

    std::cout << "\n--- Simulating Turn (Wave " << wave_ << ") ---\n";
    int killsThisTurn = 0;

    // Towers attack through virtual dispatch (TowerSlot → Tower* → computeDamage())
    for (auto& slot : towers_) {
        int idx = findNearestEnemyIndex(slot);
        if (idx != -1) {
            bool killed = slot.attack(enemies_[idx]);
            std::cout << "  " << slot.get().getName()
                      << " fired at " << enemies_[idx].getName()
                      << " (HP left: " << enemies_[idx].getHealth() << ")\n";
            // Apply freeze effect if this is a FreezeTower (dynamic_cast in Game context)
            if (auto* ft = dynamic_cast<FreezeTower*>(&slot.get())) {
                if (!killed) {
                    ft->applyFreeze(enemies_[idx]);
                    std::cout << "  ~~~ " << enemies_[idx].getName() << " FROZEN for "
                              << ft->getSlowTurns() << " turns! ~~~\n";
                }
            }
            if (killed) {
                ++killsThisTurn;
                gold_  += enemies_[idx].getReward();
                score_ += enemies_[idx].getReward() * 2;
                std::cout << "  *** " << enemies_[idx].getName()
                          << " DESTROYED! Earned +" << enemies_[idx].getReward() << " gold! ***\n";
            }
        }
    }

    // Enemies move toward the castle; tick freeze counter each turn
    for (auto& enemy : enemies_) {
        if (!enemy.isAlive()) continue;
        enemy.tickFreeze();   // reduce frozen turns, restore speed if counter reaches 0
        enemy.moveToward(castlePos_);
        std::cout << "  " << enemy.getName() << " moved to " << enemy.getPosition() << "\n";

        if (enemy.getPosition() == castlePos_) {
            --lives_;
            enemy.takeDamage(enemy.getHealth());
            std::cout << "  !!! " << enemy.getName()
                      << " BREACHED THE CASTLE! Lives remaining: " << lives_ << " !!!\n";
        }
    }

    // Remove dead enemies
    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(),
                       [](const Enemy& e) { return !e.isAlive(); }),
        enemies_.end());

    return killsThisTurn;
}

// ─── Display ──────────────────────────────────────────────────────────────

void Game::drawGrid() const {
    std::cout << "\n  ";
    for (int x = 0; x < GRID_W; ++x) std::cout << (x % 10);
    std::cout << "\n +" << std::string(GRID_W, '-') << "+\n";

    for (int y = 0; y < GRID_H; ++y) {
        std::cout << (y % 10) << "|";
        for (int x = 0; x < GRID_W; ++x) {
            Position curPos(x, y);
            char symbol = '.';

            if (curPos == castlePos_) { symbol = 'C'; }

            for (const auto& slot : towers_) {
                if (slot.get().getPosition() == curPos) {
                    symbol = slot.get().getSymbol();   // virtual dispatch
                    break;
                }
            }
            for (const auto& enemy : enemies_) {
                if (enemy.isAlive() && enemy.getPosition() == curPos) {
                    symbol = 'E';
                    break;
                }
            }
            std::cout << symbol;
        }
        std::cout << "|\n";
    }
    std::cout << " +" << std::string(GRID_W, '-') << "+\n";
    std::cout << "Legend: C=Castle, A=Archer, K=Cannon, M=Mage, Z=Freeze, E=Enemy, .=Empty\n";
}

void Game::displayStatus() const {
    std::cout << "\n------------------- GAME STATUS -------------------\n";
    std::cout << "Player: " << playerName_ << " | Castle: " << castlePos_ << "\n";
    std::cout << "Gold: " << gold_ << " | Lives: " << getLives()
              << " | Wave: " << wave_ << " | Score: " << score_
              << " | Total waves played: " << totalWavesGlobal_ << "\n";
    drawGrid();
    std::cout << "Active Towers (" << towers_.size() << "):\n";
    for (const auto& slot : towers_) {
        std::cout << "  - ";
        slot.displayStats();
        std::cout << "    Special: " << slot.getSpecialInfo() << "\n";
    }
    std::cout << "Enemies Remaining (" << enemies_.size() << "):\n";
    for (const auto& enemy : enemies_) {
        std::cout << "  - " << enemy << " | " << enemy.getDamageStats();
        if (enemy.isFrozen()) std::cout << " [FROZEN]";
        std::cout << "\n";
    }
    std::cout << "---------------------------------------------------\n";
}

std::ostream& operator<<(std::ostream& os, const Game& g) {
    os << "Game[Player: " << g.playerName_ << ", Gold: " << g.gold_
       << ", Lives: " << g.getLives() << ", Wave: " << g.wave_ << "]";
    return os;
}
