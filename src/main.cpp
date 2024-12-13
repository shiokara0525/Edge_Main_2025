#include <Arduino.h>

#include<timer.h>              //タイマーのクラス定義
#include"input.h"              //センサー類(ボール、ライン、カメラ）のクラス定義
#include"output.h"             //出力(モーター、キッカー）のクラス定義
#include"process.h"            //処理系統（アタッカーディフェンスそれぞれ）のクラス定義
#include"Sup.h"                //その他便利クラスの定義
#include"ESP_communicate.h"    //ESPとの通信関連のクラス定義
#include"central_availables.h" //ロボットの状態の一番下の部分に使う変数のクラス定義

timer MOTOR_time;




void setup(){
  Serial.begin(9600);
  Serial7.begin(115200);

  ball.begin();
  line.begin();
  kicker.setup();
  cam_front.begin();
  cam_back.begin();
  ac.setup();
  central.Mode = 0;

  ESP.sendtoESP("START");
}



void loop(){
  Vector2D go_vec(0,0);
  int set_status_flag = 0;

  central.Main_timer.reset();
  if(2500 < central.Line_period.read_us()){
    line.getLINE_Vec();
    central.Line_period.reset();
  }
  if(2500 < central.Ball_period.read_us()){
    ball.getBallposition();
    // Serial.print(" Ball ");
    // Serial.print(Ball_period.read_ms());
    // Serial.println();
    central.Ball_period.reset();
  }
  cam_front.getCamdata();
  cam_back.getCamdata();


  if(central.Mode == 0){
    if(central.Mode != central.Mode_old){
      central.Mode_old = central.Mode;
      MOTOR.motor_0();
      central.Mode_timer.reset();
      kicker.stop();
      Serial8.write(38);
      Serial8.write(10);
      Serial8.write(0);
      Serial8.write(37);
    }

    central.set_states(go_vec,0,MOTOR_STOP,0,AC_ALL,ESP.Kick);
    ESP.Kick = 0;
    set_status_flag = 1;
  }

  else if(central.Mode == 1){
    if(central.Mode != central.Mode_old){
      central.Mode_old = central.Mode;
      attack.available_set(central.Values);
      central.Mode_timer.reset();
      kicker.stop();
    }

    attack.attack();
    set_status_flag = 1;
  }

  else if(central.Mode == 2){
    if(central.Mode != central.Mode_old){
      central.Mode_old = central.Mode;
      defence.available_set();
      central.Mode_timer.reset();
      kicker.stop();
    }

    go_vec = defence.defence();
    set_status_flag = 1;
  }

  else if(central.Mode == 3){
    if(central.Mode != central.Mode_old){
      central.Mode_old = central.Mode;
      kicker.stop();
      ac.setup_2();
      Serial8.write(38);
      Serial8.write(10);
      Serial8.write(1);
      Serial8.write(37);
      MOTOR_time.reset();
    }

    if(central.test_mode == 0){
      go_vec.set_polar(ball.ang,1.0);
      float AC_val = ac.getAC_val();
      central.set_states(go_vec,200,MOTOR_ON,AC_val,AC_ALL,0);
      set_status_flag = 1;
    }
    else if(central.test_mode == 1){
      // int TIME = MOTOR_time.read_ms();
      Serial.print(" MOTOR ");
      // Serial.print(TIME);
      Serial.println();

      // for(int i = 0; i < 4; i++){
      //   if(TIME < 100){
      //     central.set_states_no_output();
      //     break;
      //   }
      //   else if(TIME < 500){
      //     central.set_states_MOTOR_test(i);
      //     break;
      //   }
      //   else{
      //     TIME -= 500;
      //   }
      // }
    }
    else if(central.test_mode == 3){
      float AC_val = ac.getAC_val();
      central.set_states_onlyAC(AC_val);
      set_status_flag = 1;
    }
    else if(central.test_mode == 4){
      MOTOR.motor_0();
      kicker.TEST_();
      central.set_states_no_output();
    }
    else if(central.test_mode == 5){
      ESP.PS4.run();
      set_status_flag = 1;
    }
  }

  else if(central.Mode == 10){
    if(central.Mode != central.Mode_old){
      central.Mode_old = central.Mode;
      kicker.stop();
      MOTOR.motor_0();
    }

    central.set_states_no_output();
    set_status_flag = 1;
  }

  else if(central.Mode == 99){
    if(central.Mode != central.Mode_old){
      central.Mode_old = central.Mode;
      kicker.stop();

    }
    central.set_states_no_output();
    set_status_flag = 1;
  }

  if(set_status_flag == 1){
    if(central.return_Motor_on() == MOTOR_STOP){
      MOTOR.motor_0();
    }
    else if(central.return_Motor_on() == MOTOR_AC_ONLY){
      MOTOR.motor_ac(central.return_AC_value());
    }
    else if(central.return_Motor_on() == MOTOR_ON){
      MOTOR.moveMotor(central.return_go_vector(),central.return_Motor_value(),central.return_AC_value(),central.return_AC_flag());
    }
    else{
      for(int i = 0; i < 4; i++){
        if(central.return_Motor_on() == 3 + i){
          MOTOR.Moutput(central.return_Motor_max(),i);
        }
      }
    }

    kicker.run(central.return_Kick_on());
  }
  else{
    MOTOR.motor_0();
    kicker.stop();
  }
}




void serialEvent7(){  
  uint8_t data[5];

  if(Serial7.available() < 5){
    return;
  }

  while(5 <= Serial7.available()){
    data[0] = Serial7.read();
    if(data[0] == 38){
      break;
    }
  }
  if(data[0] != 38){
    return;
  }
  for(int i = 1; i < 5; i++){
    data[i] = Serial7.read();
    // Serial.print(data[i]);
    // Serial.print(" ");
  }
  // Serial.println();

  byte data_content[2] = {data[2],data[3]};

  ESP.read_from_ESP(data_content,data[1]);
}



void serialEvent3(){
  uint8_t reBuf[8];
  if(Serial3.available() < 8){
    return;
  }
  reBuf[0] = Serial3.read();

  if(reBuf[0] != 255){
    return;
  }

  for(int i = 1; i < 8; i++){
    reBuf[i] = Serial3.read();
  }

  if(reBuf[0] == 255 && reBuf[7] == 254){
    if(reBuf[1] == BLUE){
      for(int i = 0; i < 5; i++){
        cam_back.data_byte_b[i] = reBuf[i+2];
      }
    }
    else{
      for(int i = 0; i < 5; i++){
        cam_back.data_byte_y[i] = reBuf[i+2];
      }
    }

    // Serial.print("sawa1");
  }

  // Serial.print(" cam_3 ");

  // for(int i = 0; i < 8; i++){
  //   Serial.print(" ");
  //   Serial.print(reBuf[i]);
  // }
  // Serial.println();
}



void serialEvent4(){
  uint8_t reBuf[8];
  if(Serial4.available() < 8){
    return;
  }
  reBuf[0] = Serial4.read();

  if(reBuf[0] != 255){
    return;
  }

  for(int i = 1; i < 8; i++){
    reBuf[i] = Serial4.read();
  }

  if(reBuf[0] == 255 && reBuf[7] == 254){
    if(reBuf[1] == BLUE){
      for(int i = 0; i < 5; i++){
        cam_front.data_byte_b[i] = reBuf[i+2];
        // Serial.print(" ");
        // Serial.print(cam_front.data_byte_b[i]);
      }
    }
    else{
      for(int i = 0; i < 5; i++){
        cam_front.data_byte_y[i] = reBuf[i+2];
        // Serial.print(" ");
        // Serial.print(cam_front.data_byte_y[i]);
      }
    }

    // Serial.print("sawa1");
  }

  // Serial.print(" cam_4 ");
  // for(int i = 0; i < 8; i++){
  //   Serial.print(" ");
  //   Serial.print(reBuf[i]);
  // }
  // Serial.println();
}



void serialEvent8(){
  // Serial.println(" sawa3 ");
  uint8_t read[6];
  if(Serial8.available() < 6){
    return;
  }
  read[0] = Serial8.read();
  if(read[0] != 38){
    return;
  } 

  for(int i = 1; i < 6; i++){
    read[i] = Serial8.read();
  }

  if(read[0] == 38 && read[5] == 37){
    for(int i = 0; i < 4; i++){
      line.data_byte[i] = read[i+1];
    }
  }

  // for(int i = 0; i < 6; i++){
  //   Serial.print(read[i]);
  //   Serial.print(" ");
  // }
  // Serial.println();
}



void serialEvent6(){
  uint8_t read[12];
  int flag_break = 0;
  while(12 <= Serial6.available()){
    read[0] = Serial6.read();
    if(read[0] == 0xFF){
      flag_break = 1;
      break;
    }
  }

  if(flag_break == 0){
    return;
  }


  for(int i = 1; i < 12; i++){
    read[i] = Serial6.read();
  }

  if(read[0] == 0xFF && read[11] == 0xAA){
    ball.get_data(read);
    // Serial.println(ball_Get.read_us());
  }
  // ball_Get.reset();

  // for(int i = 0; i < 12; i++){
  //   Serial.print(" ");
  //   Serial.print(read[i]);
  // }
  // Serial.println();
}