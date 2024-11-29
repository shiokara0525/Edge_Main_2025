#include<kicker.h>


void Kicker::setup(){
  pinMode(K,OUTPUT);
  pinMode(C,OUTPUT);
  digitalWrite(C,HIGH);
  digitalWrite(K,LOW);
}


int Kicker::run(int kick_){
  if(kick_ == 1){
    if(kick_flag == 0){
      kick_flag = 1;
      kick_time.reset();
    }
  }

  if(kick_flag == 1){
    if(kick_time.read_ms() < 10){
      digitalWrite(C,LOW);
    }
    else if(kick_time.read_ms() < 100){
      digitalWrite(K,HIGH);
      digitalWrite(led,HIGH);
    }
    else if(kick_time.read_ms() < 200){
      digitalWrite(K,LOW);
      digitalWrite(led,LOW);
    }
    else if(kick_time.read_ms() < 1000){
      digitalWrite(C,HIGH);
      digitalWrite(K,LOW);
      digitalWrite(led,LOW);
    }
    else{
      kick_flag = 0;
      digitalWrite(K,LOW);
      digitalWrite(led,LOW);
    }

  }
  return kick_flag;
}


void Kicker::TEST_(){
  digitalWrite(C,HIGH);
  digitalWrite(K,HIGH);
}



int Kicker::stop(){
  digitalWrite(C,LOW);
  digitalWrite(K,LOW);
  while(kick_time.read_ms() < 1000);
  digitalWrite(K,LOW);
  digitalWrite(C,HIGH);
  kick_flag = 0;
  kick_time.reset();
  return kick_flag;
}