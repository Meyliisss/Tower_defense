#include <iostream>
#include <vector>
#include <string>
#include "Game.h"
#include "Tower.h"
#include "Enemy.h"
#include "Position.h"

void runInteractiveMenu(Game& game) {
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

        if (!(std::cin >> choice)) {
            break;
        }

        if (choice == 1) {
            game.displayStatus();
        } else if (choice == 2) {
            int x, y, type;
            std::cout << "\nAvailable Towers:\n";
            std::cout << "1. Archer Tower (Cost: 75, Dmg: 15, Range: 8.0)\n";
            std::cout << "2. Cannon Tower (Cost: 120, Dmg: 35, Range: 5.0)\n";
            std::cout << "3. Mage Tower   (Cost: 150, Dmg: 50, Range: 10.0)\n";
            std::cout << "Enter X Y Type (e.g., '2 3 1'): ";
            if (std::cin >> x >> y >> type) {
                Position pos(x, y);
                if (type == 1) game.placeTower(Tower("Archer_Tower", pos, 15, 8.0, 75));
                else if (type == 2) game.placeTower(Tower("Cannon_Tower", pos, 35, 5.0, 120));
                else if (type == 3) game.placeTower(Tower("Mage_Tower", pos, 50, 10.0, 150));
                else std::cout << "Invalid tower type!\n";
            }
        } else if (choice == 3) {
            int count = 2;
            std::cout << "Enter number of enemies to spawn in this wave: ";
            if (std::cin >> count) {
                game.startNewWave(count);
            }
        } else if (choice == 4) {
            game.simulateTurn();
            game.drawGrid();
        } else if (choice == 5) {
            std::cout << "Exiting game loop...\n";
            break;
        } else {
            std::cout << "Invalid choice!\n";
        }
    }
}

int main() {
    std::cout << "====================================================\n";
    std::cout << "          TOWER DEFENSE - OOP PROJECT DEMO          \n";
    std::cout << "====================================================\n\n";

    std::string playerName;
    std::cout << "Enter Player Name: ";
    if (!(std::cin >> playerName)) {
        playerName = "Defender";
    }
    std::cout << "Welcome, Commander " << playerName << "!\n\n";

    Game game(playerName, 300, 5);

    std::cout << "=== DEMO: Rule of Three (Copy Constructor & Assignment) ===\n";
    Position startPos(10, 10);
    Enemy originalEnemy("Orc_Boss", startPos, 100, 2, 50);
    originalEnemy.takeDamage(25);
    originalEnemy.takeDamage(15);

    std::cout << "Original Enemy: " << originalEnemy << "\n";
    std::cout << "Original " << originalEnemy.getDamageStats() << "\n";

    Enemy copyEnemy = originalEnemy;
    std::cout << "Copied Enemy (via Copy Constructor): " << copyEnemy << "\n";

    originalEnemy.takeDamage(30);
    std::cout << "After damaging original again:\n";
    std::cout << "  Original HP: " << originalEnemy.getHealth() << " | " << originalEnemy.getDamageStats() << "\n";
    std::cout << "  Copied   HP: " << copyEnemy.getHealth() << " | " << copyEnemy.getDamageStats() << "\n";

    Enemy assignedEnemy("Dummy", Position(0,0), 10, 1, 5);
    assignedEnemy = originalEnemy;
    std::cout << "Assigned Enemy (via operator=): " << assignedEnemy << "\n\n";

    runInteractiveMenu(game);

    std::cout << "\n====================================================\n";
    std::cout << "              GAME ENDED - FINAL SCORE              \n";
    std::cout << "Final Score: " << game.getScore() << " | Wave Reached: " << game.getWave() << "\n";
    std::cout << "====================================================\n";

    return 0;
}
