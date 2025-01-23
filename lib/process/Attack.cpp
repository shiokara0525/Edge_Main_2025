#include<Attack.h>


void Attack::available_set(int *check_val){ //変数を受け取ったり三次関数を求める関数
  RA_a = central.Values[0] / 100.0;
  RA_b = central.Values[1] / 100.0;
  RA_c = central.Values[2];
  RA_d = central.Values[3];
  AC_cam_coef = central.Values[4] / 100.0;
  A = 0;
  c = 0;
  // float ang[4] = {0,20,30,45};
  // float m1 = ang_0 / ((ang[0] - ang[1]) * (ang[0] - ang[2]) * (ang[0] - ang[3]));
  // float m2 = ang_20 / ((ang[1] - ang[0]) * (ang[1] - ang[2]) * (ang[1] - ang[3]));
  // float m3 = ang_30 / ((ang[2] - ang[0]) * (ang[2] - ang[1]) * (ang[2] - ang[3]));
  // float m4 = ang_45 / ((ang[3] - ang[0]) * (ang[3] - ang[1]) * (ang[3] - ang[2]));
  // RA_a = m1 + m2 + m3 + m4;
  // RA_b = -(m1 * (ang[1] + ang[2] + ang[3]) + m2 * (ang[0] + ang[2] + ang[3]) + m3 * (ang[0] + ang[1] + ang[3]) + m4 * (ang[0] + ang[1] + ang[2]));
  // RA_c = m1 * (ang[1] * ang[2] + ang[1] * ang[3] + ang[2] * ang[3]) + m2 * (ang[0] * ang[2] + ang[0] * ang[3] + ang[2] * ang[3]) + m3 * (ang[0] * ang[1] + ang[0] * ang[3] + ang[1] * ang[3]) + m4 * (ang[0] * ang[1] + ang[0] * ang[2] + ang[1] * ang[2]);
  // RA_d = -(m1 * ang[1] * ang[2] * ang[3] + m2 * ang[0] * ang[2] * ang[3] + m3 * ang[0] * ang[1] * ang[3] + m4 * ang[0] * ang[1] * ang[2]);
  Serial.println();
  Serial.print(" RA_a : ");
  Serial.print(RA_a);
  Serial.print(" RA_b : ");
  Serial.print(RA_b);
  Serial.print(" RA_c : ");
  Serial.print(RA_c);
  Serial.print(" RA_d : ");
  Serial.print(RA_d);
  for(int i = 0; i < 6; i++){
    Serial.print(" ");
    Serial.print(i);
    Serial.print(" ");
    Serial.print(central.Values[i]);
  }
  Serial.println();
  go_val = central.return_Motor_max();
  play_time.reset();
  first_ang = ac.dir_n;
  goang_ma.setLenth(100);
  
  A = 10;
}


byte* Attack::getCheckval(){
  return_byte[0] = A;
  return_byte[1] = cam_front.ang + 60;
  if(!cam_front.on){
    return_byte[1] = 240;
  }
  return_byte[2] = cam_back.ang + 60;
  if(!cam_back.on){
    return_byte[2] = 240;
  }
  return_byte[3] = ball.world_far;  //変えるといいよ
  return return_byte;
}



void Attack::attack(){
  angle go_ang(ball.ang,true);         //進む角度のオブジェクト

  float AC_val = 100;                  //姿勢制御の出力
  int max_val = go_val;                //進む出力
  Serial.print(" go_val : ");
  Serial.print(go_val);
  float target = central.ac_tirget;           //目標角度


  int AC_flag = 0;                     //0だったら絶対的な角度とる 1だったらゴール向く
  int kick_ = 0;                       //0だったらキックしない 1だったらキック
  int M_flag = 1;                      //1だったら動き続ける 0だったら止まる
  back_flag = 0;
  rake_flag = 0;



  if(!ball.flag){
    A = 5;  //ボールなかったら止まる
  }


  //----------------------------------------------------------動きの決定----------------------------------------------------------//


  if(A == 5){  //ボールがない時止まる
    if(A != B){
      B = A;
      Timer.reset();
    }

    if(Timer.read_ms() < 500){
      go_ang = 180;
      max_val = 200;
    }
    else{
      M_flag = 0;
    }

    if(ball.flag){
      if(line.LINE_on){
        A = 20;
      }
      else{
        A = 10;
      }
    }
  }



  if(A == 10){  //回り込むやつ
    if(A != B){
      go_flag = 0;
      if(B == 20 || B == 21){
        if(line.ang_old < 0){
          if(-45 < ball.ang && ball.ang < 30){
            go_flag = 1;  //ライン際でボールにあてに行く
          }
        }
        else{
          if(-30 < ball.ang && ball.ang < 45){
            go_flag = 1;  //ライン際でボールにあてに行く
          }
        }
      }
      B = A;
      Timer.reset();
    }

    if(90 < abs(ball.ang)){
      go_flag = 0;
    }


    // float confidencial_num = (ball.vec.return_magnitude() - BALL_MAX_NUM * 0.8) * 0.025;
    int front_flag = 0;

    if(abs(ball.ang) < 20){
      go_ang = abs(ball.ang);
    }
    else if(abs(ball.ang) < 25){  //(20,20),(25,50)
      go_ang = (abs(ball.ang) - 16.66) * 6.0;
    }
    else if(abs(ball.ang) < 80){
      go_ang = abs(ball.ang) * 2.0;
      max_val = 220;
    }
    else if(abs(ball.ang) < 90){  //(60,120) (90,160)
      go_ang = 160;
    }
    else{
      go_ang = abs(ball.ang) + 70;
    }

    if(-10 < ball.ang && ball.ang < 0){
      go_ang = abs(ball.ang) * 1.2;
    }
    else if(-10 < ball.ang && ball.ang < -50){
      go_ang = abs(ball.ang) * 2.7;
    }

    if(90 < ball.world_far){
      if(90 <= abs(ball.ang)){
        go_ang = abs(ball.ang) + 50;
      }
    }

    if(30 < cam_front.Size && (abs(ball.ang) < 15 || (abs(ball.ang) < 30 && abs(cam_front.ang - ball.ang) < 10))){
      if(ball_front.readStateTimer(1) < 100){
        max_val = 200;
      }
      AC_flag = 1;
      front_flag = 1;
    }

    int goang_deff = abs(goang_ma.sum(go_ang.degree - ang_old));

    if(goang_deff < 20 && 10 < abs(ball.ang) && abs(ball.ang) < 90){
      go_ang.degree += 20;
    }


    Serial.print(" ball_ang : ");
    Serial.print(ball.ang);
    Serial.print(" ang : ");
    Serial.print(go_ang.degree);
    Serial.print(" deff : ");
    Serial.println(goang_deff);

    ball_front.enterState(front_flag);

    go_ang = go_ang.degree * (ball.ang < 0 ? -1 : 1);  //角度の正負を元に戻す

    // Serial.println();
    if(go_flag == 1 && Timer.read_ms() < 300){
      go_ang = ball.ang;
    }
    ang_old = go_ang.degree;

    if(line.LINE_on){
      A = 20;
    }
    else if(line.side_flag){
      A = 21;
    }
    else if(ball.ball_get){
      A = 11;
    }
  }



  if(A == 11){  //ボール持ってるとき前進するやつ
    if(A != B){
      B = A;
      Timer.reset();
      CFO.reset();
      c = 1;
      Catch.reset();
    }
    cam_front_on = 0;
    cam_front.print();
    Serial.println();

    if(cam_front.on == 1){  //カメラ見てるとき
      if(cam_front.on == 1 && (abs(cam_front.ang) < 20 || cam_front.center)){  //正面にゴールあってゴールもある程度近くにある時
        cam_front_on = 1;  //打っていいよ
        go_ang = 0;
        AC_flag = 1;
        // dribbler_flag = 0;
      }
      else{
        go_ang = 0;
        AC_flag = 1;
      }
    }
    else{
      go_ang = 0;
      // kick_ = 1;
      if(400 < Timer.read_ms()){
        kick_ = 1;
      }
    }

    CFO.enterState(cam_front_on);
    if(cam_front_on == 1){  //打っていいよフラグ

      if(150 < CFO.readStateTimer()){
        kick_ = 1;  //打っていいよフラグが0.2秒立ってたら打つ
      }
      if(50 < cam_front.Size){
        kick_ = 1;  //ゴールが近い時は問答無用で打つ
      }
    }


    if(1500 < Timer.read_ms()){
      rake_flag = 1;
      target += 90;
      AC_flag = 0;
    }
    else if(2000 < Timer.read_ms()){
      rake_flag = 0;
    }

    // if(setplay_flag && 100 < Timer.read_ms()){
    //   kick_ = 1;
    // }
    // cam_front.print();
    // Serial.print(" kick_ : ");
    // Serial.print(kick_);
    // Serial.println();
    if(line.LINE_on){
      A = 20;
    }
    else if(line.side_flag){
      A = 21;
    }
    else if(!ball.ball_get){
      A = 10;
    }
  }




  if(A == 20){  //ラインから逃げるやつ
    angle line_ang(line.ang,true);
    if(A != B){
      B = A;
      Timer.reset();
      line_flag = line.switchLineflag(line.vec.return_azimuth());
      go_ang = line.vec_go.return_azimuth();
    }
    back_flag = 1;
    // target = Line_target_dir


    if(line.LINE_on){
      if(Timer.read_ms() < 30){
        go_ang = line.go_ang_first;
      }
      else{
        go_ang = line.vec_go.return_azimuth();
      }
    }
    else{
      if(Timer.read_ms() < 200){
        go_ang = line.go_ang_old;
      }
    }


    if((line.LINE_change == -1 || line.LINE_on == 0) && 200 < Timer.read_ms()){  //踏んでない状態から踏んでる状態になった時
      A = 10;
      if(150 < abs(degrees(line.vec_first.return_azimuth()))){  //後ろにラインがあったら
        if(30 < abs(ball.ang) && abs(ball.ang) <= 85){
          // A = 25;  //前に行く
        }
        else if(85 < abs(ball.ang) && abs(ball.ang) < 120){
          // A = 26;  //横に行く
        }
      }
      else if(45 < (degrees(line.vec_first.return_azimuth())) && (degrees(line.vec_first.return_azimuth())) < 135){
        if(0 <= cam_back.ang){
          // A = 25;
        }
      }
      else if(-135 < (degrees(line.vec_first.return_azimuth())) && (degrees(line.vec_first.return_azimuth())) < -45){
        if(cam_back.ang <= 0){
          // A = 25;
        }
      }
      else if(abs(degrees(line.vec_first.return_azimuth())) < 45){  //前にラインがあったら
        if(cam_front.center){  //ゴール前だったら
          back_count++;
          if(back_count % 4 == 0 && abs(ball.ang) < 45 && !ball.ball_get){
            A = 22;  //ボールを押し込むやつ
            Serial.println(" line_front "); 
          }
        }
        else{  //notゴール前だったら
          back_count++;
          if(back_count % 4 == 0){
            A = 24;  //後ろに下がるやつ
          }
        }
      }
    }
  }



  if(A == 21){  //サイド読んでるとき逃げるやつ
    if(A != B){
      B = A;
      Timer.reset();
    }
    back_flag = 1;
    if(line.side_flag == 1){
      go_ang = -90;
    }
    else if(line.side_flag == 2){
      go_ang = 90;
    }
    else if(line.side_flag == 3){
      go_ang = 180;
    }
    else if(line.side_flag == 4){
      go_ang = 0;
    }


    if(100 < Timer.read_ms()){
      if(!line.side_flag && !line.LINE_on){
        if(ball.ball_get){
          A = 11;
        }
        else{
          A = 10;
        }
      }
    }
    else{
      if(line.LINE_on){
        A = 20;
      }
    }
  }



  if(A == 22){  //ボール押し込む
    if(A != B){
      B = A;
      Timer.reset();
    }
    max_val = 150;
    go_ang = 0;
  
    if((line.LINE_on == 1 && (line.dis_X < -0.75 || (30 <= abs(line.ang) && abs(line.ang) <= 150))) || 5000 < Timer.read_ms() || 60 < abs(ball.ang)){
      A = 23;
    }
    if(400 < Timer.read_ms()){
      A = 23;
    }
  }



  if(A == 23){  //ボール押し込んだ後下がる
    if(A != B){
      B = A;
    }
    go_ang = 180;

    if(!line.LINE_on){
      A = 10;  //戻ってるけどラインを踏んでる限りこのステートから出ない
    }
  }



  if(A == 24){  //後ろに下がるやつ(LOP誘発)
    if(A != B){
      B = A;
      Timer.reset();
    }
    if(Timer.read_ms() < 500){
      go_ang = 180;
      // max_val = 180;
    }
    else{
      M_flag = 0;
      if(ball.ball_get){
        A = 11;
      }
      else if(abs(ball.ang) < 10){
        A = 10;
      }
    }

    if(line.LINE_on){
      A = 20;
    }
    else if(ball.ball_get){
      A = 11;
    }
    else if(abs(ball.ang) < 30){
      A = 10;
    }
    else if(3000 < Timer.read_ms()){
      A = 10;
    }
  }



  if(A == 25){  //後ろのライン読んだとき前に進むやつ
    if(A != B){
      B = A;
      Timer.reset();
    }
    go_ang = 0;

    if(line.LINE_on == 1){
      A = 20;
    }
    else if(abs(ball.ang) <= 30 || 100 <= abs(ball.ang)){
      A = 26;
    }
  }



  if(A == 26){  //後ろのライン読んだとき横に進むやつ
    if(A != B){
      B = A;
    }
    if(ball.ang < 0){
      go_ang = -90;
    }
    else{
      go_ang = 90;
    }


    if(line.LINE_on){
      A = 20;
    }
    else if(line.side_flag){
      A = 21;
    }
    else if(abs(ball.ang) < 30 || 100 < abs(ball.ang) || 5000 < Timer.read_ms()){
      A = 10;
    }
  }




  //----------------------------------------------------------出力(ここで行ってるのはフラグの回収のみ)----------------------------------------------------------//


  if(setplay_flag == 1){
    target += 45;
    if(1000 < play_time.read_ms()){
      setplay_flag = 0;
    }
  }
  else if(setplay_flag == 2){
    target -= 45;
    if(1000 < play_time.read_ms()){
      setplay_flag = 0;
    }
  }
  else if(setplay_flag == 3){
    target = first_ang;
    Serial.print(" !!!! ");
    if(1000 < play_time.read_ms()){
      setplay_flag = 0;
    }
  }


  ac.dir_target = target;
  if(AC_flag == 0 || rake_flag){
    AC_val = ac.getAC_val();
  }
  else if(AC_flag == 1){
    AC_val = ac.getCam_val(-cam_front.ang) * AC_cam_coef;
  }

  if(back_flag){
    max_val = go_val_back;
  }

  Serial.print(" A : ");
  Serial.print(A);
  Serial.println();

  Vector2D go_vec;
  go_vec.set_polar(go_ang.degree,max_val);


  central.set_states(go_vec,max_val,M_flag,AC_val,AC_flag,kick_);
}