#include<angle.h>

angle::angle(double ang,bool flag){
    if(flag == true){
        degree = ang;
        radians = radians(ang);
    }
    else{
        radians = ang;
        degree = degrees(ang);
    }
}



angle::angle(double ang,bool ang_unit,double border,bool border_flag){
    if(ang_unit == true){
        degree = ang;
        radians = radians(ang);
    }
    else{
        radians = ang;
        degree = degrees(ang);
    }

    float max;
    float low;
    if(border_flag == true){
        max = border;
        low = border - 360;
    }
    else{
        low = border;
        max = border + 360;
    }

    while(degree < low){
        degree += 360;
    }

    while(max < degree){
        degree -= 360;
    }
    radians = radians(degree);
}



double angle::to_range(double border,bool flag){
    float max;
    float low;
    if(flag == true){
        max = border;
        low = border - 360;
    }
    else{
        low = border;
        max = border + 360;
    }

    while(this->degree < low){
       this->degree += 360;
    }

    while(max < this->degree){
        this->degree -= 360;
    }
    radians = radians(this->degree);

    return degree;
}



void angle::setAng(double ang,bool ang_unit){
    if(ang_unit == true){
        degree = ang;
        radians = radians(ang);
    }
    else{
        radians = ang;
        degree = degrees(ang);
    }
}




void angle::operator=(double ang){
    this->degree = ang;
    this->radians = radians(ang);
}



void angle::operator-=(double ang){
    this->degree -= ang;
    this->radians = radians(degree);
}



void angle::operator+=(double ang){
    this->degree += ang;
    this->radians = radians(degree);
}