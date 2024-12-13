#pragma once
#include<Arduino.h>
#include<timer.h>

class State {
private:
    int currentState;  //ステートの番号
    int change;        //1だったらそのときステートが変わった 
    timer stateTimer;  //ステートに居続けているタイマー

public:
    State() {
        currentState = 999;
        change = 0;
    }

    void reset(){
        stateTimer.reset();
        currentState = 999;
        change = 0;
    }

    void enterState(int newState) {
        if (currentState != newState) {
            currentState = newState;
            change = 1;
            stateTimer.reset();
        } else {
            change = 0;
        }
    }

    int getCurrentState() {
        return currentState;
    }

    int getChange() {
        return change;
    }

    unsigned long readStateTimer() {
        return stateTimer.read_ms();
    }

    unsigned long readStateTimer(int a){
        if(a == currentState){
            return stateTimer.read_ms();
        }
        else{
            return 0;
        }
    }
};

