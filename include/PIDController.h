#pragma once

// PID Controller is used to get an actuator to a target position or setpoint
class PIDController {

    public:

    PIDController(float p, float i, float d);

    PIDController(float p);

    float update(float error);

    void setOutputBounds(float min, float max);
    void setInegralSumBounds(float min, float max);

    float getSum();

    private:

    float p;
    float i;
    float d;

    float outputMax;
    float outputMin;

    float sum;
    float maxSum;
    float minSum;


    float time;
    float lastTime;

    float error;
    float lastError;

    bool firstLoop = true;

    bool outputBounded = false;
    bool integralBounded = false;

    bool clamped = false;

};