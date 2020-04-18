#pragma once
#include <sys/time.h>

/* struct for checking how long it has been since the start of the turn. */
struct Timer {
    double start_time;

    Timer() : start_time() {
        this->start();
    };

    // starts the timer
    void start() {
        timeval timer;
        gettimeofday(&timer, nullptr);
        start_time = timer.tv_sec + (timer.tv_usec / 1000000.0);
    };

    // returns how long it has been since the timer was last started in milliseconds
    double getTime() {
        timeval timer;
        gettimeofday(&timer, nullptr);
        double current_time = timer.tv_sec + (timer.tv_usec / 1000000.0);
        return (current_time - start_time) * 1000.0;
    };
};
