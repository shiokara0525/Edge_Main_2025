#pragma once

#include<Arduino.h>
#include<input.h>
#include<process.h>
#include<PS4_control.h>
#include<central_availables.h>

#define ESP_SEND_START 1
#define ESP_SEND_BALL 2
#define ESP_SEND_LINE 3
#define ESP_SEND_CAM_FRONT 4
#define ESP_SEND_AC_DIR 5
#define ESP_SEND_CHECK 6
#define ESP_SEND_NEOPIXEL_D 7
#define ESP_SEND_CAM_BACK 8
#define ESP_SEND_LINE_ALL 9
#define ESP_SEND_BALL_CATCH 10
#define ESP_SEND_NEOPIXEL_A 11
#define ESP_SEND_CAM_FRONT_BLOCK 12
#define ESP_SEND_CAM_BACK_BLOCK 13


class ESP_communicate{
    public:
    PS4_control PS4;
    void sendtoESP(const char* message);
    int get_message_to_ESP(const char* message,byte (&send_content)[4]);

    void read_from_ESP(byte (&data)[2],int label);

    int goal_send_count = 0;
    int line_send_count = 0;
    int Kick = 0;
};

extern ESP_communicate ESP;