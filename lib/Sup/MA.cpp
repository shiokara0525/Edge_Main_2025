#include<MA.h>


void MA::setLenth(int lenth){
    this->lenth = lenth;
}



float MA::demandAve(float result){
    record[count % lenth] = result;
    float ave = 0;
    for(int i = 0; i < lenth; i++){
        ave += record[i];
    }

    ave /= (lenth < count ? lenth : count);
    // Serial.print(ave);
    // Serial.print(" ");
    count++;
    return ave;
}


float MA::sum(float result){
    record[count % lenth] = result;
    float sum = 0;
    for(int i = 0; i < lenth; i++){
        sum += record[i];
    }
    count++;
    return sum;
} 



float MA::returnAve(){
    float ave = 0;
    for(int i = 0; i < lenth; i++){
        ave += record[i];
    }

    ave /= (lenth < count ? lenth : count);
    return ave;
}




float MA::returnSum(){
    float sum = 0;
    for(int i = 0; i < lenth; i++){
        sum += record[i];
    }
    count++;
    return sum;
} 




void MA::reset(){
    for(int i = 0; i < lenth; i++){
        record[i] = 0;
    }
}




void MA_vec::setLenth(int lenth){
    this->lenth = lenth;
}



Vector2D MA_vec::demandAve(Vector2D result){
    record[count % lenth] = result;
    Vector2D ave(0,0);
    for(int i = 0; i < lenth; i++){
        ave = ave + record[i];
    }

    ave = ave * (1 / (lenth < count ? lenth : count));
    // Serial.print(ave);
    // Serial.print(" ");
    count++;
    return ave;
}


Vector2D MA_vec::sum(Vector2D result){
    record[count % lenth] = result;
    Vector2D sum(0,0);
    for(int i = 0; i < lenth; i++){
        sum = sum + record[i];
    }
    count++;
    return sum;
}



Vector2D MA_vec::returnAve(){
    Vector2D ave(0,0);
    for(int i = 0; i < lenth; i++){
        ave = ave + record[i];
    }

    ave = ave * (1 / (lenth < count ? lenth : count));
    return ave;
}




void MA_vec::reset(){
    for(int i = 0; i < lenth; i++){
        record->set_coodinate(0,0);
    }
}