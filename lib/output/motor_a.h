#pragma once

#include<Arduino.h>
#include<Sup.h>


#define motor_max 3     //移動平均で使う配列の大きさ

class motor_attack{
    public:
        int output[4] = {0,0,0,0};
        motor_attack();
        void moveMotor_L(angle ang,int val,double ac_val,Vector2D line);
        void moveMotor_0(angle ang,int val,double ac_val,int flag);
        void moveMotor(Vector2D vec_,int speed,double ac_val,int flag);
        void motor_0();
        void motor_ac(float);
        int line_val = 2;
        int NoneM_flag = 0;
        float Moutput(int,int);
        void Moutput_all();
    private:
        int PWM_p[5][2] = {
            {4,5},{6,7},{9,8},{10,11},{2,3}
        };
        double mSin[4] = {1,1,-1,-1};  //行列式のsinの値
        double mCos[4] = {1,-1,-1,1};  //行列式のcosの値
        MA Motor[4];
};