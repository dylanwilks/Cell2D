#include "c2World.h"
#include "c2Collisions.h"
#include "c2Cell.h"

c2QuadTree::c2QuadTree(c2World* world) 
{
    level = 0;
    count = 0;

    this->world = world;
    parent = nullptr;

    node_dims.p1.Set(0, 0);
    node_dims.p2.Set(world->GetLength() - 1, world->GetHeight() - 1);
    node_dims.m_p = ((node_dims.p1 + node_dims.p2)/2).Floor();
}

void c2QuadTree::InsertCell(c2Cell* cell)
{
    c2QuadTree* node = RemoveCell(cell);
    c2Vector pos = cell->position;

    if(!cell->in_world) { return; }
    if(node == nullptr) { node = this; }

    while(true)
    {
        //use flag bits in c2Math for this
        bool edge_x, edge_y, edge_gx, edge_gy, out;
        int fx, fy;
        fx = floor(pos.x); fy = floor(pos.y);

        edge_x = (fx == node->node_dims.p1.x) || (fx == node->node_dims.p2.x);
        edge_y = (fy == node->node_dims.p1.y) || (fy == node->node_dims.p2.y);
        edge_gx = (fx == 0) || (fx == world->GetLength() - 1);
        edge_gy = (fy == 0) || (fy == world->GetHeight() - 1);

        //out = floor(pos.x) < node->p1.x || floor(pos.x) > node->p2.x 
        //  || floor(pos.y) < node->p1.y || floor(pos.y) > node->p2.y;
        out = floor(pos.x) < node->node_dims.p1.x || 
          floor(pos.x) > node->node_dims.p2.x || 
          floor(pos.y) < node->node_dims.p1.y || 
          floor(pos.y) > node->node_dims.p2.y;

        if((edge_x && !edge_gx) || (edge_y && !edge_gy) || out) {
            node = node->parent;
            continue;
        } 
        else { 
            node->insert_cell(cell);
            break; 
        }
    }
}

c2QuadTree* c2QuadTree::RemoveCell(c2Cell* cell) 
{  
    if(cell->in_tree == false) { return nullptr; } 
    c2QuadTree* common = *(cell->leaves.begin());
    int max = common->level;

    for(auto nitr = cell->leaves.begin(); nitr != cell->leaves.end(); ++nitr)
    {
        if(common->level > (*nitr)->level) { common = *nitr; }
        if(max > (*nitr)->level) { max = (*nitr)->level; }

        for(auto citr = (*nitr)->cells.begin(); citr != (*nitr)->cells.end(); ++citr)
        {
            if(*citr == cell)
            {
                (*nitr)->cells.erase(citr);     //Cell delete handled by c2World::DestroyCell(c2Cell* cell)
                if((*nitr)->count != -1)
                    (*nitr)->count -= 1;     //Removing cell from quadtree just prevents it from colliding
                
                if((*nitr)->count == 0 && (*nitr)->level != 0) { 
                    c2QuadTree* node = (*nitr)->parent->RemoveChildNodes();
                    if(node != nullptr) {
                        common = node;
                    }
                }

                break;
            }
        }
    }

    if(cell->leaves.size() > 1 && common->level == max 
        && !(common->parent == nullptr)) { 
        common = common->parent; 
    }
    
    cell->in_tree = false;
    cell->leaves.clear();
    
    if(common->level == 0) { return this; }
    else { return common; }
}

void c2QuadTree::CreateChildNodes()
{ 
    for(int i = 0; i < 4; i++)
    {
        nodes.push_back(new c2QuadTree(world));

        nodes[i]->level = this->level + 1;
        nodes[i]->parent = this;

        nodes[i]->node_dims.p1 = this->node_dims.p1;
        nodes[i]->node_dims.p2 = this->node_dims.p2;    

        switch(i)
        {
        case 0:
            nodes[0]->node_dims.p1 = this->node_dims.m_p;
            nodes[0]->node_dims.m_p = 
              ((nodes[0]->node_dims.p1 + nodes[0]->node_dims.p2)/2).Floor();

            break;
        case 1:
            nodes[1]->node_dims.p1.y = node_dims.m_p.y;
            nodes[1]->node_dims.p2.x = node_dims.m_p.x;

            nodes[1]->node_dims.m_p = 
              ((nodes[1]->node_dims.p1 + nodes[1]->node_dims.p2)/2).Floor();

            break;
        case 2:
            nodes[2]->node_dims.p2 = this->node_dims.m_p;
            nodes[2]->node_dims.m_p = 
              ((nodes[2]->node_dims.p1 + nodes[2]->node_dims.p2)/2).Floor();

            break;
        case 3:
            nodes[3]->node_dims.p1.x = node_dims.m_p.x;
            nodes[3]->node_dims.p2.y = node_dims.m_p.y;

            nodes[3]->node_dims.m_p = 
              ((nodes[3]->node_dims.p1 + nodes[3]->node_dims.p2)/2).Floor();

            break;
        } 
    }  
    
    this->count = -1;
    while(!cells.empty()) {
        InsertCell(*(cells.begin()));
    }

    cells.clear();
}

c2QuadTree* c2QuadTree::RemoveChildNodes()
{
    for(int i = 0; i < 4; i++)
    {
        if(!nodes[i]->cells.empty() || !nodes[i]->nodes.empty()) { return nullptr; }    
    }

    for(auto nitr = nodes.begin(); nitr != nodes.end(); ++nitr)
    {
        delete *nitr;
    }

    nodes.clear();
    count = 0;

    c2QuadTree* node = this;
    if(parent != nullptr) { node = parent->RemoveChildNodes(); }

    if(node == nullptr) { return this; }
    else { return node; }
}

c2QuadTree* c2QuadTree::insert_cell(c2Cell* cell)
{
    c2QuadTree* node = nullptr;
    std::vector<int> quad = GetQuadrants(cell);

    if(count > -1) 
    {
        c2Vector t_v = node_dims.p2 - node_dims.p1; 
        if(count == MAX_CELLS && t_v.Magnitude() >= 1.415) { CreateChildNodes(); }
        else
        {
            cells.push_back(cell); 
            count += 1;
            cell->in_tree = true;
            cell->leaves.push_back(this);

            return this;
        }
    }    

    for(auto itr = quad.begin(); itr != quad.end(); ++itr)
    {
        node = nodes[*itr]->insert_cell(cell);
    }

    if(quad.size() > 1) { return this; }

    return node;
}

std::vector<int> c2QuadTree::GetQuadrants(c2Cell* cell)
{
    std::vector<int> quadrants; //vector as cell may be in several quadrants at once

    int fx, fy;
    fx = floor(cell->position.x); fy = floor(cell->position.y);

    bool topQuadrant = fy >= node_dims.m_p.y;
    bool bottomQuadrant = fy <= node_dims.m_p.y;
    
    bool leftQuadrant = fx <= node_dims.m_p.x;
    bool rightQuadrant = fx >= node_dims.m_p.x;
 
    if(topQuadrant)
    {
        if(rightQuadrant) { quadrants.push_back(0); }
        if(leftQuadrant) { quadrants.push_back(1); } 
    }

    if(bottomQuadrant)
    {
        if(rightQuadrant) { quadrants.push_back(3); }
        if(leftQuadrant) { quadrants.push_back(2); }
    }

    return quadrants;
}
