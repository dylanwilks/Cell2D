#include "c2World.h"
#include "c2Cell.h"
#include "c2Collision.h"

c2Cell::c2Cell(const c2CellDef& def, c2World* world)
{
    this->cell_callback = def.cell_callback;
    this->position = def.position;

    SetColor(WHITE);
}

c2Cell::~c2Cell()
{
    nodes[0]->RemoveCell(this);
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
