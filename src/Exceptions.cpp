#include "Exceptions.h"

// ─── TowerDefenseException ─────────────────────────────────────────────────

TowerDefenseException::TowerDefenseException(const std::string& msg)
    : message_(msg) {}

const char* TowerDefenseException::what() const noexcept {
    return message_.c_str();
}

// ─── InsufficientGoldException ─────────────────────────────────────────────

InsufficientGoldException::InsufficientGoldException(int required, int available)
    : TowerDefenseException(
          "Insufficient gold: need " + std::to_string(required) +
          ", but only have " + std::to_string(available)),
      required_(required), available_(available) {}

int InsufficientGoldException::getRequired()  const { return required_;  }
int InsufficientGoldException::getAvailable() const { return available_; }

// ─── InvalidPositionException ──────────────────────────────────────────────

InvalidPositionException::InvalidPositionException(int x, int y)
    : TowerDefenseException(
          "Invalid position: (" + std::to_string(x) + ", " +
          std::to_string(y) + ") is outside the grid"),
      x_(x), y_(y) {}

int InvalidPositionException::getX() const { return x_; }
int InvalidPositionException::getY() const { return y_; }

// ─── GameOverException ─────────────────────────────────────────────────────

GameOverException::GameOverException()
    : TowerDefenseException("Cannot simulate turn: game is already over") {}
