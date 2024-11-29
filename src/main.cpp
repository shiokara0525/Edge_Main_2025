#include <Arduino.h>

#include<timer.h>
#include"input.h"
#include"output.h"
#include"process.h"
#include"Sup.h"
#include"ESP_communicate.h"


timer Ball_period;
timer Line_period;

timer Main_timer;
timer Mode_timer;
timer ball_Get;
timer line_Get;
timer ESP_send;



int goal_send_count = 0;
int line_send_count = 0;

int Kick = 0;

int testMode = 0;



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
  Main_timer.reset();
  if(2500 < Line_period.read_us()){
    line.getLINE_Vec();
    Line_period.reset();
  }
  if(2500 < Ball_period.read_us()){
    ball.getBallposition();
    // Serial.print(" Ball ");
    // Serial.print(Ball_period.read_ms());
    // Serial.println();
    Ball_period.reset();
  }
  cam_front.getCamdata();
  cam_back.getCamdata();


  if(central.Mode == 0){
    if(central.Mode != central.Mode_old){
      central.Mode_old = central.Mode;
      MOTOR.motor_0();
      Mode_timer.reset();
      kicker.stop();
      Serial8.write(38);
      Serial8.write(10);
      Serial8.write(0);
      Serial8.write(37);
    }

    kicker.run(Kick);
    Kick = 0;
  }

  else if(central.Mode == 1){
    if(central.Mode != central.Mode_old){
      central.Mode_old = central.Mode;
      attack.available_set(central.Values);
      Mode_timer.reset();
      kicker.stop();
    }

    attack.attack();
  }

  else if(central.Mode == 2){
    if(central.Mode != central.Mode_old){
      central.Mode_old = central.Mode;
      defence.available_set();
      Mode_timer.reset();
      kicker.stop();
    }

    defence.defence();
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
    }

    if(testMode == 0){
      angle ang(0,true);
      float AC_val = ac.getAC_val();
      MOTOR.moveMotor_0(ang,central.val_max,AC_val,0);
    }
    else if(testMode == 1){
      for(int i = 0; i < 4; i++){
        MOTOR.Moutput(i,255);
        delay(500);
        MOTOR.Moutput(i,0);
        delay(100);
      }
    }
    else if(testMode == 3){
      float AC_val = ac.getAC_val();
      MOTOR.motor_ac(AC_val);
    }
    else if(testMode == 4){
      MOTOR.motor_0();
      kicker.TEST_();
    }
    else if(testMode == 5){
      ESP.PS4.run();
    }
  }

  else if(central.Mode == 10){
    if(central.Mode != central.Mode_old){
      central.Mode_old = central.Mode;
      kicker.stop();
      MOTOR.motor_0();
    }
  }

  else if(central.Mode == 99){
    if(central.Mode != central.Mode_old){
      central.Mode_old = central.Mode;
      kicker.stop();

    }
    MOTOR.motor_0();
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

  ESP.read_from_ESP(data_content);
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