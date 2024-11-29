#pragma once

#include<Sup.h>

class central_aveilables{
public:
  State main_mode;  //モードの状態変数（予定）
  int Mode;         //モードの状態変数
  int Mode_old;
  int test_mode = 0;//テストのモード変数

  int color;        //ゴールの色
  int ac_tirget;    //角度の目標値
  int val_max = 200;//モーターの出力

  int Values[6];    //ESPから送られてくる変数

  timer Main_timer; //１周期の秒数のタイマー
  timer Mode_timer; //モードに居続けた変数

  timer Ball_period;//ボールの更新を管理するタイマー
  timer Line_period;//ラインの更新を管理するタイマー

  timer ball_Get;   //ボールの情報が送られてくる周期を測るタイマー
  timer line_Get;   //ラインの情報が送られてくる周期を測るタイマー
};

extern central_aveilables central;