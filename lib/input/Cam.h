#pragma once

#include<timer.h>
#include<MA.h>


#define BLUE 0
#define YELLOW 1

class Cam{
    private:
        int Yellow_x1 = 0;
        int Yellow_y1 = 0;
        int Yellow_x2 = 0;
        int Yellow_y2 = 0;
        int Yellow_x = 0;
        int yellow_h = 0;
        int blue_x1 = 0;
        int blue_y1 = 0;
        int blue_x2 = 0;
        int blue_y2 = 0;
        int blue_x = 0;
        int blue_h = 0;
        int F = 0;
    public:
        Cam(int);
        int data_byte_b[7];
        int data_byte_y[7];
        int getCamdata();
        float ang;
        float Size;
        int on = 0;
        int color = 0;
        int senter;
        MA ang_;
        void print();
        void begin();
};