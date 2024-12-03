#include<ESP_communicate.h>


ESP_communicate ESP;


int ESP_communicate::get_message_to_ESP(const char* message,byte (&send_content)[4]){
  byte flag = 0;
  int send[2] = {0,0};
  if(strcmp(message,"START") == 0){
    flag = 1;
  }
  else if(strcmp(message,"BALL") == 0){
    flag = 2;
    send[0] = ball.vec.return_x();
    send[1] = ball.vec.return_y();
  }
  else if(strcmp(message,"LINE") == 0){
    flag = 3;
    if(line.LINE_on){
      send[0] = line.vec.return_x() * 100;
      send[1] = line.vec.return_y() * 100;
    }
    else{
      send[0] = 0;
      send[1] = 0;
    }
    // Serial.print(" LINE send ");
    // line.print();
    // line.vec.print();
  }
  else if(strcmp(message,"CAM_FRONT") == 0){
    flag = 4;
    if(cam_front.on){
      send[0] = cam_front.ang + 40;
      send[1] = cam_front.Size;
    }
    else{
      send[0] = 0;
      send[1] = 0;
    }
  }
  else if(strcmp(message,"AC_DIR") == 0){
    flag = 5;
    send[0] = ac.getnowdir();
  }
  else if(strcmp(message,"CHECK") == 0){
    flag = 6;
    send[0] = attack.send_val[0];
    send[1] = attack.send_val[1];
  }
  else if(strcmp(message,"NEOPIXEL_D") == 0){
    flag = 7;
    byte *defence_ = defence.get_flag();
    for(int i = 0; i < 4; i++){
      send_content[i] = defence_[i];
    }
  }
  else if(strcmp(message,"CAM_BACK") == 0){
    flag = 8;
    if(cam_back.on){
      send[0] = cam_back.ang + 40;
      send[1] = cam_back.Size;
    }
    else{
      send[0] = 0;
      send[1] = 0;
    }
  }
  else if(strcmp(message,"LINE_ALL") == 0){
    flag = 9;
    for(int i = 0; i < 4; i++){
      send_content[i] = line.data_byte[i];
    }
    // for(int i = 0; i < 4; i++){
    //   Serial.print(" ");
    //   Serial.print(send_num[i]);
    // }
    // Serial.println();
  }
  else if(strcmp(message,"BALL_CATCH") == 0){
    flag = 10;
    send[0] = ball.catch_val;
  }
  else if(strcmp(message,"NEOPIXEL_A") == 0){
    flag = 11;
    byte *attack_ = attack.getCheckval();
    for(int i = 0; i < 4; i++){
      send_content[i] = attack_[i];
    }
  }
  else if(strcmp(message,"CAM_FRONT_1") == 0){
    flag = 12;
    if(cam_front.color == BLUE){
      send_content[0] = cam_front.data_byte_b[1];
      send_content[1] = cam_front.data_byte_b[2];
      send_content[2] = cam_front.data_byte_b[3];
      send_content[3] = cam_front.data_byte_b[4];
    }
    else{
      send_content[0] = cam_front.data_byte_y[1];
      send_content[1] = cam_front.data_byte_y[2];
      send_content[2] = cam_front.data_byte_y[3];
      send_content[3] = cam_front.data_byte_y[4];
    }
    // Serial.print(" cam_block ");
    // for(int i = 0; i < 4; i++){
    //   Serial.print(send_num[i]);
    //   Serial.print(" ");
    // }
  }
  else if(strcmp(message,"CAM_BACK_1") == 0){
    flag = 13;
    if(cam_back.color == BLUE){
      send_content[0] = cam_back.data_byte_b[1];
      send_content[1] = cam_back.data_byte_b[2];
      send_content[2] = cam_back.data_byte_b[3];
      send_content[3] = cam_back.data_byte_b[4];
    }
    else{
      send_content[0] = cam_back.data_byte_y[1];
      send_content[1] = cam_back.data_byte_y[2];
      send_content[2] = cam_back.data_byte_y[3];
      send_content[3] = cam_back.data_byte_y[4];
    }
    // Serial.print(" cam_block_back ");
    // for(int i = 0; i < 4; i++){
    //   Serial.print(send_num[i]);
    //   Serial.print(" ");
    // }
  }

  if(flag != ESP_SEND_LINE_ALL && flag != ESP_SEND_NEOPIXEL_D && flag != ESP_SEND_NEOPIXEL_A && flag != ESP_SEND_CAM_FRONT_BLOCK && flag != ESP_SEND_CAM_BACK_BLOCK){
    send_content[0] = byte(send[0] >> 8);
    send_content[1] = byte(send[0] & 0xFF);
    send_content[2] = byte(send[1] >> 8);
    send_content[3] = byte(send[1] & 0xFF);
  }

  return flag;
}


void ESP_communicate::sendtoESP(const char* message){
  byte send_contents[4];
  uint8_t matter;

  matter = get_message_to_ESP(message,send_contents);

  uint8_t send_byte[7] = {38,matter,0,0,0,0,37};

  for(int i = 0; i < 4; i++){
    send_byte[i + 2] = send_contents[i];
  }

  Serial7.write(send_byte,7);
  // for(int i = 0; i < 7; i++){
  //   Serial.print(" ");
  //   Serial.print(send_byte[i]);
  // }
  // Serial.println();
}


void ESP_communicate::read_from_ESP(byte (&data)[2],int label){
  unsigned int contain[2];
  int data_int;
  contain[0] = uint16_t(data[0]) << 8;
  contain[1] = uint16_t(data[1]);
  data_int = contain[0] | contain[1];


  if(label == 1){
    central.Mode = data_int;
    Serial.print(" Mode : ");
    Serial.print(central.Mode);
    Serial.print(" | ");
  }
  else if(label == 2){
    if(data_int == 15){
      sendtoESP("AC_DIR");
      Serial8.write(38);
      Serial8.write(10);
      Serial8.write(1);
      Serial8.write(37);
    }
    else if(data_int == 30){
      Serial8.write(38);
      Serial8.write(10);
      Serial8.write(1);
      Serial8.write(37);
      if(line_send_count == 0){
        sendtoESP("LINE");
        line_send_count = 1;
      }
      else{
        sendtoESP("LINE_ALL");
        line_send_count = 0;
      }
    }
    else if(data_int == 50){
      sendtoESP("BALL");
    }
    else if(data_int == 80){
      if(goal_send_count == 0){
        sendtoESP("CAM_FRONT");
        sendtoESP("CAM_FRONT_1");
        // Serial.print(" !!!! cam_____________");
        goal_send_count = 1;
      }
      else{
        sendtoESP("CAM_BACK");
        sendtoESP("CAM_BACK_1");
        goal_send_count = 0;
      }
      // sendtoESP("CAM_BACK");
    }
    else if(data_int == 90){
      sendtoESP("BALL_CATCH");
    }
  }
  else if(label == 3){
    central.val_max = data_int;
  }
  else if(label == 4){
    central.color = data_int;
    if(central.color == YELLOW){
      cam_front.color = YELLOW;
      cam_back.color = BLUE;
    }
    else{
      cam_front.color = BLUE;
      cam_back.color = YELLOW;
    }
  }
  else if(label == 5){
    ball.get_th = data_int;
  }
  else if(label == 6){
    MOTOR.NoneM_flag = data_int;
  }
  else if(label == 7){
    ac.getnowdir();
    central.ac_tirget = ac.dir_n;
    ac.setup_2();
    // Serial.print(" !!!!!!!!!!! ");
    // ac.print();
  }
  else if(label == 8){
    ESP.Kick = 1;
  }
  else if(label == 9){
    int y = data[0] - 128;
    int x = data[1] - 128;
    PS4.theta = degrees(atan2(y,x));
    PS4.r = sqrt(x * x + y * y);
    
    // Serial.print(" PS4_r : ");
    // Serial.print(PS4_r);
    // Serial.print(" PS4_theta : ");
    // Serial.print(PS4_theta);
    // Serial.println();
  }
  else if(label == 10){
    Serial8.write(38);
    Serial8.write(0);
    Serial8.write(data_int);
    Serial8.write(37);
  }
  else if(label == 11){
    int y = data[0] - 128;
    int x = data[1] - 128;
    PS4.R_theta = degrees(atan2(y,x));
    PS4.R_r = sqrt(x * x + y * y);
    
    if(100 < sqrt(x * x + y * y)){
      PS4.Rotate_flag = 1;
    }
    else{
      PS4.Rotate_flag = 0;
    }
    // Serial.print(" PS4R_r : ");
    // Serial.print(PS4R_r);
    // Serial.print(" PS4R_theta : ");
    // Serial.print(PS4R_theta);
    // Serial.println();
  }
  else if(label == 13){
    sendtoESP("NEOPIXEL_D");
  }
  else if(label == 14){
    central.test_mode = data_int;
  }
  else if(label == 16){
    sendtoESP("NEOPIXEL_A");
  }
  else if(label == 17){
    attack.setplay_flag = data_int;
  }
  else if(label == 18){
    PS4.Circle = 1;
  }
  else if(label == 19){
    PS4.Square = 1;
  }
  else{
    for(int i = 0; i < 6; i++){
      if(100 + i == label){
        central.Values[i] = data_int;
        // Serial.print(" num : ");
        // Serial.print(i);
        // Serial.print(" recieve : ");
        // Serial.print(data_int);
      }
    }
  }
}