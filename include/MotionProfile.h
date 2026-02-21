#ifndef MOTION_PROFILE_H
#define MOTION_PROFILE_H

struct MotionState {
    float pos, vel, acc;
};

class MotionProfile {

    public:

        MotionProfile(float startPosition, float endPosition, float maxVel, float maxAcc);

        MotionState tCurve1D(float time); // return pointer instead of vector to use with arduino

        bool atPosition();

        float totalTime;

    private:

        float startPosition, endPosition, maxVel, maxAcc = 0;
        float distance = 0;
        int direction = 1; // 1 for clockwise, -1 for counter-clockwise
        float currPos, currVel, currAcc = 0;
        bool hasCruise = false;
        float maxVelTime;
        float cruiseTime;
        float vPeak;
        float accDist;
};

#endif