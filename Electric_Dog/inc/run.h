#ifndef _RUN_H
#define _RUN_H

#include "basic.h"

#include "wifi.h"
#include "motion.h"
#include "oled.h"
#include "led.h"

Light light(255);
Motion motion(RF_PIN, RR_PIN, LF_PIN, LR_PIN);
Oled oled;
Web web;

void sleep()
{
  if (!isSleep)
  {
    light.turnOff();
    motion.takeBackLegs();
    isSleep = true;
    run_code = WAITING;
    isMotionReset = false;
  }
  oled.lookSecrately();
}

void deepSleep()
{ // 此函数只会运行一次//唤醒需要按rst
  // 其他模块在lightsleep都已经关闭 这里只需关闭montion oled即可
  motion.turnOff();
  oled.turnOff();
  Serial.println("DEEP SLEEP");
  ESP.deepSleep(0);
}

void webRun()
{
  isSleep = false;
  // web handle
  web.handleRequest();

  // led handel
  light.runLight();

  // oled handle
  oled.runEmoji();

  // servo handle
  motion.run();
}

#endif