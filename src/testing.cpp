#include <iostream>
#include <chrono>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "c2Math.h"
#include "c2World.h"
#include "c2Collisions.h"
#include "c2Cell.h"

#define HEIGHT 10
#define LENGTH 10

int main()
{
    auto seed = time(NULL);
    srand(seed);
 
    c2Vector gravity(0,0);
    c2World* world = new c2World(gravity, LENGTH, HEIGHT);

    c2CellDef cell_def;
    cell_def.position.Set(0,HEIGHT-1);
    cell_def.velocity.Set(0,0);
    cell_def.type = c2_dynamicCell;

    std::vector<c2Cell*> cells;
    c2QuadTree* tree = new c2QuadTree(world);

    for(int i = 0; i <= 4; i++) 
    {
        cells.push_back(world->CreateCell(cell_def));

        int x_pos = rand() % LENGTH;
        int y_pos = rand() % HEIGHT;

        int x_vel = -5 + (rand() % 11);
        int y_vel = -5 + (rand() % 11);
        
        cells[i]->SetPosition(x_pos, y_pos);
        cells[i]->SetVelocity(x_vel, y_vel);

        tree->InsertCell(cells[i]);
    }
    
    c2Vector p1, v1;
    int time, prev = 0;

    p1 = cells[0]->GetPosition();
    p1 = p1.Floor();

    v1 = cells[0]->GetVelocity();

    std::cout << "vx0: " << v1.x << "     vy0: " << v1.y << std::endl;
    std::cout << "x0: " << p1.x << "     y0: " << p1.y << std::endl;

    for(int i = 0; i != 1000000; i++) 
    { 
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        
        world->Step(tree);

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

        p1 = cells[0]->GetPosition();
        p1 = p1.Floor();

        v1 = cells[0]->GetVelocity();

        std::cout << "vx0: " << v1.x << "     vy0: " << v1.y << std::endl;
        std::cout << "x0: " << p1.x << "     y0: " << p1.y << std::endl;
        std::cout << world->TimeStep() << '\n' << std::endl;          
        std::cout << "Time difference = " 
          << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() 
          << "[µs]" << std::endl;        
    }

    delete tree;
    delete world;
}
