#include "MotionProfile.h"

MotionProfile::MotionProfile(float startPosition,
    float endPosition, 
    float maxVel, 
    float maxAcc)
    :startPosition(startPosition), endPosition(endPosition), maxVel(maxVel), maxAcc(maxAcc) { // use member initalizer list
    
    // check if cruise phase exists
    // time to reach maxVel
    float maxVelTime = maxVel / maxAcc;
    // distance covered by acc phase
    float accDist = 0.5 * maxAcc * maxVelTime * maxVelTime;
    // minDistance is combo of acceleration and deceleration phase
    float minDistance = accDist * 2;

    if (distance > minDistance) {
        hasCruise = true;
    }
}

int* MotionProfile::tCurve1D(float time) {
    
}