#include<Arduino.h>
#pragma once

class angle{
    public:
        angle(double,bool);
        angle(double,bool,double,bool);
        void setAng(double,bool);
        double to_range(double,bool);
        float degree;
        double radians;
        void operator=(double);
        void operator-=(double);
        void operator+=(double);
};