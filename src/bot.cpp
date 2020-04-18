#include "bot.hpp"
#include <iostream>
using namespace std;

// constructor
Bot::Bot() = default;

// plays a single game of Ants.
void Bot::playGame() {
    // reads the game parameters and sets up
    cin >> state;
    state.setup();
    endTurn();

    // continues making moves while the game is not over
    while (cin >> state) {
        state.updateVisionInformation();
        makeMoves();
        endTurn();
    }
};

// makes the bots moves for the turn
void Bot::makeMoves() {
    state.bug << "turn " << state.turn << ":" << endl;
    state.bug << state << endl;

    // picks out moves for each ant
    for (int ant = 0; ant < (int)state.my_ants.size(); ant++) {
        for (int d = 0; d < kNumDirections; d++) {
            Location loc = state.getLocation(state.my_ants[ant], d);

            if (!state.grid[loc.row][loc.col].is_water) {
                state.makeMove(state.my_ants[ant], d);
                break;
            }
        }
    }

    state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};

// finishes the turn
void Bot::endTurn() {
    if (state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};
