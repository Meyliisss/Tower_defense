#ifndef GAME_H
#define GAME_H

#include <ostream>
#include <vector>
#include <string>
#include "Tower.h"
#include "Enemy.h"
#include "Position.h"

class Game {
public:
    static constexpr int GRID_W = 20;
    static constexpr int GRID_H = 15;
private:
    std::string        playerName;
    std::vector<Tower> towers;
    std::vector<Enemy> enemies;
    Position           castlePos;
    int                gold;
    int                lives;
    int                wave;
    int                score;

    bool hasEnoughGold(int cost)               const;
    int  findNearestEnemyIndex(const Tower& t) const;

public:
    Game(const std::string& playerName, int startGold, int startLives);

    int  getGold()     const;
    int  getLives()    const;
    int  getWave()     const;
    int  getScore()    const;
    bool isGameOver()  const;

    bool placeTower(const Tower& tower);
    void addEnemy(const Enemy& enemy);
    void startNewWave(int numEnemies);
    int  simulateTurn();
    void displayStatus()   const;
    void drawGrid()        const;

    friend std::ostream& operator<<(std::ostream& os, const Game& g);
};

#endif
