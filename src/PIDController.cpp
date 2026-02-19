#include "Arduino.h"
#include "PIDController.h"

// PIDController contructors
PIDController::PIDController(float p, float i, float d) {
    this->p = p;
    this->i = i;
    this->d = d;
}

PIDController::PIDController(float p) {
    PIDController(p, 0, 0);
}


float PIDController::update(float error) {
    float time = micros(); // get current time
    float output = 0; // reset output for this cycle

    // proportional
    output += p * error;

    if (!firstLoop) { // prevent first loop which will have drastic differeces between time and last time on first loop
        // integral
        // use trapezoidal rule for integral calculation (a numerical approach to integration instead of analytical)
        // consider using Simpson's Rule
        float deltaTime = time - lastTime;
        sum += (error + lastError) * deltaTime / 2; 

        // cap integral sum (clamp integral sum)
        if (integralBounded) {
            if (sum > maxSum)
                sum = maxSum;

            if (sum < minSum)
                sum = minSum;
        }

        output += sum * i;

        // derivative
        //TODO: look into adding a low pass filter to filter out high frequency noice
        output += (error - lastError) / deltaTime * d;
    }

    firstLoop = false;

    lastError = error;
    lastTime = time;

    if (outputBounded) {
        if (output > outputMax)
            output = outputMax;

        if (output < outputMin)
            output = outputMin;
    }

    // Serial.begin(2000000);
    // Serial.println(output);

    return output;
}


// getters and setters
void PIDController::setOutputBounds(float min, float max){ 
    outputMax = max;
    outputMin = min;
    outputBounded = true;
}

void PIDController::setInegralSumBounds(float min, float max){ 
    maxSum = max;
    minSum = min;
    integralBounded = true;
}

float PIDController::getSum(){ 
    return sum;
}