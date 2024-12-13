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

void central_availables::set_states_no_output(){
    go_vector = Vector2D(0,0);
    Motor_value = 0;
    Motor_on = MOTOR_STOP;
    AC_value = 0;
    AC_flag = AC_ALL;
    Kick_on = KICK_OFF;
}


void central_availables::set_states_onlyAC(int AC_val){
    go_vector = Vector2D(0,0);
    Motor_value = AC_val;
    Motor_on = MOTOR_AC_ONLY;
    AC_value = AC_val;
    AC_flag = AC_ALL;
    Kick_on = KICK_OFF;
}


void central_availables::set_states_MOTOR_test(int motor_on){
    go_vector = Vector2D(0,0);
    Motor_value = 200;

    if(motor_on == 0){
        Motor_on = MOTOR_TEST_0;
    }
    if(motor_on == 1){
        Motor_on = MOTOR_TEST_1;
    }
    if(motor_on == 2){
        Motor_on = MOTOR_TEST_2;
    }
    if(motor_on == 3){
        Motor_on = MOTOR_TEST_3;
    }
    
    AC_value = 0;
    AC_flag = AC_ALL;
    Kick_on = KICK_OFF;
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