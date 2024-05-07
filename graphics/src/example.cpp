#include <chrono>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <vector>

#include "c2World.h"
#include "c2Collisions.h"
#include "c2Cell.h"
#include "c2Math.h"
#include "c2Graphics.h"
#include "c2Functions.h"

constexpr int HEIGHT = 100;
constexpr int WIDTH = 100;

void callback(c2GraphicsDef& settings)
{
    cursor_pan(settings);
    scroll_zoom(settings);
    key_pan(settings);
}

int main()
{
    auto seed = time(NULL);
    srand(seed);

    c2Vector gravity(0.0f, 0.0f);
    c2World* world = new c2World(gravity, WIDTH, HEIGHT);

    c2CellDef cell_def;
    cell_def.position.Set(0, HEIGHT - 1);
    cell_def.velocity.Set(0,0);
    cell_def.type = c2_dynamicCell;

    std::vector<c2Cell*> cells;
    c2QuadTree* tree = new c2QuadTree(world);

    for(int i = 0; i < 50; i++) 
    {
        cells.push_back(world->CreateCell(cell_def));
        
        int x_pos = rand() % static_cast<int>(WIDTH-1);
        int y_pos = rand() % static_cast<int>(HEIGHT-1);

        int x_vel = -1 + (rand() % static_cast<int>(3));
        int y_vel = -1 + (rand() % static_cast<int>(3));
        
        cells[i]->SetPosition(x_pos, y_pos);
        cells[i]->SetVelocity(x_vel, y_vel);

        tree->InsertCell(cells[i]);
    }

    c2GraphicsDef settings(world, tree, &callback); 
    c2DisplayWorld(settings);  //LET USERS GENERATE WORLD DISPLAY BIT BY BIT INSTEAD
}
