#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <vector>
#include <cmath>
#include <algorithm>

#include "c2Math.h"

class c2World;
class c2Cell;

enum ADJACENT_DIR
{                               //0 1 2
    TOP_LEFT,                   //7 X 3
    TOP,                        //6 5 4
    TOP_RIGHT,
    RIGHT,
    BOTTOM_RIGHT,
    BOTTOM,
    BOTTOM_LEFT,
    LEFT
};

class c2Collision
{
    public:
        virtual void InsertCell(c2Cell* cell) = 0; //should insert cell in all corresponding nodes
        virtual void RemoveCell(c2Cell* cell) = 0; //should remove cell in all nodes it resides in

        virtual c2Cell* GetCellPosition(const c2Vector& pos) const = 0;
        virtual std::array<c2Cell*, 8> GetCellsAdjacent(const c2Vector& pos) const = 0;

    protected:
        std::vector<c2Cell*> cells;
};

#endif /* C2COLLISIONS_H */
