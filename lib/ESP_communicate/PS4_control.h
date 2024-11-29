#pragma once

#include<Sup.h>
#include<input.h>
#include<output.h>

class PS4_control{
public:
    PS4_control();
    void run();
    int r,theta;
    int R_r,R_theta;
    int Rotate_flag = 0;
    int Circle,Square;
};