#include<defence.h>

void Defence::available_set(){
  go_val = central.val_max;
  A = 0;
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

void Defence::defence(){
  angle go_ang(ball.ang,true);         //進む角度のオブジェクト

  float AC_val = 100;                  //姿勢制御の出力
  int max_val = go_val;                //進む出力
  float target = central.ac_tirget;           //目標角度

  int AC_flag = 0;                     //0だったら絶対的な角度とる 1だったらゴール向く
  int kick_ = 0;                       //0だったらキックしない 1だったらキック
  int M_flag = 1;                      //1だったら動き続ける 0だったら止まる
  Stop_flag = 0;
  Lside_A = 0;

  /*---------------------------------------------------------状況判断ステート--------------------------------------------------------*/


  if(back_Flag == 1 && line.LINE_on == 0){  //角度がある程度あるかつラインの外だからゴールのほうに戻るよ
    A = 15;
    A_15_flag = 1;
    c = 1;
  }

  Camback_on.enterState(cam_back.on);


  if(c == 0){  //平常時どうするか判定
    if(line.LINE_on == 1){
      if(ball.flag == 0){  //ボールがないまたはゴールの端にいるときとまる
        A = 5;
      }
      else{
        A = 10;
      }
    }
    else{
      A = 20;
    }
  }


  /*---------------------------------------------------------動作ステート--------------------------------------------------------*/
  Center_A = 0;


  if(A == 5){  //ボールがない時止まる
    if(A != B){
      B = A;
    }
    M_flag = 3;
  }



  if(A == 10){  //ライントレース(アルゴリズムブログで書きたいな)
    if(A != B){
      if(B == 15){
        A_15_back_flag = 1;
      }
      else{
        A_15_back_flag = 0;
      }
      B = A;
      Timer.reset();
      Center.reset();
    }
    int go_flag = 0;
    int side_stop_flag = 0; //ゴールが後ろにないから横に進むフラグ
    double go_border[2];  //ボールの角度によって進む方向を変えるためのボーダーの変数(ラインに対して垂直な直線で進む角度の区分を分けるイメージ)
    angle balldir(ball.ang,true);  //ボールの角度を入れるオブジェクト
    Lside_A = 0;

    if(2 < line.num){
      line.ang = 90;
    }

    if(line.ang < 0){
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

    int back_F = 0;
    int gotoSide_flag = 0;

    if(150 < abs(go_ang.degree)){       //進む角度が真後ろにあるとき
      // go_ang += 180;
      back_F = 1;
      M_flag = 3;
    }
    else if(115 < abs(go_ang.degree)){
      MOTOR.line_val = 1.5;
      max_val -= 60;
    }
    else if(abs(go_ang.degree) < 60){  //前めに進むとき
      MOTOR.line_val = 2;
      max_val -= 60;
      if(cam_back.on && cam_back.Size < 20){
        Lside_A = 1;
      }
      else{
        Lside_A = 0;
      }
    }
    else{                              //横に進むとき
      MOTOR.line_val = 1.0;
      if(go_ang.degree < 0){
        gotoSide_flag = 1;
      }
      else{
        gotoSide_flag = 2;
      }
      
      if(cam_back.on == 0){
        if(cam_back.ang < 0){
          go_ang = -90;
        }
        else{
          go_ang = 90;
        }
        side_stop_flag = 1;
      }
      else{
        if(cam_back.senter && 135 < abs(ball.ang) && abs(ball.ang) < 172){
          M_flag = 0;
        }
      }
    }
    // Serial.print(" godir : ");
    // Serial.println(go_ang.degree);

    Center_A = 0;
    ball_fast.enterState(ball.vec_velocity.return_magnitude() > 24);
    for(int i = 0; i < 2; i++){
      int dif_val = abs(ball.ang - go_border[i]);
      if(dif_val < stop_range && back_F == 0 && side_stop_flag == 0){  //正面方向にボールがあったら停止するよ
        if(ball_fast.readStateTimer(0) < 100){
          Stop_flag = 2;  //ボールの速度を原因にストップしてないフラグ
          gotoSide_flag = 0;
        }
        else{
          M_flag = 0;
          max_val = 0;
          Stop_flag = 1;  //普通に止まるフラグ
          gotoSide_flag = 0;
        }
      }
    }

    if(Stop_flag != 1){
      last_goang = go_ang.degree;
    }

    ball_back.enterState(Stop_flag);

    if(ball_back.readStateTimer(1) < 750){
      go_ang = last_goang;
    }

    if(60 < abs(abs(go_ang.degree) - 90)){
      if(cam_back.on && cam_back.Size < 20){
        Lside_A = 1;
      }
      else{
        Lside_A = 0;
      }
    }

    // Lside.enterState(Lside_A);
    // if(300 < Lside.readStateTimer(1) && 2000 < Mode_timer.read_ms()){
    //   A = 15;
    //   A_15_flag = 2;
    //   c = 1;
    // }

    if(BALL_MAX_NUM * 1.25 < ball.far && abs(ball.ang) < 30){  //ぼーるが近くにあったら小突くやつ
      Center_A = 3;
    }

    if(push_flag){  //ロボットが押し込まれてたら
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
      M_flag = 1;
    }

    Center.enterState(Center_A);

    if(Center_A == 3 && 500 < Center.readStateTimer(3) && 2000 < A_12_t.read_ms()){
      A = 12;
      c = 1;
      Center.enterState(0);
    }

    go_ang.to_range(180,true);  //進む角度を-180 ~ 180の範囲に収める

    if(Timer.read_ms() < 300 && A_15_back_flag){
      M_flag = 3;
    }
    // Serial.print(" Lside : ");
    // Serial.print(Lside_A);
    gotoSide.enterState(gotoSide_flag);
    if(250 < gotoSide.readStateTimer() && gotoSide_flag != 0){
      Serial.print(" max_val -= 60 ");
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
    M_flag = 2;
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
        Lside_A = 1;
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
    M_flag = 2;
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
      if(Lside_A == 1){
        line_F = 1;
      }
      A_15_front = 0;
    }
    go_ang = -cam_back.ang + 180;
    M_flag = 2;
    max_val -= 45;
    Serial.print(" A_15_flag : ");
    Serial.println(A_15_front);

    if(abs(ac.dir) < 30){
      A_15_front = 1;
    }

    if(A_15_flag == 0){
      M_flag = 0;
      AC_flag = 0;
    }
    else{
      M_flag = 2;
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
      M_flag = 2;
    }
    else{
      c = 0;
    }
  }



  if(A == 20){  //ロボットがちょっと押し出されちゃったから戻る
    if(A != B){
      B = A;
      Timer.reset();
    }
    go_ang = line.ang_old;
    M_flag = 2;

    if(500 < Timer.read_ms()){
      if(90 < abs(line.ang_old)){
        A = 15;
        A_15_flag = 7;
        c = 1;
      }
      else{
        // if(1000 < Timer.read_ms()){
        //   A = 15;
        //   A_15_flag = 8;
        //   c = 1;
        // }
      }
    }
  }



  /*---------------------------------------------------------出力ステート--------------------------------------------------------*/


  back_Flag = 0;
  ac.dir_target = target;
  push_flag = 0;

  if(30 < abs(ac.dir) && A != 13){
    AC_val = ac.getAC_val() * 1.5;
    if(line.LINE_on == 0){
      if(abs(line.ang_old) < 90){
        back_Flag = 0;
      }
      else{
        back_Flag = 1;
        M_flag = 0;
      }
    }
    else{
      if(ball.ball_get){
        if(abs(line.ang_old) < 90){
          back_Flag = 0;
        }
        push_flag = 1;
        M_flag = 1;
      }
    }
  }
  else if(AC_flag == 0){
    AC_val = ac.getAC_val();
  }
  else if(AC_flag == 1){
    AC_val = ac.getCam_val(-cam_front.ang);
  }


  kicker.run(kick_);
  // Serial.print(" A : ");
  // Serial.print(A);
  // Serial.println();
  // M_flag = 3;
  Serial.print(" max_val : ");
  Serial.println(max_val);


  if(M_flag == 1){
    MOTOR.moveMotor_L(go_ang,max_val,AC_val,line.vec);
  }
  else if(M_flag == 0){
    MOTOR.motor_ac(AC_val);
  }
  else if(M_flag == 2){
    if(AC_flag){
      MOTOR.moveMotor_0(go_ang,max_val,AC_val,1);
      // cam_front.print();
    }
    else{
      MOTOR.moveMotor_0(go_ang,max_val,AC_val,0);
    }
  }
  else if(M_flag == 3){
    MOTOR.motor_0();
  }
}