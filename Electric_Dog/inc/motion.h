#ifndef _MOTION_H_
#define _MOTION_H_

#include "myServo.h"
#include "basic.h"
#define RF 0
#define RR 1
#define LF 2
#define LR 3

class Motion
{
private:
    myServo servos[4];
    uint8_t step_index;
    unsigned long last_run_time;
    uint8_t gap; // unit millseconds

    // 以下均采用非阻塞办法
    void goForward()
    {
        // 判断是否达到运行时间
        if (current_time - last_run_time >= gap)
        {
            last_run_time = current_time;
            step_index %= 8;
            switch (step_index++)
            {
            case 0:
                servos[0].swingForward();
                servos[3].swingForward();
                break;

            case 1:
                servos[1].swingBackward();
                servos[2].swingBackward();
                break;

            case 2:
                servos[0].swingRest();
                servos[3].swingRest();
                break;

            case 3:
                servos[1].swingRest();
                servos[2].swingRest();
                break;

            case 4:
                servos[1].swingForward();
                servos[2].swingForward();
                break;

            case 5:
                servos[0].swingBackward();
                servos[3].swingBackward();
                break;

            case 6:
                servos[1].swingRest();
                servos[2].swingRest();
                break;

            case 7:
                servos[0].swingRest();
                servos[3].swingRest();
                step_index = 0; // 置0以便进行下一次循环
                break;

            default:
                step_index = 0;
                break;
            }
        }
    };

    void goBackward()
    {
        if (current_time - last_run_time >= gap)
        {
            last_run_time = current_time;
            step_index %= 8;
            switch (step_index++)
            {
            case 0:
                servos[0].swingBackward();
                servos[3].swingBackward();
                break;

            case 1:
                servos[1].swingForward();
                servos[2].swingForward();
                break;

            case 2:
                servos[0].swingRest();
                servos[3].swingRest();
                break;

            case 3:
                servos[1].swingRest();
                servos[2].swingRest();
                break;

            case 4:
                servos[1].swingBackward();
                servos[2].swingBackward();
                break;

            case 5:
                servos[0].swingForward();
                servos[3].swingForward();
                break;

            case 6:
                servos[1].swingRest();
                servos[2].swingRest();
                break;

            case 7:
                servos[0].swingRest();
                servos[3].swingRest();
                step_index = 0;
                break;
            default:
                step_index = 0;
                break;
            }
        }
    };

    void goRight()
    {
        if (current_time - last_run_time >= gap)
        {
            last_run_time = current_time;
            step_index %= 4;
            switch (step_index++)
            {

            case 0:
                servos[0].swingBackward();
                servos[3].swingForward();
                break;

            case 1:
                servos[1].swingForward();
                servos[2].swingBackward();
                break;

            case 2:
                servos[0].swingRest();
                servos[3].swingRest();
                break;

            case 3:
                servos[1].swingRest();
                servos[2].swingRest();
                step_index = 0;
                break;
            default:
                step_index = 0;
                break;
            }
        }
    };

    void goLeft()
    {
        if (current_time - last_run_time >= gap)
        {
            last_run_time = current_time;
            step_index %= 4;
            switch (step_index++)
            {
            case 0:
                servos[0].swingForward();
                servos[3].swingBackward();
                break;
            case 1:
                servos[1].swingBackward();
                servos[2].swingForward();
                break;
            case 2:
                servos[0].swingRest();
                servos[3].swingRest();
                break;
            case 3:
                servos[1].swingRest();
                servos[2].swingRest();
                step_index = 0;
                break;
            default:
                step_index = 0;
                break;
            }
        }
    };

public:
    Motion(int pin_rf, int pin_rr, int pin_lf, int pin_lr)
    {
        servos[0] = myServo(pin_rf);
        servos[1] = myServo(pin_rr);
        servos[2] = myServo(pin_lf);
        servos[3] = myServo(pin_lr);

        servos[0].rotation_mode = servos[1].rotation_mode = RIGHT;
        servos[2].rotation_mode = servos[3].rotation_mode = LEFT;

        gap = 200;
        run_times = 1;
        step_index = 0;
    }
    Motion(int pin_rf, int pin_rr, int pin_lf, int pin_lr, uint8_t gap)
    {
        servos[0] = myServo(pin_rf);
        servos[1] = myServo(pin_rr);
        servos[2] = myServo(pin_lf);
        servos[3] = myServo(pin_lr);

        servos[0].rotation_mode = servos[1].rotation_mode = RIGHT;
        servos[2].rotation_mode = servos[3].rotation_mode = LEFT;

        if (gap < 100)
            gap = 100;
        this->gap = gap;
        run_times = 1;
        step_index = 0;
    }

    void init()
    {
        digitalWrite(SERVOS_POWER_CONTROL_PIN, 1);
        for (int i = 0; i < 4; i++)
            servos[i].init();
    }
    void turnOff(){
            digitalWrite(SERVOS_POWER_CONTROL_PIN, 0); // turn off servos power
    }

    void reset()
    {
        for (int i = 0; i < 4; i++)
            servos[i].swingRest(false);
    }

    void lieDown() // 由于阻塞时间比较短（小于5s）,本动作一般只运行一次，而且非阻塞办法较难实现，这里采用阻塞方法
    {
        for (uint8_t angle = 90; angle > 0; angle -= 5) //???为什么减1会导致触发看门狗呢
        {
            servos[RR].write(angle, true);
            servos[LR].write(180 - angle, true);
        }
        delay(700);

        for (uint8_t angle = 90; angle > 0; angle -= 5)
        {
            servos[RF].write(180 - angle, true);
            servos[LF].write(angle, true);
        }
    }
    void takeBackLegs()
    {                                                   // 由于阻塞时间比较短（小于5s）,本动作一般只运行一次，而且非阻塞办法较难实现，这里采用阻塞方法
        for (uint8_t angle = 90; angle > 0; angle -= 5) //???为什么减1会导致触发看门狗呢
        {
            servos[0].write(angle, true);
            servos[2].write(180 - angle, true);
        }
        delay(700);

        for (uint8_t angle = 90; angle > 0; angle -= 5)
        {
            servos[1].write(180 - angle, true);
            servos[3].write(angle, true);
        }
    }

    void swingBackAndFront() // 由于阻塞时间比较短（小于5s）,本动作一般只运行一次，而且非阻塞办法较难实现，这里采用阻塞方法
    {

        for (uint8_t i = 0; i < 4; i++)
            servos[i].swingForward(true);
        delay(200);
        for (uint8_t i = 0; i < 4; i++)
            servos[i].swingBackward(true);
        delay(200);
        for (uint8_t i = 0; i < 4; i++)
            servos[i].swingRest(true);
        delay(200);
    }
    void shakeHands()
    { // 采用阻塞办法
        for (int angle = 90; angle <= 160; angle += 5)
            servos[RF].write(angle, true);
        delay(1000);
        for (int angle = 160; angle >= 140; angle -= 4)
            servos[RF].write(angle, true);
        for (int angle = 140; angle <= 180; angle += 4)
            servos[RF].write(angle, true);
        for (int angle = 180; angle >= 160; angle -= 4)
            servos[RF].write(angle, true);
        for (int angle = 160; angle >= 90; angle -= 4)
            servos[RF].write(angle, true);
    }
    void stretch()
    {
        for (int i = 0; i < 4; i++)
            servos[i].swingForward(true);
        delay(1500);
        for (int i = 0; i < 4; i++)
            servos[i].swingRest(true);
        delay(500);
        for (int i = 0; i < 4; i++)
            servos[i].swingBackward(true);
        delay(1000);
        for (int i = 0; i < 4; i++)
            servos[i].swingRest(true);
    }

    // 模式更替
    void run()
    {
        switch (run_mode)
        {
        case 0:
            if (run_code != 0)
                isMotionReset = false;
            switch (run_code)
            {
            case GO_FORWARD:
                goForward();
                break;
            case GO_BACKWARD:
                goBackward();
                break;
            case GO_LEFT:
                goLeft();
                break;
            case GO_RIGHT:
                goRight();
                break;
            case WAITING:
            case STOP:
                if (!isMotionReset)
                {
                    reset();
                    isMotionReset = true;
                }
                break;
            }
            break;
        case 1:
            if (run_times < 1)
            {
                Serial.println("lieDown call");
                lieDown();
                run_times++;
            }
            break;
        case 2:
            reset();
            break;
        case 3:
            if (run_times < 3)
            {
                swingBackAndFront();
                run_times++;
            }

            break;
        case 4:
            if (run_times < 1)
            {
                shakeHands();
                run_times++;
            }
            break;
        case 5:
            if (run_times < 1)
            {
                stretch();
                run_times++;
            }
            break;
        }
    }
};

#endif