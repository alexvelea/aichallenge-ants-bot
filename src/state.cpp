#include "state.hpp"
#include <math.h>
#include <iostream>
#include <queue>
using namespace std;

State::State() {
    game_over = false;
    turn = 0;
    bug.open("./debug.txt");
};

State::~State() {
    bug.close();
};

// sets the state up
void State::setup() {
    grid = vector<vector<Square>>(rows, vector<Square>(cols, Square()));
};

// resets all non-water squares to land and clears the bots ant vector
void State::reset() {
    my_ants.clear();
    enemy_ants.clear();
    my_hills.clear();
    enemy_hills.clear();
    food.clear();
    for (int row = 0; row < rows; row++)
        for (int col = 0; col < cols; col++)
            if (!grid[row][col].is_water)
                grid[row][col].reset();
};

// outputs move information to the engine
void State::makeMove(const Location &loc, int direction) {
    cout << "o " << loc.row << " " << loc.col << " " << kDirectionChr[direction] << endl;

    Location n_loc = getLocation(loc, direction);
    grid[n_loc.row][n_loc.col].ant = grid[loc.row][loc.col].ant;
    grid[loc.row][loc.col].ant = -1;
};

// returns the euclidean distance between two locations with the edges wrapped
double State::distance(const Location &loc1, const Location &loc2) {
    int d1 = abs(loc1.row - loc2.row), d2 = abs(loc1.col - loc2.col), dr = min(d1, rows - d1), dc = min(d2, cols - d2);
    return sqrt(dr * dr + dc * dc);
};

// returns the new location from moving in a given direction with the edges wrapped
Location State::getLocation(const Location &start_loc, int direction) {
    return Location((start_loc.row + kDirectionOffset[direction][0] + rows) % rows, (start_loc.col + kDirectionOffset[direction][1] + cols) % cols);
};

/*
    This function will update update the lastSeen value for any squares currently
    visible by one of your live ants.

    BE VERY CAREFUL IF YOU ARE GOING TO TRY AND MAKE THIS FUNCTION MORE EFFICIENT,
    THE OBVIOUS WAY OF TRYING TO IMPROVE IT BREAKS USING THE EUCLIDEAN METRIC, FOR
    A CORRECT MORE EFFICIENT IMPLEMENTATION, TAKE A LOOK AT THE GET_VISION FUNCTION
    IN ANTS.PY ON THE CONTESTS GITHUB PAGE.
*/
void State::updateVisionInformation() {
    std::queue<Location> loc_queue;
    Location c_loc, n_loc;

    for (const auto &s_loc : my_ants) {
        loc_queue.push(s_loc);

        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
        grid[s_loc.row][s_loc.col].is_visible = true;
        visited[s_loc.row][s_loc.col] = true;

        while (!loc_queue.empty()) {
            c_loc = loc_queue.front();
            loc_queue.pop();

            for (int d = 0; d < kNumDirections; d++) {
                n_loc = getLocation(c_loc, d);

                if (!visited[n_loc.row][n_loc.col] && distance(s_loc, n_loc) <= view_radius) {
                    grid[n_loc.row][n_loc.col].is_visible = true;
                    loc_queue.push(n_loc);
                }
                visited[n_loc.row][n_loc.col] = true;
            }
        }
    }
};

/*
    This is the output function for a state. It will add a char map
    representation of the state to the output stream passed to it.

    For example, you might call "cout << state << endl;"
*/
ostream &operator<<(ostream &os, const State &state) {
    for (int row = 0; row < state.rows; row++) {
        for (int col = 0; col < state.cols; col++) {
            if (state.grid[row][col].is_water)
                os << '%';
            else if (state.grid[row][col].is_food)
                os << '*';
            else if (state.grid[row][col].is_hill)
                os << (char)('A' + state.grid[row][col].hill_player);
            else if (state.grid[row][col].ant >= 0)
                os << (char)('a' + state.grid[row][col].ant);
            else if (state.grid[row][col].is_visible)
                os << '.';
            else
                os << '?';
        }
        os << endl;
    }

    return os;
};

// input function
istream &operator>>(istream &is, State &state) {
    int row, col, player;
    string input_type, junk;

    // finds out which turn it is
    while (is >> input_type) {
        if (input_type == "end") {
            state.game_over = true;
            break;
        } else if (input_type == "turn") {
            is >> state.turn;
            break;
        } else // unknown line
            getline(is, junk);
    }

    if (state.turn == 0) {
        // reads game parameters
        while (is >> input_type) {
            if (input_type == "loadtime")
                is >> state.load_time;
            else if (input_type == "turntime")
                is >> state.turn_time;
            else if (input_type == "rows")
                is >> state.rows;
            else if (input_type == "cols")
                is >> state.cols;
            else if (input_type == "turns")
                is >> state.turns;
            else if (input_type == "player_seed")
                is >> state.seed;
            else if (input_type == "viewradius2") {
                is >> state.view_radius;
                state.view_radius = sqrt(state.view_radius);
            } else if (input_type == "attackradius2") {
                is >> state.attack_radius;
                state.attack_radius = sqrt(state.attack_radius);
            } else if (input_type == "spawnradius2") {
                is >> state.spawn_radius;
                state.spawn_radius = sqrt(state.spawn_radius);
            } else if (input_type == "ready") // end of parameter input
            {
                state.timer.start();
                break;
            } else // unknown line
                getline(is, junk);
        }
    } else {
        // reads information about the current turn
        while (is >> input_type) {
            if (input_type == "w") // water square
            {
                is >> row >> col;
                state.grid[row][col].is_water = true;
            } else if (input_type == "f") // food square
            {
                is >> row >> col;
                state.grid[row][col].is_food = true;
                state.food.emplace_back(Location(row, col));
            } else if (input_type == "a") // live ant square
            {
                is >> row >> col >> player;
                state.grid[row][col].ant = player;
                if (player == 0)
                    state.my_ants.emplace_back(Location(row, col));
                else
                    state.enemy_ants.emplace_back(Location(row, col));
            } else if (input_type == "d") // dead ant square
            {
                is >> row >> col >> player;
                state.grid[row][col].dead_ants.push_back(player);
            } else if (input_type == "h") {
                is >> row >> col >> player;
                state.grid[row][col].is_hill = true;
                state.grid[row][col].hill_player = player;
                if (player == 0)
                    state.my_hills.emplace_back(Location(row, col));
                else
                    state.enemy_hills.emplace_back(Location(row, col));

            } else if (input_type == "players") // player information
                is >> state.no_players;
            else if (input_type == "scores") // score information
            {
                state.scores = vector<double>(state.no_players, 0.0);
                for (int p = 0; p < state.no_players; p++)
                    is >> state.scores[p];
            } else if (input_type == "go") // end of turn input
            {
                if (state.game_over)
                    is.setstate(std::ios::failbit);
                else
                    state.timer.start();
                break;
            } else // unknown line
                getline(is, junk);
        }
    }

    return is;
};
