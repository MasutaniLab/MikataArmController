/*
ロボットアームの手先が，
(x, y, z) = (0.15[m], 0.20[m], 0.15[m]) → (0.15[m], -0.20[m], 0.20[m]) → (0.2[m], 0.0[m], 0.15[m])
の3点を巡回させるプログラムを作成せよ．点間の所要時間は3秒とする．
*/
#include "practice.h"
#include <iostream>
#include <Windows.h>
using namespace std;

int index;

void setup()
{
  index = 0;
}

void loop()
{
  if (statusExecuting == false) {
    Beep(440, 200);
    ArmTip target;
    target.pitch = 0.0;
    target.hand = 0.0;
    target.time = 3.0;
    if (index == 0) {
      target.x = 0.132;
      target.y = 0.0;
      target.z = 0.03;
      target.pitch = -0.55;
      cout << "index: 0" << endl;
    } else if (index == 1) {
      target.x = 0.15;
      target.y = 0.20;
      target.z = 0.15;
      cout << "index: 1" << endl;
    } else if (index == 2) {
      target.x = 0.15;
      target.y = -0.20;
      target.z = 0.20;
      cout << "index: 2" << endl;
    } else if (index == 3) {
      target.x = 0.2;
      target.y = 0.0;
      target.z = 0.15;
      cout << "index: 3" << endl;
    }
    setArmTipTarget(target);
    index++;
    if (index == 4) index = 1;
  }
}
