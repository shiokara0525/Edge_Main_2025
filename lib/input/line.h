#pragma once

#include <timer.h>
#include <Arduino.h>
#include<Sup.h>
#define Long 5



class LINE{
public:
    int getLINE_Vec(); //ラインセンサのベクトル(距離,角度)を取得する関数
    int switchLineflag(float);
    float decideGoang(float,int);
    void print();
    void print_2();
    void begin();
    double line_switch(int,double,int); 
    LINE();

    double dis; //ラインのベクトルの長さ
    double ang; //ラインの和のベクトルの角度
    double ang_old = 180;
    float ang_side;
    float dis_X;
    float dis_Y;
    int side_flag;  //1は90°で検知、2は-90°で検知、3は0°、4は180°
    int num;
    int num_first;

    int flag;
    int data_on[27];
    uint8_t data_byte[4];
    Vector2D ele[27];
    int LINE_Level;
    Vector2D vec;
    Vector2D vec_first;
    Vector2D vec_go;

    float go_ang_old;
    float go_ang_first;

    State line_state;
 
    int LINE_on; //ラインがロボットの下になかったら0,あったら1にする
    int LINE_on_old = 999;
    int LINE_change = 0;
    int line_flag = 0;
    int firstDir_flag = 0;

private:
    int A = 0;
    timer timer1; //タイマーの宣言
};