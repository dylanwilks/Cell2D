#ifndef CELL_H
#define CELL_H

#include <vector>

#include "c2Math.h"
#include "c2Collision.h"

class c2World;

enum c2CellColor
{
    WHITE = 0xFFFFFF,
    RED = 0xB20000,
    BLUE = 0X3498DB
};

struct c2CellDef //to initialize a set of cells at once
{
    c2CellDef(
      c2Vector position = c2Vector(0,0),
      void (*cell_callback)(c2World* world, c2Cell* cell) = nullptr)
    {
        this->position = position;
        this->cell_callback = cell_callback;
    }

    c2Vector position;
    void (*cell_callback)(c2World* world, c2Cell* cell);
};

class c2Cell
{
    public:
        friend class c2QuadTree;
        friend class c2World;

        struct cell_color
        {
            int hex_color;
            float r, g, b;
        };
       
        void (*cell_callback)(c2World* world, c2Cell* cell);
        void* meta_ptr;

        void SetPosition(const c2Vector& position); //performs update in collision object as well
        void SetColor(int color); //set color using hexadecimal values
        void SetColor(float r, float g, float b); //set color using RGB (0-1) values

        c2Vector GetPosition() const;
        cell_color GetColor() const; //return the color struct

    private:
        c2Cell(const c2CellDef& def, c2World* world);
        ~c2Cell();

        c2Vector position; 
        cell_color color_values;
        std::vector<c2Collision*> nodes; //stores the nodes the cell resides in
};


inline void c2Cell::SetPosition(const c2Vector& position)
{
    nodes[0]->InsertCell(this);
    this->position = position;
}

inline c2Vector c2Cell::GetPosition() const
{
    return position;
}

inline c2Cell::cell_color c2Cell::GetColor() const
{
    return color_values;
}

#endif
