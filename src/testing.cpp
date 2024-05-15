#include <iostream>
#include <chrono>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "c2Math.h"
#include "c2World.h"
#include "c2Cell.h"
#include "c2DynamicCell.h"

#define HEIGHT 30
#define LENGTH 30

int main()
{
    auto seed = time(NULL);
    srand(seed);
 
    c2World* world = new c2World(LENGTH, HEIGHT);
    c2DynamicWorld world_meta = c2DynamicWorld();
    world->step_callback = &compute_trees;
    world->meta_ptr = (void*)&world_meta;

    c2CellDef cell_def;
    cell_def.cell_callback = &dynamic_step;

    for(int i = 0; i < 11; i++) 
    {
        c2Cell* cell = world->CreateCell(cell_def);
        c2DynamicCell* cell_meta = new c2DynamicCell();
        cell->meta_ptr = (void*)cell_meta;

        int x_pos = rand() % LENGTH;
        int y_pos = rand() % HEIGHT;

        int x_vel = -2 + (rand() % 4);
        int y_vel = -2 + (rand() % 4);
        
        cell->SetPosition(c2Vector(x_pos, y_pos));
        cell_meta->velocity = c2Vector(x_vel, y_vel);
    }

    std::vector<c2Cell*> cells = world->GetCells();
    c2DynamicCell* cell_meta = static_cast<c2DynamicCell*>(cells[0]->meta_ptr);
    
    c2Vector p, v;
    int time, prev = 0;

    for(int i = 0; i != 1000000; i++) 
    { 
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        world->Step();

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        p = cells[0]->GetPosition();
        v = cell_meta->velocity;

        std::cout << "x0: " << p.x << "     y0: " << p.y << std::endl;
        std::cout << "v.x0: " << v.x << "     v.y0: " << v.y << std::endl;
        std::cout << world_meta.time_step << '\n' << std::endl;          
        std::cout << "Time difference = " 
          << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() 
          << "[µs]" << std::endl;        
    }

    delete world;
}
