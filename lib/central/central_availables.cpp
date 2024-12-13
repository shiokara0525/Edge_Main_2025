#include<central_availables.h>

central_availables central;

void central_availables::set_states(Vector2D go_vec_,int motor_value_,int motor_on_,int AC_value_,int AC_flag_,int Kick_on_){
    go_vector = go_vec_.normalize();
    Motor_value = motor_value_;
    Motor_on = motor_on_;
    AC_value = AC_value_;
    AC_flag = AC_flag_;
    Kick_on = Kick_on_;
}

void central_availables::set_Motor_max(int Motor_max_){
    Motor_max_value = Motor_max_;
}


int central_availables::return_Motor_max(){
    return Motor_max_value;
}


Vector2D central_availables::return_go_vector(){
    return go_vector;
}


int central_availables::return_AC_value(){
    return AC_value;
}

int central_availables::return_AC_flag(){
    return AC_flag;
}

int central_availables::return_Motor_on(){
    return Motor_on;
}

int central_availables::return_Motor_value(){
    return Motor_value;
}

int central_availables::return_Kick_on(){
    return Kick_on;
}