#include "timer.h"
timer::timer(){
	tm = millis();
	tu = micros();
}

void timer::reset(){
	tm = millis();
	tu = micros();
}

unsigned long timer::read_ms(){
	return (millis() - tm);
}

unsigned long timer::read_us(){
	return (micros() - tu);
}
