#include "MotionProfile.h"
#include <math.h>

MotionProfile::MotionProfile(float startPosition, //TODO: DOESN'T WORK ON NEGATIVE!!!!! FIX NOW !!!!!
    float endPosition, 
    float maxVel, 
    float maxAcc)
    :startPosition(startPosition), endPosition(endPosition), maxVel(maxVel), maxAcc(maxAcc) { // use member initalizer list
    
    // check if cruise phase exists
    // time to reach maxVel
    maxVelTime = maxVel / maxAcc;
    // distance covered by acc phase
    accDist = 0.5 * maxAcc * pow(maxVelTime, 2);
    // minDistance is combo of acceleration and deceleration phase
    distance = endPosition - startPosition;
    if (distance < 0) {
        distance = -distance;
        direction = -1;
    }

    if (distance > accDist * 2) {
        hasCruise = true;
    }

    if (hasCruise) {
        float cruiseDistance = distance - (accDist * 2);
        cruiseTime = cruiseDistance / maxVel;
        vPeak = maxVel;
    }
    else {
        maxVelTime = sqrt(distance / maxAcc); // from solve for t (acceleration time when reaching maxVel might not happen): 2 * (accDist) = 1/2 * maxAcc t^2 
        cruiseTime = 0.0;
        vPeak = maxAcc * maxVelTime;
        accDist = 0.5 * maxAcc * maxVelTime * maxVelTime;
    }
    totalTime = maxVelTime * 2 + cruiseTime;
}

MotionState MotionProfile::tCurve1D(float time) {

    /*
    base equations:
    a = a_i
    v = a t + v_i
    x = 1/2 a t^2 + v_i t + x_i
    */

    if (time < 0) {
        currAcc = 0;
        currVel = 0;
        currPos = 0;
    }

    else if (time < maxVelTime) {
        currAcc = maxAcc;
        currVel = currAcc * time;
        currPos = 0.5 * currAcc * pow(time, 2);
    }

    else if (time < maxVelTime + cruiseTime) {
        
        currAcc = 0;
        currVel = vPeak;
        currPos = vPeak * (time - maxVelTime) + accDist; // normalize time so time: 0 starts at this section
    }

    else if (time < totalTime) {
        float deccTime = time - (cruiseTime + maxVelTime); // normalize time so time: 0 starts at this section

        currAcc = -maxAcc;
        currVel = currAcc * deccTime + vPeak;
        currPos = 0.5 * currAcc * pow(deccTime, 2) + vPeak * deccTime + (accDist + vPeak * cruiseTime);
    }
    
    else {
        currAcc = 0;
        currVel = 0;
        currPos = distance;
    }

    MotionState motionState;
    motionState.acc = currAcc * direction;
    motionState.vel = currVel * direction;
    motionState.pos = (startPosition + currPos) * direction;
    return motionState;
    
}