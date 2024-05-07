#ifndef CELL_H
#define CELL_H

#include <vector>

#include "c2Math.h"
#include "c2Collisions.h"

class c2World;

enum c2CellType
{
    c2_staticCell, //controlled; not natural
    c2_dynamicCell //experience forces and gravity
};

enum c2CellColor
{
    WHITE = 0xFFFFFF,
    RED = 0xB20000,
    BLUE = 0X3498DB
};

struct c2CellDef //to initialize a set of cells at once
{
    c2CellDef()
    {
        position.SetZero();
        velocity.SetZero();

        type = c2_staticCell;
        gravityScale = 1;
        active = true;      
    }

    c2Vector position;
    c2Vector velocity;

    c2CellType type;
    float gravityScale;
    bool active;
};

class c2Cell
{
    public:
        friend class c2World; //restrict creation to c2World
        friend class c2QuadTree;

        struct cell_color
        {
            int hex_color;
            float r, g, b;
        };
        
        void SetPosition(int x, int y); //set position of individual cell
        void SetVelocity(int x, int y); //set velocity of individual cell
        void SetColor(int color); //set color using hexadecimal values
        void SetColor(float r, float g, float b); //set color using RGB (0-1) values

        c2Vector GetPosition(); //return vector position of cell
        c2Vector GetVelocity(); //return vector velocity of cell
        cell_color GetColor(); //return the color struct

        bool Moved(); //returns true if the cell has moved after a step
        bool inTree(); //returns true if the cell will do collision checks

        std::vector<c2QuadTree*> FetchNodes(); //returns the nodes the cell resides in

    private:
        c2Cell(const c2CellDef& def, c2World* world);
        ~c2Cell();
        
        int index; //index to where it is in the world list
        c2World* world; //points to world that created it
        std::vector<c2QuadTree*> leaves; //stores the leaf nodes the cell resides in

        bool in_tree; //if it is in a quad tree
        bool in_world; //if it is within the world dimensions
        bool handled; //if cell has been 'processed'
        bool moved; //if the cell has moved

        c2Cell* parent; //points to cell it will collide with
        std::vector<c2Cell*> children; //cells to collide with this
        
        c2Vector velocity; //cells per second for each component
        c2Vector position; //could possibly be stored elsewhere to make object size smaller
                           //and reduce chance of cache misses 
        cell_color color_values;
        c2CellType type;   
        float gravityScale;  //could be stored in world, as it is (mostly) 1 object
        bool active;
};

inline void c2Cell::SetPosition(int x, int y)
{
    position.Set(x, y); //SOMEHOW UPDATE ROOT TREE (EFFICIENTLY)
}

inline void c2Cell::SetVelocity(int x, int y)
{
    bool in_bounds = (floor(position.x) >= 0 
                   && floor(position.x) <= world->GetLength() - 1
                   && floor(position.y) >= 0 
                   && floor(position.y) <= world->GetHeight() - 1);

    if(in_bounds) { in_world = true; }
    else { in_world = false; }

    velocity.Set(x, y);
}

inline c2Vector c2Cell::GetPosition()
{
    return position;
}

inline c2Vector c2Cell::GetVelocity()
{
    return velocity;
}

inline c2Cell::cell_color c2Cell::GetColor()
{
    return color_values;
}

inline bool c2Cell::Moved()
{
    return moved;
}

inline bool c2Cell::inTree()
{
    return in_tree;
}

inline std::vector<c2QuadTree*> c2Cell::FetchNodes()
{
    return leaves;
}

#endif
