#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

// ─── Base exception for all Tower Defense errors ───────────────────────────
class TowerDefenseException : public std::exception {
    std::string message_;
public:
    explicit TowerDefenseException(const std::string& msg);
    const char* what() const noexcept override;
};

// ─── Thrown when placing a tower the player cannot afford ──────────────────
class InsufficientGoldException : public TowerDefenseException {
    int required_;
    int available_;
public:
    InsufficientGoldException(int required, int available);
    int getRequired()  const;
    int getAvailable() const;
};

// ─── Thrown when a position is outside the game grid ──────────────────────
class InvalidPositionException : public TowerDefenseException {
    int x_;
    int y_;
public:
    InvalidPositionException(int x, int y);
    int getX() const;
    int getY() const;
};

// ─── Thrown when simulateTurn() is called on a finished game ──────────────
class GameOverException : public TowerDefenseException {
public:
    GameOverException();
};

#endif
