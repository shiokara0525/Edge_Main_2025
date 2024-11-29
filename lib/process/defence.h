#pragma once

#include<Arduino.h>
#include<input.h>
#include<output.h>
#include<timer.h>
#include<Sup.h>
#include<central_availables.h>


class Defence{
    private:
        timer Timer;
        int M_time;

        int Lside_A = 0;
        State Lside;
        
        int Center_A = 0;
        State Center;
        State Camback_on;

        State ball_fast;

        timer A_12_t;
        int go_flag = 0;

        State ball_back;
        float last_goang;

        State gotoSide;

        int A_15_front;

        int A = 0;
        int B = 999;
        int c = 0;
        int stop_range = 20;
        const int far_th = 130;
        int go_val = 220;
        int print_flag = 1; // 1だったらシリアルプリントする
        int line_F = 0;     //A=15のとき、ステート抜けるか判定するフラグ
        int back_Flag = 0;  //A=15に行くフラグ
        int push_flag = 0;  //押し込まれてるフラグ

        int line_none_flag = 0;

        int A_15_flag;
        int A_15_back_flag;

        int Stop_flag;

        byte return_num[4];
        timer Mode_timer;
    public:
        void defence();
        void available_set();
        int get_A();
        byte* get_flag();
};