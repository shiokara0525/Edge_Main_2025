#pragma once

#include<Arduino.h>
#include<input.h>
#include<output.h>
#include<timer.h>
#include<Sup.h>
#include<central_availables.h>

#define READ_SIDE 0
#define READ_BACK 1
#define READ_BACK_ABOUT 2
#define READ_FRONT 3

#define MOVING_ONLY_AC 0
#define MOVING_LINE_TRACE 1
#define MOVING_NO_LINE 2
#define MOVING_STOP 3

#define STOP_NO 0
#define STOP_YES 1
#define STOP_NO_FAST 2

#define REAN_YES 1
#define REAN_NO 0




class Defence{
    private:
        timer Timer;
        int M_time;

        int is_on_lineside = 0;
        State Lside;
        
        int is_center = 0;
        State Center;

        State ball_fast;

        timer A_12_t;

        int A = 0;
        int B = 999;
        int c = 0;
        int stop_range = 20;
        const int far_th = 130;
        int go_val = 220;


        int line_F = 0;     //A=15のとき、ステート抜けるか判定するフラグ

        int line_none_flag = 0;

        int A_15_flag;

        int Stop_flag;

        byte return_num[4];
        timer Mode_timer;
    public:
        Vector2D defence();
        void available_set();
        int get_A();
        byte* get_flag();
};