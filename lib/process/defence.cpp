#include<defence.h>

void Defence::available_set(){
  go_val = central.return_Motor_max();
  A = 10;
  c = 0;
  Mode_timer.reset();
}

int Defence::get_A(){
  return A;
}

byte* Defence::get_flag(){
  return_num[0] = ball.vec_velocity.return_magnitude();
  return_num[1] = Stop_flag;
  return_num[2] = A;
  return_num[3] = A_15_flag; 
  return return_num;
}

Vector2D Defence::defence(){
  angle go_ang(ball.ang,true);         //進む角度のオブジェクト

  float AC_val = 100;                  //姿勢制御の出力
  int max_val = go_val;                //進む出力
  float target = central.ac_tirget;           //目標角度

  float line_val = 1.0;

  int AC_flag = 0;                     //0だったら絶対的な角度とる 1だったらゴール向く
  int kick_ = 0;                       //0だったらキックしない 1だったらキック
  int moving_sort = MOVING_LINE_TRACE; //1だったら動き続ける 0だったら止まる
  Stop_flag = STOP_NO;
  is_on_lineside = 0;
  is_center = 0;

  /*---------------------------------------------------------動作決定ステート--------------------------------------------------------*/


  if(A == 5){  //ボールがない時止まる
    if(A != B){
      B = A;
    }
    moving_sort = MOVING_STOP;
  }



  if(A == 10){  //ライントレース(アルゴリズムブログで書きたいな)
    if(A != B){
      B = A;
      Timer.reset();
      Center.reset();
    }
    int go_flag = 0;
    double go_border[2];  //ボールの角度によって進む方向を変えるためのボーダーの変数(ラインに対して垂直な直線で進む角度の区分を分けるイメージ)
    angle balldir(ball.ang,true);  //ボールの角度を入れるオブジェクト

    if(2 < line.num){  //ラインが3点以上で交わってたら例外的にする
      line.ang = 90;
    }

    if(line.ang < 0){  //ボーダーは-180~180であってほしいよね
      go_border[0] = line.ang;
      go_border[1] = line.ang + 180;
    }
    else{
      go_border[0] = line.ang - 180;
      go_border[1] = line.ang;
    }

    balldir.to_range(go_border[0],false);  //ボールの角度をボーダーの範囲に収める(go_border[0] ~ go_border[1]+180)

    if(go_border[0] < balldir.degree && balldir.degree < go_border[1]){  //ボールの角度を区分分けする
      go_flag = 0;
    }
    else{
      go_flag = 1;
    }

    go_ang = go_border[go_flag] + 90;  //進む角度決定
    go_ang.to_range(180,true);  //進む角度を-180 ~ 180の範囲に収める


    int read_flag;  //1が真後ろ 2が後ろめ 3が前 0が真横

    if(150 < abs(go_ang.degree)){       //進む角度が真後ろにあるとき
      read_flag = READ_BACK;
      go_ang += 180;
    }
    else if(115 < abs(go_ang.degree)){
      read_flag = READ_BACK_ABOUT;

      line_val = 1.5;
      max_val -= 60;
    }
    else if(abs(go_ang.degree) < 60){  //前めに進むとき
      read_flag = READ_FRONT;

      line_val = 2.0;
      max_val -= 60;
      if(cam_back.on && cam_back.Size < 20){
        is_on_lineside = 1;
      }
      else{
        is_on_lineside = 0;
      }
    }
    else{                              //横に進むとき
      read_flag = READ_SIDE;

      line_val = 1.0;
      
      if(cam_back.on == 0){
        if(cam_back.ang < 0){
          go_ang = -90;
        }
        else{
          go_ang = 90;
        }
      }
      else{
        if(cam_back.senter && 135 < abs(ball.ang) && abs(ball.ang) < 172){
          moving_sort = MOVING_ONLY_AC;
        }
      }
    }
    // Serial.print(" godir : ");
    // Serial.println(go_ang.degree);

    ball_fast.enterState(ball.vec_velocity.return_magnitude() > 24);
    for(int i = 0; i < 2; i++){
      int dif_val = abs(ball.ang - go_border[i]);
      if(dif_val < stop_range && read_flag != READ_BACK && cam_back.on){  //正面方向にボールがあったら停止するよ
        if(ball_fast.readStateTimer(0) < 100){
          Stop_flag = STOP_NO_FAST;  //ボールの速度を原因にストップしてないフラグ
        }
        else{
          moving_sort = MOVING_ONLY_AC;
          max_val = 0;
          Stop_flag = STOP_YES;  //普通に止まるフラグ
        }
      }
    }

    // if(60 < abs(abs(go_ang.degree) - 90)){
    //   if(cam_back.on && cam_back.Size < 20){
    //     is_on_lineside = 1;
    //   }
    //   else{
    //     is_on_lineside = 0;
    //   }
    // }

    Lside.enterState(is_on_lineside);
    if(600 < Lside.readStateTimer(1) && 2000 < Mode_timer.read_ms()){
      A = 15;
      A_15_flag = 2;
      c = 1;
    }

    if(BALL_MAX_NUM * 1.25 < ball.far && abs(ball.ang) < 30){  //ぼーるが近くにあったら小突くやつ
      is_center = 1;
    }

    if(ball.ball_get){  //ロボットが押し込まれてたら
      if(ac.dir < 0){
        if(go_ang.degree < 0){
        }
        else{
          go_ang += 180;  //ロボットの動きを受け流す(意図はわからん)
        }
      }
      else{
        if(0 < go_ang.degree){
        }
        else{
          go_ang += 180;
        }
      }
      moving_sort = MOVING_LINE_TRACE;
    }

    Center.enterState(is_center);

    if(500 < Center.readStateTimer(1) && 2000 < A_12_t.read_ms()){
      A = 12;
      c = 1;
      Center.enterState(0);
    }

    go_ang.to_range(180,true);  //進む角度を-180 ~ 180の範囲に収める
    // Serial.print(" Lside : ");
    // Serial.print(is_on_lineside);

    if(!line.LINE_on){
      A = 20;
    }
  }




  if(A == 12){  //ボールを小突くやつ
    if(A != B){
      B = A;
      Timer.reset();
      A_12_t.reset();
      line_none_flag = 0;
    }

    go_ang = abs(ball.ang);

    go_ang = go_ang.degree * (ball.ang < 0 ? -1 : 1);
    moving_sort = MOVING_NO_LINE;
    max_val -= 30;

    if(!line.LINE_on){
      line_none_flag = 1;
    }

    if(300 < Timer.read_ms() && ball.ball_get){
      A = 13;
      c = 1;
    }
    
    if(!ball.ball_get){
      if(350 < Timer.read_ms() && line.LINE_on && line_none_flag && cam_back.Size < 40){
        A = 15;
        A_15_flag = 3;
        c = 1;
        is_on_lineside = 1;
        Serial.println(" !!! 4 !!! ");
      }
      if(550 < Timer.read_ms()){
        A = 15;
        A_15_flag = 4;
        Serial.println(" !!! 5 !!! ");
      }
    }
    if(45 < abs(ball.ang)){
      A = 15;
      A_15_flag = 7;
      Serial.println(" !!! 6 !!! ");
    }

  }


  if(A == 13){
    if(A != B){
      B = A;
      Timer.reset();
    }
    go_ang = 0;
    moving_sort = MOVING_NO_LINE;
    AC_flag = 1;

    if(!line.LINE_on){
      line_none_flag = 1;
    }

    if(350 < Timer.read_ms()){
      kick_ = 1;
    }

    if(line.LINE_on){
      if(cam_back.Size < 20 && abs(abs(degrees(line.vec_first.return_arg())) - 90) < 20){
        A = 15;
        A_15_flag = 5;
        Serial.println(" !!! 1 !!! ");
      }
      else{
        c = 0;
        Serial.println(" !!! 2 !!! ");
      }
    }
    else if((500 < Timer.read_ms()) && line.LINE_on == 0 && 90 < abs(line.ang_old)){
      A = 15;
      A_15_flag = 6;
      Serial.println(" !!! 3 !!! ");
    }
  }



  if(A == 15){  //ゴール際に戻る
    if(A != B){
      B = A;
      Timer.reset();
      line_F = 2;
      if(is_on_lineside == 1){
        line_F = 1;
      }
    }
    go_ang = -cam_back.ang + 180;
    moving_sort = MOVING_NO_LINE;
    max_val -= 45;

    if(A_15_flag == 0){
      moving_sort = MOVING_ONLY_AC;
      AC_flag = 0;
    }
    else{
      moving_sort = MOVING_NO_LINE;
      AC_flag = 0;
    }

    if(line_F == 1){
      if(line.LINE_on == 0){
        line_F++;
      }
      // digitalWrite(LED,LOW);
    }
    if(line_F == 2){
      if(line.LINE_on == 1 && 200 < Timer.read_ms()){
        line_F++;
      }
      // digitalWrite(LED,HIGH);
    }
    if(line_F == 3){
      if(300 < Timer.read_ms()){
        line_F++;
      }
      go_ang = 0;
      // digitalWrite(LED,LOW);
    }

    if(line_F == 4){
      A = 16;
    }
  }


  if(A == 16){  //ラインに戻るときちょっと前出る
    if(A != B){
      B = A;
      Timer.reset();
    }
    if(Timer.read_ms() < 50){
      go_ang = 0;
      moving_sort = MOVING_NO_LINE;
    }
    else{
      A = 10;
    }
  }



  if(A == 20){  //ロボットがちょっと押し出されちゃったから戻る
    if(A != B){
      B = A;
      Timer.reset();
    }
    go_ang = line.ang_old;
    moving_sort = MOVING_NO_LINE;

    if(500 < Timer.read_ms()){
      if(90 < abs(line.ang_old)){
        A = 15;
        A_15_flag = 7;
        c = 1;
      }
      else if(60 < abs(abs(line.ang_old) - 90)){
        if(cam_back.on && cam_back.Size < 20){
          A = 15;
          A_15_flag = 7;
          c = 1;
        }
      }
    }



    if(30 < abs(ac.dir)){
      if(90 < abs(line.ang_old)){
        A = 15;
        A_15_flag = 1;
      }
    }

    if(line.LINE_on){
      A = 10;
    }
  }



  /*---------------------------------------------------------出力ステート--------------------------------------------------------*/


  ac.dir_target = target;

  if(AC_flag == 0){
    AC_val = ac.getAC_val();
  }
  else if(AC_flag == 1){
    AC_val = ac.getCam_val(-cam_front.ang);
  }

  Serial.print(" A : ");
  Serial.println(A);



  Vector2D go_vec;
  go_vec.set_polar(go_ang.degree,1.0);

  if(moving_sort == MOVING_LINE_TRACE){
    go_vec = go_vec + line_val * line.vec;
  }

  int motor_on = 0;

  if(moving_sort == MOVING_LINE_TRACE || moving_sort == MOVING_NO_LINE){
    motor_on = MOTOR_ON;
  }
  else if(moving_sort == MOVING_STOP){
    motor_on = MOTOR_STOP;
  }
  else if(moving_sort == MOVING_ONLY_AC){
    motor_on = MOTOR_AC_ONLY;
  }

  central.set_states(go_vec,max_val,motor_on,AC_val,AC_flag,kick_);

  return go_vec;
}