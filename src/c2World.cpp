#include "c2World.h"
#include "c2Collisions.h"
#include "c2Cell.h"

c2World::c2World(const c2Vector& gravity, int x, int y)
{
    m_cellCount = 0;
    m_gravity = gravity;

    m_origin.SetZero();
    time_step = 1/30.0;

    this->x = x;
    this->y = y;
}

c2World::~c2World()
{
    for(auto itr = cells.begin(); itr != cells.end(); ++itr)
    { delete *itr; }
}

void c2World::Step(c2QuadTree* tree)
{
    std::vector<c2Cell*> roots;
    roots.clear();
     
    for(auto itr = cells.begin(); itr != cells.end(); ++itr)                //Iterate through all the cells
    {                                                                       //and calculate their new position
        if(!(*itr)->active) 
        { 
            (*itr)->moved = false;
            continue;
        }

        if(!(*itr)->in_world) { continue; }

        c2Vector new_pos = (*itr)->position + ((*itr)->velocity * time_step);     

        //IF THE CELL MOVES INTO ANOTHER GRID SPOT
        if(new_pos.Floor() != (*itr)->position.Floor())
        {   
            if(new_pos.Floor().x < 0 || new_pos.Floor().x >= this->x)
            {
                c2Matrix22 reflect_y(-1, 0, 0, 1); //Reflection about the y-axis     
                (*itr)->velocity = reflect_y.Solve((*itr)->velocity);
                
                (*itr)->handled = true;                                //Check if the cell will collide
                continue;                                              //with the floor instead.
            }                                                          //If so, update velocity and continue

            if(new_pos.Floor().y < 0 || new_pos.Floor().y >= this->y)
            {
                c2Matrix22 reflect_x(1, 0, 0, -1); //Reflection about the x-axis
                (*itr)->velocity = reflect_x.Solve((*itr)->velocity);

                (*itr)->handled = true;
                continue;
            }
            
            if((*itr)->in_tree == false)
            {
                (*itr)->position = new_pos;               //If the cell moves but will not
                (*itr)->moved = true;                     //perform any collision checks, move it
                (*itr)->handled = true;                   //and continue to handle the next cell.
                                                          //Otherwise perform collision checks
                continue;
            }

            for(auto nitr = (*itr)->leaves.begin(); nitr != (*itr)->leaves.end();
                ++nitr)                                                           //Iterate through the current
            {                                                                     //cells (**itr) leaf nodes
                std::vector<c2Cell*> clist = (*nitr)->cells;                      //and compare with other
                for(auto citr = clist.begin(); citr != clist.end(); ++citr)       //cells in that node
                {
                    if((*citr)->active == false || *citr == *itr) { continue; }     //Skip cell if it compares  
                                                                                    //with itself/is inactive
                    //IF THE CELL COLLIDES WITH ANOTHER
                    if(new_pos.Floor() == (*citr)->position.Floor())
                    {
                        if((*citr)->parent == nullptr) {                      
                            if((*citr)->handled && (*citr)->children.empty()) {                           
                                roots.push_back(*citr);        //Set the cell being collided with
                            }                                  //as a 'root' if it qualifies
                        }
                        else                                   //Check for cycles in the collision
                        {
                            if((*citr)->parent == *itr)
                            {
                                roots.push_back(*itr);            //If a collision is being made head-on
                                (*itr)->moved = false;            //then set the current cell as the root
                                goto COLLISION;                   //and dont set parent/children
                            } 
                            
                            c2Cell* cparent = (*citr)->parent;
                            while(!(cparent == nullptr))          //Otherwise check for cycles where
                            {                                     //collisions must not occur
                                if(*itr == cparent) {
                                    goto NOCOLLISION;
                                }
                                cparent = cparent->parent;
                            }
                        }
                        (*citr)->children.push_back(*itr);                    //Set current cell as child
                        (*itr)->parent = *citr;                               //to the cell it is
                        (*itr)->moved = false;                                //colliding with and set it
                                                                              //as the current cells parent
                        goto COLLISION;              //To exit nested for loop and not be processed
                    }                                //as a cell that doesn't move
                }
            }

            //IF THE CELL DOESN'T COLLIDE WITH ANOTHER
            NOCOLLISION:
            (*itr)->position = new_pos; 
            (*itr)->moved = true;                  //Meaning it moves into an empty space, hence
            c2Cell* ref_cell = *itr;               //update its position and set it as the
                                                   //reference cell. 
            tree->InsertCell(*itr);                //Update tree as it may have moved into a new node

            std::vector<c2Cell*> cell_queue;

            while(!ref_cell->children.empty())                               //Keep updating the left-most
            {                                                                //child cell if it exists.
                c2Cell* child_cell = ref_cell->children[0];                  //'child_cell' variable exists   
                child_cell->position += child_cell->velocity * time_step;    //incase the current cell's     
                child_cell->moved = true;                                    //children vector gets cleared

                if(ref_cell->children.size() > 1)
                {
                    cell_queue.push_back(ref_cell);             //If the current cell has more than 1 child
                }                                               //this means the left-most child cell will
                else                                            //update into the current cell's previous
                {                                               //position, in which there exists other cells
                    ref_cell->children.clear();                 //moving into it.
                }                                               //Hence push the current cell into 'cell_queue'
                                                                //for further action
                if(child_cell->parent->children.empty())
                {  
                    child_cell->parent = nullptr;               //...otherwise, if there exist only 1 child
                }                                               //cell clear the current cell's children vector.
                                                                
                tree->InsertCell(child_cell);               //If the parent cell has no children (and hence
                ref_cell = child_cell;                      //does not reference the current cell) set
           }                                                //parent cell to null (the parent will only
                                                            //have no children if it has only 1 child!)
           if(!cell_queue.empty())
           {
              for(auto qitr = cell_queue.rbegin(); qitr != cell_queue.rend(); ++qitr)
              {
                  for(auto pitr = (*qitr)->children.begin() + 1;          //For each cell in 'cell_queue',
                      pitr != (*qitr)->children.end(); ++pitr)            //update '**qitr's children into
                  {                                                       //the left-most child (excluding itself).
                      (*qitr)->children[0]->children.push_back(*pitr);
                      (*pitr)->parent = (*qitr)->children[0];
                  }                                                        //...then set the left-most child's
                                                                           //parent cell to null and clear
                  (*qitr)->children[0]->parent = nullptr;                  //**qitr's children vector.
                  roots.push_back((*qitr)->children[0]);                   //Left-most child also becomes
                                                                           //a collision root
                  (*qitr)->children.clear();
              }
           } 
        }

        //IF THE CELL DOESN'T MOVE
        else 
        {
            (*itr)->moved = false;
            (*itr)->position = new_pos;

            if(!(*itr)->children.empty())    //If it doesn't move the only factor to consider is
            {                                //a cell will move into it. If so then it is a collision root.
                roots.push_back(*itr);
            }
        }
       
        //AFTER CELL HAS BEEN 'HANDLED' 
        COLLISION:
        (*itr)->handled = true;
    }

    //COLLISION TREE IS COMPLETE
    for(auto rootitr = roots.begin(); rootitr != roots.end(); ++rootitr)
    {
        std::vector<c2Cell*> ref_queue;
        ref_queue.clear();
        ref_queue.push_back(*rootitr);

        c2Cell* ref_cell = *(ref_queue.begin());
        
        while(!ref_queue.empty())
        {
            for(auto childitr = ref_cell->children.begin();
               childitr != ref_cell->children.end(); ++childitr)
            {
                if(!(*childitr)->children.empty()) 
                { 
                  ref_queue.push_back(*childitr); 
                }
                
                //CHILD COMPONENT
                c2Vector k1 = ref_cell->position.Floor() - (*childitr)->position.Floor();
                k1 = k1.Normalize();

                c2Vector r1 = k1 * ((*childitr)->velocity.x * k1.x
                    + (*childitr)->velocity.y * k1.y);

                //PARENT COMPONENT
                c2Vector k2 = (*childitr)->position.Floor() - ref_cell->position.Floor();
                k2 = k2.Normalize();

                c2Vector r2 = k2 * (ref_cell->velocity.x * k2.x
                    + ref_cell->velocity.y * k2.y); 

                (*childitr)->velocity = ((*childitr)->velocity - r1) + r2;
                ref_cell->velocity = (ref_cell->velocity - r2) + r1;
            } 
            ref_queue.erase(ref_queue.begin());
            ref_cell = *(ref_queue.begin());
        }
    }
    
    //ALL PROCESSES ARE FINISHED
    for(auto itr = cells.begin(); itr != cells.end(); ++itr)
    {   
        if((*itr)->type == c2_dynamicCell && (*itr)->active && (*itr)->in_world)
            (*itr)->velocity += m_gravity * (*itr)->gravityScale;

        if((*itr)->velocity.Magnitude() != 0)
        {
            if(1 / (*itr)->velocity.Magnitude() < time_step || time_step == 0)     
            {                                                  
                time_step = 1 / (*itr)->velocity.Magnitude();
            }
        }

        (*itr)->parent = nullptr;
        (*itr)->children.clear();
        (*itr)->handled = false;
    }
}

c2Cell* c2World::CreateCell(const c2CellDef& def)
{
    c2Cell* cell = new c2Cell(def, this);
    
    if(def.type == c2_dynamicCell)
    {
        cell->velocity += m_gravity * cell->gravityScale;
    }
    
    if(cell->velocity.Magnitude() != 0)
    {
        if(1 / cell->velocity.Magnitude() < time_step || time_step == 0)
        {
            time_step = 1 / cell->velocity.Magnitude();
        }
    }

    cell->index = cells.size();
    cells.push_back(cell);
    m_cellCount += 1;

    return cell;
}

void c2World::DestroyCell(c2Cell* cell)
{
    cells.back()->index = cell->index;
    std::iter_swap(cells.begin() + cell->index, cells.end() - 1);
    cells.pop_back();

    m_cellCount -= 1;

    delete cell;
}
