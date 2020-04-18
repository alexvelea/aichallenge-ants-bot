#pragma once
#include "bug.hpp"
#include "location.hpp"
#include "square.hpp"
#include "timer.hpp"
#include <vector>

/* constants */
const int kNumDirections = 4;
const char kDirectionChr[4] = {'N', 'E', 'S', 'W'};
const int kDirectionOffset[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}}; //{N, E, S, W}

/* struct to store current state information */
struct State {
    int rows, cols, turn, turns, no_players;
    double attack_radius, spawn_radius, view_radius;
    double load_time, turn_time;
    std::vector<double> scores;
    bool game_over;
    int64_t seed;

    std::vector<std::vector<Square>> grid;
    std::vector<Location> my_ants, enemy_ants, my_hills, enemy_hills, food;

    Timer timer;
    Bug bug;

    State();
    ~State();

    void setup();
    void reset();

    void makeMove(const Location &loc, int direction);

    double distance(const Location &loc1, const Location &loc2);
    Location getLocation(const Location &start_loc, int direction);

    void updateVisionInformation();
};

std::ostream &operator<<(std::ostream &os, const State &state);
std::istream &operator>>(std::istream &is, State &state);
