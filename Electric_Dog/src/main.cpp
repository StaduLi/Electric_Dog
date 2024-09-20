#include <Arduino.h>
#include <FastLED.h>

#include "inc/basic.h"
#include "inc/run.h"

// light web  oled motion 在run.h中已经实例化

void setup()
{
  // delay(1000);  // 系统稳定性等待
  Serial.begin(115200);
  Serial.println("");

  if (!SPIFFS.begin())
  {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else
  {
    Serial.println("SPIFFS mounted successfully");

    Dir dir = SPIFFS.openDir("/");
    while (dir.next())
    {
      Serial.println(dir.fileName());
    }
  }

  pinMode(CHARGING_ATTACH_PIN, INPUT);
  pinMode(SERVOS_POWER_CONTROL_PIN, OUTPUT);

  web.dnsServerInit();
  oled.init();
  light.init();
  motion.init();
  Serial.println("Initial Accomplished!");
}

void loop()
{

  current_time = millis();
  if (isCharging())
  {
    Serial.println("Charging now I am sleeping");
    sleep(); // 直接休眠
    web.handleRequest();
  }
  else if (web.getConnectUserNum() == 0) // 判断是否有用户连接
  {
    if (current_time - last_active_time > DELAY_AND_THEN_DEEP_SLEEP)//长时间无人连接 进入深度睡眠
      deepSleep();
    else if (current_time - last_active_time > DELAY_AND_THEN_LIGHT_SLEEP) // 等一等再休眠
      sleep();
    else // 在没有用户连接到睡眠的这一段时间内小狗停止运动
    {
      isMotionReset = false;
      run_code = STOP;
      webRun();
    }
  }
  else
  {
    last_active_time = current_time;
    webRun();
  }
}
