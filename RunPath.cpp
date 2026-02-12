# include "PathPlanning.h"
#include <iostream>
using namespace std;

int main() {

    PathPlanning pathPlanning;

    Node a = {0, 0, 0, 0, false, nullptr};
    pathPlanning.grid.generateGrid(&a, 0);

    Node start = {0, 0, 0, 0, false, nullptr};
    Node end  = {0, 0, 2, 2, false, nullptr};
    Node* nodePath[pathPlanning.listSize];
    int size = pathPlanning.aStar(&start, &end, nodePath);

     cout << size;

    return 0;
}