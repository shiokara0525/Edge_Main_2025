#include"Sup.h"
#include"input.h"
#include"central_availables.h"
#include"ESP_communicate.h"

void mytest(){
    Vector2D go_vec;
    if(central.test_mode == 0){
        go_vec.set_polar(ball.ang,1.0);
        float AC_val = ac.getAC_val();
        central.set_states(go_vec,200,MOTOR_ON,AC_val,AC_ALL,0);
    }
    else if(central.test_mode == 1){
      // int TIME = MOTOR_time.read_ms();
      // Serial.print(" MOTOR: ");
      // Serial.print(TIME);
      // Serial.println();
      // if(TIME < 200){
      //   central.set_states_no_output();
      // }
      // else if(TIME < 1200){
      //   central.set_states_MOTOR_test(MOTOR_test_num);
      // }
      // else{
      //   MOTOR_time.reset();
      //   MOTOR_test_num++;
      //   if(4 <= MOTOR_test_num){
      //     MOTOR_test_num = 0;
      //   }
      //   central.set_states_no_output();
      // }
        go_vec.set_polar(180,1.0);
        central.set_states(go_vec,200,MOTOR_ON,0,AC_ALL,0);
    }
    else if(central.test_mode == 3){
        float AC_val = ac.getAC_val();
        central.set_states_onlyAC(AC_val);
    }
    else if(central.test_mode == 4){
        MOTOR.motor_0();
        kicker.TEST_();
        central.set_states_no_output();
    }
    else if(central.test_mode == 5){
        ESP.PS4.run();
    }
}