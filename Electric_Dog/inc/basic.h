#ifndef _BASIC_H
#define _BASIC_H

#include <Arduino.h>

#define RF_PIN D0
#define RR_PIN D1
#define LF_PIN D2 
#define LR_PIN D3
#define LED_PIN D4
#define SCL D5
#define SDA D6
#define CHARGING_ATTACH_PIN D7
#define SERVOS_POWER_CONTROL_PIN D8
// 板子到要验证：舵机电源控制，adc检测

#define GO_FORWARD 1
#define GO_BACKWARD 2
#define GO_RIGHT 3
#define GO_LEFT 4
#define STOP 0
#define WAITING 5

#define LED_COUNT 10
#define DELAY_AND_THEN_LIGHT_SLEEP 10 * 1000    // 10s
#define DELAY_AND_THEN_DEEP_SLEEP 3 * 60 * 1000 // 3min
#define FULL_BATTERY_POWER 1023 * 4.2 * 1.0 / (4.7 + 1.0)
#define LOW_BATTERY_POWER 1023 * 3.4 * 1.0 / (4.7 + 1.0)

// RUNING PARAMETER
uint8_t run_code = 0;  // 0标志着停止
uint8_t run_mode = 0;  // 0-Run 1-action0 2-action1
uint8_t run_times = 0; // 在接收到action code的时候置0
unsigned long current_time;
unsigned long last_active_time;
bool isMotionReset = true;
bool isSleep = false;

// BASIC FUNCTION
bool isCharging()
{
    return HIGH == digitalRead(CHARGING_ATTACH_PIN);
}

int getBatteryVoltage()
{
    return analogRead(A0);
}

bool isLowBattery()
{
    return getBatteryVoltage() < LOW_BATTERY_POWER;
}

#endif