#include <fstream>
#include "MotionProfile.h"


int main() {

    std::ofstream file("test\\profile.csv");
    file << "t,x,v,a\n";

    MotionProfile profile(0, 10, 5, 10);
    double dt = 0.001;
    for (double t = 0; t <= profile.totalTime; t += dt) {
        MotionState s = profile.tCurve1D(t);
        file << t << "," << s.pos << "," << s.vel << "," << s.acc << "\n";
    }

    file.close();
    return 0;
}