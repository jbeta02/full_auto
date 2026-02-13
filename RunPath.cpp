# include "PathPlanning.h"
#include <iostream>
using namespace std;

int main() {

    PathPlanning pathPlanning;

    // g_cost, h_cost, x, y, isWall, parent
    Node a = {0, 0, 1, 1, true, nullptr};
    Node b = {0, 0, 2, 1, true, nullptr};
    Node c = {0, 0, 3, 1, true, nullptr};
    Node d = {0, 0, 1, 2, true, nullptr};
    Node e = {0, 0, 1, 3, true, nullptr};
    Node f = {0, 0, 1, 4, true, nullptr};
    Node* blockers[] = {&a, &b, &c, &d, &e, &f};
    pathPlanning.grid.generateGrid(blockers, 6); // change grid size in Grid.h

    Node* start = &pathPlanning.grid.values[0][0]; // x, y tile/node position
    Node* end  = &pathPlanning.grid.values[3][3];
    Node* nodePath[pathPlanning.listSize];
    int size = pathPlanning.aStar(start, end, nodePath);

    for (int i=0; i < size; i++) {
        cout << nodePath[i]->x << ", " << nodePath[i]->y << "\n";
    }

    cout << "Path Size: " << size << "\n";

    pathPlanning.print_open_closed();

    return 0;
}