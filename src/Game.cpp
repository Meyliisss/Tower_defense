#include "Game.h"
#include <iostream>
#include <limits>
#include <algorithm>

Game::Game(const std::string& playerName, int startGold, int startLives)
    : playerName(playerName), castlePos(Position(0, 0)),
      gold(startGold), lives(startLives), wave(0), score(0) {}

int Game::getGold() const {
    return gold;
}

int Game::getLives() const {
    return lives;
}

int Game::getWave() const {
    return wave;
}

int Game::getScore() const {
    return score;
}

bool Game::isGameOver() const {
    return getLives() <= 0;
}

bool Game::hasEnoughGold(int cost) const {
    return getGold() >= cost;
}

bool Game::placeTower(const Tower& tower) {
    if (!hasEnoughGold(tower.getCost())) {
        std::cout << "[GAME] Not enough gold to build " << tower.getName()
                  << " (Cost: " << tower.getCost() << ", Available: " << getGold() << ")\n";
        return false;
    }
    gold -= tower.getCost();
    towers.push_back(tower);
    std::cout << "[GAME] Successfully built " << tower.getName() << " at " << tower.getPosition() << "!\n";
    return true;
}

void Game::addEnemy(const Enemy& enemy) {
    enemies.push_back(enemy);
}

void Game::startNewWave(int numEnemies) {
    wave++;
    std::cout << "\n=================== WAVE " << wave << " STARTING ===================\n";
    for (int i = 1; i <= numEnemies; ++i) {
        int startX = std::min(GRID_W - 1, 8 + i * 2);
        int startY = std::min(GRID_H - 1, 5 + i * 2);
        int hp = 20 + wave * 10 + i * 5;
        int speed = 1 + (i % 2);
        int reward = 15 + i * 5;
        std::string enemyName = "Goblin_" + std::to_string(wave) + "_" + std::to_string(i);
        addEnemy(Enemy(enemyName, Position(startX, startY), hp, speed, reward));
    }
}

int Game::findNearestEnemyIndex(const Tower& t) const {
    int nearestIdx = -1;
    double minDistance = std::numeric_limits<double>::max();

    for (size_t i = 0; i < enemies.size(); ++i) {
        if (!enemies[i].isAlive()) continue;
        if (t.canAttack(enemies[i])) {
            double dist = t.getPosition().distanceTo(enemies[i].getPosition());
            if (dist < minDistance) {
                minDistance = dist;
                nearestIdx = static_cast<int>(i);
            }
        }
    }
    return nearestIdx;
}

int Game::simulateTurn() {
    if (isGameOver()) {
        std::cout << "[GAME OVER] The castle has fallen!\n";
        return 0;
    }

    std::cout << "\n--- Simulating Turn (Wave " << wave << ") ---\n";
    int killsThisTurn = 0;

    for (auto& tower : towers) {
        int targetIdx = findNearestEnemyIndex(tower);
        if (targetIdx != -1) {
            bool killed = tower.attack(enemies[targetIdx]);
            std::cout << "  " << tower.getName() << " fired at " << enemies[targetIdx].getName()
                      << " (HP left: " << enemies[targetIdx].getHealth() << ")\n";
            if (killed) {
                killsThisTurn++;
                gold += enemies[targetIdx].getReward();
                score += enemies[targetIdx].getReward() * 2;
                std::cout << "  *** " << enemies[targetIdx].getName() << " DESTROYED! Earned +"
                          << enemies[targetIdx].getReward() << " gold! ***\n";
            }
        }
    }

    for (auto& enemy : enemies) {
        if (!enemy.isAlive()) continue;

        enemy.moveToward(castlePos);
        std::cout << "  " << enemy.getName() << " moved to " << enemy.getPosition() << "\n";

        if (enemy.getPosition() == castlePos) {
            lives--;
            enemy.takeDamage(enemy.getHealth());
            std::cout << "  !!! " << enemy.getName() << " BREACHED THE CASTLE! Lives remaining: "
                      << getLives() << " !!!\n";
        }
    }

    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
                       [](const Enemy& e) { return !e.isAlive(); }),
        enemies.end()
    );

    return killsThisTurn;
}

void Game::drawGrid() const {
    std::cout << "\n  ";
    for (int x = 0; x < GRID_W; ++x) {
        std::cout << (x % 10);
    }
    std::cout << "\n +" << std::string(GRID_W, '-') << "+\n";

    for (int y = 0; y < GRID_H; ++y) {
        std::cout << (y % 10) << "|";
        for (int x = 0; x < GRID_W; ++x) {
            Position curPos(x, y);
            char symbol = '.';

            if (curPos == castlePos) {
                symbol = 'C';
            }

            for (const auto& tower : towers) {
                if (tower.getPosition() == curPos) {
                    if (tower.getName().find("Archer") != std::string::npos) symbol = 'A';
                    else if (tower.getName().find("Cannon") != std::string::npos) symbol = 'K';
                    else if (tower.getName().find("Mage") != std::string::npos) symbol = 'M';
                    else symbol = 'T';
                    break;
                }
            }

            for (const auto& enemy : enemies) {
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
    std::cout << "Legend: C=Castle, A=Archer, K=Cannon, M=Mage, E=Enemy, .=Empty\n";
}

void Game::displayStatus() const {
    std::cout << "\n------------------- GAME STATUS -------------------\n";
    std::cout << "Player: " << playerName << " | Castle: " << castlePos << "\n";
    std::cout << "Gold: " << getGold() << " | Lives: " << getLives()
              << " | Wave: " << getWave() << " | Score: " << getScore() << "\n";
    drawGrid();
    std::cout << "Active Towers (" << towers.size() << "):\n";
    for (const auto& tower : towers) {
        std::cout << "  - ";
        tower.displayStats();
    }
    std::cout << "Enemies Remaining (" << enemies.size() << "):\n";
    for (const auto& enemy : enemies) {
        std::cout << "  - " << enemy << " | " << enemy.getDamageStats() << "\n";
    }
    std::cout << "---------------------------------------------------\n";
}

std::ostream& operator<<(std::ostream& os, const Game& g) {
    os << "Game[Player: " << g.playerName << ", Gold: " << g.getGold()
       << ", Lives: " << g.getLives() << ", Wave: " << g.getWave() << "]";
    return os;
}
