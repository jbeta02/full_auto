#include <Arduino.h>
#include "DriveBase.h"
#include "PathPlanning.h"

DriveBase drive;
PathPlanning pathPlanning;

const int MOTOR1_PIN1 = 6; // motor 1 is on right side of robot
const int MOTOR1_PIN2 = 5;
const int MOTOR2_PIN1 = 4; // motor 2 is on left side of robot
const int MOTOR2_PIN2 = 3;
const int PWM_PINA = 7; // pwm for motor 1
const int PWM_PINB = 2; // pwm for motor 2

const int TILE_SIZE = 12; // in

// global angle
//        90
//        |
// 180 --------- 0
//        |
//        270

int globalAngle = 90; // degrees
int localAngle = 0;

Node* nodePath[pathPlanning.listSize];
int pathSize = 0;

int currentPathIndex = 0;

int state = 0; // determine turn angle, perform turn, determine move, perform move

int getNewGlobalAngle(int xDif, int yDif); // in degrees
int solveMaze();

void setup() {
  // Serial.begin(9600);
  drive.init(MOTOR1_PIN1, MOTOR1_PIN2, MOTOR2_PIN1, MOTOR2_PIN2, PWM_PINA, PWM_PINB);

  pathSize = solveMaze();

  Serial.println(pathSize);
}

void loop() {
  Node* currentNode = nodePath[currentPathIndex];

  // Serial.print("loc A ");
  // Serial.print(localAngle);
  // Serial.print("targ Ang ");
  // Serial.print(drive.getTargetAngle());
  // Serial.print(" global A, ");
  // Serial.print(globalAngle);
  // Serial.print(" curr Ang ");
  // Serial.print(drive.getCurrentRobotAngle());
  // Serial.print(" curr Pos ");
  // Serial.print(drive.getCurrentWheelPosition());
  // Serial.print(" atTargAng ");
  // Serial.print(drive.atTargetAngle());
  // Serial.print(" atTargPos ");
  // Serial.print(drive.atTargetAngle());
  // Serial.print(" state ");
  // Serial.print(state);
  // Serial.print(" error ");
  // Serial.print(drive.out3);
  // Serial.print(" FB, ");
  // Serial.print(drive.out);
  // Serial.print(" targetVel ");
  // Serial.print(drive.targetVel);
  // Serial.print(" FF ");
  // Serial.print(drive.out2);
  // Serial.print(" currVel ");
  // Serial.print(drive.out4);
  // Serial.print(" angle vel ");
  // Serial.print(drive.getTurnVel());
  // Serial.print(" angle acc ");
  // Serial.println(drive.getTurnAcc());
  // Serial.println();

  switch (state) { // determine turn angle, perform turn, determine move, perform move
    // case -2:
    //   drive.turn(-90);
    //   state--;
    //   break;

    // case -3:
    //   drive.updateTurn();
    //   if (drive.atTargetAngle()) {
    //     drive.resetEncoder();
    //     drive.resetGyro();
    //     state--;
    //   }
    //   break;

    case 0:
      if (currentPathIndex + 1 < pathSize) {
        int xDif = nodePath[currentPathIndex + 1]->x - currentNode->x;
        int yDif = nodePath[currentPathIndex + 1]->y - currentNode->y;

        int newGlobalAngle = getNewGlobalAngle(xDif, yDif);

        localAngle = -(newGlobalAngle - globalAngle);
        globalAngle = newGlobalAngle;
      }
      else {
        state = -1;
      }
      state++;
      break;

    case 1:
      drive.turn(localAngle);
      state++;

    case 2:
      if (globalAngle == localAngle) {
        state++;
        break;
      }

      drive.updateTurn();

      if (drive.atTargetAngle()) {
        drive.resetEncoder();
        drive.resetGyro();
        state++;
      }
      break;

    case 3:
      drive.move(TILE_SIZE);
      state++;
    
    case 4:
      drive.updateMove();

      if (drive.atTargetPosition()) {
        drive.resetEncoder();
        currentPathIndex++;
        state = 0;
        if (currentPathIndex >= pathSize - 1) {
          state = -1;
        }
      }
      break;
    
    default:
      drive.stop();
      break;
  }
}

int getNewGlobalAngle(int xDif, int yDif) {
  if (xDif == 0) {
    if (yDif > 0) {
      return 90;
    }
    else {
      return 270;
    }
  }

  if (yDif == 0) {
    if (xDif > 0) {
      return 0;
    }
    else {
      return 180;
    }
  }

  return globalAngle;

}

int solveMaze() {
  // g_cost, h_cost, x, y, isWall, parent
  // DEFINE BLOCKERS
  Node a = {0, 0, 1, 1, true, nullptr};
  Node b = {0, 0, 2, 1, true, nullptr};
  Node c = {0, 0, 3, 1, true, nullptr};
  Node d = {0, 0, 1, 2, true, nullptr}; 
  Node e = {0, 0, 1, 3, true, nullptr};
  Node f = {0, 0, 1, 4, true, nullptr};
  Node* blockers[] = {&a, &b, &c, &d, &e, &f};
  int blockersSize = sizeof(blockers) / sizeof(blockers[0]);
  pathPlanning.grid.generateGrid(blockers, blockersSize); // change grid size in Grid.h

  // DEFINE START AND END (x, y)
  Node* start = &pathPlanning.grid.values[0][0]; // x, y tile/node position
  Node* end  = &pathPlanning.grid.values[3][3];
  return pathPlanning.aStar(start, end, nodePath);
}