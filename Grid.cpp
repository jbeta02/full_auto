# include "Grid.h"
# include <math.h>

void Grid::generateGrid(Node* blockers, int blockerSize) {

    for (int i=0; i < sizeX; i++) {
        for (int j=0; j < sizeY; j++) {
            Node curr = {0, 0, i, j, false, nullptr}; 
            values[i][j] = &curr;
        }
    }

    for (int i=0; i < blockerSize; i++) {
        values[blockers[i].x][blockers[i].y] = &blockers[i];
    }
}

void Grid::getNeighbours(Node* node, Node* nei[4]) { // nei[4]: front, back, left, right
    if (node->y+1 < sizeY) {
        nei[0] = values[node->x][node->y+1];
    }
    else {
        nei[0] = nullptr;
    }
    if (node->y-1 > 0) {
        nei[1] = values[node->x][node->y-1];
    }
    else {
        nei[1] = nullptr;
    }
    if (node->x-1 > 0) {
        nei[2] = values[node->x-1][node->y];
    }
    else {
        nei[2] = nullptr;
    }
    if (node->x+1 < sizeX) {
        nei[3] = values[node->x+1][node->y];
    }
    else {
        nei[3] = nullptr;
    }
}

// get distance non diagonal
int Grid::getDistance(Node* nodeA, Node* nodeB) {
    int x = abs(nodeA->x - nodeB->x);
    int y = abs(nodeA->y - nodeB->y);
    return x + y;
}