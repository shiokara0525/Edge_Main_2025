#include<input.h>

AC ac;             //姿勢制御のオブジェクト
Cam cam_front(3);  //前ゴール
Cam cam_back(4);   //後ろゴール
BALL ball;         //ボールのオブジェクト
LINE line;         //ラインセンサーのオブジェクト