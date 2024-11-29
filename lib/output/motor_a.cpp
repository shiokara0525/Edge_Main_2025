#include<motor_a.h>



motor_attack::motor_attack(){
  for(int i = 0; i < 5; i++){
    pinMode(PWM_p[i][0],OUTPUT);
    pinMode(PWM_p[i][1],OUTPUT);
  }
  for(int i = 0; i < 4; i++){
    Motor[i].setLenth(motor_max);
  }
  for(int i = 2; i < 12; i++){
    analogWriteFrequency(i,90000);
  }
    //モーターのピンと行列式に使う定数の設定
}



void motor_attack::moveMotor(Vector2D vec_,int speed,double ac_val,int flag){
  Vector2D vec(vec_.normalize());
  double h = 0;
  double Mval[4] = {0,0,0,0};  //モーターの値×4
  double max_val = speed;        //モーターの値の上限値
  double mval_x = vec.return_y();  //進みたいベクトルのx成分
  double mval_y = vec.return_x();  //進みたいベクトルのy成分

  if(flag == 1){
    mval_y = 0;
  }
  else if(flag == 2){
    mval_x = 0;
  }

  max_val -= ac_val;  //姿勢制御とその他のモーターの値を別に考えるために姿勢制御の値を引いておく
  
  for(int i = 0; i < 4; i++){
    Mval[i] = -mSin[i] * mval_x + mCos[i] * mval_y; //モーターの回転速度を計算(行列で管理)
  }

  for(int i = 0; i < 4; i++){
    Mval[i] = Mval[i] * max_val + ac_val;
    if(abs(Mval[i]) > h){  //絶対値が一番高い値だったら
      h = abs(Mval[i]);    //一番大きい値を代入
    }
  }

  for(int i = 0; i < 4; i++){  //モーターの値を計算するところだよ
    if(i == 0 || i == 3){
      Mval[i] = Mval[i] / h * max_val;  //モーターの値を計算(進みたいベクトルの値と姿勢制御の値を合わせる)
    }
    else{
      Mval[i] = Mval[i] / h * max_val + ac_val * 1.3;  //モーターの値を計算(進みたいベクトルの値と姿勢制御の値を合わせる)
    }
    Moutput(i,Mval[i]);
  }
}




void motor_attack::moveMotor_L(angle ang,int val,double ac_val,Vector2D line){  //モーター制御する関数
  double g = 0;                //モーターの最終的に出る最終的な値の比の基準になる値
  double h = 0;
  double Mval[4] = {0,0,0,0};  //モーターの値×4
  double max_val = val;        //モーターの値の上限値
  double mval_x = cos(ang.radians);  //進みたいベクトルのx成分
  double mval_y = sin(ang.radians);  //進みたいベクトルのy成分
  
  max_val -= ac_val;  //姿勢制御とその他のモーターの値を別に考えるために姿勢制御の値を引いておく
  
  for(int i = 0; i < 4; i++){
    Mval[i] = -mSin[i] *(mval_x + (1.0 + line.return_magnitude() * cos(radians(line.return_azimuth()))) * line_val)  + mCos[i] *(mval_y +line.return_magnitude() * sin(radians(line.return_azimuth())) * line_val);

    if(abs(Mval[i]) > g){  //絶対値が一番高い値だったら
      g = abs(Mval[i]);    //一番大きい値を代入
    }
  }

  for(int i = 0; i < 4; i++){  //移動平均求めるゾーンだよ
    Mval[i] /= g;  //モーターの値を制御(常に一番大きい値が1になるようにする)

    Mval[i] = Motor[i].demandAve(Mval[i]);

    if(abs(Mval[i]) > h){  //絶対値が一番高い値だったら
      h = abs(Mval[i]);    //一番大きい値を代入
    }
  }

  for(int i = 0; i < 4; i++){  //モーターの値を計算するところだよ
    
    if(i == 0 || i == 3){
      Mval[i] = Mval[i] / h * max_val;  //モーターの値を計算(進みたいベクトルの値と姿勢制御の値を合わせる)
    }
    else{
      Mval[i] = Mval[i] / h * max_val + ac_val * 1.3;  //モーターの値を計算(進みたいベクトルの値と姿勢制御の値を合わせる)
    }
    Moutput(i,Mval[i]);
  }
}


void motor_attack::moveMotor_0(angle ang,int val,double ac_val,int flag){
  double h = 0;
  double Mval[4] = {0,0,0,0};  //モーターの値×4
  double max_val = val;        //モーターの値の上限値
  double mval_x = cos(ang.radians);  //進みたいベクトルのx成分
  double mval_y = sin(ang.radians);  //進みたいベクトルのy成分

  if(flag == 1){
    mval_y = 0;
  }
  else if(flag == 2){
    mval_x = 0;
  }

  max_val -= ac_val;  //姿勢制御とその他のモーターの値を別に考えるために姿勢制御の値を引いておく
  
  for(int i = 0; i < 4; i++){
    Mval[i] = -mSin[i] * mval_x + mCos[i] * mval_y; //モーターの回転速度を計算(行列で管理)
  }

  for(int i = 0; i < 4; i++){
    Mval[i] = Mval[i] * max_val + ac_val;
    if(abs(Mval[i]) > h){  //絶対値が一番高い値だったら
      h = abs(Mval[i]);    //一番大きい値を代入
    }
  }

  for(int i = 0; i < 4; i++){  //モーターの値を計算するところだよ
    if(i == 0 || i == 3){
      Mval[i] = Mval[i] / h * max_val;  //モーターの値を計算(進みたいベクトルの値と姿勢制御の値を合わせる)
    }
    else{
      Mval[i] = Mval[i] / h * max_val + ac_val * 1.3;  //モーターの値を計算(進みたいベクトルの値と姿勢制御の値を合わせる)
    }
    Moutput(i,Mval[i]);
  }
}


void motor_attack::motor_ac(float ac_val){
  for(int i = 0; i < 4; i++){
    Moutput(i,ac_val);
  }
}


void motor_attack::motor_0(){  //モーターの値を0にする関数
  for(int i = 0; i < 4; i++){
    Moutput(i,0);
  }
}



float motor_attack::Moutput(int i,int Mval){
  if(NoneM_flag == 1){
    Mval = 0;
  }
  // if(i == 0){
  //   Mval *= 0.80;
  // }
  // if(i == 1){
  //   Mval *= 0.7;
  // }
  // if(i == 2){
  //   Mval *= 0.85;
  // }

  if(Mval < -255){
    Mval = -255;
  }
  else if(255 < Mval){
    Mval = 255;
  }
  if(0 < Mval){
    analogWrite(PWM_p[i][0],0);
    analogWrite(PWM_p[i][1],abs(Mval));
  }
  else if(Mval < 0){
    analogWrite(PWM_p[i][0],abs(Mval));
    analogWrite(PWM_p[i][1],0);
  }
  else if(Mval == 0){
    analogWrite(PWM_p[i][0],0);
    analogWrite(PWM_p[i][1],0);
  }
  return Mval;
}



void motor_attack::Moutput_all(){
  for(int i = 0; i < 4; i++){
    Moutput(i,output[i]);
  }
}