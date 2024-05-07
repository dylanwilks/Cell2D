#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <algorithm>

#include "c2Math.h"

struct c2CellDef;
class c2Cell;
class c2QuadTree;

class c2World
{
    public:
        c2World(const c2Vector& gravity, int x, int y); //create world with gravity and # of rows/columns
        ~c2World(); //destruct the world
        
        void Step(c2QuadTree* tree); //takes a step, uses c2QuadTree for collision detection

        c2Cell* CreateCell(const c2CellDef& def); //create cell with defining structure
        void DestroyCell(c2Cell* cell); //deallocate a particular cell

        void SetGravity(const c2Vector &gravity); //change world gravity
        c2Vector GetGravity(); //returns world gravity

        std::vector<c2Cell*> GetCellList(); //returns list of cell pointers
        int GetCellCount(); //returns # of cells in the world

        void ShiftOrigin(const c2Vector& neworigin); //changes origin of the world
        
        int GetHeight(); //returns corresponding dimensions
        int GetLength(); 

        /* Cell position is incremented by time_step * velocity.
         * time_step is automatically adjusted to ensure that
         * time_step * velocity is at most 1 if a cell is too
         * fast to prevent tunneling.                            */
        float TimeStep();
    
    private:
        std::vector<c2Cell*> cells; 
        int m_cellCount;

        c2Vector m_gravity; //cells per second^2
        c2Vector m_origin; //(0,0) is bottom-left corner

        int x; //grid size of the world
        int y;

        float time_step;
};


inline void c2World::ShiftOrigin(const c2Vector& neworigin)
{
    m_origin = neworigin;
}

inline void c2World::SetGravity(const c2Vector& gravity)
{
    m_gravity = gravity;
}

inline std::vector<c2Cell*> c2World::GetCellList()
{
    return cells;
}

inline c2Vector c2World::GetGravity()
{
    return m_gravity;
}

inline int c2World::GetCellCount()
{
    return m_cellCount;
}

inline int c2World::GetLength()
{
    return x;
}

inline int c2World::GetHeight()
{
    return y;
}

inline float c2World::TimeStep()
{
    return time_step;
}

#endif
