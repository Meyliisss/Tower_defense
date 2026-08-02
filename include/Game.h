#ifndef GAME_H
#define GAME_H

#include <ostream>
#include <vector>
#include <string>
#include <memory>
#include "TowerSlot.h"
#include "Tower.h"
#include "Enemy.h"
#include "Position.h"

// ─── Wave configuration loaded from file ──────────────────────────────────
struct WaveConfig {
    int baseHp;
    int hpPerWave;
    int baseReward;
    int rewardIncrement;
};

// ─── Game ─────────────────────────────────────────────────────────────────
class Game {
public:
    static constexpr int GRID_W = 20;
    static constexpr int GRID_H = 15;

private:
    std::string            playerName_;
    std::vector<TowerSlot> towers_;
    std::vector<Enemy>     enemies_;
    Position               castlePos_;
    int                    gold_;
    int                    lives_;
    int                    wave_;
    int                    score_;
    WaveConfig             waveConfig_;

    static int             totalWavesGlobal_;   // across all Game instances

    static bool isValidPosition(const Position& pos);
    int  findNearestEnemyIndex(const TowerSlot& ts) const;

    static WaveConfig loadWaveConfig(const std::string& filename);

public:
    Game(const std::string& playerName, int startGold, int startLives,
         const std::string& waveConfigFile);

    int  getGold()    const;
    int  getLives()   const;
    int  getWave()    const;
    int  getScore()   const;
    bool isGameOver() const;
    static int getTotalWaves();

    // Throws InsufficientGoldException or InvalidPositionException on failure
    void placeTower(std::unique_ptr<Tower> tower);

    void addEnemy(const Enemy& enemy);
    void startNewWave(int numEnemies);

    // Throws GameOverException if the game is already over
    int  simulateTurn();

    void displayStatus() const;
    void drawGrid()      const;

    friend std::ostream& operator<<(std::ostream& os, const Game& g);
};

#endif
