#pragma once


#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>


class AC{
public:
  double getAC_val(); //姿勢制御用の値返す関数
  float getCam_val(float);
  float getnowdir();
  int flag = 0;  //モーターが突然反転しないようにするやつ
  void print();  //姿勢制御関連のやつを表示
  void setup();  //姿勢制御のセットアップ
  void setup_2();
  double dir = 0;  //現Fの方向
  double dir_n = 0;  //現Fの方向(生の値)
  double c_ang = 0;
  float dir_target;

  double kkp = 0;  //比例制御の値
  double kkd = 0;  //微分制御の値

  double val = 0;  //姿勢制御の値
  double kkp_old = 0;  //前Fの方向
  float first;
  int Time;

private:
  double nowTime = 0;  //関数で見た時の時間
  double time_old = 0; //1F前の時間

  double val_old = 0;  //1F前の姿勢制御の値
  double dir_old = 0;
  double c_ang_old = 0;

  const float kp = 2.0;  //比例制御の係数
  const float kd = 1.5; //微分制御の係数

  const float kp_c = 1.0;
  const float kd_c = 0.3;
  const float time = 100 / 3;


  sensors_event_t event;  //ジャイロのいろんな値入れるやつ
  Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
};