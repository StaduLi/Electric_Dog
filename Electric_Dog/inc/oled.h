#include <time.h>
#ifndef _OLED_H_
#define _OLED_H_

#include <U8g2lib.h>

#include "basic.h"
#include "eyeMode.h"
#include "happy.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// 运行模式标志 3是已经闭眼停止标志 2已经睁眼停止标志 0 即将睁眼 1即将闭眼
#define WILL_OPEN 0
#define WILL_CLOSE 1
#define HAVE_OPEN 2
#define HAVE_CLOSE 3

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SCL, SDA);

class Oled
{
private:
  uint8_t step_index;
  uint8_t mode;        // 运行模式标志 3是已经闭眼停止标志 2已经睁眼停止标志 0 即将睁眼 1即将闭眼
  uint8_t blink_times; // 眨眼次数标志
  uint16_t delay_time;
  unsigned long last_run_time;

public:
  void init()
  {
    u8g2.begin();
    u8g2.setPowerSave(0); // 开启省电模式，关闭显示

    mode = blink_times = step_index = 0;
    last_run_time = 0;
    delay_time = 0;
  }
  void turnOff()
  {
    u8g2.clearBuffer();   // 清除内部缓冲区
    u8g2.sendBuffer();    // transfer internal memory to the display
    u8g2.setPowerSave(1); // 开启省电模式，关闭显示
  }

  void play1024Image(const uint8_t *image)
  {
    if (current_time - last_run_time > 3 * 1000)
    {
      last_run_time = current_time;
      u8g2.clearBuffer(); // 清除内部缓冲区
      u8g2.drawXBMP(0, 0, 128, 64, image);
      u8g2.sendBuffer(); // transfer internal memory to the display
    }
  }

  uint8_t getMode()
  {
    return mode;
  }
  void setMode(bool isWantBlink)
  {
    if (isWantBlink)
      switch (mode)
      {
      case HAVE_CLOSE:
        mode = WILL_OPEN;
        break;
      case HAVE_OPEN:
        mode = WILL_CLOSE;
        break;
      }
    else
      switch (mode)
      {
      case WILL_CLOSE:
        mode = HAVE_CLOSE;
        break;
      case WILL_OPEN:
        mode = HAVE_OPEN;
        break;
      default:
        break;
      }
  }

  void eyeChange() // 本函数在完成一个动画后就不再继续进入后面的逻辑 实现复杂动画需要结合setmode 函数
  {
    switch (mode)
    {
    case WILL_CLOSE:
      step_index %= 4;
      if (current_time - last_run_time >= 100)
      {
        last_run_time = current_time;

        u8g2.clearBuffer(); // 清除内部缓冲区
        u8g2.drawXBMP(0, 0, 128, 64, emoji[step_index]);
        u8g2.sendBuffer(); // transfer internal memory to the display

        if (step_index <= 0)
          mode = HAVE_CLOSE;
        else
          step_index--;
      }
      break;
    case WILL_OPEN:
      step_index %= 4;
      if (current_time - last_run_time >= 100)
      {
        last_run_time = current_time;

        u8g2.clearBuffer(); // 清除内部缓冲区
        u8g2.drawXBMP(0, 0, 128, 64, emoji[step_index]);
        u8g2.sendBuffer(); // transfer internal memory to the display

        if (step_index >= 3)
          mode = HAVE_OPEN;
        else
          step_index++;
      }
      break;
    default:
      break;
    }
  }

  void eyeMode1()
  {
    if (current_time - last_run_time >= 100)
    {
      last_run_time = current_time;
      switch (step_index++)
      {
      case 0:
        u8g2.clearBuffer(); // 清除内部缓冲区
        u8g2.drawXBMP(23, 0, 72, 64, gImage_eye1);
        u8g2.sendBuffer(); // transfer internal memory to the display
        break;

      case 1:
        u8g2.clearBuffer(); // 清除内部缓冲区
        u8g2.drawXBMP(23, 0, 72, 64, gImage_eye2);
        u8g2.sendBuffer(); // transfer internal memory to the display
        break;

      case 2:
        u8g2.clearBuffer(); // 清除内部缓冲区
        u8g2.drawXBMP(23, 0, 72, 64, gImage_eye3);
        u8g2.sendBuffer(); // transfer internal memory to the display
        break;

      case 3:
        u8g2.clearBuffer(); // 清除内部缓冲区
        u8g2.drawXBMP(23, 0, 72, 64, gImage_eye3);
        u8g2.sendBuffer(); // transfer internal memory to the display
        break;

      case 4:
        u8g2.clearBuffer(); // 清除内部缓冲区
        u8g2.drawXBMP(23, 0, 72, 64, gImage_eye2);
        u8g2.sendBuffer(); // transfer internal memory to the display
        break;

      case 5:
        u8g2.clearBuffer(); // 清除内部缓冲区
        u8g2.drawXBMP(23, 0, 72, 64, gImage_eye1);
        u8g2.sendBuffer(); // transfer internal memory to the display
        step_index = 0;
        break;

      default:
        step_index = 0;
        break;
      }
    }
  }
  void eyeBlink(uint8_t target_blink_times) // 默认调用后是睁眼状态
  {
    // 长时间睁着眼睛 眨一下
    if (current_time - last_run_time >= 4500 && blink_times >= target_blink_times) // 一个周期眨眼两次
    {
      step_index = 3;
      mode = WILL_CLOSE;
      blink_times = 0;
      Serial.println("I will blink soon");
    }
    if (blink_times < target_blink_times)
    {
      eyeChange();

      if (mode == HAVE_OPEN) // 最后停在睁眼位置
        blink_times++;
      if (mode > 1)//mode=2 3 时不在执行变化 也即进行停留
        setMode(true);
    }
  }
  void eyeBlink(uint8_t target_blink_times, bool isFinallOpen)
  {
    // 长时间 眨一下
    if (current_time - last_run_time >= 4500 && blink_times >= target_blink_times) // 一个周期眨眼两次
    {
      step_index = 3;
      mode = WILL_CLOSE;
      blink_times = 0;
      Serial.println("I will blink soon");
    }
    if (blink_times < target_blink_times)
    {
      eyeChange();

      if (mode == (isFinallOpen ? HAVE_OPEN : HAVE_CLOSE))
        blink_times++;
      if (mode > 1)
        setMode(true);
    }
  }
  void lookSecrately()
  {
    eyeChange();
    if (current_time - last_run_time > delay_time)
      setMode(true);

    if (mode == HAVE_OPEN)
      delay_time = 2 * 1000;
    else if (mode == HAVE_CLOSE)
      delay_time = 10 * 1000;
  }

  void runEmoji()
  {

    switch (run_mode)
    {
    case 0:
      switch (run_code)
      {
      case GO_FORWARD:
        if (current_time - last_run_time >= 100)
        {
          last_run_time = current_time;
          u8g2.clearBuffer(); // 清除内部缓冲区
          u8g2.drawXBMP(0, 0, 128, 64, emoji_run[2]);
          u8g2.sendBuffer();
        } // transfer internal memory to the display
        break;
      case GO_BACKWARD:
        if (current_time - last_run_time >= 100)
        {
          last_run_time = current_time;
          u8g2.clearBuffer(); // 清除内部缓冲区
          u8g2.drawXBMP(0, 0, 128, 64, emoji[2]);
          u8g2.sendBuffer();
        } // transfer internal memory to the display
        break;
      case GO_LEFT:
        if (current_time - last_run_time >= 100)
        {
          last_run_time = current_time;
          u8g2.clearBuffer(); // 清除内部缓冲区
          u8g2.drawXBMP(0, 0, 128, 64, emoji_run[0]);
          u8g2.sendBuffer();
        } // transfer internal memory to the display
        break;
      case GO_RIGHT:
        if (current_time - last_run_time >= 100)
        {
          last_run_time = current_time;
          u8g2.clearBuffer(); // 清除内部缓冲区
          u8g2.drawXBMP(0, 0, 128, 64, emoji_run[1]);
          u8g2.sendBuffer();
        } // transfer internal memory to the display
        break;
      case WAITING:
      case STOP:
        eyeBlink(2);
        break;
      }
      break;
    case 6:
      if (current_time - last_run_time >= 100)
      {
        last_run_time = current_time;
        u8g2.clearBuffer(); // 清除内部缓冲区
        u8g2.drawXBMP(0, 0, 128, 64, gImage_happy1);
        u8g2.sendBuffer();
      } // transfer internal memory to the display
      break;
    }
  }
};

#endif