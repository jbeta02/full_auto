# ifndef GRID_H
# define GRID_H

struct Node {
    int g_cost = 0; // dist from start
    int h_cost = 0; // dist to end
    int x = 0; // grid pos
    int y = 0; // grid pos
    bool isWall = false;
    Node* parent;
};

class Grid {
    public:

        static constexpr int sizeX = 3; // class var and comiple time constant
        static constexpr int sizeY = 3; // class var and comiple time constant

        void generateGrid(Node* blockers, int blockerSize); // generate grid with blockers

        Node* values[sizeX][sizeY]; // store grid nodes

        void getNeighbours(Node* node, Node* nei[4]); // nei[4]: front, back, left, right

        int getDistance(Node* nodeA, Node* nodeB); // get distance non diagonal
};

# endif