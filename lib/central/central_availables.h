#pragma once

#include<Sup.h>

class central_aveilables{
public:
  State main_mode;
  int Mode;
  int Mode_old;
  int test_mode = 0;
  int color;
  int ac_tirget;
  int val_max = 200;
  int Values[6];
};

extern central_aveilables central;