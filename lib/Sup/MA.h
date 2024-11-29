#include<Arduino.h>
#include<myVector.h>
#pragma once

class MA{   
    private:
        int lenth;
        float record[1002];
        int count = 0;
    public:
        void setLenth(int);
        float demandAve(float);
        float sum(float);
        float returnAve();
        float returnSum();
        void reset();
};


class MA_vec{
        int lenth;
        Vector2D record[1002];
        int count = 0;
    public:
        void setLenth(int);
        Vector2D demandAve(Vector2D);
        Vector2D sum(Vector2D);
        Vector2D returnAve();
        void reset();
};