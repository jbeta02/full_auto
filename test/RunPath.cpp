# include "PathPlanning.h"
#include <iostream>
#include <fstream>

using namespace std;

void writeData(Node* const* list, int count, std::ofstream* file);

int main() {

    PathPlanning pathPlanning;

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
    Node* nodePath[pathPlanning.listSize];
    int size = pathPlanning.aStar(start, end, nodePath);

    for (int i=0; i < size; i++) {
        cout << nodePath[i]->x << ", " << nodePath[i]->y << "\n";
    }

    cout << "Path Size: " << size << "\n";

    // pathPlanning.print_open_closed();

    std::ofstream fileO("test\\planning_open.csv");
    fileO << "i,x,y\n";
    writeData(pathPlanning.getOpen(), pathPlanning.getOpenCount(), &fileO);

    std::ofstream fileC("test\\planning_closed.csv");
    fileC << "i,x,y\n";
    writeData(pathPlanning.getClosed(), pathPlanning.getClosedCount(), &fileC);

    std::ofstream fileP("test\\planning_path.csv");
    fileP << "i,x,y\n";
    writeData(nodePath, size, &fileP);

    std::ofstream fileB("test\\planning_blockers.csv");
    fileB << "i,x,y\n";
    writeData(blockers, blockersSize, &fileB);

    Node* s_[] = {start};
    std::ofstream fileS("test\\planning_start.csv");
    fileS << "i,x,y\n";
    writeData(s_, 1, &fileS);

    Node* e_[] = {end};
    std::ofstream fileE("test\\planning_end.csv");
    fileE << "i,x,y\n";
    writeData(e_, 1, &fileE);

    Node gridNode = {0, 0, pathPlanning.grid.sizeX, pathPlanning.grid.sizeY, true, nullptr};
    Node* g_[] = {&gridNode};
    std::ofstream fileG("test\\planning_grid.csv");
    fileG << "i,x,y\n";
    writeData(g_, 1, &fileG);

    return 0;
}


void writeData(Node* const* list, int count, std::ofstream* file) {
    for (int i = 0; i < count; i++) {

        if (list[i] == 0) continue;

        *file << i << "," << list[i]->x << "," << list[i]->y << "\n";

        if (list[i+1] == 0) { // break after no longer stream of values
            break;
        }
    }

    file->close();
}