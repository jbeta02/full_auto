# include "PathPlanning.h"
#include <iostream>
using namespace std;

int main() {

    PathPlanning pathPlanning;

    Node a = {0, 0, 0, 0, false, nullptr};
    pathPlanning.grid.generateGrid(&a, 0);

    Node* start = &pathPlanning.grid.values[0][0];
    Node* end  = &pathPlanning.grid.values[2][2];
    Node* nodePath[pathPlanning.listSize];
    int size = pathPlanning.aStar(start, end, nodePath);

    for (int i=0; i < size; i++) {
        cout << nodePath[i]->x << ", " << nodePath[i]->y << "\n";
    }

    cout << size;

    return 0;
}