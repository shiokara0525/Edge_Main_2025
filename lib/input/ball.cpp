#include<ball.h>

BALL::BALL(){
    ball_catch_MA.setLenth(20);
    for(int i = 0; i < 4; i++){
        ball_down[i].setLenth(10);
    }
    x_MA.setLenth(5);
    y_MA.setLenth(5);
    B_dif_.setLenth(20);
    B_v.setLenth(10);
    Bang_dif_.setLenth(40);
    Far.setLenth(10);
    pinMode(CATCH_PIN,INPUT);
}


void BALL::begin(){
    Serial8.begin(115200);
}


int BALL::getBallposition(){
    unsigned int contain[4];

    contain[0] = (uint16_t(data_byte[0]) << 8);
    contain[1] = (uint16_t(data_byte[1]));
    float x = x_MA.demandAve(int16_t(contain[0] | contain[1]));

    contain[2] = (uint16_t(data_byte[2]) << 8);
    contain[3] = (uint16_t(data_byte[3]));
    float y = y_MA.demandAve(int16_t(contain[2] | contain[3]));


    catch_val = ball_catch_MA.demandAve(analogRead(CATCH_PIN));

    float x_down = 0;
    float y_down = 0;
    for(int i = 0; i < 4; i++){
        down_val[i] = ball_down[i].demandAve(data_byte[i+6]);
        x_down += cos(radians(45 + 90 * i)) * down_val[i];
        y_down += sin(radians(45 + 90 * i)) * down_val[i];
        // Serial.print(45 + 90 * i);
        // Serial.print(" ");
    }
    vec_down.set_coodinate(x_down,y_down);
    // Serial.print(" x : ");
    // Serial.print(x_down);
    // Serial.print(" y : ");
    // Serial.print(y_down);

    if(x == 0 && y == 0){
        flag = 0;
        return flag;
    }
    else{
        flag = 1;
    }

    if(catch_val < get_th){
        ball_get = 1;
    }
    else{
        ball_get = 0;
    }
    is_get.enterState(ball_get);
    vec.set_coodinate(x,y);
    ang = vec.return_azimuth();
    far = Far.demandAve(vec.return_magnitude());
    Bang_dif = Bang_dif_.sum(abs(ang) - abs(ang_old));
    ang_old = ang;

    world_far = 220 - far;
    world_vec.set_polar(ang,world_far);
    vec_velocity = B_dif_.sum(world_vec - world_vec_old);
    vec_acc = B_v.sum(vec_velocity - vec_velocity_old);
    world_vec_old = world_vec;
    vec_velocity_old = vec_velocity;
    return flag;
}




void BALL::print(){
    Serial.print(" ang : ");
    Serial.print(ang);
    Serial.print(" far : ");
    Serial.print(world_far);
    // Serial.print(" x : ");
    // Serial.print(x_pos);
    // Serial.print(" y : ");
    // Serial.print(y_pos);
    // Serial.print(" dx : ");
    // Serial.print(dx);
    // Serial.print(" get_val : ");
    // Serial.print(get_val);
    // Serial.print(" get : ");
    // Serial.print(ball_get);
    // Serial.print(" get_1 : ");
    // Serial.print(get_1);
    // Serial.print(" get_2 : ");
    // Serial.print(get_2);
    // for(int i = 0; i < 4; i++){
    //     Serial.print(" ");
    //     Serial.print(i);
    //     Serial.print(" : ");
    //     Serial.print(down_val[i]);
    // }
    vec_down.print();
    Serial.print(" flag : ");
    Serial.print(flag);
}