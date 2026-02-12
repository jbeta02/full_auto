# include "PathPlanning.h"

Grid PathPlanning::grid;

int PathPlanning::aStar(Node* start, Node* end, Node* nodePath[]) {

    grid.values[start->x][start->y] = start;
    grid.values[end->x][end->y] = end;

    addOpen(start); // add start to open
    while (openCount > 0) { 
        int lowestIndex = openLowestCost();

        Node* current = open[lowestIndex];
        removeOpen(current);
        addclosed(current);

        if (current == end){
            return retracePath(start, end, nodePath);
        }

        Node* nei[4];
        grid.getNeighbours(current, nei);

        for (Node* neighbour: nei) {
            if (neighbour->isWall || inClosed(neighbour)){
                continue;
            }

            int newCostToNeighbour = grid.getDistance(current, neighbour) + current->g_cost;
            if (newCostToNeighbour || !inOpen(neighbour)) {
                neighbour->g_cost = newCostToNeighbour;
                neighbour->h_cost = grid.getDistance(neighbour, end);
                neighbour->parent = current;
            }

            if (!inOpen(neighbour)) {
                addOpen(neighbour);
            }
        }
    }
}


void PathPlanning::addOpen(Node* node) {
    open[openCount++] = node;
}
void PathPlanning::addclosed(Node* node) {
    closed[closedCount++] = node;
}
void PathPlanning::removeOpen(Node* node) {
    open[--openCount] = node;
}
void PathPlanning::removeclosed(Node* node) {
    closed[--closedCount] = node;
}

bool PathPlanning::inOpen(Node* node) {
    for (int i=0; i < openCount; i++) {
        if (open[i] == node) {
            return true;
        }
    }
    return false;
}
bool PathPlanning::inClosed(Node* node) {
    for (int i=0; i < closedCount; i++) {
        if (closed[i] == node) {
            return true;
        }
    }
    return false;
}

int PathPlanning::openLowestCost() {
    int lowestFCost = 10000;
    int lowestIndex = 0;
    for (int i=0; i < openCount; i++) {
        int f_cost = open[i]->g_cost + open[i]->h_cost;
        if (f_cost < lowestFCost || lowestFCost == f_cost && open[i]->h_cost < open[lowestIndex]->h_cost) {
            lowestFCost = f_cost;
            lowestIndex = i;
        }
    }
    return lowestIndex;
}


int PathPlanning::retracePath(Node* start, Node* end, Node* nodePath[]) {
    Node* curr = end;
    int index = 0;
    while (curr != start) {
        nodePath[index++] = curr;
        curr = curr->parent;
    }
    nodePath[index] = start;

    reversePath(nodePath, index+1);
    return index+1;
}

void PathPlanning::reversePath(Node* nodePath[], int len) {
    int i = 0;
    int j = len-1;
    while (i < j) {
        Node* temp = nodePath[i];
        nodePath[i] = nodePath[j];
        nodePath[j] = temp;
        i++;
        j--;
    }
}