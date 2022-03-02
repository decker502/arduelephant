#ifndef __ELEPHANT_H__
#define __ELEPHANT_H__

#include <BricktronicsMotor.h>
#include <BricktronicsMegashield.h>
#include <BricktronicsButton.h>

#include "player.h"

#define MAX_POWER 255

#define NECK_MOTOR 0
#define HEAD_MOTOR 1
#define WALK_MOTOR 2
class Elephant
{
public:
    Elephant(uint8_t data, uint8_t busy) : player(data, busy)
    {
    }

    void begin()
    {
        neck.begin();
        head.begin();
        walk.begin();
        neckBtn.begin();
        headBtn.begin();
        player.begin();

        reset();
    }

    void run(int motor, int power, int degree, int timeout = 3000)
    {
        switch (motor)

        {
        case NECK_MOTOR:
            neck._pid.SetOutputLimits(-power * MAX_POWER / 100, power * MAX_POWER / 100);
            neck.goToPositionWaitForArrivalOrTimeout(degree, timeout);

            break;
        case HEAD_MOTOR:
            head._pid.SetOutputLimits(-power * MAX_POWER / 100, power * MAX_POWER / 100);
            head.goToPositionWaitForArrivalOrTimeout(degree, timeout);
            break;
        case WALK_MOTOR:
            walk._pid.SetOutputLimits(-power * MAX_POWER / 100, power * MAX_POWER / 100);
            walk.goToPositionWaitForArrivalOrTimeout(degree, timeout);
            break;
        }
    }
    void move(int step)
    {
        int n = step;
        int degree = 720;
        if (step < 0)
        {
            n = -step;
            degree = -degree;
        }
        for (int i = 0; i < n; i++)
        {
            run(WALK_MOTOR, 100, walk.getPosition()+degree, 2000);
            player.play(2);
            delay(200);
        }
    }

    void forward()
    {
        move(5);
    }

    void backward()
    {
        move(-5);
    }

    void lift()
    {
        // head.goToPositionWaitForArrivalOrTimeout(700, 2000);
        run(HEAD_MOTOR, 100, 700, 2000);
        neck.setFixedDrive(60 * MAX_POWER / 100);
        do
        {
            if (neckBtn.isPressed())
            {
                neck.coast();
                break;
            }
        } while (true);

        // neck.goToPositionWaitForArrivalOrTimeout(-60, 2000);

        reset();
    }

    void grab()
    {
        // reset();

        run(HEAD_MOTOR, 100, -700);

        run(NECK_MOTOR, 100, 400);

        run(HEAD_MOTOR, 100, 1000, 3000);

        head.setFixedDrive(60 * MAX_POWER / 100);
        delay(1000);

        run(NECK_MOTOR, 80, -1000);

        run(HEAD_MOTOR, 80, -800);
    }

    void roar()
    {
        player.play(1);
    }

    void reset()
    {
        Serial.println(F("Elephant reset neck to end"));

        neck.setFixedDrive(60 * MAX_POWER / 100);
        do
        {
            if (neckBtn.isPressed())
            {
                neck.coast();
                neck.setPosition(0);

                break;
            }
        } while (true);
        Serial.println(F("Elephant reset head to end"));

        head.setFixedDrive(-100 * MAX_POWER / 100);
        do
        {
            if (headBtn.isPressed())
            {
                head.coast();
                head.setPosition(0);

                break;
            }
        } while (true);
        Serial.println(F("Elephant reset play"));

        roar();

        delay(2000);

        Serial.println(F("Elephant reset neck"));
        // neck.goToPositionWaitForArrivalOrTimeout(neck.getPosition() - 1540, 3000);
        run(NECK_MOTOR, 90, -2000);

        Serial.println(F("Elephant reset head"));
        // head.goToPositionWaitForArrivalOrTimeout(head.getPosition() + 1540, 3000);
        run(HEAD_MOTOR, 80, 1540);

        neck.setPosition(0);
        head.setPosition(0);

        Serial.println(F("Elephant reset done"));
    }

private:
    BricktronicsMotor neck = BricktronicsMotor(BricktronicsMegashield::MOTOR_1);
    BricktronicsMotor head = BricktronicsMotor(BricktronicsMegashield::MOTOR_2);
    BricktronicsMotor walk = BricktronicsMotor(BricktronicsMegashield::MOTOR_3);
    BricktronicsButton headBtn = BricktronicsButton(BricktronicsMegashield::SENSOR_2);
    BricktronicsButton neckBtn = BricktronicsButton(BricktronicsMegashield::SENSOR_4);
    Player player;
};
#endif