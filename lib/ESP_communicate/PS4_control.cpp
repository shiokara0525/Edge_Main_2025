#include<PS4_control.h>

PS4_control::PS4_control(){

}


void PS4_control::run(){
  float AC_val = ac.getAC_val();
  int kick_ = 0;
  angle go_ang(0,true);
  Vector2D go_vec(0,0);

  if(200 < ball.is_get.readStateTimer(1)){
    kick_ = 1;
  }

  if(10 < R_r){
    ac.dir_target = ac.first + R_theta;
    AC_val = ac.getAC_val() * 2;
    if(80 < AC_val){
      AC_val = 80;
    }
    else if(AC_val < -80){
      AC_val = -80;
    }
    MOTOR.motor_ac(AC_val);

    central.set_states(go_vec,AC_val,MOTOR_AC_ONLY,AC_val,AC_ALL,kick_);
  }
  else if(10 < r){
    go_ang = theta;
    if(abs(theta - ball.ang) < 45){
      go_ang = ball.ang;
    }

    if(line.LINE_on){
      go_ang = degrees(line.vec_go.return_arg());
    }
    go_vec.set_polar(go_ang.degree,1.0);

    central.set_states(go_vec,r * 1.5,MOTOR_ON,AC_val,AC_ALL,kick_);
  }
  else{
    central.set_states(go_vec,AC_val,MOTOR_AC_ONLY,AC_val,AC_ALL,kick_);
  }
}
