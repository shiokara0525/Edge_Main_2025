#pragma once

#include<Arduino.h>
#include<timer.h>
#define led 13

class Kicker{
    public:
        void setup();
        int run(int);
        int stop();
        void TEST_();
        int kick_flag = 0;

    private:
        timer kick_time;
        const int C = 32;
        const int K = 31;
};