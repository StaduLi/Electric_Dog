#ifndef _MYSERVO_H_
#define _MYSERVO_H_

#define RIGHT true
#define LEFT false
// angle<90 右侧向后  左侧向前

class myServo
{
private:
    int servo_pin;

public:
    bool rotation_mode;
    void init()
    {
        pinMode(this->servo_pin, OUTPUT);
    }

    myServo(int servo_pin)
    {
        this->servo_pin = servo_pin;
    }

    myServo() {}

    void write(int angle)
    {
        angle %= 180;
        int delay_gap = 500; // 500us
        delay_gap += angle * (2600 - 500) / 180;

        digitalWrite(this->servo_pin, 1);
        delayMicroseconds(delay_gap); // 高电平脉冲

        digitalWrite(this->servo_pin, 0);
        // 由于实际上该项目中单个舵机的运行时间间隔远远大于20ms （motion中的gap>20）
        // 因此这句话也可以注释 但是如果有同一舵机连续写入的要求则不能使用该函数
        // 如此处理极大的提高了项目的运行速度
        // delayMicroseconds(1000 * 20 - delay_gap);
    }
    void write(int angle, bool is_use_low_delay)
    {
        angle %= 180;
        int delay_gap = 500; // 500us
        delay_gap += angle * (2600 - 500) / 180;

        digitalWrite(this->servo_pin, 1); // 高电平脉冲
        delayMicroseconds(delay_gap);

        digitalWrite(this->servo_pin, 0); // 低电平脉冲
        if (is_use_low_delay)
            delayMicroseconds(1000 * 20 - delay_gap);
    }

    void swingForward()
    {
        if (rotation_mode == RIGHT)
            write(130);
        else
            write(50);
    }

    void swingBackward()
    {
        if (rotation_mode == RIGHT)
            write(50);
        else
            write(130);
    }
    void swingRest()
    {
        write(90);
    }

    void swingForward(bool is_use_low_delay)
    {
        if (rotation_mode == RIGHT)
            write(130, is_use_low_delay);
        else
            write(50, is_use_low_delay);
    }

    void swingBackward(bool is_use_low_delay)
    {
        if (rotation_mode == RIGHT)
            write(50, is_use_low_delay);
        else
            write(130, is_use_low_delay);
    }

    void swingRest(bool is_use_low_delay)
    {
        write(90, is_use_low_delay);
    }
};
#endif