#include "c2World.h"
#include "c2Cell.h"
#include "c2DynamicCell.h"

void dynamic_step(c2World* world, c2Cell* cell)
{
    c2DynamicCell* cell_meta = static_cast<c2DynamicCell*>(cell->meta_ptr);
    c2DynamicWorld* world_meta = static_cast<c2DynamicWorld*>(world->meta_ptr);
    c2Vector new_pos = cell->GetPosition() + (cell_meta->velocity * world_meta->time_step);     

    if(cell_meta->velocity.Magnitude() != 0 
      && (1 / cell_meta->velocity.Magnitude() < world_meta->time_step
      || world_meta->time_step == 0))
        world_meta->time_step = 1 / cell_meta->velocity.Magnitude();

    if(new_pos.Floor() != cell->GetPosition().Floor())  //cell moves
    {   
        if(new_pos.Floor().x < 0 || new_pos.Floor().x >= world->GetLength())
        {
            c2Matrix22 reflect_y(-1, 0, 
                                  0, 1);  //Reflection about the y-axis     
            cell_meta->velocity = reflect_y.Solve(cell_meta->velocity);
            cell_meta->handled = true;  //Check if the cell will collide
                                        //with the floor instead.
            return;                     //If so, update velocity and continue
        }                                                          

        if(new_pos.Floor().y < 0 || new_pos.Floor().y >= world->GetHeight())
        {
            c2Matrix22 reflect_x(1, 0, 
                                 0, -1);  //Reflection about the x-axis
            cell_meta->velocity = reflect_x.Solve(cell_meta->velocity);
            cell_meta->handled = true;

            return;
        }

        c2Cell* clcell = world->GetCellPosition(new_pos);  //HANDLE CASE WHERE STRUCT OF CELL DIFFERS
        bool COLLISION = clcell != nullptr ? 1 : 0;
        if(COLLISION) //collision detected, cell moves
        {
            c2DynamicCell* clcell_meta = 
              static_cast<c2DynamicCell*>(clcell->meta_ptr);
            c2Cell* clparent = clcell_meta->parent;
            bool child_flag = true;

            if(clparent == nullptr) {
                if(clcell_meta->handled && clcell_meta->children.empty())
                    world_meta->roots.push_back(clcell);
            }
            else 
            {
                if(clparent == cell) {
                    world_meta->roots.push_back(cell);
                    child_flag = false;
                }
                else 
                {
                    while(clparent != nullptr) {
                        if(cell == clparent) {
                            child_flag = false; //SHOULD MOVE INTO ELSE STATEMENT
                            COLLISION = false;
                            break;
                        }

                        c2DynamicCell* clparent_meta =
                          static_cast<c2DynamicCell*>(clparent->meta_ptr);
                        clparent = clparent_meta->parent;
                    }
                }
            }

            if(child_flag) {
                clcell_meta->children.push_back(cell);
                cell_meta->parent = clcell;
            }
        }
        
        if(!COLLISION) //no collision, cell moves
        {
            std::vector<c2Cell*> cell_queue;

            cell->SetPosition(new_pos);
            c2Cell* ref_cell = cell;
            c2DynamicCell* rcell_meta = 
              static_cast<c2DynamicCell*>(ref_cell->meta_ptr);

            while(!rcell_meta->children.empty())
            {
                c2Cell* child_cell = rcell_meta->children[0];
                c2DynamicCell* child_meta = 
                  static_cast<c2DynamicCell*>(child_cell->meta_ptr);

                child_cell->SetPosition(child_cell->GetPosition() +
                  child_meta->velocity * world_meta->time_step);

                if(rcell_meta->children.size() > 1)
                    cell_queue.push_back(ref_cell);
                else
                    rcell_meta->children.clear();

                c2DynamicCell* cparent_meta = 
                  static_cast<c2DynamicCell*>(child_meta->parent->meta_ptr);
                if(cparent_meta->children.empty())
                    child_meta->parent = nullptr;

                ref_cell = child_cell;
                rcell_meta = static_cast<c2DynamicCell*>(ref_cell->meta_ptr);
            }

            if(!cell_queue.empty())
            {
                for(auto qcell_itr = cell_queue.rbegin(); 
                    qcell_itr != cell_queue.rend(); ++qcell_itr)
                {
                    c2Cell* qcell = *qcell_itr;
                    c2DynamicCell* qcell_meta = 
                      static_cast<c2DynamicCell*>(qcell->meta_ptr);

                    c2DynamicCell* qcell_meta2 = 
                      static_cast<c2DynamicCell*>(
                      qcell_meta->children[0]->meta_ptr);

                    for(auto qchild_itr = qcell_meta->children.begin() + 1;
                        qchild_itr != qcell_meta->children.end(); ++qchild_itr)
                    {
                        c2Cell* qchild = *qchild_itr;
                        c2DynamicCell* qchild_meta = 
                          static_cast<c2DynamicCell*>(qchild->meta_ptr);

                        qcell_meta2->children.push_back(qchild);
                        qchild_meta->parent = qcell_meta->children[0];
                    }

                    qcell_meta2->parent = nullptr;
                    world_meta->roots.push_back(qcell_meta->children[0]);

                    qcell_meta->children.clear();
                }
            }
        }
    }
    else  //cell doesn't move
    {
        cell->SetPosition(new_pos);
        if(!cell_meta->children.empty())
            world_meta->roots.push_back(cell);
    }

    cell_meta->handled = true;
}

void compute_trees(c2World* world)
{
    c2DynamicWorld* world_meta = static_cast<c2DynamicWorld*>(world->meta_ptr);
    std::vector<c2Cell*> cells = world->GetCells();

    for(auto& rcell : world_meta->roots)
    {
        std::queue<c2Cell*> ref_queue;
        ref_queue.push(rcell);

        c2Cell* ref_cell = rcell;

        while(!ref_queue.empty())
        {
            c2DynamicCell* ref_meta = 
              static_cast<c2DynamicCell*>(ref_cell->meta_ptr);

            for(auto& child : ref_meta->children)
            {
                c2DynamicCell* child_meta = 
                  static_cast<c2DynamicCell*>(child->meta_ptr);

                if(!child_meta->children.empty())
                    ref_queue.push(child);

                //CHILD COMPONENT
                c2Vector k1 = ref_cell->GetPosition().Floor()
                  - child->GetPosition().Floor();
                k1 = k1.Normalize();

                c2Vector r1 = k1 * (child_meta->velocity.x * k1.x
                  + child_meta->velocity.y * k1.y);

                //PARENT COMPONENT
                c2Vector k2 = -k1;
                
                c2Vector r2 = k2 * (ref_meta->velocity.x * k2.x
                  + ref_meta->velocity.y * k2.y);

                child_meta->velocity = (child_meta->velocity - r1) + r2;
                ref_meta->velocity = (ref_meta->velocity - r2) + r1;
            }

            ref_queue.pop();
            ref_cell = ref_queue.front();
        }
    }

    for(auto& cell : cells)
    {
        c2DynamicCell* cell_meta = static_cast<c2DynamicCell*>(cell->meta_ptr);

        cell_meta->velocity += world_meta->gravity;
        cell_meta->parent = nullptr;
        cell_meta->children.clear();
        cell_meta->handled = false;
    }

    world_meta->roots.clear();
}
