# ifndef PATH_PLANNING_H
# define PATH_PLANNING_H

#include "Grid.h"

class PathPlanning {
    public:
        int aStar(Node* start, Node* end, Node* nodePath[]); // return size of nodepath
        static Grid grid;
        static constexpr int listSize = grid.sizeX * grid.sizeY;

        void print_open_closed();

        Node* const* getOpen() const;
        Node* const* getClosed() const;

        int getOpenCount();
        int getClosedCount();

    private:
        int pathSize = 0;
        int openCount = 0;
        int closedCount = 0;
        Node* open[listSize];
        Node* closed[listSize];

        Node* blockers[listSize];
        int blockersSize = sizeof(blockers) / sizeof(blockers[0]);

        void addOpen(Node* node);
        void addclosed(Node* node);

        void removeOpen(Node* node);
        void removeClosed(Node* node);

        bool inOpen(Node* node);
        bool inClosed(Node* node);

        int openLowestCost(); // returns index of open with lowest f_cost

        int retracePath(Node* start, Node* end, Node* nodePath[]);

        void reversePath(Node* nodePath[], int len);
};

# endif