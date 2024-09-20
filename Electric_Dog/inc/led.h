#ifndef _LED_H
#define _LED_H

#include <FastLED.h>
#include "basic.h"

CRGB leds[LED_COUNT];

class Light
{
public:
  // Adafruit_NeoPixel strip;

  uint8_t max_bright; // LED亮度控制变量，可使用数值为 0 ～ 255， 数值越大则光带亮度越高
  uint8_t running_step0;
  uint8_t running_step1;
  unsigned long last_run_time;
  // basic function
  uint8_t pos(uint8_t oldPos, bool direction)
  { // true表示原灯带顺序（也是按照机身的同向顺序）  false表示按照机身顺序的U型顺序
    if (direction)
      return oldPos;
    else
    {
      if (oldPos < LED_COUNT / 2)
        return oldPos;
      else
        return LED_COUNT - 1 - oldPos + LED_COUNT / 2;
    }
  }
  void show()
  {
    FastLED.show();
  }

  void fill(uint8_t begin_pos, uint8_t display_num, uint8_t hue)
  {
    if (begin_pos < LED_COUNT)
    {
      if (begin_pos + display_num > LED_COUNT)
        display_num = LED_COUNT - begin_pos;

      for (int i = 0; i < display_num; i++)
        leds[i + begin_pos] = CHSV(hue, 255, 255);
    }
  }
  void fill(uint8_t begin_pos, uint8_t display_num, uint8_t hue, uint8_t value)
  {
    if (begin_pos < LED_COUNT)
    {
      if (begin_pos + display_num > LED_COUNT)
        display_num = LED_COUNT - begin_pos;

      for (int i = 0; i < display_num; i++)
        leds[i + begin_pos] = CHSV(hue, 255, value);
    }
  }
  void fillRainbow(uint8_t begin_pos, uint8_t display_num, uint8_t start_hue, uint8_t deltaHue)
  {
    if (begin_pos < LED_COUNT)
    {
      if (begin_pos + display_num > LED_COUNT)
        display_num = LED_COUNT - begin_pos;

      uint8_t hue = start_hue;
      for (int i = 0; i < display_num; i++)
      {
        leds[i + begin_pos] = CHSV(hue, 255, 255);
        hue += deltaHue;
      }
    }
  }

  // mode function
  void backward()
  {
    if (current_time - last_run_time >= 70)
    {
      last_run_time = current_time;
      running_step0 %= LED_COUNT / 2;

      fill(0, LED_COUNT, 200);
      leds[running_step0] = CHSV(40, 255, 255);
      leds[running_step0 + LED_COUNT / 2] = CHSV(40, 255, 255);

      running_step0++;
      running_step0 %= LED_COUNT / 2;
      show();
    }
  }
  void forward()
  {
    if (current_time - last_run_time >= 70)
    {
      last_run_time = current_time;
      running_step0 %= LED_COUNT / 2;

      fill(0, LED_COUNT, 200);
      leds[LED_COUNT / 2 - 1 - running_step0] = CHSV(40, 255, 255);
      leds[LED_COUNT - 1 - running_step0] = CHSV(40, 255, 255);

      running_step0++;
      running_step0 %= LED_COUNT / 2;

      show();
    }
  }
  void left()
  {
    if (current_time - last_run_time > 5)
    {
      fill(0, 5, 200);
      last_run_time = current_time;

      if (running_step1 > 0)
      {
        fill(5, 5, 121, running_step0++);
        if (running_step0 >= max_bright)
          running_step1 = 0;
      }
      else
      {
        fill(5, 5, 121, running_step0--);
        if (running_step0 <= 0)
          running_step1 = 1;
      }

      last_run_time = current_time;
      show();
    }
  }
  void right()
  {
    if (current_time - last_run_time > 5)
    {
      fill(5, 5, 200);
      last_run_time = current_time;

      if (running_step1 > 0)
      {
        fill(0, 5, 121, running_step0++);
        if (running_step0 >= max_bright)
          running_step1 = 0;
      }
      else
      {
        fill(0, 5, 121, running_step0--);
        if (running_step0 <= 0)
          running_step1 = 1;
      }

      last_run_time = current_time;
      show();
    }
  }

public:
  Light(uint8_t max_bright)
  {
    this->max_bright = max_bright;

    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
    FastLED.setBrightness(max_bright);
    running_step0 = running_step1 = 0;
    last_run_time = 0;
  }
  Light()
  {
    max_bright = 128;

    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
    FastLED.setBrightness(max_bright);
    running_step0 = running_step1 = 0;
    last_run_time = 0;
  }
  void init()
  {
    fill_solid(leds, LED_COUNT, CRGB::Black);
    FastLED.show();
    Serial.println("light initial");
  }

  void turnOff()
  {
    fill_solid(leds, LED_COUNT, CRGB::Black);
    FastLED.show();
  }

  void allRed() // 红温了
  {
    fill(0, 10, 0);
    show();
  }
  void sameDirectionCommonColorRainbowFlow() // 同向共色彩虹流动
  {
    if (current_time - last_run_time >= 10) // 时间间隔最少10
    {

      uint16_t Hue;
      Hue = running_step0 * 0xff;
      fillRainbow(0, 5, Hue, 25);
      fillRainbow(5, 5, Hue, 25);
      show();
      running_step0++;
      running_step0 %= 256;
      last_run_time = current_time;
    }
  }
  void tenColorsCoverFlow()
  {
    running_step0 %= 10;
    running_step1 %= LED_COUNT;

    if (current_time - last_run_time >= 50)
    {
      Serial.println("tenColors function call");
      uint8_t hue = running_step0 * 256 / 10;

      leds[running_step1] = CHSV(hue, 255, 255);
      show();

      last_run_time = current_time;
      running_step1++;
      if (running_step1 % LED_COUNT == 0) // 循环一次后颜色改变
        running_step0++;
    }
  }
  void breath()
  {
    if (current_time - last_run_time >= 5)
    {
      // fill(0, 10, 12);
      if (running_step1 > 0)
      {
        FastLED.setBrightness(running_step0++);
        if (running_step0 >= max_bright)
          running_step1 = 0;
      }
      else
      {
        FastLED.setBrightness(running_step0--);
        if (running_step0 <= 0)
          running_step1 = 1;
      }

      last_run_time = current_time;
      show();
    }
  }

  void runLight()
  {
    switch (run_code)
    {
    case GO_FORWARD:
      forward();
      break;
    case GO_BACKWARD:
      FastLED.setBrightness(max_bright);
      tenColorsCoverFlow();
      break;
    case GO_LEFT:
      FastLED.setBrightness(max_bright);
      left();
      break;
    case GO_RIGHT:
      FastLED.setBrightness(max_bright);
      right();
      break;
    case STOP:
      breath();
      backward(); // FOR TEST
      break;
    case WAITING:
      fillRainbow(0, 10, 0, 20);
      breath();
      break;
    }
  }
}; // 类的声明和实现一定要加分号哈

#endif