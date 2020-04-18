#pragma once
#include <vector>

/* struct for representing a square in the grid. */
struct Square {
    bool is_visible, is_water, is_hill, is_food;
    int ant, hill_player;
    std::vector<int> dead_ants;

    Square() {
        is_visible = is_water = is_hill = is_food = false;
        ant = hill_player = -1;
    };

    // resets the information for the square except water information
    void reset() {
        is_visible = false;
        is_hill = false;
        is_food = false;
        ant = hill_player = -1;
        dead_ants.clear();
    };
};
