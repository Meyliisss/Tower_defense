#include <iostream>
#include <vector>
#include <string>
#include "Game.h"
#include "Tower.h"
#include "Enemy.h"
#include "Position.h"
#include "TowerFactory.h"
#include "Exceptions.h"

// ─── Interactive menu ─────────────────────────────────────────────────────

void runInteractiveMenu(Game& game, const std::vector<TowerConfig>& configs) {
    int choice = 0;
    while (!game.isGameOver()) {
        std::cout << "\n========================================\n";
        std::cout << "           TOWER DEFENSE MENU           \n";
        std::cout << "========================================\n";
        std::cout << "1. Display Game Grid & Status\n";
        std::cout << "2. Place a Tower\n";
        std::cout << "3. Start New Wave\n";
        std::cout << "4. Simulate Next Turn\n";
        std::cout << "5. Exit Game\n";
        std::cout << "Select option (1-5): ";

        if (!(std::cin >> choice)) break;

        if (choice == 1) {
            game.displayStatus();

        } else if (choice == 2) {
            int x, y, type;
            std::cout << "\nAvailable Towers:\n";
            for (size_t i = 0; i < configs.size(); ++i) {
                const auto& cfg = configs[i];
                std::cout << "  " << (i + 1) << ". " << cfg.type
                          << " (Cost: " << cfg.cost
                          << ", Dmg: " << cfg.damage
                          << ", Range: " << cfg.range << ") — " << cfg.description << "\n";
            }
            std::cout << "Enter X Y Type (e.g., '2 3 1'): ";
            if (std::cin >> x >> y >> type) {
                const size_t idx = static_cast<size_t>(type - 1);
                if (idx < configs.size()) {
                    try {
                        auto tower = TowerFactory::create(configs[idx].type, Position(x, y));
                        game.placeTower(std::move(tower));
                    } catch (const InsufficientGoldException& e) {
                        std::cout << "[ERROR] " << e.what()
                                  << " (need " << e.getRequired()
                                  << ", have " << e.getAvailable() << ")\n";
                    } catch (const InvalidPositionException& e) {
                        std::cout << "[ERROR] " << e.what()
                                  << " (x=" << e.getX() << ", y=" << e.getY() << ")\n";
                    }
                } else {
                    std::cout << "Invalid tower type!\n";
                }
            }

        } else if (choice == 3) {
            int count = 2;
            std::cout << "Enter number of enemies to spawn: ";
            if (std::cin >> count) {
                game.startNewWave(count);
            }

        } else if (choice == 4) {
            try {
                game.simulateTurn();
                game.drawGrid();
            } catch (const GameOverException& e) {
                std::cout << "[GAME OVER] " << e.what() << "\n";
                break;
            }

        } else if (choice == 5) {
            std::cout << "Exiting game loop...\n";
            break;
        } else {
            std::cout << "Invalid choice!\n";
        }
    }
}

// ─── Main ─────────────────────────────────────────────────────────────────

int main() {
    std::cout << "====================================================\n";
    std::cout << "          TOWER DEFENSE - OOP PROJECT DEMO          \n";
    std::cout << "====================================================\n\n";

    // ── Demo: Rule of Three on Enemy (Tema 1 requirement) ──────────────────
    {
        std::cout << "=== DEMO: Rule of Three (Enemy copy/assign) ===\n";
        Enemy original("Orc_Boss", Position(10, 10), 100, 2, 50);
        original.takeDamage(25);
        original.takeDamage(15);
        std::cout << "Original: " << original << "\n";
        std::cout << "  " << original.getDamageStats() << "\n";

        Enemy copy = original;                          // copy constructor
        original.takeDamage(30);
        std::cout << "After damaging original:\n";
        std::cout << "  Original HP: " << original.getHealth()
                  << " | " << original.getDamageStats() << "\n";
        std::cout << "  Copy     HP: " << copy.getHealth()
                  << " | " << copy.getDamageStats() << "\n";

        Enemy assigned("Dummy", Position(0, 0), 10, 1, 5);
        assigned = original;                            // operator=
        std::cout << "Assigned: " << assigned << "\n\n";
    }

    // ── Demo: Tower hierarchy & virtual dispatch ────────────────────────────
    {
        std::cout << "=== DEMO: Tower hierarchy (virtual dispatch) ===\n";
        std::cout << "Tower count before creation: " << Tower::getTowerCount() << "\n";
        {
            auto archer = TowerFactory::create("archer", Position(2, 2));
            auto cannon = TowerFactory::create("cannon", Position(5, 5));
            auto mage   = TowerFactory::create("mage",   Position(8, 3));
            std::cout << "  " << *archer << "\n";
            std::cout << "  " << *cannon << "\n";
            std::cout << "  " << *mage   << "\n";
            std::cout << "Tower count (3 created): " << Tower::getTowerCount() << "\n";
        }
        std::cout << "Tower count after scope: " << Tower::getTowerCount() << "\n\n";
    }

    // ── Load configurations from data files ────────────────────────────────
    std::vector<TowerConfig> towerConfigs;
    try {
        towerConfigs = TowerFactory::loadConfigs("assets/tower_config.txt");
    } catch (const TowerDefenseException& e) {
        std::cerr << "[FATAL] " << e.what() << "\n";
        return 1;
    }

    // ── Start game ─────────────────────────────────────────────────────────
    std::string playerName;
    std::cout << "Enter Player Name: ";
    if (!(std::cin >> playerName)) {
        playerName = "Defender";
    }
    std::cout << "Welcome, Commander " << playerName << "!\n\n";

    try {
        Game game(playerName, 300, 5, "assets/wave_config.txt");
        runInteractiveMenu(game, towerConfigs);

        std::cout << "\n====================================================\n";
        std::cout << "              GAME ENDED - FINAL SCORE              \n";
        std::cout << "Final Score: " << game.getScore()
                  << " | Wave Reached: " << game.getWave()
                  << " | Total Waves Ever: " << Game::getTotalWaves() << "\n";
        std::cout << "====================================================\n";

    } catch (const TowerDefenseException& e) {
        std::cerr << "[FATAL] " << e.what() << "\n";
        return 1;
    }

    return 0;
}
