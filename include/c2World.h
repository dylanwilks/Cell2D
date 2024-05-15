#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <array>
#include <algorithm>

#include "c2Math.h"

struct c2CellDef;
class c2Cell;
class c2Collision;

class c2World
{
    public:
        c2World(int x, int y); //create world with gravity and # of rows/columns
        ~c2World(); //destruct the world
        
        void Step(); //takes a step; can provide callback optionally
        void (*step_callback)(c2World* world);
        void* meta_ptr;

        c2Cell* CreateCell(const c2CellDef& def); //create cell with defining structure
        void DestroyCell(c2Cell* cell); //deallocate given cell

        std::vector<c2Cell*> GetCells() const; //returns list of cell pointers
        
        int GetLength() const;
        int GetHeight() const;

        //supplementary functions for the user
        c2Cell* GetCellPosition(const c2Vector& pos) const;
        std::array<c2Cell*, 8> GetCellsAdjacent(const c2Vector& pos) const;
        std::vector<c2Cell*> GetCellsRange(const c2Vector& p1, const c2Vector& p2) const;
    
    private:
        int x, y;
        c2Collision* collision;  //perhaps need a way to determine what child type?
        std::vector<c2Cell*> cells;  //try sorting based on 
};


inline std::vector<c2Cell*> c2World::GetCells() const
{
    return cells;
}

inline int c2World::GetLength() const
{
    return x;
}

inline int c2World::GetHeight() const
{
    return y;
}

#endif
