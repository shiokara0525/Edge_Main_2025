#pragma once

#include<Arduino.h>
#include<output.h>
#include<input.h>
#include<central_availables.h>

class Attack{
    private:
        int A = 0;
        int B = 999;
        int c = 0;

        float RA_a = 1.8;  //45~90°の時の回り込みの係数
        float RA_b = 2.5;
        float RA_c = 1.8;
        float RA_d;
        float RA_e;
        float AC_cam_coef = 1.0;

        float ang_0 = 10;
        float ang_20 = 45;
        float ang_30 = 72;
        float ang_45 = 135;

        float ang_old = 0;

        int cam_front_on = 0;
        State Catch;

        int back_count = 0;
        int go_flag = 0;     //ライン際でボールにアタックするかしないか
        int back_flag = 0;   //ラインから逃げるときは問答無用で出力マックス


        int rake_flag = 0;   //ボールの書き出し
        State rake;

        int first_ang = 0;   //セットプレイの時の初期角度

        timer play_time;

        int go_val;
        int go_val_back = 255;


        timer Timer;
        State CFO;

        byte return_byte[4];

        State ball_front;

        MA goang_ma;
    public:
        void attack();
        void available_set(int*);
        byte* getCheckval();
        int check_val[6];
        int send_val[2];
        int setplay_flag = 0;
        int line_flag;
};