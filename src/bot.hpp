#pragma once
#include "state.hpp"

/* This struct represents your bot in the game of Ants */
struct Bot {
  public:
    State state;

    Bot();

    void playGame(); // plays a single game of Ants

    void makeMoves(); // makes moves for a single turn
    void endTurn();   // indicates to the engine that it has made its moves
};
