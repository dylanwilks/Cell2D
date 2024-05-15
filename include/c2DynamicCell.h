#ifndef C2DYNAMICCELL_H
#define C2DYNAMICCELL_H 

#include <queue>

class c2Cell;
class c2World;

struct c2DynamicWorld
{
    c2DynamicWorld()
    {
        time_step = 1/30.0f;
        gravity.SetZero();
    }

    float time_step;
    c2Vector gravity;
    std::vector<c2Cell*> roots;
};

struct c2DynamicCell
{
    c2DynamicCell()
    {
        handled = false;
        velocity.SetZero();
        parent = nullptr;
    }

    bool handled;
    c2Vector velocity;
    c2Cell* parent;
    std::vector<c2Cell*> children;
};

void dynamic_step(c2World* world, c2Cell* cell);
void compute_trees(c2World* world);

#endif /* C2DYNAMICCELL_H */
