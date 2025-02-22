#include<Cam.h>


Cam::Cam(int flag){
    ang_.setLenth(20);
    ang_.reset();
    if(flag == 3){
        F = 3;
    }
    else if(flag == 4){
        F = 4;
    }
}


void Cam::begin(){
    if(F == 3){
        Serial3.begin(115200);
    }
    else if(F == 4){
        Serial4.begin(115200);
    }
}



int Cam::getCamdata(){
    blue_x1 = data_byte_b[1];
    blue_y1 = data_byte_b[2];
    blue_x2 = data_byte_b[1] + data_byte_b[3];
    blue_y2 = data_byte_b[2] + data_byte_b[4];
    blue_h = data_byte_b[4];
    Yellow_x1 = data_byte_y[1];
    Yellow_y1 = data_byte_y[2];
    Yellow_x2 = data_byte_y[1] + data_byte_y[3];
    Yellow_y2 = data_byte_y[2] + data_byte_y[4];
    yellow_h = data_byte_y[4];
    if(color == BLUE){
        if(blue_h != 0){
            ang = ang_.demandAve((data_byte_b[0] - 80)*3/4);
            Size = blue_h;
            if(blue_x1 < 80 && 80 < blue_x2){
                center = 1;
            }
            else{
                center = 0;
            }
            on = 1;
            return 1;
        }
    }
    else if(color == YELLOW){
        if(yellow_h != 0){
            ang = ang_.demandAve((data_byte_y[0] - 80)*3/4);
            Size = yellow_h;
            if(Yellow_x1 < 80 && 80 < Yellow_x2){
                center = 1;
            }
            else{
                center = 0;
            }
            on = 1;
            return 1;
        }
    }
    on = 0;
    center = 0;
    return 0;
}


void Cam::print(){
    Serial.print(" color : ");
    Serial.print(color);
    if(on == 0){
        Serial.print(" No block detected");
    }
    else{
        Serial.print("  ang : ");
        Serial.print(ang);
        Serial.print("  size : ");
        Serial.print(Size);
        Serial.print(" center : ");
        Serial.print(center);
    }
}