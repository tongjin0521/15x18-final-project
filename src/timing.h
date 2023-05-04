/**
 * @file timing.h
 * @brief Timing class declaration
 * @details This class is used to time the execution of the program. It is borrowed from previous assignments.
 */

#ifndef TIMING_H
#define TIMING_H

#include <chrono>

class Timer
{
public:
    Timer() : beg_(clock_::now()) {}
    void reset() { beg_ = clock_::now(); }
    double elapsed() const
    {
        return std::chrono::duration_cast<second_>(clock_::now() - beg_).count();
    }

private:
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1>> second_;
    std::chrono::time_point<clock_> beg_;
};

#endif