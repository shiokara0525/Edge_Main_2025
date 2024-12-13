#pragma once

#define DELTA_T 0.025
#define BALL_MAX_NUM 100
#include<Arduino.h>
#include<Sup.h>


class BALL{
    public:
        BALL();
        int getBallposition();
        void print();
        void begin();
        void get_data(byte *data){
            for(int i = 0; i < 10; i++){
                data_byte[i] = data[i + 1];
            }
        }
        MA Bang_dif_;
        MA_vec B_dif_;
        MA_vec B_v;
        MA ball_catch_MA;
        MA ball_down[4];
        MA Far;
        MA x_MA;
        MA y_MA;
        float ang;
        float ang_old;
        float far;
        float Bang_dif;

        float world_far;
        Vector2D world_vec;
        Vector2D world_vec_old;

        Vector2D vec;
        Vector2D vec_down;
        Vector2D vec_old;
        Vector2D vec_velocity;
        Vector2D vec_velocity_old;
        Vector2D vec_acc;

        int flag = 1;
        int catch_val; //補足センサのアナログの値
        int ball_get;
        State is_get;
        int get_th = 80;


        int get_1;
        int get_2;
        int down_val[4];
    private:
        byte data_byte[10];
        const int CATCH_PIN = A9;
};