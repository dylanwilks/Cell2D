#include "c2World.h"
#include "c2Collisions.h"
#include "c2Cell.h"

c2Cell::c2Cell(const c2CellDef& def, c2World* world)
{
    velocity.SetZero();
    in_tree = false;
    handled = false;
    moved = false;

    bool in_bounds = (floor(position.x) >= 0 
                   && floor(position.x) <= world->GetLength() - 1
                   && floor(position.y) >= 0 
                   && floor(position.y) <= world->GetHeight() - 1);

    if(in_bounds) { in_world = true; }
    else { in_world = false; }

    this->position = def.position; //copy c2CellDef values
    this->velocity = def.velocity;

    SetColor(WHITE);

    this->type = def.type;
    this->gravityScale = def.gravityScale;
    this->active = def.active;

    this->world = world; 
    
    parent = nullptr;
}

c2Cell::~c2Cell()
{
    if(in_tree)
    {
        leaves[0]->RemoveCell(this);
    } 
}

void c2Cell::SetColor(int color)
{
    color_values.hex_color = color;  

    color_values.r = ((color >> 16) & 0xFF) / 255.0f;
    color_values.g = ((color >> 8) & 0xFF) / 255.0f;
    color_values.b = (color & 0xFF) / 255.0f;
}

void c2Cell::SetColor(float r, float g, float b)
{
    color_values.hex_color = ((static_cast<int>(255.0f * r) << 16) |
                              (static_cast<int>(255.0f * g) << 8)  |
                              (static_cast<int>(255.0f * b))) & 0xFFFFFF;

    color_values.r = r;
    color_values.g = g;
    color_values.b = b;
}
